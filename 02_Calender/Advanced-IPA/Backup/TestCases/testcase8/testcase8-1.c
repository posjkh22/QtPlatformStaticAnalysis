#include <stdlib.h>
#include <pthread.h>

char charGlobal1;
int IntegerGlobalVariable = 3;
double DoubleGlobalVariable= 3.5;
int *IntegerPointerGV;
char charGlobal2;


void t1_fnc1(int *ptr1){

	static int var_t1_func1 = 2;
	free(ptr1);
	var_t1_func1 = 3;
}


void t2_fnc1(int *ptr1, int *ptr2){
	
	static int var_t2_func1 = 2;

	 free(ptr1);
	 free(ptr2);

	var_t2_func1 = 3;
}

int *shared_func2(int *ptr1, int *ptr2){
	DoubleGlobalVariable= 3.7;
	return ptr1;
}

int shared_func1()
{
	int a = 1;

}

void task1(void *ptr){

	int *ptr1 = (int *)malloc(sizeof(int));
	int *ptr2 = (int *)malloc(sizeof(int));
	int *ret;

	shared_func1();

	IntegerGlobalVariable = 1;

	int a = 0;
	if(a == 0){
		/* ptr2 not free */
		t1_fnc1(ptr1);

	}else{

		/* no error */
		ret = shared_func2(ptr1, ptr2);
		free(ret);
		free(ptr2);
	}
}

void task2(void *ptr){

	int *ptr1 = (int *)malloc(sizeof(int));
	int *ptr2 = (int *)malloc(sizeof(int));
	int *ret;

	shared_func1();
	
	int a = 0;
	if( a == 0){

		/* double free ptr2 */
		ret = ptr2;
		free(ret);
		ret = shared_func2(ptr1, ptr2);
		t2_fnc1(ptr1, ptr2);

	}else{
	
		/* double free ptr1 */
		free(ptr1);
		free(ptr2);
		free(ptr2);
	}	

}


int main(){

	int status;
	int thread_id1;
	int thread_id2;	

	int a = 0;
	int b = 1;

	pthread_t thread[2];
	
	thread_id1 = pthread_create(&thread[0], NULL, task1, (void*)&a);
	thread_id2 = pthread_create(&thread[1], NULL, task2, (void*)&b);

	pthread_join(thread[0], &status);
	pthread_join(thread[1], &status);


	return 0;
}

