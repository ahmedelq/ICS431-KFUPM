/*
Ahmed Alelg - 201507470
*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_COUNT_VAL 1000

typedef struct {
    unsigned int data;
    sem_t mutex;
    sem_t read; // producer checks if it has been read before writing 
    sem_t wrote; // consumer checks if it has been written before reading
} Buffer;

Buffer buf[2];


Buffer* init_buffer() {
    Buffer *buf = (Buffer *) malloc(sizeof(Buffer));
    sem_init(&buf->mutex, 0, 1);    
    sem_init(&buf->read,  0, 1);
    sem_init(&buf->wrote, 0, 0);
    return buf;
}

void buf_write(Buffer* buf, unsigned int val, int pr){
   // printf("[%d] Trying to write\n", pr);
    sem_wait(&buf->read);
   // printf("[%d] No one is reading\n", pr);
    sem_wait(&buf->mutex);
    //printf("[%d] Mutex is free\n", pr);
        buf->data = val;
    //printf("[%d] wrote %u\n",pr, val);
    sem_post(&buf->mutex);
    //printf("[%d] closed mutex\n", pr);
    sem_post(&buf->wrote);
    //printf("[%d] Closed writing\n\n", pr);

}

unsigned int buf_read(Buffer* buf) {
    unsigned int val;
    //printf("\nTrying to read\n");
    sem_wait(&buf->wrote);
   // printf("No one is writing\n");
    sem_wait(&buf->mutex);
    //printf("Mutex is free\n");
        val = buf->data;
    //printf("Read %u\n", val);
    sem_post(&buf->mutex);
    //printf("Mutex closed\n");
    sem_post(&buf->read);
    //printf("Read closed\n\n");
    return val;
}

/* p1 */
void* begin_count(void * args){
    for(unsigned int i = 1; i<MAX_COUNT_VAL+1; i++){
        buf_write(&buf[0], i, 1);
    }
    buf_write(&buf[0], 0, 1);
    return NULL;
}

/* p2 */
void* div_two(void * args){
    for(;;) {

        unsigned int tmp = buf_read(&buf[0]);
        if( (tmp % 2) != 0 ){
            buf_write(&buf[1], tmp, 2);
        }
        
        if(tmp == 0){
                buf_write(&buf[1], tmp, 2);
                return NULL;
                break;
        }
    }
}


/* p3 */
void* div_three(void * args){
    for(;;){  
        unsigned int tmp = buf_read(&buf[1]);
        if(tmp == 0){
                return NULL;
                break;
        }
        if( (tmp % 3) != 0 ) {
            printf("%u\n", tmp);
        }
            
    }
}

int main () {
    buf[0] = *init_buffer();
    buf[1] = *init_buffer();
    pthread_t p1, p2, p3;

    pthread_create(&p1, NULL, begin_count, NULL);
    pthread_create(&p2, NULL, div_two, NULL);
    pthread_create(&p3, NULL, div_three, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);

    return 0;
    
}