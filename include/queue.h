/***********************************************************
  > File Name: queue.h
  > Author: Aklzzz
  > Mail: 2429605732@qq.com
  > Created Time: Sun 17 Mar 2024 05:31:50 PM CST
  > Modified Time:Sun 17 Mar 2024 05:31:50 PM CST
 *******************************************************/

// 任务循环队列

#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h> // for int32_t
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc and free
#include <string.h> // for memeset
#include <unistd.h> // for sleep
#include <assert.h>

typedef struct Queue Queue;
typedef int32_t TypeSize;

// 任务结构体
typedef struct Task {
	void*(*function)(void* arg);
	void* arg;
} Task;

// 线程池结构体
typedef struct ThreadPool {
	Queue* que_; // 阻塞队列

	pthread_t manager_id_; // 管理者线程ID
	pthread_t* threads_id_; // 工作线程ID
	TypeSize min_num_; // 最小线程数量
	TypeSize max_num_; // 最大线程数量
	TypeSize busy_num_; // 忙的线程数量
	TypeSize alive_num_; // 存活线程数量
	TypeSize exit_num_; // 如果空闲线程多，则需要销毁的数量
	pthread_mutex_t mutex_pool_; // 锁整个线程池
	pthread_mutex_t mutex_busy_; // 锁busy_num_变量
	pthread_cond_t is_full_; // 任务队列是否满
	pthread_cond_t is_empty_; // 任务队列是否为空

	TypeSize destroy_; // 是否需要销毁线程池，销毁为1，不销毁为0
} ThreadPool;

typedef struct Queue {
	Task** arr_; // 队列
	TypeSize front_; // 队头
	TypeSize back_; // 队尾
	TypeSize size_; // 元素个数
	TypeSize capacity_; // 容量
} Queue;

// 初始化n字节的容器
Queue* initQueue(TypeSize n);

// 销毁容器,错误返回-1，成功返回1，内部还有元素返回0
int destroyQueue(Queue* que);

// 插入元素，错误返回-1，失败返回0，成功返回1
int push(Queue* que, Task* x);

// 删除元素，错误返回-1，失败返回0，成功返回1
int pop(Queue* que);

TypeSize size(Queue* que);

TypeSize capacity(Queue* que);

Task* front(Queue* que);

Task* back(Queue* que);

#define Queue() static_assert(false, "create null container");

#endif // QUEUE_H
