#include "main.hpp"

Aura::Aura(int duration, int bonus, StatType stat, LifeStyle life) :
	totalDuration(duration),duration(duration),bonus(bonus),stat(stat),life(life){ 
}

void Aura::save(TCODZip &zip) {
	zip.putInt(totalDuration);
	zip.putInt(duration);
	zip.putInt(bonus);
	zip.putInt(stat);
	zip.putInt(life);
}

void Aura::load(TCODZip &zip) {
	totalDuration = zip.getInt();
	duration = zip.getInt();
	bonus = zip.getInt();
	stat = (StatType)zip.getInt();
	life = (LifeStyle)zip.getInt();
}

void Aura::apply(Actor *target) {
	switch(stat) {
		case NONE: break;
		case ALL:
			if (target->attacker) {
				target->attacker->totalStr += bonus;
				target->attacker->totalDex += bonus;
				target->attacker->totalPower += bonus;
			}
			if (target->destructible) {
				target->destructible->totalDR += bonus;
				target->destructible->totalDodge += bonus;
				target->destructible->maxHp += bonus;
				if (target->destructible->maxHp < target->destructible->hp) {
					target->destructible->hp = target->destructible->maxHp;
				}
			}
			/* if (target->caster) {
				target->caster->totalIntel += bonus;
			} */
		break;
		case TOTALPOWER: if (target->attacker) target->attacker->totalPower += bonus; break;
		case TOTALSTR: if (target->attacker) target->attacker->totalStr += bonus; break;
		case TOTALDEX: if (target->attacker) target->attacker->totalDex += bonus; break;
		case TOTALINTEL: /* if (target->caster) target->caster->totalIntel += bonus; */ break;
		case TOTALDODGE: if (target->destructible) target->destructible->totalDodge += bonus; break;
		case TOTALDR: if (target->destructible) target->destructible->totalDR += bonus; break;
		case HEALTH: if (target->destructible) target->destructible->heal(bonus); break;
		case MAXHEALTH: 
			if (target->destructible){
				target->destructible->maxHp += bonus; 
				if (target->destructible->maxHp < target->destructible->hp) {
					target->destructible->hp = target->destructible->maxHp;
				}
			}
			break;
		case LIGHT: engine.fovRadius += bonus; break;
		default: break;
	}
}

void Aura::unApply(Actor *target) {
	if (life == CONTINUOUS) {
		switch (stat) {
			case NONE: break;
			case ALL:
				if (target->attacker) {
					target->attacker->totalStr -= bonus;
					target->attacker->totalDex -= bonus;
					target->attacker->totalPower -= bonus;
				}
				if (target->destructible) {
					target->destructible->totalDR -= bonus;
					target->destructible->totalDodge -= bonus;
					target->destructible->maxHp -= bonus;
					if (target->destructible->maxHp < target->destructible->hp) {
						target->destructible->hp = target->destructible->maxHp;
					}
				}
				/* if (target->caster) {
					target->caster->totalIntel -= bonus;
				} */
			break;
			case TOTALPOWER: if (target->attacker) target->attacker->totalPower -= bonus; break;
			case TOTALSTR: if (target->attacker) target->attacker->totalStr -= bonus; break;
			case TOTALDEX: if (target->attacker) target->attacker->totalDex -= bonus; break;
			case TOTALINTEL: /* if (target->caster) target->caster->totalIntel -= bonus; */ break;
			case TOTALDODGE: if (target->destructible) target->destructible->totalDodge -= bonus; break;
			case TOTALDR: if (target->destructible) target->destructible->totalDR -= bonus; break;
			case HEALTH: break; //DoT or Heal, nothing needs to happen
			case MAXHEALTH: 
				if (target->destructible){
					target->destructible->maxHp -= bonus; 
					if (target->destructible->maxHp < target->destructible->hp) {
						target->destructible->hp = target->destructible->maxHp;
					}
				}
				break;
			case LIGHT: engine.fovRadius -= bonus; break;
			default: break;
		}
	} else if (life == ITERABLE) {
		switch (stat) {
			case NONE: break;
			case ALL:
				if (target->attacker) {
					target->attacker->totalStr -= totalDuration*bonus;
					target->attacker->totalDex -= totalDuration*bonus;
					target->attacker->totalPower -= totalDuration*bonus;
				}
				if (target->destructible) {
					target->destructible->totalDR -= totalDuration*bonus;
					target->destructible->totalDodge -= totalDuration*bonus;
					target->destructible->maxHp -= totalDuration*bonus;
					if (target->destructible->maxHp < target->destructible->hp) {
						target->destructible->hp = target->destructible->maxHp;
					}
				}
				/* if (target->caster) {
					target->caster->totalIntel -= totalDuration*bonus;
				} */
			break;
			case TOTALPOWER: if (target->attacker) target->attacker->totalPower -= totalDuration*bonus; break;
			case TOTALSTR: if (target->attacker) target->attacker->totalStr -= totalDuration*bonus; break;
			case TOTALDEX: if (target->attacker) target->attacker->totalDex -= totalDuration*bonus; break;
			case TOTALINTEL: /* if (target->caster) target->caster->totalIntel -= totalDuration*bonus; */ break;
			case TOTALDODGE: if (target->destructible) target->destructible->totalDodge -= totalDuration*bonus; break;
			case TOTALDR: if (target->destructible) target->destructible->totalDR -= totalDuration*bonus; break;
			case HEALTH: break; //DoT or Heal, nothing needs to happen
			case MAXHEALTH: 
				if (target->destructible){
					target->destructible->maxHp -= totalDuration*bonus; 
					if (target->destructible->maxHp < target->destructible->hp) {
						target->destructible->hp = target->destructible->maxHp;
					}
				}
				break;
			case LIGHT: engine.fovRadius -= totalDuration*bonus; break;
			default: break;
		}
	}
}