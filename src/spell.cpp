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
		default: return false; break;
		case SELF: {
			targets.push(caster);
			break;
		}
		case ADJACENT_TILE: {
			engine.getAllActorsInRadius(targets, caster->x, caster->y, 1.6,this->actual,caster->hostile);
			if (targets.isEmpty()) {
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
				return false;	
			}
			break;	
		}
		case RANDOM_IN_LOS: {
			engine.getAllActorsInRadius(targets, caster->x, caster->y, engine.fovRadius,this->actual,caster->hostile);
			if (targets.isEmpty()) {
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
					return false;
				}

			} else {
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
					return false;
				} else {
					if ( ((this->actual == Spell::NEUTRAL) || (this->actual == Spell::ENEMY && actor->hostile != caster->hostile) || (this->actual == Spell::FRIENDLY && actor->hostile == caster->hostile)) ){
						targets.push(actor);
					}
				}
			} else {
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
				return false;
			}
			break;
		} case ALL_CREATURES_IN_LOS: {
			
			
			break;
		}
	}
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