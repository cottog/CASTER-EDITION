#include "main.hpp"

Aura::Aura(int duration, int bonus, StatType stat, LifeStyle life) :
	totalDuration(duration),duration(duration),bonus(bonus),stat(stat),life(life){ 
}

void Aura::save(TCODZip &zip) {
	zip.putInt(stat);
	zip.putInt(totalDuration);
	zip.putInt(duration);
	zip.putInt(bonus);
	zip.putInt(life);
}

void Aura::load(TCODZip &zip) {
	totalDuration = zip.getInt();
	duration = zip.getInt();
	bonus = zip.getInt();
	life = (LifeStyle)zip.getInt();
}

Aura *Aura::create(TCODZip &zip){
	StatType type = (StatType)zip.getInt();
	Aura *aura = NULL;
	switch(type){
		case NONE: break;
		case TOTALSTR: aura = new StrBoost(0,0); break;
		case TOTALPOWER: aura = new PowerBoost(0,0); break;
		case TOTALDEX: aura = new DexBoost(0,0); break;
		case TOTALINTEL: aura = new IntBoost(0,0); break;
		case TOTALDODGE: aura = new DodgeBoost(0,0); break;
		case TOTALDR: aura = new DRBoost(0,0); break;
		case HEALTH: aura = new HealAura(0,0); break;
		case MAXHEALTH: aura = new HealthBoost(0,0); break;
		case LIGHT: aura = new LightBoost(0,0); break;
		case SHIELD: aura = new ShieldAura(0,0); break;
		case REFLECTION: aura = new ReflectionAura(0); break;
		case ABSORPTION: aura = new AbsorptionAura(0,0); break;
		case DAMAGING_AURA: aura = new DamagingAura(0,0,0,false); break;
		case DOOM: aura = new DoomAura(0,0); break;
	}
	aura->load(zip);
	return aura;
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


HealAura::HealAura(int duration, int tick, LifeStyle life) :
	Aura(duration, tick, HEALTH, life){
}

void HealAura::apply(Actor *target) {
	TCODRandom *rng = TCODRandom::getInstance();
	int realAmount = rng->getInt(bonus*0.8,bonus*1.25);

	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) { 
		if (bonus > 0) {
			if (target->destructible) target->destructible->heal(realAmount);
		} else {
			if (target->destructible) target->destructible->takeDamage(target,NULL,-1*realAmount);
		}
	}
}

void HealAura::unApply(Actor *target) {	//a heal/damage over time doesn't need to do anything
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

DodgeBoost::DodgeBoost(int duration, int boost, LifeStyle life) :
	Aura(duration, boost, TOTALDODGE, life){
}

void DodgeBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) {
		if (target->destructible) target->destructible->totalDodge += bonus;
	}
}

void DodgeBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}
	if (target->destructible) target->destructible->totalDodge -= bonus;
}

IntBoost::IntBoost(int duration, int boost, LifeStyle life) :
	Aura(duration, boost, TOTALINTEL, life){
}

void IntBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) {
		/* if (target->caster) target->caster->totalIntel += bonus; */
	}
}

void IntBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}
	/* if (target->caster) target->caster->totalIntel -= bonus; */
}

DexBoost::DexBoost(int duration, int boost, LifeStyle life) : 
	Aura(duration, boost, TOTALDEX, life){
}

void DexBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) {
		if (target->attacker) target->attacker->totalDex += bonus;
	}
}

void DexBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}
	if (target->attacker) target->attacker->totalDex -= bonus;
}

PowerBoost::PowerBoost(int duration, int boost, LifeStyle life) :
	Aura(duration, boost, TOTALPOWER, life){
}

void PowerBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) {
		if (target->attacker) target->attacker->totalPower += bonus;
	}
}

void PowerBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}
	if (target->attacker) target->attacker->totalPower -= bonus;
}

StrBoost::StrBoost(int duration, int boost, LifeStyle life) :
	Aura(duration, boost, TOTALSTR, life){
}

void StrBoost::apply(Actor *target){
	if ( (life == CONTINUOUS && duration == totalDuration) || life == ITERABLE ) {
		if (target->attacker) target->attacker->totalStr += bonus;
	}
}

void StrBoost::unApply(Actor *target){
	if (life == ITERABLE) {
		bonus *= totalDuration;
	}
	if (target->attacker) target->attacker->totalStr -= bonus;
}

ReflectionAura::ReflectionAura(int duration) :
	Aura(duration, 0, REFLECTION, CONTINUOUS){
}

void ReflectionAura::apply(Actor *target){
}

void ReflectionAura::unApply(Actor *target){
}

AbsorptionAura::AbsorptionAura(int duration, int percentage) :
	Aura(duration, percentage, ABSORPTION, CONTINUOUS){
}

void AbsorptionAura::apply(Actor *target){
}

void AbsorptionAura::unApply(Actor *target){
}

DamagingAura::DamagingAura(int duration, int damage, int radius, bool smart) :
	Aura(duration, damage, DAMAGING_AURA, ITERABLE),radius(radius),smart(smart){
}

void DamagingAura::save(TCODZip &zip){
	zip.putInt(stat);
	zip.putInt(totalDuration);
	zip.putInt(duration);
	zip.putInt(bonus);
	zip.putInt(life);
	zip.putInt(radius);
	zip.putInt(smart);
}

void DamagingAura::load(TCODZip &zip){
	totalDuration = zip.getInt();
	duration = zip.getInt();
	bonus = zip.getInt();
	life = (LifeStyle)zip.getInt();
	radius = zip.getInt();
	smart = zip.getInt();
}

void DamagingAura::apply(Actor *target){
	TCODRandom *rng = TCODRandom::getInstance();
	int realDamage = rng->getInt(bonus*0.8,bonus*1.25);

	TCODList<Actor *> inRadius;
	if (!smart){
		engine.getAllActorsInRadius(inRadius,target->x, target->y, radius);
	} else {
		engine.getAllActorsInRadius(inRadius,target->x, target->y, radius, TargetingSystemBase::ENEMY, target->hostile);
	}

	if (!inRadius.isEmpty()){

		for (Actor **iter = inRadius.begin();
			iter != inRadius.end(); iter++) {
			Actor *act1 = *iter;

			if (act1->destructible) act1->destructible->takeDamage(act1, target, realDamage);
		}
	}

	inRadius.clearAndDelete();
}

void DamagingAura::unApply(Actor *target){
}

DoomAura::DoomAura(int duration, int damage) :
	Aura(duration, damage, DOOM, CONTINUOUS){
}

void DoomAura::apply(Actor *target){
}

void DoomAura::unApply(Actor *target){
	if (target->destructible) target->destructible->takeDamage(target,NULL,bonus);
}