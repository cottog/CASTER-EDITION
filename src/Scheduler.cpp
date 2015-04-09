#include "main.hpp"

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
		for (int i = 0; i < heap.size(); ++i){
			heap.peek(i)->ai->waitTime -= timePassed; //when update is called, this value (waitTime) needs to be increased or it will loop at the first actor forever
		}
	}

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

	addFromList(updatedActors);

}

void Scheduler::add(Actor *actor){
	heap.push(actor);
}

void Scheduler::addFromList(TCODList<Actor *> list){
	for (Actor **iterator = list.begin();
		iterator != list.end(); iterator++) {

		Actor *actor = *iterator;
		heap.push(actor);
	}
}

void Scheduler::clear(){
	heap.clear();
}
