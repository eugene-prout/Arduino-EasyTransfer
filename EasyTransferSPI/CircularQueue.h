#ifndef CircularQueue_h
#define CircularQueue_h

// Not neccessary, but just in case.
#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class CircularQueue
{
public:
    CircularQueue(int size);
    ~CircularQueue();

    void enqueue(uint8_t *);
    uint8_t* dequeue();
    bool isEmpty();

private:
    int size;
    uint8_t **arr;
    int front;
    int rear;
};

#endif
