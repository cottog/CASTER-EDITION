#include "main.hpp"

Aura::Aura(int duration, int bonus, StatType stat, LifeStyle life) :
	totalDuration(duration),duration(duration),bonus(bonus),stat(stat),life(life){ 
}

void Aura::save(Actor *target) {
	zip.putInt(stat);
	zip.putInt(totalDuration);
	zip.putInt(duration);
	zip.putInt(bonus);
	zip.putInt(life);
}

void Aura::load(Actor *target) {
	totalDuration = zip.getInt();
	duration = zip.getInt();
	bonus = zip.getInt();
	life = (LifeStyle)zip.getInt();
}

void Aura::apply(Actor *target) {
	engine.gui->message(TCODColor::red,"aura applied");
	switch(stat) {
		case NONE: break;
		case TOTALPOWER: if (target->attacker) target->attacker->totalPower += bonus; break;
		case TOTALSTR: if (target->attacker) target->attacker->totalStr += bonus; break;
		case TOTALDEX: if (target->attacker) target->attacker->totalDex += bonus; break;
		case TOTALINTEL: /* if (target->caster) target->caster->totalIntel += bonus; */ break;
		case TOTALDODGE: if (target->destructible) target->destructible->totalDodge += bonus; break;
		default: break;
	}
}

void Aura::unApply(Actor *target) {
	if (life == CONTINUOUS) {
		switch (stat) {
			case NONE: break;
			case TOTALPOWER: if (target->attacker) target->attacker->totalPower -= bonus; break;
			case TOTALSTR: if (target->attacker) target->attacker->totalStr -= bonus; break;
			case TOTALDEX: if (target->attacker) target->attacker->totalDex -= bonus; break;
			case TOTALINTEL: /* if (target->caster) target->caster->totalIntel -= bonus; */ break;
			case TOTALDODGE: if (target->destructible) target->destructible->totalDodge -= bonus; break;
			default: break;
		}
	}
}

ShieldAura::ShieldAura(int duration, int bonus, LifeStyle life) :
	 Aura(duration, bonus, SHIELD, life){
}

void ShieldAura::apply(Actor *target) {
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) { 
		if (target->destructible) {
			target->destructible->maxShield += bonus;
			target->destructible->shield += bonus;
		}
	}
}


void ShieldAura::unApply(Actor *target) {
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}

	if (target->destructible) {
		target->destructible->maxShield -= bonus;
		target->destructible->shield -= bonus;

		if (target->destructible->shield < 0) target->destructible->shield = 0;
		if (target->destructible->maxShield < 0) target->destructible->maxShield = 0;
	}
}


Heal::Heal(int duration, int tick, LifeStle life) :
	Aura(duration, tick, HEALTH, life){
}

void Heal::apply(Actor *target) {
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) { 
		if (target->destructible) target->destructible->heal(bonus);
	}
}

void Heal::unApply(Actor *target) {	//a heal/damage over time doesn't need to do anything
}

HealthBoost::HealthBoost(int duration, int boost, LifeStyle life) : 
	Aura(duration, boost, MAXHEALTH, life){
}

void HealthBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) { 
		if (target->destructible){
			target->destructible->maxHp += bonus; 
			if (target->destructible->maxHp < target->destructible->hp) {
				target->destructible->hp = target->destructible->maxHp;
			}
		}
	}
}

void HealthBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}

	if (target->destructible){
		target->destructible->maxHp -= bonus; 
		if (target->destructible->maxHp < target->destructible->hp) {
			target->destructible->hp = target->destructible->maxHp;
		}
	}
}

LightBoost::LightBoost(int duration, int boost, LifeStyle life) : 
	Aura(duration, boost, LIGHT, life){	
}

void LightBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) {
		engine.fovRadius += bonus;
	}
}

void LightBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}
	engine.fovRadius -= bonus;
}

DRBoost::DRBoost(int duration, int boost, LifeStyle life) : 
	Aura(duration, boost, TOTALDR, life){
}

void DRBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) {
		if (target->destructible) target->destructible->totalDR += bonus;
	}
}

void DRBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}
	if (target->destructible) target->destructible->totalDR -= bonus;
}