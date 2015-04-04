#include <math.h>
#include "main.hpp"

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col, bool hostile, int xpLevel):
	ID(0),x(x),y(y),ch(ch),trueCh(ch),col(col),trueCol(col),hostile(hostile),
	blocks(true),xpLevel(xpLevel),attacker(NULL),destructible(NULL),ai(NULL),
	pickable(NULL),container(NULL),caster(NULL){
	this->name = strdup(name);
}

Actor::Actor(const Actor &actor):
	ID(0),x(actor.x),y(actor.y),ch(actor.ch),col(actor.col),hostile(actor.hostile),
	blocks(actor.blocks),attacker(NULL),destructible(NULL),ai(actor.ai),pickable(NULL),container(NULL),caster(NULL){
	
	attacker = new Attacker(*actor.attacker);
	destructible = new MonsterDestructible(actor.destructible->maxHp,actor.destructible->baseDodge,actor.destructible->corpseName,0);
	container = new Container(actor.container->size);
	this->name = strdup(name);
}

Actor::~Actor(){
	delete(name);
	if (attacker) delete attacker;
	if (destructible) delete destructible;
	if (ai) delete (ai);
	if (pickable) delete pickable;
	if (container) delete container;
	auras.clearAndDelete();
}

void Actor::save(TCODZip &zip) {
	zip.putInt(ID);
	zip.putInt(x);
	zip.putInt(y);
	zip.putInt(ch);
	zip.putInt(trueCh);
	zip.putColor(&col);
	zip.putColor(&trueCol);
	zip.putString(name);
	zip.putInt(blocks);
	zip.putInt(hostile);
	zip.putInt(xpLevel);
	
	zip.putInt(auras.size());
	for (Aura **it = auras.begin(); it!= auras.end(); it++) {
		(*it)->save(zip);
	}
		
	zip.putInt(attacker != NULL);
	zip.putInt(destructible != NULL);
	zip.putInt(ai != NULL);
	zip.putInt(pickable != NULL);
	zip.putInt(container != NULL);
	zip.putInt(caster != NULL);
	
	if (attacker) attacker->save(zip);
	if (destructible) destructible->save(zip);
	if (ai) ai->save(zip);
	if (pickable) pickable->save(zip);
	if (container) container->save(zip);
	if (caster) caster->save(zip);
}

void Actor::load(TCODZip &zip) {
	ID = zip.getInt();
	x = zip.getInt();
	y = zip.getInt();
	ch = zip.getInt();
	trueCh = zip.getInt();
	col = zip.getColor();
	trueCol = zip.getColor();
	name = strdup(zip.getString());
	blocks = zip.getInt();
	hostile = zip.getInt();
	xpLevel = zip.getInt();
	
	int nbAuras = zip.getInt();
	while (nbAuras > 0) {
		Aura *aura = Aura::create(zip);
		auras.push(aura);
		nbAuras--;
	}
	
	bool hasAttacker = zip.getInt();
	bool hasDestructible = zip.getInt();
	bool hasAi = zip.getInt();
	bool hasPickable = zip.getInt();
	bool hasContainer = zip.getInt();
	bool hasCaster = zip.getInt();
	
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
	if (hasCaster) {
		caster = new Caster(0.0f);
		caster->load(zip);
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
			aura->apply(this);
			aura->duration--;

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