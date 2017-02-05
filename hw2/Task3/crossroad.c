#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int n=0,e=0,s=0,w=0;
int cars[4];
int lock[4];
int holdlock[4];
int t=3;


pthread_mutex_t mutex;

void* Ncross(void* threadid){
   int number=0;
   while(cars[0]>0){
    if(n==0){
    pthread_mutex_lock(&mutex); // get 1st lock 
       t++;
       if(lock[0]==0){  
        lock[0]=1;
        n++;
       // printf("%d N %d acquire lock_0.\n",(t/4),number);
       }else{
       // printf("%d N %d acquire lock_0(fail)\n",(t/4),number);
       }
    pthread_mutex_unlock(&mutex);//end get 1st
    sleep(1);
    } 
   
    if(n==1){
     pthread_mutex_lock(&mutex);//get 2nd lock
       t++;
       if(lock[2]==0){
        //printf("%d N %d acquire lock_2.\n",(t/4),number);
        lock[2]=1;
        n++;
       }else{
        holdlock[0]=1;
       // printf("%d N %d acquire lock_2(fail)\n",t/4,number);
        if(holdlock[0]==1 && holdlock[1]==1 &&  holdlock[2]==1 && holdlock[3]==1){
         printf("A DEADLOCK HAPPENS at %d\n",t/4);
         //printf("%d N %d releases lock_0\n",t/4,number);
         lock[0]=0;
         holdlock[0]=0;
         n=0;
        }
       }
     pthread_mutex_unlock(&mutex);//end get 2nd
    sleep(1);
    }  

       if(n==2){
        pthread_mutex_lock(&mutex);// cross road
         t++;
         printf("N %d leave at %d\n",number,t/4);
        pthread_mutex_unlock(&mutex);// end cross
        sleep(1);
       }
    
      if(n==2){
        pthread_mutex_lock(&mutex);//release lock
         t++;
         //printf("%d N %d releases lock_0 and lock_2.\n",t/4,number);
         number++;
         cars[0]--;
         lock[0]=0;
         lock[2]=0;
         holdlock[0]=0;
         n=0;
        pthread_mutex_unlock(&mutex);//end release lock
       sleep(1);
      } 
   }
   while(cars[0]==0){
     pthread_mutex_lock(&mutex);
     if(cars[1]>0 || cars[2]>0 || cars[3]>0){
       t++;
       pthread_mutex_unlock(&mutex);
     }  
     else{
       pthread_mutex_unlock(&mutex);  
       break;
     }     
    sleep(1); 
   }
pthread_exit(NULL);
}

void* Ecross(void* threadid){
   int number=0;
   while(cars[1]>0){
    if(e==0){   
    pthread_mutex_lock(&mutex); // get 1st lock  
      t++;
      if(lock[1]==0){
        lock[1]=1;
        e++;
       // printf("%d E %d acquire lock_1.\n",t/4,number);
      }else{
       // printf("%d E %d acquire lock_1(fail)\n",t/4,number);
      }   
    pthread_mutex_unlock(&mutex);//end get 1st
    sleep(1);
    }

    if(e==1){
     pthread_mutex_lock(&mutex);//get 2nd lock
     t++;
       if(lock[0]==0){
       // printf("%d E %d acquire lock_0.\n",t/4,number);
        lock[0]=1;
        e++;
       }else{
        holdlock[1]=1;
       // printf("%d E %d acquire lock_0(fail)\n",t/4,number);
        if(holdlock[0]==1 && holdlock[1]==1 &&  holdlock[2]==1 && holdlock[3]==1){
         printf("A DEADLOCK HAPPENS at %d\n",t/4);
        // printf("%d E %d releases lock_1\n",t/4,number);
         lock[1]=0;
         holdlock[1]=0;
         e=0;
        }
       }
     pthread_mutex_unlock(&mutex);//end get 2nd
    sleep(1);
    }  

       if(e==2){
        pthread_mutex_lock(&mutex);// cross road
         t++;
         printf("E %d leave at %d\n",number,t/4);
        pthread_mutex_unlock(&mutex);// end cross
        sleep(1);
       }
    
      if(e==2){
        pthread_mutex_lock(&mutex);//release lock
         t++;
        // printf("%d E %d releases lock_1 and lock_0.\n",t/4,number);
         number++;
         cars[1]--;
         lock[1]=0;
         lock[0]=0;
         holdlock[1]=0;
         e=0;
        pthread_mutex_unlock(&mutex);//end release lock
       sleep(1);
      } 
   }
   while(cars[1]==0){
     pthread_mutex_lock(&mutex);
     if(cars[0]>0 || cars[2]>0 || cars[3]>0){
       t++;
       pthread_mutex_unlock(&mutex);
     }  
     else{
       pthread_mutex_unlock(&mutex);  
       break;
     }
     sleep(1);
   }
pthread_exit(NULL);
}

void* Scross(void* threadid){
   int number=0;
   while(cars[2]>0){
    if(s==0){   
    pthread_mutex_lock(&mutex); // get 1st lock  
     t++;
      if(lock[3]==0){
        lock[3]=1;
        s++;
       // printf("%d S %d acquire lock_3.\n",t/4,number);
      }else{
       // printf("%d S %d acquire lock_3(fail)\n",t/4,number);
      }    
    pthread_mutex_unlock(&mutex);//end get 1st
    sleep(1);
    }

    if(s==1){
     pthread_mutex_lock(&mutex);//get 2nd lock
      t++;
       if(lock[1]==0){
       // printf("%d S %d acquire lock_1.\n",t/4,number);
        lock[1]=1;
        s++;
       }else{
        holdlock[3]=1;
       // printf("%d S %d acquire lock_1(fail)\n",t/4,number);
        if(holdlock[0]==1 && holdlock[1]==1 &&  holdlock[2]==1 && holdlock[3]==1){
         printf("A DEADLOCK HAPPENS at %d\n",t/4);
       //  printf("%d S %d releases lock_3\n",t/4,number);
         lock[3]=0;
         holdlock[3]=0;
         s=0;
        }
       }
     pthread_mutex_unlock(&mutex);//end get 2nd
    sleep(1);
    }  

       if(s==2){
        pthread_mutex_lock(&mutex);// cross road
         t++;
         printf("S %d leave at %d\n",number,t/4);
        pthread_mutex_unlock(&mutex);// end cross
        sleep(1);
       }
    
      if(s==2){
        pthread_mutex_lock(&mutex);//release lock
         t++;
        // printf("%d S %d releases lock_3 and lock_1.\n",t/4,number);
         number++;
         lock[3]=0;
         lock[1]=0;
         cars[2]--;
         holdlock[3]=0;
         s=0;
        pthread_mutex_unlock(&mutex);//end release lock
       sleep(1);
      } 
    }
  while(cars[2]==0){
     pthread_mutex_lock(&mutex);
     if(cars[1]>0 || cars[0]>0 || cars[3]>0){
       t++;
       pthread_mutex_unlock(&mutex);
     }  
     else{
       pthread_mutex_unlock(&mutex);  
       break;
     }
    sleep(1); 
   }
pthread_exit(NULL);
}

void* Wcross(void* threadid){
   int number=0;
   while(cars[3]>0){
    if(w==0){   
    pthread_mutex_lock(&mutex); // get 1st lock  
      t++;
      if(lock[2]==0){
        lock[2]=1;
        w++;
       // printf("%d W %d acquire lock_2.\n",t/4,number);
      }else{
       // printf("%d W %d acquire lock_2(fail)\n",t/4,number);
      }    
    pthread_mutex_unlock(&mutex);//end get 1st
    sleep(1);
    }
    if(w==1){
     pthread_mutex_lock(&mutex);//get 2nd lock
       t++;
       if(lock[3]==0){
       // printf("%d W %d acquire lock_3.\n",t/4,number);
        lock[3]=1;
        w++;
       }else{
        holdlock[2]=1;
       // printf("%d W %d acquire lock_3(fail)\n",t/4,number);
        if(holdlock[0]==1 && holdlock[1]==1 &&  holdlock[2]==1 && holdlock[3]==1){
         printf("A DEADLOCK HAPPENS at %d\n",t/4);
        // printf("%d W %d releases lock_2\n",t/4,number);
         lock[2]=0;
         holdlock[2]=0;
         w=0;

        }
       }
     pthread_mutex_unlock(&mutex);//end get 2nd
    sleep(1);
    }  

       if(w==2){
        pthread_mutex_lock(&mutex);// cross road
         t++;
         printf("W %d leave at %d\n",number,t/4);
        pthread_mutex_unlock(&mutex);// end cross
        sleep(1);
       }
    
      if(w==2){
        pthread_mutex_lock(&mutex);//release lock
        t++;
         //printf("%d W %d releases lock_2 and lock_3.\n",t/4,number);
         number++;
         lock[2]=0;
         lock[3]=0;
         holdlock[2]=0;
         cars[3]--;
         w=0;
        pthread_mutex_unlock(&mutex);//end release lock
       sleep(1);
      } 
         }
   while(cars[3]==0){
     pthread_mutex_lock(&mutex);
     if(cars[1]>0 || cars[2]>0 || cars[0]>0){
       t++;
       pthread_mutex_unlock(&mutex);
     }  
     else{
       pthread_mutex_unlock(&mutex);  
       break;
     }      
    sleep(1); 
   }
 pthread_exit(NULL);
}



int main(int argc,char *argv[]){
 if(argc<5) return 0;
 
 int i;
 for(i=0;i<4;i++) cars[i]=atoi(argv[i+1]);
 for(i=0;i<4;i++) {
   lock[i]=0;
   holdlock[i]=0;
 }
 pthread_mutex_init(&mutex,NULL);
 pthread_t tid[4];


 pthread_create(&tid[0],NULL,Ncross,0);
 pthread_create(&tid[1],NULL,Ecross,0);
 pthread_create(&tid[2],NULL,Scross,0);
 pthread_create(&tid[3],NULL,Wcross,0);



 for(i=0;i<4;i++)
    pthread_join(tid[i],NULL);

return 0;
}
