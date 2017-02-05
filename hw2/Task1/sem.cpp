#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<semaphore.h>
using namespace std;

sem_t sem;

class Counter{
 int value;
 public:
  Counter() { 
   value = 0;
  }
  void Increment(){
    sem_wait(&sem);  
    value++;
    sem_post(&sem);  
  }
  void Print(){
   cout<<value;
  }
};

Counter x;

void* ThreadRunner(void*){
 int k;

  for (k = 0; k < 100000000; k++){
    x.Increment();
  }
  pthread_exit(NULL);
}

int main(){

sem_init(&sem,0,1);
int threads=3;
pthread_t tid[threads];

int i;

for ( i = 0; i < threads; i++)
 pthread_create(&tid[i], NULL, ThreadRunner, 0);

for ( i = 0; i < threads; i++)
 pthread_join(tid[i], NULL);

x.Print();
return 0;
}
