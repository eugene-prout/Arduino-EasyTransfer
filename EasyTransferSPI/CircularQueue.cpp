#include "CircularQueue.h"

CircularQueue::CircularQueue(int _size)
{
  size = _size;
  arr = (uint8_t **) malloc(size * sizeof(uint8_t*));
  front = 0;
  rear = 0;
}

CircularQueue::~CircularQueue() 
{
  free(arr);
}

void CircularQueue::enqueue(uint8_t *element) 
{
  if ((front +1) % size == rear)
  {
    // Queue is full => Move rear pointer along
    rear = (rear + 1) % size;
  }

  arr[front] = element;
  front = (front + 1) % size;
}

uint8_t* CircularQueue::dequeue()
{
  if (isEmpty())
  {
    return NULL;
  }
  
  uint8_t *result = arr[rear];
  rear = (rear + 1) % size;
  return result;
}

bool CircularQueue::isEmpty()
{
  return rear == front;
}
