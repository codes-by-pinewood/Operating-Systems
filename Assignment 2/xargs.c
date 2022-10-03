#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
# include <string.h>

//handles non option arguments such as -t and -n
void arg_handler(char* cmd_exec, int argc, char* argv[], int tflag, int iter);

void arg_handler(char* cmd_exec, int argc, char* argv[], int tflag, int iter){

pid_t pid;
int status;

if (tflag == 1){ 
  fprintf(stderr,"echo ");
  for (int i = 1; i <= iter; i++){
    fprintf(stderr,"%s ", argv[i]);
  }
  fprintf(stderr, "\n");
  execvp(cmd_exec, &argv[0]);
  }

  pid = fork();
  
if (pid == 0) {


  int statuscode = execvp(cmd_exec, &argv[0]);
  if (statuscode == -1){
    fprintf(stderr, "execvp call failed");
  }
  exit(0);
} else if (pid > 0) {
 if( waitpid(pid, &status, 0) == -1){ //wait for child process
    fprintf(stderr, "wait call failed");
  }
  
} else {
  /* handle failure to fork */
}
//return 0;
}


int main(int argc, char* argv[]){
  char cmd_exec[100] = "/bin/echo"; 
  
  int nflag = 0;  
  int tflag = 0;
  int n = 0;
  int opt = 0;
  char *buffer;
  size_t buf_size = 0;
  
  //handles option arguments
  while ((opt =getopt(argc, argv, "tn:")) != -1){
    switch(opt){
      case 't': 
        tflag = 1;
        break;
      case 'n':
        nflag = 1;
        n = atoi(optarg);
        break;
      case '?': 
        if (optopt == 'n'){
          fprintf(stderr, "Option -%c requires an argument", optopt);
        } else if (optopt != 'n' && optopt != 't'){ //unknown option
            fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            exit(1);
        }
          else {
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        }
        break;
      default: 
        abort();
    }
  }
  
  buffer = (char *)malloc(buf_size * sizeof(char));
  if (buffer == NULL){
        perror("unable to allocate buffer");
	exit(1);
  }
 
  int iter = 0; 
  
 //read output of command from stdin
   while (getline(&buffer, &buf_size, stdin) != -1){
      int len = strlen(buffer);   
      if (buffer[len - 1] == '\n'){
          buffer[len - 1] = '\0';
      }
     argv[iter+1] = strdup(buffer);
     
      iter++;

  } 
  argv[iter+1] = NULL;
  
  strcpy(cmd_exec,"/bin/echo");

  arg_handler(cmd_exec, argc, argv, tflag, iter);

}
