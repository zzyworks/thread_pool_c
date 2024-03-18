/***********************************************************
  > File Name: thread_pool.h
  > Author: Aklzzz
  > Mail: 2429605732@qq.com
  > Created Time: Wed 13 Mar 2024 08:27:56 PM CST
  > Modified Time:Wed 13 Mar 2024 08:27:56 PM CST
 *******************************************************/

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

#include "queue.h"

#define NUMBER 2

// 创建线程池并初始化
ThreadPool* initPool(TypeSize min, TypeSize max, TypeSize size);

// 销毁线程池，失败返回0，错误返回-1，成功返回1
int destroyPool(ThreadPool* pool);

// 给线程池添加任务
void pushTask(ThreadPool* pool, Task* task); 

// 获取线程池中工作线程的个数
int busyThread(ThreadPool* pool);

// 获取线程中存存活线程的个数
int aliveThread(ThreadPool* pool);

// 工作的线程（消费者线程）任务函数
void* worker(void* arg);

// 管理者线程任务函数
void* manager(void* arg);

// 单个线程退出
void exitThread(ThreadPool* pool);

#endif // THREAD_POOL_H
