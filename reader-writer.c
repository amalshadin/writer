#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
sem_t mutex1,mutex2,r_sem,wrt;
int data=1,readcount=0,writecount=0;
void* reader(void *arg){
    int id=*(int *)arg;
    sem_wait(&r_sem);
    sem_wait(&mutex1);
    readcount++;
    if(readcount==1) sem_wait(&wrt);
    sem_post(&mutex1);
    sem_post(&r_sem);
    printf("Reader %d is READING. Data: %d (Active Readers: %d)\n", id, data, readcount);
    sleep(1);           
    sem_wait(&mutex1);
    readcount--;
    if (readcount == 0) sem_post(&wrt); 
    sem_post(&mutex1);     
    return NULL;
}
void* writer(void *arg){
    int id=*(int*)arg;
    sem_wait(&mutex2);
    writecount++;
    if(writecount==1) sem_wait(&r_sem);
    sem_post(&mutex2);
    sem_wait(&wrt);
    data*=2;
    printf("Writer %d is WRITING. New Data: %d\n", id, data);
    sleep(1);
    sem_post(&wrt);       
    sem_wait(&mutex2);    
    writecount--;
    if (writecount == 0) sem_post(&r_sem);  
    sem_post(&mutex2);    
    return NULL;
}
int main() {
    pthread_t r[5], w[5];
    int ids[5] = {1, 2, 3, 4, 5};
    sem_init(&r_sem, 0, 1);
    sem_init(&mutex1, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&wrt, 0, 1);
    for(int i = 0; i < 5; i++) {
        pthread_create(&r[i], NULL, reader, &ids[i]);
        pthread_create(&w[i], NULL, writer, &ids[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(r[i], NULL);
        pthread_join(w[i], NULL);
    }
    sem_destroy(&r_sem);
    sem_destroy(&mutex1);
    sem_destroy(&mutex2);
    sem_destroy(&wrt);
    return 0;
}