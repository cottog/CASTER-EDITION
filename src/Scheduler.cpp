#include "main.hpp"

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
	int counter = 0;
	while (actor && actor->speedy->speed >= 1) {
		counter++;
		heap.pop();
		if (actor->destructible && !actor->destructible->isDead()) {
			if (actor == engine.player){
				engine.render();
    			TCODConsole::flush();
				while (engine.gameStatus == Engine::IDLE){
					TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&engine.lastKey,NULL,true);
					if (engine.lastKey.vk == TCODK_ESCAPE) {
						engine.save();
						engine.load(true);
					}
					if (engine.lastKey.vk == TCODK_ENTER && engine.lastKey.ralt) {
						engine.fullscreen();
					}
					actor->update();
				}
			} else {
				actor->update();
			}
			actor->updateAuras();
			actor->speedy->speed -= 1;
			if (actor->speedy->speed < 1.0f) {
				actor->speedy->speed += actor->speedy->maxSpeed; //an actor with speed of 1.5 takes two turns one every other round (3 turns/2 rounds)
				updatedActors.push(actor);
			} else {
				heap.push(actor);
			}
		}
		actor = heap.peek();
	}
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
