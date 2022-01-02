#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5 
#define C 3 

struct args {

};
typedef struct args Args;

pthread_t consumers[C];
pthread_t producers[C];

int store[N];
int head=0;
int tail=0;
int i=0;
int good=0;
sem_t semWait;
sem_t semFull;
sem_t semEmpty;

void* produce(void *arguments);
void* consume(void *arguments);

int main(){

	sem_init(&semWait,0,1);
	sem_init(&semFull,0,N);
	sem_init(&semEmpty,0,0);
	srand(1);

	for(int i=0;i<C;i++){
		if(pthread_create(&consumers[i],NULL,&consume,NULL)!=0){
			printf("Error creating thread.");
			exit(1);
		}
	}

	for(int i=0;i<C;i++){
		if(pthread_create(&producers[i],NULL,&produce,NULL)!=0){
			printf("Error creating thread.");
			exit(1);
		}
	}

	for(int i=0;i<(C);i++){
		pthread_join(consumers[i],NULL);

	}

	for(int i=0;i<(C);i++){
		pthread_join(producers[i],NULL);
	}

	printf("Program terminated successfully.\n");
	
	return 0;
}

void* produce(void* arguments){
	
	//if(pthread_self()==producers[i]){
	//printf("id=%ld\n",pthread_self());
	int counter=0;
	while(counter<N){
		sem_wait(&semFull);
		sem_wait(&semWait);
		good=rand();
		store[tail]=good;
		printf("Producer id=%ld ProducedGood=%d SavedGood=%d tail=%d\n",pthread_self(),good,store[tail],tail);
		tail+=1;
		tail=(tail%N);
		sem_post(&semEmpty);
		sem_post(&semWait);
		counter+=1;
	}

	//}

	return NULL;
}	

void* consume(void* arguments){

	int counter=0;
	while(counter<N){
		sem_wait(&semEmpty);
		sem_wait(&semWait);
		good=store[head];
		printf("Consumer id=%ld ConsumedGood=%d RemovedGood=%d head=%d\n",pthread_self(),good,store[head],head);
		store[head]=0;
		head+=1;
		head=head%N;
		sem_post(&semFull);
		sem_post(&semWait);
		counter+=1;
	}

	return NULL;
}	
