#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include<iostream>
#include<vector>
#include<set>
using namespace std;

set<int> process;
int fg_pid=0;

int mysh_cd(string path) {
   if(chdir(path.c_str()))
     cout<<"\e[0;32m"<<"cd "<<path<<": No such directory\n";  
}

int mysh_fg(int spid) {
     int status;
     fg_pid=spid;

     kill(-spid, SIGTSTP);
     tcsetpgrp(STDIN_FILENO, spid);  
     kill(-spid,SIGCONT);
     int waited= waitpid(spid,&status,WUNTRACED);
     if(WIFEXITED(status)) process.erase(waited);
     tcsetpgrp(STDIN_FILENO, getpid());  
}

int mysh_bg(int spid) {
     kill(-spid,SIGCONT);
}

int mysh_kill(int spid){
   kill(spid,SIGCONT);
   kill(spid, SIGINT);
   process.erase(spid);
}

void  mysh_exit() {
    for(set<int>::iterator it = process.begin() ; it != process.end() ; ++it){
        kill(*it, SIGINT);
        process.erase(*it);
    }   

    cout <<"\e[0;32m"<< "Goodbye!" <<"\e[1;37m"<< endl; 
   exit(0);
}

vector <vector<string> > parse_command(string str){
   vector  < vector<string> > command;
   vector<string> a_command;
     a_command.push_back("");
       
     for(int i=0;i<(int)str.size();i++){
           if(str[i]=='|'){
            if(a_command.back().size()==0) a_command.pop_back();

             command.push_back(a_command);
             a_command.clear();
             a_command.push_back("");
           }else if(str[i]=='&'){
             if(a_command.back().size()!=0)  
                a_command.push_back("&");
             else
                a_command.back()="&";
           }else if(str[i]==' '){
              if(a_command.back().size()!=0) 
                 a_command.push_back("");
           }else{ 
              a_command.back().push_back(str[i]);
           }

          if(i==(int)str.size()-1){
            if(a_command.back().size()==0) a_command.pop_back();
             command.push_back(a_command);
             a_command.clear();
          }
       }   
   
  return command;
}

void execute_extrnal(vector< string > a_command){
   char **args;
   int size=a_command.size()+1;
   args = new char*[size];
   for(int i=0;i<size;i++){
     if(i!=size-1){ 
        args[i] = new char[a_command[i].size()];
        strcpy(args[i],a_command[i].c_str());
     }else{
        args[i]=0;
      }  
   }

   if(execvp(args[0],args)){
     cout<<"\e[0;32m"<<args[0]<<": command not found"<<"\e[1;37m"<<endl;
     exit(1);
   }  
   for(int i=0;i<size-1;i++)
        delete [] args[i];
    delete [] args; 

}

void execute_command(string str){
  vector < vector <string> > command = parse_command(str);
  bool background = false;
  if(command.size()==0) return;
  if(command[0][0]=="cd") {
    if(command[0].size()>1)
      mysh_cd(command[0][1]);  
  }else if(command[0][0]=="fg"){   mysh_fg(atoi(command[0][1].c_str()));
  }else if(command[0][0]=="bg"){    mysh_bg(atoi(command[0][1].c_str()));
  }else if(command[0][0]=="kill"){  mysh_kill(atoi(command[0][1].c_str()));
  }else if(command[0][0]=="exit"){  mysh_exit(); 
  }else{
    int pipes=command.size()-1;
    int pd[100][2];
    int head_pid=0;
    if(command.back().back()=="&"){ 
        background=true; 
        command.back().pop_back();
    }
    for(int i=0;i<pipes;i++) pipe(pd[i]);
    for(int i=0;i<(int)command.size();i++){
      int pid=fork();
       if(pid==0){
          if(i==0){
            if(background)  cout<<"\e[0;32m"<<"Command excuted by pid="<<getpid()<<" in background"<<"\e[1;37m"<<endl;
            else  cout<<"\e[0;32m"<<"Command excuted by pid="<<getpid()<<"\e[1;37m"<<endl;
          }
          if(pipes){
            if(i!=pipes) dup2(pd[i][1],1);
            if(i!=0)     dup2(pd[i-1][0],0);
          }   
          for(int j=0;j<pipes;j++){
             close(pd[j][0]);
             close(pd[j][1]);
           }
         execute_extrnal(command[i]);
         tcsetpgrp(STDIN_FILENO, head_pid);
       }else{
          setpgid(pid,head_pid);
          process.insert(pid);
         if(head_pid == 0) head_pid=pid;
         if(!background)fg_pid=head_pid;
         else fg_pid=0;
       }
    }
     
      for(int j=0;j<pipes;j++){
             close(pd[j][0]);
             close(pd[j][1]);
      }
     int status;
      if(!background)
      for(int i=0;i<(int)command.size();i++){
       int waited = waitpid(-head_pid,&status,WUNTRACED);
          if(WIFEXITED(status)) process.erase(waited);
      }
          
      tcsetpgrp(STDIN_FILENO, getpid());
  }
}

void mysh_promt(){
  char user[1024];
  char path[1024];
    getcwd(path, 1024);
    getlogin_r(user, 1024);
 
  cout<<"\e[0;36m"<<string(user);
  cout<<"\e[1;37m"<<" in ";
  cout<<"\e[1;33m"<<string(path)<<endl;
  cout<<"\e[1;30m"<<"mysh> "<<"\e[1;37m";

}

void ctrl_c_handler(int sig){
     if(fg_pid){
       kill(-fg_pid, SIGINT);
       process.erase(fg_pid);
       fg_pid=0;
       cout<<endl;
     }  
}

void ctrl_z_handler(int sig){
 if(fg_pid){  
   kill(-fg_pid,SIGTSTP);  
   tcsetpgrp(STDIN_FILENO, getpid());
   fg_pid=0;
   cout<<endl;
 }
}
void zombie_handler(int sig){
  int status;
  int waited = waitpid(-1,&status,WNOHANG); 
  process.erase(waited);
}

void mysh_loop(){
   string str;
   mysh_promt();
  while(getline(cin,str)){
     execute_command(str); 
     mysh_promt();
  }
}

void mysh_init(){
 signal(SIGINT,ctrl_c_handler);
 signal(SIGTSTP,ctrl_z_handler);
 signal(SIGCHLD,zombie_handler);
 signal(SIGTTOU,SIG_IGN);
 signal(SIGTTIN,SIG_IGN);
}

int main(){
cout << "Welcome to mysh by 0312209!" << endl;
mysh_init();
mysh_loop();

return 0;
}

