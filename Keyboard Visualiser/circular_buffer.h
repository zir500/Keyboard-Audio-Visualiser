#pragma once

/* A FIFO Circular Buffer of float values.
	If the buffer is full, value at the back of the queue will begin to overwrite those at the front; However the front of the queue will advance so that the oldest value in the buffer will be the first to be popes.
*/
class CircularBuffer {
	float* buffer;
	int size;
	int head; // Index of the head element in the buffer
	int tail; // Index of the tail element in the buffer
	bool isFull; // Indicated by the head catching up with the tail.  Cleared when the tail moves aaway from the head.

public:
	CircularBuffer(int size);

	~CircularBuffer();

	/** Adds a value to the back of the buffer */
	void push(float& val);

	/** Pops the next value in the buffer */
	float pop();


private:

	/** Advances Head, If the queue is full, the Tail will be advanced along with Head */
	void increment_head();


	/** Tries to advance Tail. If the Queue is empty Tail will not be advanced */
	void increment_tail();

	/** Wraps an integer around the size of the buffer */
	int wrap(int i);
};