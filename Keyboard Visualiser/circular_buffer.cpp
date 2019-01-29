#include "circular_buffer.h"
#include <stdexcept>


CircularBuffer::CircularBuffer(int size) {
	buffer = new float[size];
	head = 0;
	tail = 0;
	isFull = false;
}

CircularBuffer::~CircularBuffer() {
	delete buffer;
}

void CircularBuffer::push(float& val) {
	increment_head();
	buffer[head] = val;
}

float CircularBuffer::pop() {
	increment_tail();
	if (head == tail && !isFull) {
		// Buffer is Empty
		return NAN;
	} else {
		return buffer[tail];
	}
}

// PRIVATE	

void CircularBuffer::increment_head() {
	if (isFull) {
		tail = wrap(tail);
	}

	head = wrap(head);

	if (head == tail) {
		// Head has caught up with tail so the buffer must be full.
		isFull = true;
	}
}

void CircularBuffer::increment_tail() {

	if (head == tail && !isFull) {
		// Buffer must be empty, Don't increment tail.
		return;
	} else if (isFull) {
		// Tail is moving away from head, so buffer is no londer full.
		isFull = false;
	} 
	tail = wrap(tail);
}

int CircularBuffer::wrap(int i) {
	return (i + 1) % size;
}