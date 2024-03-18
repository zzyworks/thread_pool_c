#include "queue.h"

Queue*
initQueue(TypeSize n) {
  Queue* que = (Queue*)malloc(sizeof(Queue));
  do {
    if (!que) {
      printf("Queue init failed.\n");
      break;
    }
    que->arr_ = (Task**)malloc(n);
    que->capacity_ = n / sizeof(Task());
    que->size_ = 0;
    que->front_ = 0;
    que->back_ = que->capacity_ - 1;  // 当加入元素时，先移动，在插入
    memset(que->arr_, 0, n);
    
		return que;

  } while (0);
  if (que->arr_) {
    free(que->arr_);
    que->arr_ = NULL;
  }
  if (que) {
    free(que);
    que = NULL;
  }
  return NULL;
}

int
destroyQueue(Queue* que) {
  if (!que) return -1;
	while(que->size_ != 0) pop(que);
	if(que->size_ == 0) {
		free(que->arr_);
		que->arr_ = NULL;
		free(que);
		que = NULL;
		return 1;
	}
	return 0;
}

int
push(Queue* que, Task* x) {
	// 容器创建失败 || 传入元素为空
  if (!que || !x)
    return -1;
  else if (que->size_ == que->capacity_)
    return 0;
	// 当加入元素时，先移动，在插入
  que->back_ = (que->back_ + 1) % que->capacity_;
  que->arr_[que->back_] = x;
  ++que->size_;
  return 1;
}

int
pop(Queue* que) {
  if (!que)
    return -1;
  else if (que->size_ == 0)
    return 0;
	Task* task = que->arr_[que->front_];
	if(task) {
		free(task->arg);
		task->arg = NULL;
		task->function = NULL;
		free(task);
		task = NULL;
	}
  que->front_ = (que->front_ + 1) % que->capacity_;
  --que->size_;
  return 1;
}

TypeSize
size(Queue* que) {
  return que->size_;
}

TypeSize
capacity(Queue* que) {
  return que->capacity_;
}

Task*
front(Queue* que) {
  if (!que || !que->arr_ || que->size_ == 0)
    return NULL;
  return que->arr_[que->front_];
}

Task*
back(Queue* que) {
  if (!que || !que->arr_ || que->size_ == 0)
    return NULL;
  return que->arr_[que->back_];
}
