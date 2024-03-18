INCLUDE_PATH=-I /home/zzy0/vim_workspace/thread_poll_c/include
LIB_PATH=-L /home/zzy0/vim_workspace/thread_poll_c/lib

lib1=-l thread_pool
lib2=-l queue

all:test

test:test_thread_pool.c
	gcc -g -o test test_thread_pool.c $(INCLUDE_PATH) $(LIB_PATH) $(lib1) $(lib2) -l pthread

clean:
	rm -rf test
