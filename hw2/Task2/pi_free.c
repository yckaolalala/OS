#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
int threads=2;
int total_number_of_points=0;
int number_of_points_in_circle=0;


void* pie_counter(void* arg){
int count=0;
 unsigned seed=(unsigned)time(NULL);
 int times=total_number_of_points/threads;
 while((times--)>0){
  float x=rand_r(&seed);
      x = x / (float)RAND_MAX*2-1 ;
  float y=rand_r(&seed);
      y = y / (float)RAND_MAX*2-1 ; 
 
  if((x*x+y*y)<=1){
   count++;
  }
 }
 number_of_points_in_circle+=count;
 pthread_exit(NULL);
}


int main(int argc,const char *argv[]){
 if(argc < 1) return 0;
 total_number_of_points=atoi(argv[1]);

 pthread_t tid[threads];
 int i;

 for(i=0;i<threads;i++)
   pthread_create(&tid[i],NULL,pie_counter,0);
 for(i=0;i<threads;i++)
   pthread_join(tid[i],NULL);

  printf("pi estimate = %f\n",(float)4*number_of_points_in_circle/(total_number_of_points));
return 0;
}
