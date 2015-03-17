#include <math.h>
#include <stdio.h>
#include "main.hpp"

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col):
	ID(0),x(x),y(y),ch(ch),col(col),
	blocks(true),attacker(NULL),destructible(NULL),ai(NULL),
	pickable(NULL),container(NULL){
	this->name = strdup(name);
}

Actor::~Actor(){
	delete(name);
	if (attacker) delete attacker;
	if (destructible) delete destructible;
	if (ai) delete (ai);
	if (pickable) delete pickable;
	if (container) delete container;
}

void Actor::save(TCODZip &zip) {
	zip.putInt(ID);
	zip.putInt(x);
	zip.putInt(y);
	zip.putInt(ch);
	zip.putColor(&col);
	zip.putString(name);
	zip.putInt(blocks);
	
	zip.putInt(auras.size());
	for (Aura **it = auras.begin(); it!= auras.end(); it++) {
		(*it)->save(zip);
	}
		
	zip.putInt(attacker != NULL);
	zip.putInt(destructible != NULL);
	zip.putInt(ai != NULL);
	zip.putInt(pickable != NULL);
	zip.putInt(container != NULL);
	
	if (attacker) attacker->save(zip);
	if (destructible) destructible->save(zip);
	if (ai) ai->save(zip);
	if (pickable) pickable->save(zip);
	if (container) container->save(zip);
}

void Actor::load(TCODZip &zip) {
	ID = zip.getInt();
	x = zip.getInt();
	y = zip.getInt();
	ch = zip.getInt();
	col = zip.getColor();
	name = strdup(zip.getString());
	blocks = zip.getInt();
	
	int nbAuras = zip.getInt();
	while (nbAuras > 0) {
		Aura *aura = new Aura();
		aura->load(zip);
		auras.push(aura);
		nbAuras--;
	}
	
	bool hasAttacker = zip.getInt();
	bool hasDestructible = zip.getInt();
	bool hasAi = zip.getInt();
	bool hasPickable = zip.getInt();
	bool hasContainer = zip.getInt();
	
	if (hasAttacker) {
		attacker = new Attacker(0.0f);
		attacker->load(zip);
	}
	
	if (hasDestructible) {
		destructible = Destructible::create(zip);
	}
	
	if (hasAi) {
		ai = Ai::create(zip);
	}
	
	if (hasPickable) {
		pickable = Pickable::create(zip);
	}
	
	if (hasContainer) {
		container = new Container(0);
		container->load(zip);
	}
}

void Actor::render() const {
	if (engine.map->isInFov(x,y)) {
		engine.mapcon->setChar(x,y,ch);
		engine.mapcon->setCharForeground(x,y,col);
	} else if (engine.map->isExplored(x,y)) {
		//need to create a lastX and lastY for actors
		//display their remembered locations, so they can move
		//while out of FoV without being seen
		engine.mapcon->setChar(x,y,ch);
		engine.mapcon->setCharForeground(x,y,TCODColor::lightGrey);
	}
}

void Actor::update(){
	if(ai) ai->update(this);
}

float Actor::getDistance(int cx, int cy) const {
	int dx = x-cx;
	int dy = y-cy;
	return sqrtf(dx*dx+dy*dy);
}

bool Actor::isVisible(){
	return engine.map->isInFov(x,y);
}

void Actor::updateAuras() {
	if (!this->auras.isEmpty()) {
		for (Aura **iter = this->auras.begin(); iter != this->auras.end(); iter++) {
			Aura *aura = *iter;
			aura->duration--;
			if (aura->life == Aura::ITERABLE) {
				aura->apply(this);
			}
			if (aura->duration == 0) {
				aura->unApply(this);
				delete *iter;
				iter = this->auras.remove(iter);
			}
		}
	}
}

const char *Actor::getName(bool real) {
	if (real == true) {
		return this->name;
	} else {
		if (this->destructible && this->destructible->isDead()) {
			return this->destructible->corpseName;
		} else {
			return this->name;
		}
	}
}

void Actor::setName(const char *name) {
	this->name = name;
}