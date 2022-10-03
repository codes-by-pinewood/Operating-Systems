#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
extern char** environ;

void printenv(){
   char **env = environ;
   for (; *env ;env++){
       printf("%s\n", *env);
   }
}


void func_handler(int argc, char* argv[], char* command){
  pid_t pid;
  pid = fork();
  if (pid == -1)
    {
        printf("error in forking");
        //continue;
    }
  if (pid == 0){
    if (strcmp(command, argv[1]) == 0){
      execvp(command, &argv[1]); 
      fprintf(stderr, "%s :execvp: %s\n", argv[0], strerror(errno));
      _exit(EXIT_FAILURE);
    }
     else {
            execvp(command, &argv[0]);
            fprintf(stderr, "%s :execvp: %s\n", argv[0], strerror(errno));
            _exit(EXIT_FAILURE);
        }
  }
  }



void get_trace(int argc, char *argv[], int uflag, int vflag, char* cvalue){ 
    if (vflag == 1){
    
      if (argc == 2 && uflag == 0){
         // fprintf(stderr, "#env get-trace:      %s", argv[argc-1]);
      }
      
      else if (argc > 2 && uflag == 0){
            printf("setenv:    %s\n", argv[argc-1]);
      } 
      
      else if (argc > 2 && uflag == 1){
            fprintf(stderr, "unset:    %s\n", cvalue);
      }
      
      else {
            fprintf(stderr, "unknown arguments:    %d", optopt);
        } 
    }
}

int main(int argc, char **argv){
  int uflag = 0;
  int vflag = 0;
  char *cvalue = NULL;
  int opt = 0;
  char command[100];
  //int opterr = 0;
  
  while ((opt = getopt (argc, argv, "u:v")) != -1){
    switch (opt){
        case 'u':
            uflag = 1;
            cvalue = optarg;
            unsetenv(cvalue);
            break;
        case 'v':
            vflag = 1;
           // get_trace(argc, argv, uflag, vflag);
            break;
        case '?':
            if (optopt == 'u'){
              fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            else if (optopt != 'u' && optopt != 'v'){
            fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            }
            else
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return 1;
        default:
            abort ();
}
}
  

printf ("uflag = %d, vflag = %d, cvalue = %s\n", uflag, vflag, cvalue);

if (vflag == 1){
  get_trace(argc, argv, uflag, vflag, cvalue);
} 

//check if its a "name=value" pair
for (int i = optind; i < argc; i++){
   if(strchr(argv[i], '=') != NULL){
      putenv(argv[i]);
    
  } else {
   strcpy(command, argv[i]);
   func_handler(argc, argv, command);
    
}
}
 printenv();

 
  return 0;
}
