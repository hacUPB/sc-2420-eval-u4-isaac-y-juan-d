/*Shared resources in a process*/
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define ITERATIONS 100


/*Declare a global variable as shared resource*/
int shared = 0;
void *function(void *args)
{
for(int i=0; i<ITERATIONS; i++)
{
	shared++;
}
return NULL;
}
int main(int argc, char const *argv[])
{
pthread_t thr1, thr2;
if(pthread_create(&thr1,NULL,&function,NULL) != 0)
{
return 1;
}
if(pthread_create(&thr2,NULL,&function,NULL) != 0)
{
return 2;
}
if(pthread_join(thr1,NULL) != 0)
{
return 3;
}
if(pthread_join(thr2,NULL) != 0)
{
return 4;
}
printf("Shared variable value: %d\n",shared);
return 0;
}