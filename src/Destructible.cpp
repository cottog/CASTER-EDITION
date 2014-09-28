#include <stdio.h>
#include "main.hpp"

Destructible::Destructible(float maxHp, float dodge, const char *corpseName, int xp, float DR) : 
	maxHp(maxHp),hp(maxHp),baseDodge(dodge),totalDodge(dodge),
	baseDR(DR),totalDR(DR),xp(xp){
	this->corpseName = strdup(corpseName);
}

Destructible::~Destructible() {
	free(corpseName);
}

void Destructible::save(TCODZip &zip) {
	zip.putFloat(maxHp);
	zip.putFloat(hp);
	zip.putFloat(baseDodge);
	zip.putFloat(totalDodge);
	zip.putFloat(baseDR);
	zip.putFloat(totalDR);
	zip.putString(corpseName);
	zip.putInt(xp);
}

void Destructible::load(TCODZip &zip) {
	maxHp = zip.getFloat();
	hp = zip.getFloat();
	baseDodge = zip.getFloat();
	totalDodge = zip.getFloat();
	baseDR = zip.getFloat();
	totalDR = zip.getFloat();
	corpseName = strdup(zip.getString());
	xp = zip.getInt();
}

Destructible *Destructible::create(TCODZip &zip) {
	DestructibleType type = (DestructibleType)zip.getInt();
	Destructible *destructible = NULL;
	switch(type) {
		case MONSTER: destructible = new MonsterDestructible(0,0,NULL,0); break;
		case PLAYER: destructible = new PlayerDestructible(0,0,NULL); break;
	}
	destructible->load(zip);
	return destructible;
}

float Destructible::takeDamage(Actor *owner, Actor *attacker, float damage){
	if (owner->attacker && (owner->attacker->lastTarget == NULL || owner->attacker->lastTarget->destructible->isDead())) {
		owner->attacker->lastTarget = attacker;
	}

	damage -= totalDR;
	if (damage > 0) {
		hp -= damage;
		if (hp <= 0) {
			die(owner);
		}
	} else {
		damage = 0;
	}
	return damage;
}

void Destructible::die(Actor *owner){
	//transform the actor into a corpse
	owner->ch = '%';
	owner->col = TCODColor::darkRed;
	//owner->setName(corpseName);
	owner->blocks = false;
	//make sure corpses are drawn before living actors
	engine.sendToBack(owner);
}

float Destructible::heal(float amount){
	hp+=amount;
	if (hp > maxHp) {
		amount -= hp-maxHp;
		hp = maxHp;
	}
	return amount;
}

MonsterDestructible::MonsterDestructible(float maxHp,float defense, const char *corpseName, int xp) :
	Destructible(maxHp,defense,corpseName,xp) {
}

void MonsterDestructible::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	Destructible::save(zip);
}

void MonsterDestructible::die(Actor *owner){
	//transform it into a nasty corpse. doesn't block, can't be hit, doesn't move
	engine.gui->message(TCODColor::white,"%s is dead!",owner->getName());
	engine.player->destructible->xp += xp;
	Destructible::die(owner);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp,defense,corpseName) {
}

void PlayerDestructible::save(TCODZip &zip) {
	zip.putInt(PLAYER);
	Destructible::save(zip);
}

void PlayerDestructible::die(Actor *owner){
	engine.gui->message(TCODColor::red,"You died!");
	Destructible::die(owner);
	engine.gameStatus=Engine::DEFEAT;
}



