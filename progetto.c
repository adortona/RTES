#include <stdlib.h>
#include <allegro.h>
#include <stdio.h>
#include <unistd.h> 
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include "ptask.h"

#define PER 80 // period in ms
#define CAMERA_PERIOD 250
#define SENSOR_PERIOD 400
#define PRI 80 // priority of the task

//----CAR COSTANTS-----

#define GREY 195
#define CAR_STEP 4

//----SENSOR COSTANTS

#define STEP 20

BITMAP *sfondo;

pthread_mutex_t m_traffic;
pthread_mutex_t m_car;
pthread_cond_t cond_car;
pthread_mutex_t mutex;


struct status{

  int color;
  int x1;
  int x2;
  int y1;
  int y2;
  int stop_car;
  int distanza_sicurezza;
}; 

struct status car;
struct status car_1;
struct status car_2;
struct status car_3;
struct status car_4;
struct status car_5;
struct status car_6;
struct status car_7;
struct status car_8;
struct status car_9;

int colore_strada;  

void inizializza()
{
  allegro_init();
  ptask_init(SCHED_FIFO, GLOBAL, NO_PROTOCOL);
  install_timer(); 
  install_mouse();
  install_keyboard(); 
  set_color_depth(32); 
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); 
  srand(time(NULL)); //initialize seed for rand function
  pmux_create_pi(&m_traffic);
  pmux_create_pi(&m_car);
}

void set_background(){
  
    int x_sfondo = 0;
    int y_sfondo = 0;
    sfondo = load_bmp("background.bmp", NULL);
    if (sfondo == NULL)
    {
      printf("file not found");
      printf("please, bring in file background.bmp");
      exit(1);
    }
    blit(sfondo, screen, 0, 0, x_sfondo, y_sfondo, sfondo->w, sfondo->h);
}

void init_car(struct status *c, int colore, int x_1, int y_1, int x_2, int y_2)
{ 
  c->color = colore;
  c->x1 = x_1;
  c->x2 = x_2;
  c->y1 = y_1;
  c->y2 = y_2;
  c->stop_car = 1;
  c->distanza_sicurezza = 1;  
}

void draw_car(int i, struct status *c)
{ 
  int x_1, x_2, y_1, y_2;
  x_1 = c->x1;
  x_2 = c->x2;
  y_1 = c->y1;
  y_2 = c->y2;
  rectfill(screen, x_1, y_1, x_2, y_2, c->color);
}

//----CAMERA FUNCTION----------------------------------------

void get_image(int x0, int y0, struct status *c)
{
  
  int image[14][14];
  int i,j;
  int x,y;
  for(i = 0; i < 14; i++){
    for(j = 0; j < 14; j++){
      x = x0 - 7 + i;
      y = y0 - 7 + j;
      image[i][j] = getpixel(screen, x, y);
      if(image[i][j] == getpixel(screen, 639, 479)){
        c->stop_car = 0;
      }
      else if(image[i][j] == getpixel(screen, 638, 478)){
        c->stop_car = 1;
      }
    } 
  }
}

int x0_sensor, y0_sensor;

//----SEONSOR FUNCTION--------------------------------------

void read_sensor_right(struct status *c)
{
  int x, y; //sensor coordinates
  while(1){
  x = c->x2 + STEP;
  y = c->y2 - 10;
  int color_forward = getpixel(screen, x, y);

  if(color_forward != colore_strada)
  {
    c->distanza_sicurezza = 0;
  }
  else if(color_forward == colore_strada)
  {
    c->distanza_sicurezza = 1;
  }
  
}
}

void read_sensor_left(struct status *c)
{
  int x, y; //sensor coordinates
  while(1){
  x = c->x1 - STEP;
  y = c->y1 + 5;
  int color_forward = getpixel(screen, x, y);

  if(color_forward != colore_strada)
  {
    c->distanza_sicurezza = 0;
  }
  else if(color_forward == colore_strada)
  {
    c->distanza_sicurezza = 1;
  }
  
}
}

void read_sensor_down(struct status *c)
{
  int x, y; //sensor coordinates
  while(1){
  x = c->x2;
  y = c->y2 + STEP;
  int color_forward = getpixel(screen, x, y);

  if(color_forward != colore_strada)
  {
    c->distanza_sicurezza = 0;
  }
  else if(color_forward == colore_strada)
  {
    c->distanza_sicurezza = 1;
  }
  
}
}

void read_sensor_up(struct status *c)
{
  int x, y; //sensor coordinates
  while(1){
  x = c->x2;
  y = c->y2 - STEP;
  int color_forward = getpixel(screen, x, y);

  if(color_forward != colore_strada)
  {
    c->distanza_sicurezza = 0;
  }
  else if(color_forward == colore_strada)
  {
    c->distanza_sicurezza = 1;
  }
  
}
}




//------MOVEMENT FUNCTIONS------------------------------------


void move_car_right(int i, struct status *c)
{
  while(c->x1 < 610 )
  {
    pthread_mutex_lock(&m_car);
    if (c->stop_car == 1 && c->distanza_sicurezza == 1)
    {
    rectfill(screen, c->x1, c->y1, c->x2, c->y2, makecol(GREY, GREY, GREY));
    c->x1 = c->x1 + CAR_STEP;
    c->x2 = c->x2 + CAR_STEP;
    
    draw_car(i, c);
    
    }
    pthread_mutex_unlock(&m_car);
    ptask_wait_for_period(i);
  }
}

void move_car_left(int i, struct status *c) 
{
   while(c->x1 > 10 )
  {
    pthread_mutex_lock(&m_car);
    if (c->stop_car == 1 &&c->distanza_sicurezza == 1)
    {
    rectfill(screen, c->x1, c->y1, c->x2, c->y2, makecol(GREY, GREY, GREY));
    c->x1 = c->x1 - CAR_STEP;
    c->x2 = c->x2 - CAR_STEP;
    
    draw_car(i, c);
    
    }
    pthread_mutex_unlock(&m_car);
    ptask_wait_for_period(i);
  }
}

void move_car_down(int i, struct status *c) 
{
   while(c->y1 < 470 )
  {
    pthread_mutex_lock(&m_car);
    if (c->stop_car == 1 && c->distanza_sicurezza == 1)
    {
    rectfill(screen, c->x1, c->y1, c->x2, c->y2, makecol(GREY, GREY, GREY));
    c->y1 = c->y1 + CAR_STEP;
    c->y2 = c->y2 + CAR_STEP;
    
    draw_car(i, c);
    
    }
    pthread_mutex_unlock(&m_car);
    ptask_wait_for_period(i);
  }
}

void move_car_up(int i, struct status *c) 
{
   while(c->y1 > -10 )
  {
    pthread_mutex_lock(&m_car);
    if (c->stop_car == 1 && c->distanza_sicurezza == 1)
    {
    rectfill(screen, c->x1, c->y1, c->x2, c->y2, makecol(GREY, GREY, GREY));
    c->y1 = c->y1 - CAR_STEP;
    c->y2 = c->y2 - CAR_STEP;
    
    draw_car(i, c);
    
    }
    pthread_mutex_unlock(&m_car);
    ptask_wait_for_period(i);
  }
}

//------------------------------------------------------------------

//----------TRAFFIC LIGHTS CONFIGURATIONS----------------------------

void traffic_lights_configuration_1()
{

  //---destra

  circlefill(screen, 360, 75, 10, makecol(0, 255, 0));
  circlefill(screen, 360, 185, 10, makecol(255, 0, 0));
  circlefill(screen, 360, 280, 10, makecol(0, 255, 0));
  circlefill(screen, 360, 397, 10, makecol(255, 0, 0));

  //---sinistra
  
  circlefill(screen, 220, 75, 10, makecol(255, 0, 0));
  circlefill(screen, 220, 185, 10, makecol(0, 255, 0));
  circlefill(screen, 220, 280, 10, makecol(255, 0, 0));
  circlefill(screen, 220, 397, 10, makecol(0, 255, 0));
}

void traffic_lights_configuration_2()
{

  //---destra

  circlefill(screen, 360, 75, 10, makecol(255, 0, 0));
  circlefill(screen, 360, 185, 10, makecol(0, 255, 0));
  circlefill(screen, 360, 280, 10, makecol(255, 0, 0));
  circlefill(screen, 360, 397, 10, makecol(0, 255, 0));

  //---sinistra
  
  circlefill(screen, 220, 75, 10, makecol(0, 255, 0));
  circlefill(screen, 220, 185, 10, makecol(255, 0, 0));
  circlefill(screen, 220, 280, 10, makecol(0, 255, 0));
  circlefill(screen, 220, 397, 10, makecol(255, 0, 0));
}

//----------------------------------------------------------------


//--------PERIODIC TASKS------------------------------------------

ptask traffic_lights()
{
  int i;
  i = ptask_get_index;
  while(1){
  pthread_mutex_lock(&m_traffic);
  traffic_lights_configuration_1();
  pthread_mutex_unlock(&m_traffic);
  usleep(7000000);
  pthread_mutex_lock(&m_traffic);
  traffic_lights_configuration_2();
  pthread_mutex_unlock(&m_traffic);
  ptask_wait_for_period(i);
  }
}

//----CAR 0 TASKS--------------------------------------------------
ptask car_task()
{
  int i;
  i = ptask_get_index();
  init_car(&car, makecol(0, 0, 0), 10, 140, 30 ,155);
  move_car_right(i, &car);
  
}

ptask camera_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car.x1, car.y1 + 40, &car);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_right(&car);
    ptask_wait_for_period(i);
  }
  
  
}



//----CAR 1 TASKS-----------------------------------------------------

ptask car_task_1()
{
  int i;
  i = ptask_get_index();
  init_car(&car_1, makecol(102, 254, 254), 10, 360, 30, 375);
  
  move_car_right(i, &car_1);
  
}

ptask camera_1_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_1.x1, car_1.y1 + 40, &car_1);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_1_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_right(&car_1);
    ptask_wait_for_period(i);
  }
  
  
}
//----CAR 2 TASKS----------------------------------------------------------

ptask car_task_2()
{
  int i;
  i = ptask_get_index();
  init_car(&car_2, makecol(255, 254, 254), 610, 100, 630, 115);
  move_car_left(i, &car_2);
}

ptask camera_2_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_2.x1, car_2.y1 - 40, &car_2);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_2_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_left(&car_2);
    ptask_wait_for_period(i);
  }
  
  
}

//----CAR 3 TASKS---------------------------------------------------------

ptask car_task_3()
{
  int i;
  i = ptask_get_index();
  init_car(&car_3, makecol(255, 0, 0), 610, 310, 630, 325);
  move_car_left(i, &car_3);
}

ptask camera_3_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_3.x1, car_3.y1 - 40, &car_3);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
}

ptask sensor_3_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_left(&car_3);
    ptask_wait_for_period(i);
  }
  
  
}

//----CAR 4 TASKS-------------------------------------------------------

ptask car_task_4()
{
  int i;
  i = ptask_get_index();
  init_car(&car_4, makecol(0, 0, 255), 10, 140, 30, 155);
  
  move_car_right(i, &car_4);
  
}

ptask camera_4_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_4.x1, car_4.y1 + 40, &car_4);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_4_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_right(&car_4);
    ptask_wait_for_period(i);
  }
}

//-------CAR 5 TASK----------------------------

ptask car_task_5()
{
  int i;
  i = ptask_get_index();
  init_car(&car_5, makecol(0, 210, 0), 250, 10, 265, 30);
  
  move_car_down(i, &car_5);
  
}

ptask camera_5_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_5.x1 - 30, car_5.y1 + 10, &car_5);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_5_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_down(&car_5);
    ptask_wait_for_period(i);
  }
}

//---------CAR 6 TASK----------------------------

ptask car_task_6()
{
  int i;
  i = ptask_get_index();
  init_car(&car_6, makecol(0, 0, 0), 610, 100, 630, 115);
  move_car_left(i, &car_6);
}

ptask camera_6_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_6.x1, car_6.y1 - 40, &car_6);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_6_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_left(&car_6);
    ptask_wait_for_period(i);
  }
}

//----------CAR 7 TASK------------------------

ptask car_task_7()
{
  int i;
  i = ptask_get_index();
  init_car(&car_7, makecol(0, 0, 255), 250, 10, 265, 30);
  
  move_car_down(i, &car_7);
  
}

ptask camera_7_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_7.x1 - 30, car_7.y1 + 10, &car_7);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_7_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_down(&car_7);
    ptask_wait_for_period(i);
  }
}

//----------CAR 8 TASK------------------------

ptask car_task_8()
{
  int i;
  i = ptask_get_index();
  init_car(&car_8, makecol(200, 200, 0), 320, 470, 335, 450);
  
  move_car_up(i, &car_8);
  
}

ptask camera_8_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_8.x2 + 20, car_8.y1 - 10, &car_8);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_8_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_up(&car_8);
    ptask_wait_for_period(i);
  }
}

//----------CAR 9 TASK------------------------

ptask car_task_9()
{
  int i;
  i = ptask_get_index();
  init_car(&car_9, makecol(200, 50, 150), 320, 470, 335, 450);
  
  move_car_up(i, &car_9);
  
}

ptask camera_9_task(struct status *c)
{
  int i;
  i = ptask_get_index();
  while(1)
  {
    get_image(car_9.x2 + 20, car_9.y1 - 10, &car_9);
    //usleep(100000);
    ptask_wait_for_period(i);
  }
} 

ptask sensor_9_task(struct status *c)
{
 ;
  int i;
  
  i = ptask_get_index();
  while(1)
  {
    read_sensor_up(&car_9);
    ptask_wait_for_period(i);
  }
}

//----------------------------------------------------------------------------

void car_0_on_the_road(){

  ptask_create(car_task, PER, PRI, NOW);
  ptask_create(camera_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_task, SENSOR_PERIOD , PRI, NOW);
  usleep(2000000);
}

void car_1_on_the_road()
{
  ptask_create(car_task_1, PER, PRI, NOW);
  ptask_create(camera_1_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_1_task, SENSOR_PERIOD , PRI, NOW);
  usleep(3000000);

}

void car_2_on_the_road()
{
  ptask_create(car_task_2, PER, PRI, NOW);
  ptask_create(camera_2_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_2_task, SENSOR_PERIOD , PRI, NOW);
}

void car_3_on_the_road()
{
  ptask_create(car_task_3, PER, PRI, NOW);
  ptask_create(camera_3_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_3_task, SENSOR_PERIOD , PRI, NOW);
  usleep(2000000);
}

void car_4_on_the_road()
{
  ptask_create(car_task_4, PER, PRI, NOW);
  ptask_create(camera_4_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_4_task, SENSOR_PERIOD , PRI, NOW);

  usleep(3000000);
}

void car_5_on_the_road()
{
  ptask_create(car_task_5, PER, PRI, NOW);
  ptask_create(camera_5_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_5_task, SENSOR_PERIOD , PRI, NOW);

  usleep(4000000);
}

void car_6_on_the_road()
{
  ptask_create(car_task_6, PER, PRI, NOW);
  ptask_create(camera_6_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_6_task, SENSOR_PERIOD , PRI, NOW);

  usleep(4000000);
}

void car_7_on_the_road()
{
  ptask_create(car_task_7, PER, PRI, NOW);
  ptask_create(camera_7_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_7_task, SENSOR_PERIOD , PRI, NOW);

  usleep(4000000);
}

void car_8_on_the_road()
{
  ptask_create(car_task_8, PER, PRI, NOW);
  ptask_create(camera_8_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_8_task, SENSOR_PERIOD , PRI, NOW);

  usleep(4000000);
}

void car_9_on_the_road()
{
  ptask_create(car_task_9, PER, PRI, NOW);
  ptask_create(camera_9_task, CAMERA_PERIOD, PRI, NOW);
  ptask_create(sensor_9_task, SENSOR_PERIOD , PRI, NOW);

  usleep(4000000);
}


void sequence_1(){

//  car_8_on_the_road();
  car_9_on_the_road();
  car_5_on_the_road();
 // car_0_on_the_road();
  car_1_on_the_road();
//  car_4_on_the_road();
  car_7_on_the_road();
//  car_2_on_the_road();
  car_3_on_the_road();
  car_6_on_the_road();
}

void sequence_2(){
  
  car_5_on_the_road();
//  car_3_on_the_road();
  car_1_on_the_road();
  car_8_on_the_road();
  car_9_on_the_road();
//  car_2_on_the_road();
  car_7_on_the_road();
//  car_4_on_the_road();
  car_6_on_the_road();
 // car_0_on_the_road();

}

void sequence_3(){
  
  car_5_on_the_road();
//  car_2_on_the_road();
  car_7_on_the_road();
  car_8_on_the_road();
  car_9_on_the_road();
//  car_1_on_the_road();
  car_6_on_the_road();
//  car_3_on_the_road();
  car_0_on_the_road();
//  car_4_on_the_road();
  
}

void sequence_4(){

  car_2_on_the_road();
  car_7_on_the_road();
  car_8_on_the_road();
  car_9_on_the_road();
  car_1_on_the_road();
}

void sequence_5(){

  car_0_on_the_road();
  car_7_on_the_road();
  car_4_on_the_road();
  car_2_on_the_road();
  car_9_on_the_road();
}

void sequence_6(){

  car_1_on_the_road();
  car_7_on_the_road();
  car_4_on_the_road();
  car_2_on_the_road();
  car_3_on_the_road();
}
void sequence_7(){

  car_2_on_the_road();
  car_7_on_the_road();
  car_1_on_the_road();
  car_8_on_the_road();
  car_9_on_the_road();
}

void sequence_8(){

  car_1_on_the_road();
  car_7_on_the_road();
  
  car_3_on_the_road();
  car_4_on_the_road();
  car_8_on_the_road();
}

void sequence_9(){

  car_3_on_the_road();
  car_7_on_the_road();
  car_1_on_the_road();
  car_5_on_the_road();
  car_9_on_the_road();
}



int main(int argc, char* argv[]){
  
  inizializza();
  set_background();
  
  
  // RETURNS 2273612 for green(?)
  // RETURN  12829635 for grey(?)


  putpixel(screen, 639, 479, makecol(255, 0, 0)); //pixel di riferimento rosso
  putpixel(screen, 638, 478, makecol(0, 255, 0)); //pixel di riferimento verde
  colore_strada = getpixel(screen, 1, 140);
  putpixel(screen, 637, 477, colore_strada); // pixel di riferimento strada
  

  ptask_create(traffic_lights, 18000, 1, NOW);
  usleep(4000000);

  int selector;
  
  
  
    selector = rand()%9;
    switch(selector){
      case 0:
      {
        sequence_1();
        
        
        break;
      }
      case 1:
      {
        sequence_2();
        
        
        break;
      }
      case 2:
      {
        sequence_3();
       
       
        break;
      }
      case 3:
      {
        sequence_4();
        break;
      }
      case 4:
      {
        sequence_5();
        break;
      }
      case 5:
      {
        sequence_6();
        break;
      }
      case 6:
      {
        sequence_7();
        break;
      }
      case 7:
      {
        sequence_8();
        break;
      }

      case 8:
      {
        sequence_9();
        break;
      }
    
    }
      
  while(!key[KEY_ESC]){

  }


 
return 0;
}
