#include <stdio.h>
#include "main.hpp"

Destructible::Destructible(float maxHp, float dodge, const char *corpseName, int xp, float DR, float maxShield, Spell::ElementalSubtype element) : 
	maxHp(maxHp),hp(maxHp),maxShield(maxShield),shield(maxShield),baseDodge(dodge),totalDodge(dodge),
	baseDR(DR),totalDR(DR),xp(xp),element(element){
	this->corpseName = strdup(corpseName);
}

Destructible::~Destructible() {
	free(corpseName);
}

void Destructible::save(TCODZip &zip) {
	zip.putFloat(maxHp);
	zip.putFloat(hp);
	zip.putFloat(maxShield);
	zip.putFloat(shield);
	zip.putFloat(baseDodge);
	zip.putFloat(totalDodge);
	zip.putFloat(baseDR);
	zip.putFloat(totalDR);
	zip.putString(corpseName);
	zip.putInt(xp);
	zip.putInt(element);
}

void Destructible::load(TCODZip &zip) {
	maxHp = zip.getFloat();
	hp = zip.getFloat();
	maxShield = zip.getFloat();
	shield = zip.getFloat();
	baseDodge = zip.getFloat();
	totalDodge = zip.getFloat();
	baseDR = zip.getFloat();
	totalDR = zip.getFloat();
	corpseName = strdup(zip.getString());
	xp = zip.getInt();
	element = (Spell::ElementalSubtype)zip.getInt();
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
	if (attacker && owner->attacker && (owner->attacker->lastTarget == NULL || owner->attacker->lastTarget->destructible->isDead())) {
		owner->attacker->lastTarget = attacker;
	}
	float damageTaken = 0;

	if (shield) {
		shield -= damage;
		damageTaken = damage;
		damage = 0; //assume that the shield reduce all the damage they could have taken
	}

	if (shield < 0 ) {	//the shield was too small to absorb all the damage
		damage -= shield;	//increase the damage left to take by the amount the shield couldnt absorb
		damageTaken += shield; //decrease the damageTaken counter by the amount the shield couldn't absorb;
		shield = 0;	//make sure the shield amount goes back to zero (it shouldnt be negative for real)
	}

	if (damage) {
		damage -= totalDR;
		if (damage > 0) {
			damageTaken += damage;
			hp -= damage;
			if (hp <= 0) {
				die(owner);
			}
		} else {
			damageTaken += 0;
		}
	}

	return damageTaken;
}

void Destructible::die(Actor *owner){
	//transform the actor into a corpse
	owner->ch = '%';
	owner->col = TCODColor::darkRed;
	//owner->setName(corpseName);
	owner->blocks = false;
	engine.map->tiles[owner->x+owner->y*engine.mapWidth].blocked=false;
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
	//transform it into a corpse. doesn't block, can't be hit, doesn't move
	engine.player->destructible->xp += xp;
	Destructible::die(owner);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName,float maxShield) :
	Destructible(maxHp,defense,corpseName,0,0, maxShield) {
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



