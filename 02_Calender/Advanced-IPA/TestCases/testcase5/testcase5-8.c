
/* Not free Error Case */

#include <stdlib.h>

int gv = 1;
int a = 0;


int *fnc1(int *ptr1){

	int *a = ptr1;
	return a;
}

int *fnc2(int *ptr2, int *ptr3){

	return ptr2;
}

void fnc3(int *ptr2, int *ptr3){

	int a = 0;
	free(ptr2);
	free(ptr3);

}

void c1(){
	int a = 0;
}
void c2(){
	int a = 0;
}
void c3(){
	int a = 0;
}
void c4(){
	int a = 0;
}
void c5(){
	int a = 0;
}

int main(){

	int *ptr1 = (int *)malloc(sizeof(int));
	int *ptr2 = (int *)malloc(sizeof(int));
	int *ptr3 = (int *)malloc(sizeof(int));

	int a = 0;
	int b = 1;
	int c = 2;
	int *returnPtr;

	if( a == 0){

		if(b == 0){

			/* Not free for ptr2, ptr3 */
			c1();
			returnPtr = fnc1(ptr1);
			free(returnPtr);
		}

		else{

			/* Double free for ptr3 */
			c2();
			returnPtr = fnc1(ptr1);
			free(returnPtr);
			fnc3(ptr2, ptr3);
			free(ptr3);
		}

	}
	else{
		if( b == 0){
			/* No error */
			c3();
			returnPtr = fnc2(ptr2, ptr3);	
			fnc3(returnPtr, ptr3);
			free(ptr1);
		}else{
		
			if(c == 2){
				/* double free for ptr2, Not free for ptr3 */
				c4();
				free(ptr1);
				free(ptr2);
				free(ptr2);
				// ptr2 is double freed
				// ptr3 is not free
			}
			else{
				/* not allocated memory free (returnPtr) */
				c5();
				free(returnPtr);
				free(ptr1);
				free(ptr2);
				free(ptr3);
			}
		}
	}
	return 0;

}
