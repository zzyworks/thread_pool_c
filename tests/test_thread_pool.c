#include "thread_pool.h"

void* taskFunc(void* arg) {
	printf("thread %ld is working, number = %d\n", pthread_self(), 
																								 (int)(long)arg);
	sleep(1);
	return (void*)0;
}

int main(int argc, char* argv[]) {
	ThreadPool* pool = initPool(3, 10, 100);
	for(int i = 0; i != 100; ++i) {
		Task* task = (Task*)malloc(sizeof(Task));
		int* x = (int*)malloc(sizeof(int));
		*x = i + 100;
		task->function = taskFunc;
		task->arg = x;
		pushTask(pool, task);
	}
	sleep(30);

	if(destroyPool(pool) != 1) {
		perror("destroyPool failed.\n");
		exit(-1);
	} else {
		printf("succeed\n");
	}
	return 0;
}
