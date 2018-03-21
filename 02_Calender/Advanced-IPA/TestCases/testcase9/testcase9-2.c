
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex_lock1;
pthread_mutex_t mutex_lock2;

int SharedResource1 = 1;
int SharedResource2 = 1;


void task1()
{

	pthread_mutex_lock(&mutex_lock1);

	SharedResource1++;

	pthread_mutex_unlock(&mutex_lock1);


	pthread_mutex_lock(&mutex_lock2);

	SharedResource2++;

	pthread_mutex_unlock(&mutex_lock2);
	pthread_mutex_unlock(&mutex_lock2);



};


void task2()
{


	SharedResource2++;

	pthread_mutex_unlock(&mutex_lock2);
	
	pthread_mutex_lock(&mutex_lock1);

	SharedResource1++;


};



int main()
{
	
	int* ptr = malloc(sizeof(int));
		
	pthread_t p_thread1;
	pthread_t p_thread2;

	int state;

	pthread_mutex_init(&mutex_lock1, NULL);
	pthread_mutex_init(&mutex_lock2, NULL);
	
	pthread_create(&p_thread1, NULL, task1, NULL);
	pthread_create(&p_thread2, NULL, task2, NULL);

	pthread_join(&p_thread1, (void** )&state);
	pthread_join(&p_thread2, (void** )&state);
	

	return 0;
}
