#include<stdio.h>
#include<iostream>
#include<pthread.h>
using namespace std;

pthread_spinlock_t spinlock;

class Counter{
 int value;
 public:
  Counter() { 
   value = 0;
  }
  void Increment(){
   pthread_spin_lock(&spinlock);
   value++;
   pthread_spin_unlock(&spinlock);
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
int threads=3; 
pthread_spin_init(&spinlock,0);
pthread_t tid[threads];

int i;

for ( i = 0; i < threads; i++)
 pthread_create(&tid[i], NULL, ThreadRunner, 0);

for ( i = 0; i < threads; i++)
 pthread_join(tid[i], NULL);

x.Print();
return 0;
}
