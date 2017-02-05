#include<stdio.h>
#include<iostream>
#include<pthread.h>
using namespace std;

pthread_spinlock_t spinlock;

void homemade_spin_lock(int* spinlock_addr) {
 asm(
 "spin_lock: \n\t"
 "xorl %%ecx, %%ecx \n\t"
 "incl %%ecx \n\t"
 "spin_lock_retry: \n\t"
 "xorl %%eax, %%eax \n\t"
 "lock; cmpxchgl %%ecx, (%0) \n\t"
 "jnz spin_lock_retry \n\t"
 : : "r" (spinlock_addr) : "ecx", "eax" );
}

void homemade_spin_unlock(int* spinlock_addr) {
 asm(
 "spin_unlock: \n\t"
 "movl $0, (%0) \n\t"
 : : "r" (spinlock_addr) : );
}

int spinlock_addr=0;

class Counter{
 int value;
 public:
  Counter() { 
   value = 0;
  }
  void Increment(){
   homemade_spin_lock(&spinlock_addr);
   value++;
   homemade_spin_unlock(&spinlock_addr);
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
pthread_t tid[threads];

int i;

for ( i = 0; i < threads; i++)
 pthread_create(&tid[i], NULL, ThreadRunner, 0);

for ( i = 0; i < threads; i++)
 pthread_join(tid[i], NULL);

x.Print();
return 0;
}
