#include "thread_pool.h"

ThreadPool*
initPool(TypeSize min, TypeSize max, TypeSize size) {
  ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
  do {
    if (!pool) {
      printf("malloc ThreadPool failed.\n");
			break;
    }
    pool->threads_id_ = (pthread_t*)malloc(sizeof(pthread_t) * max);
    if (!pool->threads_id_) {
      printf("malloc threads_id failed.\n");
			break;
    }
    memset(pool->threads_id_, 0, sizeof(pthread_t) * max);
    pool->min_num_ = min;
    pool->max_num_ = max;
    pool->busy_num_ = 0;
    pool->alive_num_ = min;
    pool->exit_num_ = 0;
    if (pthread_mutex_init(&pool->mutex_pool_, NULL) != 0 ||
        pthread_mutex_init(&pool->mutex_busy_, NULL) != 0 ||
        pthread_cond_init(&pool->is_empty_, NULL) != 0 ||
        pthread_cond_init(&pool->is_full_, NULL) != 0) {
      printf("mutex or condition init failed.\n");
      break;
    }
    // 任务队列
		pool->que_ = initQueue(sizeof(Task) * size);

    pool->destroy_ = 0;

    // 创建线程
    pthread_create(&pool->manager_id_, NULL, manager, pool);
    for (int i = 0; i != min; ++i)
      pthread_create(&pool->threads_id_[i], NULL, worker, pool);

    return pool;
  } while (0);
  if (pool && pool->threads_id_) {
    free(pool->threads_id_);
    pool->threads_id_ = NULL;
  }
  if (pool && pool->que_) {
		destroyQueue(pool->que_);
		pool->que_ = NULL;
	}
  if (pool) {
    free(pool);
    pool = NULL;
  }
  return NULL;
}

// 销毁线程池
int
destroyPool(ThreadPool* pool) {
	printf("destroyPool begin\n");
  if (!pool)
    return -1;
  pool->destroy_ = 1;
  pthread_join(pool->manager_id_, NULL); // 管理者线程等待
  for (int i = 0; i != pool->alive_num_; ++i)
    pthread_cond_signal(&pool->is_empty_);
	if(destroyQueue(pool->que_) == 0) {
		printf("the queue has elements.\n");
		return 0;
	}
	if(pool->threads_id_) {
		free(pool->threads_id_);
		pool->threads_id_ = NULL;
	}
  pthread_mutex_destroy(&pool->mutex_pool_);
  pthread_mutex_destroy(&pool->mutex_busy_);
  pthread_cond_destroy(&pool->is_empty_);
  pthread_cond_destroy(&pool->is_full_);
  if (pool) {
    free(pool);
    pool = NULL;
  }
  return 1;
}

// 给线程池添加任务
void
pushTask(ThreadPool* pool, Task* task) {
	pthread_mutex_lock(&pool->mutex_pool_);
	while(size(pool->que_) == capacity(pool->que_) && !pool->destroy_)
		pthread_cond_wait(&pool->is_full_, &pool->mutex_pool_);
	if(pool->destroy_) {
		pthread_mutex_unlock(&pool->mutex_pool_);
		return;
	}
	push(pool->que_, task);
	pthread_cond_signal(&pool->is_empty_);
	pthread_mutex_unlock(&pool->mutex_pool_);
}

// 获取线程池中工作线程的个数
TypeSize 
busyThread(ThreadPool* pool) {
	pthread_mutex_lock(&pool->mutex_busy_);
	TypeSize busy_num = pool->busy_num_;
	pthread_mutex_unlock(&pool->mutex_busy_);
	return busy_num;
}

// 获取线程中存存活线程的个数
int 
aliveThread(ThreadPool* pool) {
	pthread_mutex_lock(&pool->mutex_pool_);
	TypeSize alive_num = pool->alive_num_;
	pthread_mutex_unlock(&pool->mutex_pool_);
	return alive_num;
}

// 工作的线程（消费者线程）任务函数
void* 
worker(void* arg) {
	ThreadPool* pool = (ThreadPool*)arg;
	while(1) {
		pthread_mutex_lock(&pool->mutex_pool_);
		// 如果线程池还没有被销毁，且当前任务队列为空（没有任务），则阻塞消费者
		while(size(pool->que_) == 0 && !pool->destroy_) {
			pthread_cond_wait(&pool->is_empty_, &pool->mutex_pool_); // 阻塞工作线程
			if(pool->exit_num_ > 0) { // 判断是不是要销毁线程
				--pool->exit_num_;
				if(pool->alive_num_ > pool->min_num_) {
					--pool->alive_num_;
					pthread_mutex_unlock(&pool->mutex_pool_);
					exitThread(pool);
				}
			}
		}
		if(pool->destroy_) { // 判断线程池是否被关闭了
			pthread_mutex_unlock(&pool->mutex_pool_);
			exitThread(pool);
		}
		// 从任务队列中取出一个任务
		Task* task = front(pool->que_);

		pthread_mutex_unlock(&pool->mutex_pool_);
		pthread_cond_signal(&pool->is_full_);

		printf("thread %ld start work.\n", pthread_self());
		pthread_mutex_lock(&pool->mutex_busy_);
		++pool->busy_num_;
		pthread_mutex_unlock(&pool->mutex_busy_);

		task->function(task->arg); // 执行任务

		pop(pool->que_); // 删除队头元素

		printf("thread %ld stop work.\n", pthread_self());
		pthread_mutex_lock(&pool->mutex_busy_);
		--pool->busy_num_;
		pthread_mutex_unlock(&pool->mutex_busy_);
	}
	return NULL;
}

// 管理者线程任务函数
void* 
manager(void* arg) {
	ThreadPool* pool = (ThreadPool*)arg;
	while (!pool->destroy_) {
		sleep(3); // 每隔3s检查一次
		// 取出线程池中任务的数量和当前线程的数量
		// 取出忙的线程的数量
		pthread_mutex_lock(&pool->mutex_pool_);
		TypeSize que_size = size(pool->que_);
		TypeSize alive_num = pool->alive_num_;
		TypeSize busy_num = pool->busy_num_;
		pthread_mutex_unlock(&pool->mutex_pool_);
		// 创建线程
		// 任务的个数>存活的线程个数 && 存活的线程个数<最大个数
		if(que_size > alive_num && alive_num < pool->max_num_) {
			pthread_mutex_lock(&pool->mutex_pool_);
			int cnt = 0;
			for(int i = 0;
					i != pool->max_num_ && cnt < NUMBER && pool->alive_num_;
					++i) {
				if(pool->threads_id_[i] == 0) {
					pthread_create(&pool->threads_id_[i], NULL, worker, pool);
					++cnt;
					++pool->alive_num_;
				}
			}
			pthread_mutex_unlock(&pool->mutex_pool_);
		}
		// 销毁线程
		// 忙的线程*2<存活的线程数 && 存活的线程>最小线程数
		if(busy_num * 2 < alive_num && alive_num > pool->min_num_) {
			pthread_mutex_lock(&pool->mutex_busy_);
			pool->exit_num_ = NUMBER;
			pthread_mutex_unlock(&pool->mutex_busy_);
			// 让工作中线程自杀
			for(int i = 0; i != NUMBER; ++i) 
				pthread_cond_signal(&pool->is_empty_);
		}
	}
	return NULL;
}

// 单个线程退出
void 
exitThread(ThreadPool* pool) {
	pthread_t tid = pthread_self();
	for(int i = 0; i != pool->max_num_; ++i) {
		if(pool->threads_id_[i] == tid) {
			pool->threads_id_[i] = 0;
			printf("threadExit called, %ld exiting\n", tid);
			break;
		}
	}
	pthread_exit(NULL);
}
