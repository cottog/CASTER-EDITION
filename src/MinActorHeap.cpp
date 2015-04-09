#include "main.hpp"
#include <vector>

MinActorHeap::MinActorHeap(){
}

MinActorHeap::~MinActorHeap(){
	_vector.clear(); //we only clear the vector because we know that the actor pointers will be deleted by Engine::term()
}

void MinActorHeap::Heapify() {
	int length = _vector.size();
	for (int i = length-1; i>=0; --i){
		BubbleDown(i);
	}
}

void MinActorHeap::BubbleDown(int index){
	
	int length = _vector.size();
	int leftChildIndex = 2*index+1;
	int rightChildIndex = 2*index+2;

	if (leftChildIndex >= length){
		return;	//no children, it is already a leaf
	}

	int minIndex = index;

	if (_vector[index]->ai->waitTime > _vector[leftChildIndex]->ai->waitTime){ //left child is smaller
		minIndex = leftChildIndex;	
	}

	if ((rightChildIndex < length) && (_vector[minIndex]->ai->waitTime > _vector[rightChildIndex]->ai->waitTime)) {	//right child is smallest
		minIndex = rightChildIndex;
	}

	if (minIndex != index){	//need to bubble down, as a child is smaller than parent
		Actor *temp = _vector[index];
		_vector[index] = _vector[minIndex]; //smaller child is new parent
		_vector[minIndex] = temp; //larger parent is now child
		BubbleDown(minIndex); //see if the larger parent needs to go lower
	}
}

void MinActorHeap::BubbleUp(int index){
	if (index == 0){
		return; //its lonely at the top
	}

	int parentIndex = (int)((index-1)/2); //cast to an int just to make sure

	if (_vector[parentIndex]->ai->waitTime > _vector[index]->ai->waitTime){ //the parent is bigger than the child
		Actor *temp = _vector[parentIndex];
		_vector[parentIndex] = _vector[index]; //the smaller child is the new parent
		_vector[index] = temp; //the larger parent is the new child
		BubbleUp(parentIndex); //see if the smaller child needs to go higher
	}
}

void MinActorHeap::push(Actor *newActor){
	if (newActor->ai){
		_vector.push_back(newActor);

		int length = _vector.size();
		BubbleUp(length-1);
	} else {
		return;
	}
}

Actor *MinActorHeap::peek(int i){
	if (_vector.size() == 0){
		return NULL;
	}
	return _vector[i];
}

Actor *MinActorHeap::pop(){
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

void MinActorHeap::clear(){
	int length = _vector.size();
	for (int i = length-1; i>= 0; --i){
		delete _vector[i];
	}
	_vector.clear();	
}