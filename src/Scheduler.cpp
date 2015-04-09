#include "main.hpp"
#include <iostream>

Scheduler::Scheduler(){
}

Scheduler::~Scheduler(){
}

void Scheduler::run(){
	if (heap.isEmpty()){
		return;
	}

	float timePassed = heap.peek()->ai->waitTime;	//how much time has passed for this actor to take a turn
	if (timePassed > 0) {
		for (int i = 0; i < heap.size(); ++i){	//iterate through the heap once and decrease their wait time by how much time has passed
			heap.peek(i)->ai->waitTime -= timePassed; //when update is called, this value (waitTime) needs to be increased or it will loop at the first actor forever
		}
	} //I iterate through the heap once here to modify wait time, instead of doing it below, because this ensures that an actor only lowers its wait time one time for one action, without having to store a (list of) bool(s) somewhere

	//update all actors who have waitTime <= 0, as they have waited long enough
	TCODList<Actor *> updatedActors;

	Actor *actor = heap.peek();
	while (actor && actor->ai->waitTime <= 0) {
		updatedActors.push(actor);
		heap.pop();
		actor->update();
		actor->updateAuras();
		actor = heap.peek();
	}
	std::cout << "here" << std::endl;
	addFromList(updatedActors);
	updatedActors.clear();

}

void Scheduler::add(Actor *actor){
	heap.push(actor);
}

void Scheduler::addFromList(TCODList<Actor *> &list){
	//this list is always added from within Scheduler::run() for now
	//either way, since dead Actors can't update, make sure they aren't dead befor they are added to the scheduler

	for (Actor **iterator = list.begin();
		iterator != list.end(); iterator++) {

		Actor *actor = *iterator;
		if (actor->destructible && !actor->destructible->isDead()){
			heap.push(actor);
		}
	}
}

void Scheduler::clear(){
	heap.clear();
}
