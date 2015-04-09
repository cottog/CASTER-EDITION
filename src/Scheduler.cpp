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

	TCODList<Actor *> updatedActors;

	Actor *actor = heap.peek();
	while (actor && actor->speedy->speed >= 1) {
		heap.pop();
		actor->update();
		actor->updateAuras();
		actor = heap.peek();
		actor->speedy->speed -= 1;
		if (actor->speedy->speed < 1) {
			actor->speedy->speed = actor->speedy->maxSpeed;
			updatedActors.push(actor);
		} else {
			heap.push(actor);
		}
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
