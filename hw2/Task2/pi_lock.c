#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
int threads=2;
int total_number_of_points=0;
int number_of_points_in_circle=0;

pthread_mutex_t mutex;

void* pie_counter(void* arg){
unsigned seed=(unsigned)time(NULL);
 while(1){
     
   pthread_mutex_lock(&mutex);
   if(total_number_of_points==0){
       pthread_mutex_unlock(&mutex);
       break;
   }
   total_number_of_points--;
   pthread_mutex_unlock(&mutex);
  
  float x=rand_r(&seed);
      x = x / (float)RAND_MAX*2-1 ;
  float y=rand_r(&seed);
      y = y / (float)RAND_MAX*2-1 ; 
 
  if((x*x+y*y)<=1){
   pthread_mutex_lock(&mutex);   
   number_of_points_in_circle++;
   pthread_mutex_unlock(&mutex);
  }
 } 
 pthread_exit(NULL);
}


int main(int argc,const char *argv[]){
 if(argc < 1) return 0;
 total_number_of_points=atoi(argv[1]);
 
 pthread_mutex_init(&mutex,NULL);
 pthread_t tid[threads];
 int i,times=total_number_of_points;

 for(i=0;i<threads;i++)
   pthread_create(&tid[i],NULL,pie_counter,0);
 for(i=0;i<threads;i++)
   pthread_join(tid[i],NULL);

  printf("pi estimate = %f\n",(float)4*number_of_points_in_circle/times);
return 0;
}
