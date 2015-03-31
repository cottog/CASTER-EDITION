#include <iostream>
#include <cmath>
#include "libtcod.hpp"
#include "main.hpp"

Spell::Spell(float target, SpellIntensity intensity, TargetSystem targeting, SpellEffect effect, 
		ExpectedTarget expected , float cost) :
	target(target), intensity(intensity),targeting(targeting),effect(effect),expected(expected),cost(cost) {}


float Spell::setTarget(){
	TCODRandom *rng = TCODRandom::getInstance();
	target = 0;

	for (int x=0; x<4; x++){
		target += pow((rng->getInt(0,25)),(int)(intensity));
	}
	return target;
}

void Spell::chooseTargetSystem(){
	TCODRandom *rng = TCODRandom::getInstance();
	int targetswitch = rng->getInt(1,15);
	switch (targetswitch) {
		case 1:
			targeting = SELF;
			cost *= 1;
			break;
		case 2:
			targeting = ADJACENT_TILE;
			cost *= 1;
			break;
		case 3: 
			targeting = BOLT_SPELL;
			cost *= 1.1;
			break;
		case 4:
			targeting = VISUAL_HOLOCAUST;
			cost *= 1.5;
			break;
		case 5:
			targeting = LEVEL_WIDE_HOLOCAUST;
			cost *= 1.8;
			break;
		case 6: 
			targeting = RANDOM_IN_LOS;  
			cost *= 0.8;
			break;
		case 7:
			targeting = RANDOM_IN_LEVEL; 
			cost *= 1.2;
			break;
		case 8:
			targeting = ALL_ADJACENT_TILES;
			cost *= 1.3;
			break;
		case 9:
			targeting = ALL_CREATURES_CHAINING;
			cost *= 1.3;
			break;
		case 10:
			targeting = ALL_CREATURES_IN_LEVEL;
			cost *= 2.0;
			break;
		case 11:
			targeting = ALL_CREATURES_IN_LINE;
			cost *= 1.4;
			break;
		case 12:
			targeting = SINGLE_CREATURE_IN_SIGHT;
			cost *= 1;
			break;
		case 13:
			targeting = ALL_CREATURES_IN_RADIUS;
			cost *= 1.3;
			break;
		case 14: 
			targeting = X_CREATURES_IN_SIGHT;
			cost *= 1.2;
			break;
		case 15:
			targeting = ALL_CREATURES_IN_LOS;
			cost *= 1.4;
			break;
		default: targeting = NO_TARGET; break;
	}
}

void Spell::chooseIntensity(int level){
	TCODRandom *rng = TCODRandom::getInstance();
	int dingle = rng->getInt(1,100);

	//determine the possible intensities based on character level
	if (level <= 11) {
		intensity = MINOR;
		cost = 3;
	} else if (level <= 25) {
		if (dingle <= 55) {
			intensity = NORMAL;
			cost = 9;
		} else {
			intensity = MINOR;
			cost = 3;
		}
	} else if (level <= 35) {
		if (dingle <= 30) {
			intensity = MAJOR;
			cost = 26;
		} else {
			intensity = NORMAL;
			cost = 9;
		}
	} else if (level <= 45) {
		if (dingle <= 60) {
			intensity = MAJOR;
			cost = 26;
		} else {
			intensity = NORMAL;
			cost = 9;
		}
	} else {
		intensity = EPIC;
		cost = 54;
	}
}

void CreatureSpell::chooseEffect(){
	int baseCost = cost;
	TCODRandom *rng = TCODRandom::getInstance();
	int effectSwitch = 0;
	if (targeting == SELF) {
		int fart = rng->getInt(1,500);  //make it very unlikely for a bad spell to become a self-targeting spell
		if (fart <= 499) {
			effectSwitch = rng->getInt(1,17);
		} else {
			effectSwitch = rng->getInt(1,38);
		}
	} else {
		effectSwitch = rng->getInt(1,38);
	}
	
	switch (effectSwitch) {
		case 1:
			effect = STRAIGHT_HEAL;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1;
			break;
		case 2:
			effect = HEAL_OVER_TIME;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= .8;
			break;
		case 3:
			effect = STAT_BOOST;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1;
			break;
		case 4:
			effect = CURE_DEBUFFS;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.1;
			break;
		case 5: 
			effect = DETECTION;
			expected = CREATURE;
			preferred = NEUTRAL;
			cost *= 1.1;
			break;
		case 6:
			effect = POLYMORPH;
			expected = CREATURE;
			preferred = NEUTRAL;
			cost *= 1.5;
			break;
		case 7: 
			effect = SHAPE_SHIFTING;
			expected = CREATURE;
			preferred = NEUTRAL;
			cost *= 1.1;
			break;
		case 8:
			effect = SIMULACRUM; //NOTE: simulacrum is similar to illusion in that it copies a creature, but this spell targets a creature and creates a simulacrum of it a short distance away on a random tile
			expected = CREATURE; // A simulacrum is intended to be much hardier than an illusion, and possibly to possess some attributes of its target
			preferred = ENEMY;      //NOTE: not actually harmful, but it should prefer 
			cost *= 1.4;
			break;               //simulacrum success should be dependent on how close the resources spent resemble the relative mixture of each element a creature possesses (a glass creature should not be copied by a simulacrum spell of the radiation subtype)  
		case 9:
			effect = SHIELD;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.1;
			break;
		case 10:
			effect = REFLECTION;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.2;
			break;
		case 11:
			effect = ABSORPTION;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.2;
			break;
		case 12:
			effect = DAMAGING_TELEPORT;
			expected = CREATURE;
			preferred = NEUTRAL;
			cost *= 1.4;
			break;
		case 13:
			effect = WEAPON_ENHANCEMENT;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.3;
			break;
		case 14:
			effect = PROTECT_FROM_HAZARDS;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.2;
			break;
		case 15:
			effect = TELEPORT;
			expected = CREATURE;
			preferred = NEUTRAL;
			cost *= 1.2;
			break;
		case 16:
			effect = DAMAGING_AURA;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.3;
			break;
		case 17:
			effect = INTELLIGENT_DAMAGING_AURA;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.4;
			break;
		
		//THESE SPELLS ARE BAD FOR SELF-TARGETING SPELLS 
		case 18: 
			effect = DAMAGING_PULL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.2;
			break;
		case 19:
			effect = DAMAGING_PUSH;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 20:
			effect = STRAIGHT_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 21:
			effect = BLEED_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1;
			break;
		case 22:
			effect = STAT_DRAIN;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 23:
			effect = STAT_SAPPING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 24:
			effect = LIFE_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 25:
			effect = MANA_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 26:
			effect = INSTAKILL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 2.1;
			break;
		case 27: 
			effect = DEBUFFING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 28:
			effect = UNSUMMON;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 29:
			effect = BANISHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 30:
			effect = MANA_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 31:
			effect = DOOM_TIMER;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 32:
			effect = PERCENTILE_DAMAGE;
			expected = CREATURE;
			preferred =  ENEMY;
			cost *= 1.3;
			break;
		case 33:
			effect = RESURRECT;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.5;
			break;
		case 34:
			effect = PULL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1;
			break;
		case 35:
			effect = PUSH;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		//THESE SPELL EFFECTS NEED SPECIFIC TARGETING TYPES
		case 36:
			effect = ITEM_CREATION;
			expected = CREATURE;
			preferred = NEUTRAL;
			targeting = SELF;
			cost = baseCost;
			cost *= 1.4;
			break;
		case 37:
			effect = LINK;
			expected = CREATURE;
			preferred = ENEMY;
			targeting = SINGLE_CREATURE_IN_SIGHT;
			cost = baseCost;
			cost *= 1.1;
			break;
		case 38:
			effect = LIFE_TAP;
			expected = CREATURE;
			preferred = NEUTRAL;
			targeting = SELF;
			cost = baseCost;
			break;
		
		default: effect = NO_EFFECT; break;
	}

	actual = preferred;

	if (targeting != SELF) { //we will roll some dice and see if the targets its preferred type, or something else (which could be better or worse)
		int roll = rng->getInt(1,100);
		std::cout << roll << std::endl;
		if (roll <= 5) { //5 percent chance to actually target something opposite to what's preferred
			if (preferred == ENEMY) {
				actual = FRIENDLY;
			} 
			if (preferred == FRIENDLY) {
				actual = ENEMY;
			}
		} else if (roll <= 25) { //20% chance to go from neutral targeting to something more discriminating
			if (preferred == NEUTRAL) {
				int roll2 = rng->getInt(0,1);
				if (roll2) {
					actual = ENEMY;
				} else {
					actual = FRIENDLY;
				}
			}
		} else { //75% chance to target the preferred target
			actual = preferred;
		}
	}
}

CreatureSpell::CreatureSpell( float target, SpellIntensity intensity, TargetSystem targeting, SpellEffect effect, 
		ExpectedTarget expected, float cost, TargetType preferred, TargetType actual) :
	Spell(target,intensity,targeting,effect,expected,cost), preferred(preferred),actual(actual) {}


bool CreatureSpell::cast(Actor *caster){
	std::cout << preferred << std::endl;
	std::cout << actual << std::endl;
	std::cout << effect << std::endl;
	std::cout << "creature spell casted" << std::endl;

	TCODRandom *rng = TCODRandom::getInstance();
	TCODList<Actor *> targets;

	switch(targeting) {
		default: targets.clearAndDelete(); return false; break;
		case SELF: {
			targets.push(caster);
			break;
		}
		case ADJACENT_TILE: {
			engine.getAllActorsInRadius(targets, caster->x, caster->y, 1.6,this->actual,caster->hostile);
			if (targets.isEmpty()) {
				targets.clearAndDelete();
				return false;
			}
			while (targets.size() > 1) {
				int elNum = rng->getInt(0,targets.size()-1);
				Actor *toRev = targets.get(elNum);
				targets.remove(toRev);
			}
			break;
		}
		case BOLT_SPELL: {
			int x = caster->x;
			int y = caster->y;
			int stepX = caster->x;
			int stepY = caster->y;
			if (engine.pickATile(&x,&y,4+2*((int)intensity))){
				TCODLine::init(caster->x, caster->y, x, y);
				do {
					if ( !engine.map->canWalk(stepX,stepY)) {
						Actor *actor = engine.getActor(stepX,stepY);
						if (!actor) {
							break;
						} else if ( actor != caster && ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ) {
							targets.push(actor);
							break;
						}
					}
				} while (TCODLine::step(&stepX,&stepY));
			} else {
				targets.clearAndDelete();
				return false;
			}
			break;
		}
		case VISUAL_HOLOCAUST: {
			int x = caster->x;
			int y = caster->y;
			if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
				Actor *actor2 = engine.getActor(x,y);
				if (!actor2) {
					targets.clearAndDelete();
					return false;
				} else {
					Spell::ElementalSubtype targetedElement = actor2->destructible->element; //we know the actor has a destructible since getActor checks for it
					targets.push(actor2);
					for (Actor **iterator = engine.actors.begin();iterator != engine.actors.end(); iterator++) {
					Actor *actor = *iterator;
						if (actor != caster && actor->destructible && !actor->destructible->isDead() && actor->isVisible() && actor->destructible->element == targetedElement && ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ){
							targets.push(actor);
						}
					}
				}
			} else {
				targets.clearAndDelete();
				return false;	
			}
			break;
		}
		case LEVEL_WIDE_HOLOCAUST: {
			int x = caster->x;
			int y = caster->y;
			if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
				Actor *actor2 = engine.getActor(x,y);
				if (!actor2) {
					targets.clearAndDelete();
					return false;
				} else {
					Spell::ElementalSubtype targetedElement = actor2->destructible->element; //we know the actor has a destructible since getActor checks for it
					targets.push(actor2);
					for (Actor **iterator = engine.actors.begin();iterator != engine.actors.end(); iterator++) {
					Actor *actor = *iterator;
						if (actor != caster && actor->destructible && !actor->destructible->isDead() && actor->destructible->element == targetedElement && ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ){  //same as above, but we don't check if its visible
							targets.push(actor);
						}
					}
				}
			} else {
				targets.clearAndDelete();
				return false;	
			}
			break;	
		}
		case RANDOM_IN_LOS: {
			engine.getAllActorsInRadius(targets, caster->x, caster->y, engine.fovRadius,this->actual,caster->hostile);
			if (targets.isEmpty()) {
				targets.clearAndDelete();
				return false;
			}
			while (targets.size() > 1) {
				int elNum = rng->getInt(0,targets.size()-1);
				Actor *toRev = targets.get(elNum);
				targets.remove(toRev);
			}
			break;
		}
		case RANDOM_IN_LEVEL: {
			engine.getAllActorsInRadius(targets, caster->x, caster->y, 0.0,this->actual,caster->hostile);
			if (targets.isEmpty()) {
				targets.clearAndDelete();
				return false;
			}
			while (targets.size() > 1) {
				int elNum = rng->getInt(0,targets.size()-1);
				Actor *toRev = targets.get(elNum);
				targets.remove(toRev);
			}
			break;
		}
		case ALL_ADJACENT_TILES: {
			engine.getAllActorsInRadius(targets, caster->x, caster->y, 1.6,this->actual, caster->hostile);
			if (targets.isEmpty()) {
				targets.clearAndDelete();
				return false;
			}

			break;
		}
		case ALL_CREATURES_CHAINING: {
			int x = caster->x;
			int y = caster->y;
			TCODList<Actor *> possibleJumps; //this TCODList holds the actors to which this spell could possibly jump
			if (engine.pickATile(&x,&y,4+2*((int)intensity))){
				Actor *actor = engine.getActor(x,y);
				if (!actor) {
					break;
				} else if ( ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ) {
					targets.push(actor);
					break;
				}
			}

			if (targets.isEmpty()) {
				possibleJumps.clearAndDelete();
				targets.clearAndDelete();
				return false;
			}

			while ( targets.size() < 1+2*((int)intensity) ) {
				engine.getAllActorsInRadius(possibleJumps, targets.get(targets.size()-1)->x, targets.get(targets.size()-1)->y, 1+2*((int)intensity),this->actual, caster->hostile);

				if (possibleJumps.isEmpty()) {
					possibleJumps.clearAndDelete();
					break;
				} else {
					int jumpTarget = rng->getInt(0, possibleJumps.size() - 1);
					targets.push(possibleJumps.get(jumpTarget));
					possibleJumps.clear();
				}

			}
			possibleJumps.clearAndDelete();
			break;
		}
		case ALL_CREATURES_IN_LEVEL: {
			engine.getAllActorsInRadius(targets,caster->x, caster->y, 0.0, this->actual, caster->hostile);
			if (targets.isEmpty()) {
				targets.clearAndDelete();
				return false;
			}
			break;
		}
		case ALL_CREATURES_IN_LINE: {
			int x = caster->x;
			int y = caster->y;
			int stepX = caster->x;
			int stepY = caster->y;
			if (engine.pickATile(&x,&y,4+2*((int)intensity))){
				TCODLine::init(caster->x, caster->y, x, y);
				do {
					if ( !engine.map->canWalk(stepX,stepY)) {
						Actor *actor = engine.getActor(stepX,stepY);
						if ( actor && actor != caster && ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ) {
							targets.push(actor);
						}
					}
				} while (TCODLine::step(&stepX,&stepY));
			} else {
				targets.clearAndDelete();
				return false;
			}
			break;
		}
		case ALL_CREATURES_IN_RADIUS: {
			int x = caster->x;
			int y = caster->y;

			if (engine.pickATile(&x,&y,4+2*((int)intensity))){
				engine.getAllActorsInRadius(targets,caster->x, caster->y, 2+2*((int)intensity), this->actual, caster->hostile);

				if (targets.isEmpty()){
					targets.clearAndDelete();
					return false;
				}

			} else {
				targets.clearAndDelete();
				return false;
			}
			break;
		}
		case SINGLE_CREATURE_IN_SIGHT: {
			int x = caster->x;
			int y = caster->y;
			if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
				Actor *actor = engine.getActor(x,y);
				if (!actor) {
					targets.clearAndDelete();
					return false;
				} else {
					if ( ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ){
						targets.push(actor);
					}
				}
			} else {
				targets.clearAndDelete();
				return false;	
			}
			break;
		}
		case X_CREATURES_IN_SIGHT: {
			while ( targets.size() < 2+((int)intensity) ) {
				
				int x = caster->x;
				int y = caster->y;
				if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
					Actor *actor = engine.getActor(x,y);
					if (!actor) {
						break;
					} else if ( ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ) {
						targets.push(actor);
					}
				} else {
					break;	
				}
			}
			if (targets.isEmpty()) {
				targets.clearAndDelete();
				return false;
			}
			break;
		} case ALL_CREATURES_IN_LOS: {
			for (Actor **iterator = engine.actors.begin();iterator != engine.actors.end(); iterator++) {
				Actor *actor = *iterator;
				if (actor != caster && actor->isVisible() && actor->destructible && !actor->destructible->isDead() && ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ){ 
					targets.push(actor);
				}
			}

			break;
		}
	}
	//now all possible TargetSystems have been accounted for, and the TargetType should be accounted for as well
	//check if targets is empty, and print a message so you know its empty here
	if (targets.isEmpty()) {
		engine.gui->message(TCODColor::lightPurple,"targets is empty");
		targets.clearAndDelete();
		return false;
	}

	for (Actor **iterator = targets.begin();
		iterator != targets.end(); iterator++) {
		
		Actor *actor = *iterator;
		for (Aura **it = actor->auras.begin(); it!= actor->auras.end(); it++) {
			Aura *aura = *it;
			if(aura->stat == Aura::REFLECTION){
				targets.push(caster);
				continue;
			}
			if(aura->stat == Aura::ABSORPTION){
				//if(target->caster) target->caster->gainMana(cost*(aura->bonus/100));	//gain mana proportial to cost of spell, dependent on absorption magnitude
				continue;
			}
		}

		switch(effect) {
			default: targets.clearAndDelete(); return false; break;
			case STRAIGHT_HEAL: {

				int heal = rng->getInt( (5+10*((int)intensity))*0.8, (5+10*((int)intensity))*1.25); //choose a random amount of HP to heal
				actor->destructible->heal(heal);	//assume that all actors here are Destructible and !isDead(); if it crashes, fix it above, where they are added to targets

				break;
			}
			case HEAL_OVER_TIME: {
				Aura *hot = new HealAura(10+((int)intensity),3*((int)intensity),Aura::ITERABLE); //this will have to change when auras are changed around
				actor->auras.push(hot);
				break;
			}
			case STAT_BOOST: {
				//determine what elements correspond to what stat and then add this functionality

				break;
			}
			case CURE_DEBUFFS: {
				//once you figure out what buffs correspond to what element, do the inverse here

				break;
			}
			case DETECTION: {
				engine.map->exploreTile(actor->x,actor->y);
				break;
			}
			case POLYMORPH: {
				//once you have an enemyFactory class, use that here to generate a new body for the target actor
				//this new body will be determined by the element of the spell

				break;
			}
			case SHAPE_SHIFTING: {
				//do the smae as polymorph, but make an Aura for shape_shifting so they can go back to their old body

				break;
			}
			case SIMULACRUM: {
				//make a copy of the selected actor and place it nearby
				int x = actor->x;
				int y = actor->y;
				bool tileOpen = engine.findNearbyOpenTile(&x,&y);
				if (tileOpen){
					Actor *simulacrum = new Actor(*actor);
					simulacrum->x = x;
					simulacrum->y = y;
					simulacrum->hostile = caster->hostile;	//simulacrums are friendly towards their creator
					simulacrum->blocks = true;
					simulacrum->ai = new MonsterAi();
					engine.actors.push(simulacrum);
				}
				break;
			}
			case SHIELD: {
				int shieldAmount = rng->getInt( (10*((int)intensity))*0.8, (10*((int)intensity))*1.25);	//find a random amount to shield the target by
				Aura *shield = new ShieldAura(10+3*((int)intensity),shieldAmount);
				actor->auras.push(shield);
				break;
			}
			case REFLECTION: {
				//essentially, this should add a ReflectionAura to the target, which simply holds a duration and an elemental subtype
				//above, where we iterate through the list of targets, we will then check if they have an aura of type REFLECTION, and if that Aura's elemental subtype matches that of this spell
				//if both of the above conditions hold, target is instead changed to caster, effectively reflecting the spell
				//this functionality is partially implemented; the elemental subtype functionality isn't there yet
				Aura *reflection = new ReflectionAura(5+2*((int)intensity));
				actor->auras.push(reflection);
				break;
			}
			case ABSORPTION: {
				//this will function similarly to the above case, where it will be handled by an aura
				//for an AbsorptionAura, however, instead of adding the caster as a new target, it instead gives the target a portion of the mana cost of the spell, depending on the intensity of the absorption
				//spells absorbed in this way have no other effect on the target
				//this functionality is partially implemented; the elemental subtype functionality isn't there yet
				Aura *absorption = new AbsorptionAura(10+((int)intensity),20*((int)intensity));
				actor->auras.push(absorption);
				break;
			}
			case DAMAGING_TELEPORT:{
				//this spell is intended to teleport the target and damage those in a certain radius around his location
				int minX = actor->x + ((int)intensity)*2;
				int maxX = actor->x + ((int)intensity)*6;
				int minY = actor->y + ((int)intensity)*2;
				int maxY = actor->y + ((int)intensity)*6;
				int x = 0;
				int y = 0;
				bool locationFound = false;

				TCODList<Actor *> inRadius;
				engine.getAllActorsInRadius(inRadius,actor->x, actor->y, 3+((int)intensity));

				if (!inRadius.isEmpty()){

					for (Actor **iter = inRadius.begin();
						iter != inRadius.end(); iter++) {
						Actor *act1 = *iter;

						int damage = rng->getInt( (3+((int)intensity)*((int)intensity))*.8, (3+((int)intensity)*((int)intensity))*1.25); //choose a random amount of damage

						if (act1->destructible) act1->destructible->takeDamage(act1, caster, damage);
					}
				}

				while (!locationFound){
					x = rng->getInt(minX, maxX);
					y = rng->getInt(minY, maxY);

					if (engine.map->canWalk(x,y)) {
						locationFound = true;
					}
				}

				actor->x = x;
				actor->y = y;

				inRadius.clearAndDelete();
				break;
			}
			case WEAPON_ENHANCEMENT: {
				//implement a means to add additional damage dice to a weapon, with an elemental subtype associated with it
				//this will most likely be a struct and the weapon stores a TCODList of them
				break;
			}
			case PROTECT_FROM_HAZARDS: {
				//I'm not sure how to implement this; possibly just an Aura that a hazard object searches for before it damages you
				//not sure if that's the best way
				break;
			}
			case TELEPORT: {
				int minX = actor->x + ((int)intensity)*2;
				int maxX = actor->x + ((int)intensity)*6;
				int minY = actor->y + ((int)intensity)*2;
				int maxY = actor->y + ((int)intensity)*6;
				int x = 0;
				int y = 0;
				bool locationFound = false;
				
				while (!locationFound){
					x = rng->getInt(minX, maxX);
					y = rng->getInt(minY, maxY);

					if (engine.map->canWalk(x,y)) {
						locationFound = true;
					}
				}

				actor->x = x;
				actor->y = y;
				break;
			} 
			case DAMAGING_AURA: {
				Aura *damaging = new DamagingAura(10+((int)intensity), 2+2*((int)intensity), 3+((int)intensity), false);
				actor->auras.push(damaging);
				break;
			}
			case INTELLIGENT_DAMAGING_AURA: {
				Aura *damaging = new DamagingAura(10+((int)intensity), 2+2*((int)intensity), 3+((int)intensity), true);
				actor->auras.push(damaging);
				break;
			}
			case DAMAGING_PULL: {
				int x = actor->x;
				int y = actor->y;
				TCODLine::init(x, y, caster->x, caster->y);
				int steps = 0;
				do {
					if ( steps == 1+((int)intensity) || !engine.map->canWalk(actor->x,actor->y) ) {
						break;
					} else {
						actor->x = x;
						actor->y = y;
					}
				} while (TCODLine::step(&x,&y));

				int damage = rng->getInt( (3+5*((int)intensity))*.8, (3+5*((int)intensity))*1.25);	//choose a random amount of damage

				if (actor->destructible) actor->destructible->takeDamage(actor, caster, damage);

				break;
			}
			case DAMAGING_PUSH: {
				int startX = actor->x;
				int startY = actor->y;
				int endX = 2*actor->x - caster->x;
				int endY = 2*actor->y - caster->y;

				TCODLine::init(startX,startY,endX,endY);
				int steps = 0;

				do {
					if ( steps == 1+((int)intensity) || !engine.map->canWalk(actor->x,actor->y) ) {
						break;
					} else {
						actor->x = startX;
						actor->y = startY;
					}
				} while (TCODLine::step(&startX,&startY));

				int damage = rng->getInt( (3+5*((int)intensity))*.8, (3+5*((int)intensity))*1.25);	//choose a random amount of damage
				if (actor->destructible) actor->destructible->takeDamage(actor, caster, damage);

				break;
			}
			case STRAIGHT_DAMAGE: {
				int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				int damage = rng->getInt(.9*potentialDamage,1.11*potentialDamage);
				if (actor->destructible) actor->destructible->takeDamage(actor, caster, damage);
				
				break;
			}
		}
	}	

	targets.clearAndDelete();
	return true;
}

void TileSpell::chooseEffect(){
	int baseCost = cost;
	TCODRandom *rng = TCODRandom::getInstance();
	int effectSwitch = 0;
	if (targeting == SELF) {
		int fart = rng->getInt(1,500);  //make it very unlikely for a bad spell to become a self-targeting spell
		if (fart <= 499) {
			effectSwitch = rng->getInt(1,3);
		} else {
			effectSwitch = rng->getInt(1,7);
		}
	} else {
		effectSwitch = rng->getInt(1,7);
	}
	
	switch (effectSwitch) {
		case 1: 
			effect = LIGHT;
			expected = TILE;
			cost *= 1;
			break;
		case 2: 
			effect = ILLUSION;  //NOTE: illusion with an effect range of all_in_level might be bad, but then again it might not. Essentially, I plan to have it fill the targeted tiles with an illusory player
			expected = TILE;	//the player's illusory form could possess an elemental subtype. Creatures of opposing subtypes could be naturally hateful towards each other
			cost *= 1.2;
			break;
		case 3: 
			effect = ANTIMAGIC_ZONE;
			expected = TILE;
			cost *= 1.3;
			break;
		
		//THESE SPELLS ARE BAD FOR SELF-TARGETING SPELLS 
		case 4:
			effect = DARKNESS;  //no idea how an elemental darkness would work, but I'll figure it out
			expected = TILE;
			cost *= 1;
			break;
		case 5:
			effect = ALTER_TERRAIN;
			expected = TILE;
			cost *= 1.2;
			break;
		//THESE SPELL EFFECTS NEED SPECIFIC TARGETING TYPES
		case 6:
			effect = TELEPORTAL;
			expected = TILE;
			targeting = SINGLE_CREATURE_IN_SIGHT;
			cost = baseCost;
			cost *= 1;
			break;
		case 7:
			effect = SUMMON;
			expected = TILE;
			targeting = ADJACENT_TILE;
			cost = baseCost;
			cost *= 1.6;
			break;
		default: effect = NO_EFFECT; break;
	}
}

TileSpell::TileSpell( float target, SpellIntensity intensity, TargetSystem targeting, SpellEffect effect, 
		ExpectedTarget expected, float cost) :
	Spell(target,intensity,targeting,effect,expected,cost) {}

bool TileSpell::cast(Actor *caster){


	std::cout << "tile spell casted" << std::endl;
	return true;
}