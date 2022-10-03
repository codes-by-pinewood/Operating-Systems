#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>

void get_trace(){
  printf("print trace");
}

static int is_perfect(uint64_t num, int e);
void* routine(void* arg);


int main(int argc, char* argv[]) {  
    //clock_t start_t, end_t;
    //double total_t;
    
    int opt = 0;
    //default values of s and e
    int svalue = 1;
    int evalue = 10000;
    int vflag = 0;
    int tflag = 0;
    int t = 0;
    
    //option arguments
    while ((opt = getopt (argc, argv, "s:e:t:v")) != -1){
    switch (opt){
        case 's':
            //sflag = 1;
            svalue = atoi(optarg);
            break;
        case 'e':
            //eflag = 1;
            evalue = atoi(optarg);
            break;
        case 't':
            tflag = 1;
            t = atoi(optarg);
            if (t < 0){
              fprintf(stderr, "Invalid argument %d to t\n", t);
            }
            break;
        case 'v':
            vflag = 1;
           // get_trace();
            break;
        case '?':
            if (optopt == 's' || optopt == 'e' || optopt == 't'){
              fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            else if (optopt != 's' && optopt != 'e' && optopt != 't' && optopt != 'v'){
            fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            }
            else
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return 1;
        default:
            abort ();
  }
  }
  
 //start_t = clock();
    
  // if tflag is 0 
  if (tflag == 0 && t == 0){
    
  int* b = malloc(sizeof(int)* 2);
  b[0] = svalue;
  b[1] = evalue;
  
  routine(b);
  
}
  //if tflag is 1
  else if (tflag == 1 && t > 0){
  
    pthread_t th[t];
    int step = floor(evalue/t);
   
    for (int i = 0; i < t; i++) {
    int* a = malloc(sizeof(int)* 3);
    int j = i +1;
    if (i == 0){
        a[0] = svalue;
    } else {
        a[0] = i * step + 1;
    }
    a[1] = (j)*step;
    a[2] = vflag;
         
    if (pthread_create(&th[i], NULL, &routine,(void *)a) != 0) {
        perror("Failed to created thread");
    }
    
    }
    
    for (int i = 0; i < t; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
}
 // end_t = clock();
 // total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
 // printf("Total time taken by CPU: %f\n", total_t);
    
    return 0;
}

static int is_perfect(uint64_t num, int e){
if (num <= e){
  uint64_t i, sum;
  if (num < 2) {  
    return 0;
  }
  
  for (i = 2, sum = 1; i*i <= num; i++) {
    if (num % i == 0) {
      sum += (i*i == num) ? i : i + num / i;
    }
  }
  return (sum == num);
  } 
  return 0; 
}


void* routine(void* arg) {
    
    sleep(1);
  
    int *val_p = (int *) arg;
    int start = val_p[0];
    int stop = val_p[1];
    
    int vflag = val_p[2];
    
    if (vflag == 1){
     printf("perfect: t%ld searching [%d, %d]\n", pthread_self(), start, stop);
     }
  
    if (start >= 0 && stop >start){
    for (int i = start; i < stop; i++){
      if (is_perfect(i, stop) == 1){
     
        printf("%d\n", i);
       
      }
    
    }
  
  }
  return NULL;
    free(arg);
   
}
