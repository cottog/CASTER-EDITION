#include "main.hpp"

Container::Container(int size) : size(size), wallet(0),head(NULL),chest(NULL),
	legs(NULL),feet(NULL),hand1(NULL),hand2(NULL),gloves(NULL),ranged(NULL) {
}

Container::~Container() {
	inventory.clearAndDelete();
}

void Container::save(TCODZip &zip) {
	zip.putInt(wallet);
	zip.putInt(size);
	zip.putInt(inventory.size());
	for (Actor **it = inventory.begin(); it!=inventory.end(); it++) {
		(*it)->save(zip);
	}
	
	zip.putInt(head != NULL);
	zip.putInt(chest != NULL);
	zip.putInt(legs != NULL);
	zip.putInt(feet != NULL);
	zip.putInt(hand1 != NULL);
	zip.putInt(hand2 != NULL);
	zip.putInt(gloves != NULL);
	zip.putInt(ranged != NULL);
	if(head != NULL) head->save(zip);
	if(chest != NULL) chest->save(zip);
	if(legs != NULL) legs->save(zip);
	if(feet != NULL) feet->save(zip);
	if(hand1 != NULL) hand1->save(zip);
	if(hand2 != NULL) hand2->save(zip);
	if(gloves != NULL) gloves->save(zip);
	if(ranged != NULL) ranged->save(zip);
}

void Container::load(TCODZip &zip) {
	wallet = zip.getInt();
	size = zip.getInt();
	int nbActors = zip.getInt();
	while (nbActors > 0) {
		Actor *actor = new Actor(0,0,0,NULL,TCODColor::white);
		actor->load(zip);
		inventory.push(actor);
		nbActors--;
	}
	
	bool has_head = zip.getInt();
	bool has_chest = zip.getInt();
	bool has_legs = zip.getInt();
	bool has_feet = zip.getInt();
	bool has_hand1 = zip.getInt();
	bool has_hand2 = zip.getInt();
	bool has_gloves = zip.getInt();
	bool has_ranged = zip.getInt();
	if(has_head){
		head = new Actor(0,0,0,NULL,TCODColor::white);
		head->load(zip);
	}
	if(has_chest){
		chest = new Actor(0,0,0,NULL,TCODColor::white);
		chest->load(zip);
	}
	if(has_legs){
		legs = new Actor(0,0,0,NULL,TCODColor::white);
		legs->load(zip);
	}
	if(has_feet){
		feet = new Actor(0,0,0,NULL,TCODColor::white);
		feet->load(zip);
	}
	if(has_hand1){
		hand1 = new Actor(0,0,0,NULL,TCODColor::white);
		hand1->load(zip);
	}
	if(has_hand2){
		hand2 = new Actor(0,0,0,NULL,TCODColor::white);
		hand2->load(zip);
	}if(has_gloves){
		gloves = new Actor(0,0,0,NULL,TCODColor::white);
		gloves->load(zip);
	}
	if(has_ranged){
		ranged = new Actor(0,0,0,NULL,TCODColor::white);
		ranged->load(zip);
	}
}

bool Container::add(Actor *actor) {
	if (size > 0 && inventory.size() >= size) {
		return false;
	}
	
	bool wasIn = false;
	if (actor->pickable->stacks) {
		for (Actor **it = inventory.begin(); it != inventory.end(); it++) {
			Actor *act2 = *it;
			if (act2->ID == actor->ID) {
				wasIn = true;
				act2->pickable->stackSize += actor->pickable->stackSize;
				delete actor;
			}
		}
	}
	
	if (wasIn == false) {
		inventory.push(actor);
	}
	return true;
}

void Container::remove(Actor *actor){
	inventory.remove(actor);
}

void Container::sendToBegin(Actor *actor) {
	inventory.remove(actor);
	inventory.insertBefore(actor,0);
}
