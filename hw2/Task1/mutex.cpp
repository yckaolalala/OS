#include<stdio.h>
#include<iostream>
#include<pthread.h>
using namespace std;

pthread_mutex_t mutex;

class Counter{
 int value;
 public:
  Counter() { 
   value = 0;
  }
  void Increment(){
   pthread_mutex_lock(&mutex);
   value++;
   pthread_mutex_unlock(&mutex);
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

pthread_mutex_init(&mutex,NULL);
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
