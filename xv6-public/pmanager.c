#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "elf.h"
#include "stat.h"
#include "user.h"

char *argv[50];

int main() {
   char cmd[50];

   int pid, limit, stacksize;

   printf(1, "Process Manager\n");
   printf(1, "> ");
   memset(cmd, 0, sizeof(cmd));

   while (gets(cmd, sizeof(cmd))>=0) {
      

      if (cmd[0] == 'l' && cmd[1] == 'i' && cmd[2] == 's' && cmd[3] == 't') {
         
         list_processes();

    } else if (cmd[0] == 'k' && cmd[1] == 'i' && cmd[2] == 'l' && cmd[3] == 'l') {
         
         int i = 5;
         int j=0;
         char pids[100];

         while (cmd[i]!=' '&&cmd[i]!='\n'){
            pids[j]=cmd[i];
            i++;
            j++;
         }
         pid = atoi(pids);
      
         if (kill(pid) == 0) {
            printf(1, "Succeeded to kill!!\n");
         } else {
            printf(1, "Failed to kill!!\n");
         }
    } 
    else if (cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'e' && cmd[3] == 'c' && cmd[4] == 'u' && cmd[5] == 't' && cmd[6] == 'e') {

         int i=8;
         int j=0;
         char path[100]={0,};
         char stacksizes[50]={0,};

         while (cmd[i]!=' '){
            path[j]=cmd[i];
            i++;
            j++;
         }
         i++;
         j=0;

         argv[0] = path;
         argv[1] = 0;
         
         while (cmd[i]!=' ' && cmd[i]!='\n'){
            stacksizes[j]=cmd[i];
            i++;
            j++;
         }
         stacksize = atoi(stacksizes);

         pid = fork();

         if(pid == 0) {
            pid = fork();
            if(pid == 0) {
               exec2(argv[0], argv, stacksize);
               printf(1, "Failed to execute!!\n");
            } 
            else if(pid < 0) {
               printf(1, "Failed to execute!!\n");
            }
            exit();

         } 
         else if(pid > 0) {
            wait();
         } 
         else {
            printf(1, "Failed to execute!!\n");
         }
         
    } else if (cmd[0] == 'm' && cmd[1] == 'e' && cmd[2] == 'm' && cmd[3] == 'l' && cmd[4] == 'i' && cmd[5] == 'm') {
         
         int i = 7;
         int j=0;
         char pids[100];
         char limits[100];
         while (cmd[i]!=' '){
            pids[j]=cmd[i];
            i++;
            j++;
         }
         pid = atoi(pids);
         i++;
         j=0;

         while (cmd[i]!=' ' && cmd[i]!='\n'){
            limits[j]=cmd[i];
            i++;
            j++;
         }
         limit = atoi(limits);
         if (setmemorylimit(pid, limit) == 0) {
            printf(1, "Succeeded to set limit!!\n");
         } else {
            printf(1, "Failed to set limit!!\n");
         }
    } else if (cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't') {
         printf(1,"the end\n");
         exit();
    } else {
        printf(1, "No command\n");
    }
      memset(cmd, 0, sizeof(cmd));
      printf(1, "> ");

  }

  return 0;
}