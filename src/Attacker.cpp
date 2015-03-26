#include <stdio.h>
#include <iostream>
#include "main.hpp"

Attacker::Attacker(float power, float str, float dex) : 
	basePower(power), totalPower(power), baseStr(str), totalStr(str), baseDex(dex), totalDex(dex),
	lastTarget(NULL) {
}

Attacker::Attacker(const Attacker &attacker) : 
	basePower(attacker.basePower), totalPower(attacker.totalPower), baseStr(attacker.baseStr), totalStr(attacker.totalStr), baseDex(attacker.baseDex), totalDex(attacker.totalDex),
	lastTarget(NULL) {
}

void Attacker::save(TCODZip &zip) {
	zip.putFloat(basePower);
	zip.putFloat(totalPower);
	zip.putFloat(baseStr);
	zip.putFloat(totalStr);
	zip.putFloat(baseDex);
	zip.putFloat(totalDex);
}

void Attacker::load(TCODZip &zip) {
	basePower = zip.getFloat();
	totalPower = zip.getFloat();
	baseStr = zip.getFloat();
	totalStr = zip.getFloat();
	baseDex = zip.getFloat();
	totalDex = zip.getFloat();
}

/* void Attacker::attack(Actor *owner, Actor *target) {
	if (target->destructible && !target->destructible->isDead()){
		if (totalPower - target->destructible->totalDR > 0) {
			engine.gui->message(TCODColor::red,"%s attacks %s for %g hit points.",owner->name, target->name,
				totalPower - target->destructible->totalDR);
			target->destructible->takeDamage(target,owner,totalPower);
		} else {
			engine.gui->message(TCODColor::lightGrey,"%s attacks %s but it has no effect!",owner->name, target->name);
		}
	} else {
		engine.gui->message(TCODColor::lightGrey,"%s attacks %s in vain.",owner->name,target->name);
	}
	lastTarget = target;
} */

void Attacker::attack(Actor *owner, Actor *target) {
	if (target->destructible && !target->destructible->isDead()) {
		//roll the d20 to see if you hit
		TCODRandom *dicer = TCODRandom::getInstance();
		int roll1 = dicer->getInt(1,20);
		int attackRoll1 = roll1 + owner->attacker->totalStr;
		//roll 1d4 for standard unarmed damage first
		int diceNum = 1;
		int diceType = 4;
		float damage1 = (float) dicer->getInt(1,diceType);
		float critMult = 2;
		float critRange = 20;
		int hands = 1;

		lastTarget = target;

		if (owner->container && owner->container->hand1 != NULL) {
			
			diceNum = ((Weapon*)owner->container->hand1->pickable)->diceNum;
			diceType = ((Weapon*)owner->container->hand1->pickable)->diceType;
			damage1 = 0;
			for (int i = 0; i < diceNum; i++) {
				damage1 += (float)dicer->getInt(1,diceType);
				engine.gui->message(TCODColor::red,"%f",damage1);
			}
			critMult = ((Weapon*)owner->container->hand1->pickable)->critMult;
			critRange = ((Weapon*)owner->container->hand1->pickable)->critRange;
			if (((Weapon*)owner->container->hand1->pickable)->wType == Weapon::HEAVY) {
				hands = 2;
				engine.gui->message(TCODColor::white,"heavy hit!");
			}
		}
		
		if (roll1 >= critRange) {
			if (engine.map->isInFov(owner->x,owner->y)) {
				engine.gui->message(TCODColor::red,"%s MH CRITICAL HIT!",owner->getName());
			}
			for (int j = 0; j < critMult-1; j++) {
				for (int i = 0; i < diceNum; i++) {
					damage1 += dicer->getInt(1,diceType);
					engine.gui->message(TCODColor::red,"%f",damage1);
				}
			}
		} else if (roll1 <= 1) {
			if (engine.map->isInFov(owner->x,owner->y)) {
				engine.gui->message(TCODColor::lightGrey,"%s MH critical miss...",owner->getName());
			}
			damage1 = 0;
		}
		damage1 += (hands * owner->attacker->totalPower);
		if (attackRoll1 > target->destructible->totalDodge) { 	
			if (damage1 > target->destructible->totalDR) {
				engine.gui->message(TCODColor::red,"%s does %d damage to %s",owner->getName(),(int)target->destructible->takeDamage(target,owner,damage1),target->getName());
			} else {
				engine.gui->message(TCODColor::grey,"%s attacks %s to no avail!",owner->getName(),target->getName());
			}
		} else {
			engine.gui->message(TCODColor::grey,"%s misses %s!",owner->getName(),target->getName());
		}
		
		if (owner->container && owner->container->hand2 != NULL) {
			int roll2 = dicer->getInt(1,20);
			int attackRoll2 = roll2 + owner->attacker->totalStr;
			diceNum = ((Weapon*)owner->container->hand2->pickable)->diceNum;
			diceType = ((Weapon*)owner->container->hand2->pickable)->diceType;
			int damage2 = 0;
			hands = 1;
			for (int i = 0; i < diceNum; i++) {
				damage2 += (float)dicer->getInt(1,diceType);
				engine.gui->message(TCODColor::red,"%f",damage2);
			}
			critMult = ((Weapon*)owner->container->hand2->pickable)->critMult;
			critRange = ((Weapon*)owner->container->hand2->pickable)->critRange;
			if (roll2 >= critRange) {
				if (engine.map->isInFov(owner->x,owner->y)) {
					engine.gui->message(TCODColor::red,"%s OH CRITICAL HIT!",owner->getName());
				}
				for (int j = 0; j < critMult-1; j++) {
					for (int i = 0; i < diceNum; i++) {
						damage2 += dicer->getInt(1,diceType);
						engine.gui->message(TCODColor::red,"%f",damage2);
					}
				}
			} else if (roll2 <= 1) {
				if (engine.map->isInFov(owner->x,owner->y)) {
					engine.gui->message(TCODColor::lightGrey,"%s OH critical miss...",owner->getName());
				}
				damage2 = 0;
			}
			damage2 += (hands * owner->attacker->totalPower);
			if (attackRoll2 > target->destructible->totalDodge){
				if (damage2 > target->destructible->totalDR) {
					engine.gui->message(TCODColor::red,"%s does %d damage to %s",owner->getName(),(int)target->destructible->takeDamage(target,owner,damage2),target->getName());
				} else {
					engine.gui->message(TCODColor::grey,"%s attacks %s to no avail!",owner->getName(),target->getName());
				}
			} else {
				engine.gui->message(TCODColor::grey,"%s misses %s!",owner->getName(),target->getName());
			}
		
		}
		
	} else {
		if(engine.map->isInFov(owner->x, owner->y) || engine.map->isInFov(target->x, target->y)) {
			engine.gui->message(TCODColor::lightGrey,"The %s attacks the %s in vain.", owner->getName(),target->getName());
		}
	}
	if (target->destructible->isDead()) {
		engine.gui->message(TCODColor::white,"%s is now dead!",target->getName(true));
		lastTarget = NULL;
	}
}

