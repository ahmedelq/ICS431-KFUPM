#include<stdio.h>
#include <pthread.h>
#include<stdint.h>
#include <unistd.h>
#include <stdlib.h>

static int bridge = 0; 
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void* farmerFunc(void *arg) {
    int s;
    int turn = *((int *) arg);

    

    s = pthread_mutex_lock(&lock);
    sleep( rand() % 3);
    bridge ++;
    s = pthread_mutex_unlock(&lock);
    if(turn % 2)
        printf("Farmer from the south crossed the bridge: %d\n", bridge);
    else
        printf("Farmer from the north crossed the bridge: %d\n", bridge);



    return NULL;
}

int main() {
   int total_farmers = 10;
   pthread_t farmers[10];

   for (int i=0; i<10; i+=1) { 
    int *p1 = malloc(sizeof(*p1));
    *p1 = i;
    pthread_create(&farmers[i], NULL, farmerFunc, p1 );
   }

   for (int i = 0; i<10; i+=1)
    pthread_join(farmers[i], NULL);

   printf("Total farmers: %d\nTotal farmers passed: %d\n", total_farmers, bridge);
    return 0;
}
