#include "main.hpp"
#include <vector>

MaxActorHeap::MaxActorHeap(){
}

MaxActorHeap::~MaxActorHeap(){
	_vector.clear(); //we only clear the vector because we know that the actor pointers will be deleted by Engine::term()
}

void MaxActorHeap::Heapify() {
	int length = _vector.size();
	for (int i = length-1; i>=0; --i){
		BubbleDown(i);
	}
}

void MaxActorHeap::BubbleDown(int index){
	
	int length = _vector.size();
	int leftChildIndex = 2*index+1;
	int rightChildIndex = 2*index+2;

	if (leftChildIndex >= length){
		return;	//no children, it is already a leaf
	}

	int maxIndex = index;

	if (_vector[index]->speedy->speed < _vector[leftChildIndex]->speedy->speed){ //left child is speedier
		maxIndex = leftChildIndex;	
	}

	if ((rightChildIndex < length) && (_vector[maxIndex]->speedy->speed < _vector[rightChildIndex]->speedy->speed)) {	//right child is speediest
		maxIndex = rightChildIndex;
	}

	if (maxIndex != index){	//need to bubble down, as a child is speedier than parent
		Actor *temp = _vector[index];
		_vector[index] = _vector[maxIndex]; //speedier child is new parent
		_vector[maxIndex] = temp; //slower parent is now child
		BubbleDown(maxIndex); //see if the slower parent needs to go lower
	}
}

void MaxActorHeap::BubbleUp(int index){
	if (index == 0){
		return; //its lonely at the top
	}

	int parentIndex = (int)((index-1)/2); //cast to an int just to make sure

	if (_vector[parentIndex]->speedy->speed < _vector[index]->speedy->speed){ //the parent is slow than the child
		Actor *temp = _vector[parentIndex];
		_vector[parentIndex] = _vector[index]; //the faster child is the new parent
		_vector[index] = temp; //the slower parent is the new child
		BubbleUp(parentIndex); //see if the faster child needs to go higher
	}
}

void MaxActorHeap::push(Actor *newActor){
	if (newActor->speedy && newActor->ai){
		_vector.push_back(newActor);

		int length = _vector.size();
		BubbleUp(length-1);
	} else {
		return;
	}
}

Actor *MaxActorHeap::peek(int i){
	if (_vector.size() == 0){
		return NULL;
	}
	return _vector[i];
}

Actor *MaxActorHeap::pop(){
	int length = _vector.size();
	if (length == 0){
		return NULL;
	}

	Actor *actor = _vector[0]; //store the top Actor to return at the end
	_vector[0] = _vector[length-1]; //copy the last element to the top of the heap
	_vector.pop_back(); //remove the last element
	BubbleDown(0); //bubbledown the thing you just put at the root

	return actor; //return the actor that used to be at the root
}	

void MaxActorHeap::clear(){
	int length = _vector.size();
	for (int i = length-1; i>= 0; --i){
		delete _vector[i];
	}
	_vector.clear();	
}