#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

// pthread_t tid[100];
int NUM_PRISONERS = 100;
int counter, count, iter, val;
pthread_mutex_t lock;
int found[100] = {0};
int drawers_global_arr[100];
int cnt_success = 0;

void shuffle(int n);
static double timeit(int n, void *(*proc)(void *));

static void run_threads(int n, void *(*proc)(void *));

void *randomGlobal(void *arg);
void *strategyGlobal(void *arg);
void *randomLocal(void *arg);
void *strategyLocal(void *arg);

int main(int argc, char *argv[])
{
    srand(time(0));
    int opt = 0;
    int nval = 100;
   // int seedvalue;
   // int sflag =1;

    while ((opt = getopt(argc, argv, "s:n:")) != -1)
    {
        switch (opt)
        {
        case 's':
            // sflag = 1;
            // seedvalue = atoi(optarg);
            break;
        case 'n':

            nval = atoi(optarg);
            break;

        case '?':
            if (optopt == 's' || optopt == 'n')
            {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            else if (optopt != 's' && optopt != 'n')
            {
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            }
            else
            {
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            }
        default:
            abort();
        }
    }
    
   /* if (sflag == 1){
      srand(time(seedvalue));
    } */
    
    int *success = malloc(sizeof(int) * nval);

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }


    pthread_mutex_destroy(&lock);

    /* RANDOM GLOBAL */
    int win_count = 0;
    double time1 = timeit(nval, randomGlobal);

    for (int i = 0; i < nval; i++)
    {

        win_count += success[i];
    }
    
    printf("method random_global %d/%d wins = %f%% %f ms\n", cnt_success, nval, 100.0 * cnt_success / nval, time1);
    
    /* RANDOM LOCAL */
    win_count = 0;
    double time2 = timeit(nval, randomLocal);

    for (int i = 0; i < nval; i++)
    {

        win_count += success[i];
    }
    
    printf("method random_local %d/%d wins = %f%% %f ms\n", cnt_success, nval, 100.0 * cnt_success / nval, time2);
    

  
    /* STRATEGY GLOBAL */
    win_count = 0;
    double time3 = timeit(nval, strategyGlobal);
    for (int i = 0; i < nval; i++)
    {
        win_count += success[i];
    }
    printf("method strategy_global %d/%d wins = %f%% %f ms\n", cnt_success, nval, 100.0 * cnt_success / nval, time3);
    
    /* STRATEGY LOCAL */
    win_count = 0;
    double time4 = timeit(nval, strategyLocal);
    for (int i = 0; i < nval; i++)
    {
        win_count += success[i];
    }
    printf("method strategy_local %d/%d wins = %f%% %f ms\n", cnt_success, nval, 100.0 * cnt_success / nval, time4);
    


    return 0;
}

void shuffle(int n)
{

    size_t i;
    //srand(time(NULL));
    for (i = n - 1; i > 0; i--)
    {
        size_t j = rand() % (i + 1);
        int t = drawers_global_arr[j];
        drawers_global_arr[j] = drawers_global_arr[i];
        drawers_global_arr[i] = t;
    }
}

void *randomGlobal(void *arg)
{
    int *prisonerNumber = (int *)arg;
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 50; i++)
    {
        if (*prisonerNumber == drawers_global_arr[rand() % 100])
        {
            found[*prisonerNumber] = 1;
        }     
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

void *strategyGlobal(void *arg)
{
    int *prisonerNumber = (int *)arg;
    pthread_mutex_lock(&lock);
    int currentDrawer = *prisonerNumber;
    for (int i = 0; i < 50; i++)
    {
        if (*prisonerNumber == drawers_global_arr[currentDrawer] - 1)
        {
            /* success */
            found[*prisonerNumber] = 1;
        }
        else
        {
            currentDrawer = drawers_global_arr[currentDrawer] - 1;
        }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}


void *randomLocal(void *arg)
{
    
    int *prisonerNumber = (int *)arg;
    
    for (int i = 0; i < 50; i++)
    {
    pthread_mutex_lock(&lock);
        if (*prisonerNumber == drawers_global_arr[rand() % 100])
        {
            found[*prisonerNumber] = 1;
        }  
    pthread_mutex_unlock(&lock);
    }

    

    return NULL;
}

// some problems with this one!
void *strategyLocal(void *arg)
{
    
    int *prisonerNumber = (int *)arg;
    pthread_mutex_lock(&lock);
    int currentDrawer = *prisonerNumber;
    for (int i = 0; i < 50; i++)
    {
      pthread_mutex_lock(&lock);
        if (*prisonerNumber == drawers_global_arr[currentDrawer] - 1)
        {
            /* success */
            found[*prisonerNumber] = 1;
        }
        else
        {
            currentDrawer = drawers_global_arr[currentDrawer] - 1;
        }
    pthread_mutex_unlock(&lock);
    }
    
    return NULL;
}

static void run_threads(int n, void *(*proc)(void *))
{

    // drawers initialised
    for (int i = 0; i < 100; i++)
    {
        drawers_global_arr[i] = i;
    }
    //srand(time(0));
    shuffle(100);

    for (int i = 0; i < 100; i++)
    {
        drawers_global_arr[i] = drawers_global_arr[i] + 1;
       // printf("%d ", drawers_global_arr[i]);
    }

    pthread_t tid[NUM_PRISONERS];
    int status;

    // for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < 100; i++)
        {
            found[i] = 0;
        }
        shuffle(100);
        for (int i = 0; i < 100; i++)
        {
            int *prisonerNumber = malloc(sizeof(int));
            *prisonerNumber = i;
            status = pthread_create(&tid[i], NULL, proc, &(*prisonerNumber));
            if (status)
            {
                printf("Invalid thread creation: %d", status);
            }
        }
        for (int i = 0; i < 100; i++)
        {
            if (tid[i])
            {
                pthread_join(tid[i], NULL);
            }
        }
    }
    int flag = 1;
    for (int i = 0; i < 100; i++)
    {
        if (found[i] == 0)
        {
            flag = 0;
            break;
        }
    }
    if (flag == 1)
    {
        cnt_success++;
       // printf("success\n");
    }
    else
    {
       //printf("failure\n");
    }
    for (int i = 0; i < 100; i++)
    {
        //printf("%d ", found[i]);
    }
}

static double timeit(int n, void *(*proc)(void *))
{
    clock_t t1, t2;
    cnt_success = 0;
    t1 = clock();
    for (int i = 0; i < 100; i++)
        run_threads(n, proc);
    t2 = clock();
    return ((double)t2 - (double)t1) / CLOCKS_PER_SEC * 1000;
}
