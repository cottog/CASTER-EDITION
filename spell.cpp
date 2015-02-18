#include <iostream>
#include <cmath>
#include "libtcod.hpp"
#include "spell.hpp"

Spell::Spell(int level){
	TCODRandom *rng = TCODRandom::getInstance();
	coeffs = new int[4];
	powers = new int[4];
	values = new int[4];
	target = 0;
	int baseCost = 0;
	int powerfart = 0;
	int dingle = rng->getInt(1,100);

	//determine the possible intensities based on 
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
	baseCost = cost;
	
	switch(intensity) {
		case MINOR:
			powerfart = rng->getInt(2,4);
			for (int i = 0; i < 4; i++) {
				coeffs[i] = rng->getInt(powerfart,pow(2,powerfart));
			}
		break;
		case NORMAL:
			powerfart = rng->getInt(2,4);
			for (int i = 0; i < 4; i++) {
				coeffs[i] = rng->getInt(powerfart,pow(2,powerfart));
			}
		break;
		case MAJOR:
			powerfart = rng->getInt(2,3);
			for (int i = 0; i < 4; i++) {
				coeffs[i] = rng->getInt(powerfart,pow(2,powerfart));
			}
		break;
		case EPIC:
			powerfart = rng->getInt(2,3);
			for (int i = 0; i < 4; i++) {
				coeffs[i] = rng->getInt(powerfart,pow(2,powerfart));
			}
		break;
		default: break;
	}

	
	for (int i = 0; i < 4; i++) {
		powers[i] = powerfart;
	}
	
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
	int effectSwitch = 0;
	if (targeting == SELF) {
		int fart = rng->getInt(1,500);  //make it very unlikely for a bad spell to become a self-targeting spell
		if (fart <= 499) {
			effectSwitch = rng->getInt(1,20);
		} else {
			effectSwitch = rng->getInt(1,45);
		}
	} else {
		effectSwitch = rng->getInt(1,45);
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
			effect = LIGHT;
			expected = TILE;
			preferred = NEUTRAL;
			cost *= 1;
			break;
		case 9: 
			effect = ILLUSION;  //NOTE: illusion with an effect range of all_in_level might be bad, but then again it might not. Essentially, I plan to have it fill the targeted tiles with an illusory player
			expected = TILE;	//the player's illusory form could possess an elemental subtype. Creatures of opposing subtypes could be naturally hateful towards each other
			preferred = NEUTRAL;    //TODO: give a creature a will save?
			cost *= 1.2;
			break;
		case 10:
			effect = SIMULACRUM; //NOTE: simulacrum is similar to illusion in that it copies a creature, but this spell targets a creature and creates a simulacrum of it a short distance away on a random tile
			expected = CREATURE; // A simulacrum is intended to be much hardier than an illusion, and possibly to possess some attributes of its target
			preferred = ENEMY;      //NOTE: not actually harmful, but it should prefer 
			cost *= 1.4;
			break;               //simulacrum success should be dependent on how close the resources spent resemble the relative mixture of each element a creature possesses (a glass creature should not be copied by a simulacrum spell of the radiation subtype)  
		case 11:
			effect = SHIELD;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.1;
			break;
		case 12:
			effect = REFLECTION;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.2;
			break;
		case 13:
			effect = ABSORPTION;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.2;
			break;
		case 14: 
			effect = ANTIMAGIC_ZONE;
			expected = TILE;
			preferred = NEUTRAL;
			cost *= 1.3;
			break;
		case 15:
			effect = DAMAGING_TELEPORT;
			expected = CREATURE;
			preferred = NEUTRAL;
			cost *= 1.4;
			break;
		case 16:
			effect = WEAPON_ENHANCEMENT;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.3;
			break;
		case 17:
			effect = PROTECT_FROM_HAZARDS;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.2;
			break;
		case 18:
			effect = TELEPORT;
			expected = CREATURE;
			preferred = NEUTRAL;
			cost *= 1.2;
			break;
		case 19:
			effect = DAMAGING_AURA;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.3;
			break;
		case 20:
			effect = INTELLIGENT_DAMAGING_AURA;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.4;
			break;
		
		//THESE SPELLS ARE BAD FOR SELF-TARGETING SPELLS 
		case 21: 
			effect = DAMAGING_PULL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.2;
			break;
		case 22:
			effect = DAMAGING_PUSH;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 23:
			effect = STRAIGHT_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 24:
			effect = BLEED_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1;
			break;
		case 25:
			effect = STAT_DRAIN;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 26:
			effect = STAT_SAPPING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 27:
			effect = LIFE_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 28:
			effect = MANA_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 29:
			effect = INSTAKILL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 2.1;
			break;
		case 30: 
			effect = DEBUFFING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 31:
			effect = UNSUMMON;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 32:
			effect = BANISHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 33:
			effect = MANA_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 34:
			effect = DOOM_TIMER;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 35:
			effect = DARKNESS;  //no idea how an elemental darkness would work, but I'll figure it out
			expected = TILE;
			preferred = NEUTRAL;
			cost *= 1;
			break;
		case 36:
			effect = PERCENTILE_DAMAGE;
			expected = CREATURE;
			preferred =  ENEMY;
			cost *= 1.3;
			break;
		case 37:
			effect = RESURRECT;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.5;
			break;
		case 38:
			effect = PULL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1;
			break;
		case 39:
			effect = PUSH;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 41:
			effect = ALTER_TERRAIN;
			expected = TILE;
			preferred = NEUTRAL;
			cost *= 1.2;
			break;
		//THESE SPELL EFFECTS NEED SPECIFIC TARGETING TYPES
		case 40:
			effect = TELEPORTAL;
			expected = TILE;
			preferred = NEUTRAL;
			targeting = SINGLE_CREATURE_IN_SIGHT;
			cost = baseCost;
			cost *= 1;
			break;
		case 42:
			effect = ITEM_CREATION;
			expected = CREATURE;
			preferred = NEUTRAL;
			targeting = SELF;
			cost = baseCost;
			cost *= 1.4;
			break;
		case 43:
			effect = LINK;
			expected = CREATURE;
			preferred = ENEMY;
			targeting = SINGLE_CREATURE_IN_SIGHT;
			cost = baseCost;
			cost *= 1.1;
			break;
		case 44:
			effect = LIFE_TAP;
			expected = CREATURE;
			preferred = NEUTRAL;
			targeting = SELF;
			cost = baseCost;
			break;
		case 45:
			effect = SUMMON;
			expected = TILE;
			preferred = NEUTRAL;
			targeting = ADJACENT_TILE;
			cost = baseCost;
			cost *= 1.6;
			break;
		
		default: effect = NO_EFFECT; break;
	}

	if (targeting != SELF && expected == CREATURE) { //we will roll some dice and see if the targets its preferred type, or something else (which could be better or worse)
		int roll = rng->getInt(1,100);
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
	

float Spell::setTarget(){
	TCODRandom *rng = TCODRandom::getInstance();
	int low = 0, high = 0, lowest = 0, highest = 0, lowT = 0, highT = 0;
	
	cost = ceil(cost);
	
	low = rng->getInt((int).8*cost,(int)cost);
	high = rng->getInt((int)cost,(int)1.2*cost);
	

	lowest = coeffs[0] * pow(low,powers[0]);
	highest =coeffs[0] * pow(high,powers[0]);
	
	for (int i = 1; i < 4; i++) {
		lowT = coeffs[i] * pow(low,powers[i]);
		highT =coeffs[i] * pow(high,powers[i]);

		if (lowT < lowest) {
			lowest= lowT;
		}
		if (highT > highest) {
			highest = highT;
		}
	}
	target = rng->getInt(lowest,highest);
	return target;
	
}

bool Spell::input(){
	
	int input = 0;
	
	std::cout << "How much yellow bile do you stake?" << std::endl;
	std::cin >> input;
	values[0] = input;
	
	std::cout << "How much black bile do you stake?" << std::endl;
	std::cin >> input;
	values[1] = input;

	std::cout << "How much sanguine humor do you stake?" << std::endl;
	std::cin >> input;
	values[2] = input;

	std::cout << "How much phlegm do you stake?" << std::endl;
	std::cin >> input;
	values[3] = input;

	return true;
}

bool Spell::cast(){
	int w = 0;	
	int x = 0;
	int y = 0;
	int z = 0;	
	
	w = coeffs[0] * pow(values[0],powers[0]);
	x = coeffs[1] * pow(values[1],powers[1]);
	y = coeffs[2] * pow(values[2],powers[2]);
	z = coeffs[3] * pow(values[3],powers[3]);
	
	float result = 100*(((w + x + y + z)*1.0) / target);

	if (result < 80 ) {
		std::cout << "you have fizzled" << std::endl;
		return false;
	} else if (result > 120) {
		std::cout << "you have overloaded" << std::endl;
		return false;
	}

	std::cout << "success" << std::endl;
	return true;
}

