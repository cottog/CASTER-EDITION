#include <iostream>
#include <cmath>
#include "libtcod.hpp"
#include "spell.hpp"

Spell::Spell(float level){
	TCODRandom *rng = TCODRandom::getInstance();
	coeffs = new int[4];
	powers = new int[4];
	values = new int[4];
	operators = new char[3];
	target = 0;
	int powerfart = 0;
	
	//determine the possible intensities based on 
	if (level <= 11) {
		intensity = MINOR;
		cost = 1;
	} else if (level <= 25) {
		int dingle = rng->getInt(1,100);
		if (dingle <= 55) {
			intensity = NORMAL;
			cost = 3;
		} else {
			intensity = MINOR;
			cost = 1;
		}
	} else if (level <= 35) {
		int dingle = rng->getInt(1,100);
		if (dingle <= 30) {
			intensity = MAJOR;
			cost = 9;
		} else {
			intensity = NORMAL;
			cost = 3;
		}
	} else if (level <= 45) {
		int dingle = rng->getInt(1,100);
		if (dingle <= 60) {
			intensity = MAJOR;
			cost = 9;
		} else {
			intensity = NORMAL;
			cost = 3;
		}
	} else {
		intensity = EPIC;
		cost = 15;
	}
	
	
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
	}

	
	for (int i = 0; i < 4; i++) {
		powers[i] = powerfart;
	}

	for (int i = 0; i<3; i++) {
		int opp = rng->getInt(1,1);
		
		//this will only result in operators[] being filled with plus signs. I have yet to find a good way to mix in '*'
		//multiplication results in too much of a range for the target number, and gives much greater weight to only a few variables
		switch(opp){
			case 1: operators[i] = '+'; break;
			case 2: operators[i] = '*'; break; 
			default: break;
		}
		std::cout << operators[i] << std::endl;	
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
			cost *= 2;
			break;
		case 6: 
			targeting = RANDOM_IN_LOS;
			break;
		case 7:
			targeting = RANDOM_IN_LEVEL;
			break;
		case 8:
			targeting = ALL_ADJACENT_TILES;
			break;
		case 9:
			targeting = ALL_CREATURES_CHAINING;
			break;
		case 10:
			targeting = ALL_CREATURES_IN_LEVEL;
			break;
		case 11:
			targeting = ALL_CREATURES_IN_LINE;
			break;
		case 12:
			targeting = SINGLE_CREATURES_IN_SIGHT;
			break;
		case 13:
			targeting = ALL_CREATURES_IN_RADIUS;
			break;
		case 14: 
			targeting = X_CREATURES_IN_SIGHT;
			break;
		case 15:
			targeting = ALL_CREATURES_IN_LOS;
			break;
		default: targeting = NONE; break;
	}
	if (targeting == SELF) {
		int fart = rng->getInt(1,500);  //make it very unlikely for a bad spell to become a self-targeting spell
		if (fart <= 499) {
			int effectSwitch = rng->getInt(1,20);
		} else {
			int effectSwitch = rng->getInt(1,41);
		}
	} else {
		int effectSwitch = rng->getInt(1,41);
	}
	
	switch (effectSwitch) {
		case 1:
			effect = STRAIGHT_HEAL;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 2:
			effect = HEAL_OVER_TIME;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 3:
			effect = STAT_BOOST;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 4:
			effect = CURE_DEBUFFS;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 5: 
			effect = DETECTION;
			expected = CREATURE;
			preferred = NEUTRAL;
			break;
		case 6:
			effect = POLYMORPH;
			expected = CREATURE;
			preferred = NEUTRAL;
			break;
		case 7: 
			effect = SHAPE_SHIFTING;
			expected = CREATURE;
			preferred = NEUTRAL;
			break;
		case 8: 
			effect = LIGHT;
			expected = TILE;
			preferred = NEUTRAL;
			break;
		case 9: 
			effect = ILLUSION;  //NOTE: illusion with an effect range of all_in_level might be bad, but then again it might not. Essentially, I plan to have it fill the targeted tiles with an illusory player
			expected = TILE;	//the player's illusory form could possess an elemental subtype. Creatures of opposing subtypes could be naturally hateful towards each other
			preferred = NEUTRAL;    //TODO: give a creature a will save?
			break;
		case 10:
			effect = SIMULACRUM; //NOTE: simulacrum is similar to illusion in that it copies a creature, but this spell targets a creature and creates a simulacrum of it a short distance away on a random tile
			expected = CREATURE; // A simulacrum is intended to be much hardier than an illusion, and possibly to possess some attributes of its target
			preferred = ENEMY;      //NOTE: not actually harmful, but it should prefer 
			break;               //simulacrum success should be dependent on how close the resources spent resemble the relative mixture of each element a creature possesses (a glass creature should not be copied by a simulacrum spell of the radiation subtype)  
		case 11:
			effect = SHIELD;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 12:
			effect = REFLECTION;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 13:
			effect = ABSORPTION;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 14: 
			effect = ANTIMAGIC_ZONE;
			expected = TILE;
			preferred = NEUTRAL;
			break;
		case 15:
			effect = DAMAGING_TELEPORT;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 16: 
			effect = DAMAGING_PULL;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 17:
			effect = DAMAGING_PUSH;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 18:
			effect = WEAPON_ENHANCEMENT;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 19:
			effect = PROTECT_FROM_HAZARDS;
			expected = CREATURE;
			preferred = FRIENDLY;
			break;
		case 20:
			effect = TELEPORT;
			expected = CREATURE;
			preferred = NEUTRAL;
			break;
			
		//THESE SPELLS ARE BAD FOR SELF-TARGETING SPELLS 
		case 21:
			effect = STRAIGHT_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 22:
			effect = BLEED_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 23:
			effect = STAT_DRAIN;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 24:
			effect = STAT_SAPPING;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 25:
			effect = LIFE_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 26:
			effect = MANA_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 27:
			effect = INSTAKILL;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 28: 
			effect = DEBUFFING;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 29:
			effect = UNSUMMON;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 30:
			effect = BANISHING;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 31:
			effect = MANA_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 32:
			effect = DOOM_TIMER;
			expected = CREATURE;
			preferred = ENEMY;
			break;
		case 33:
			effect = DARKNESS;  //no idea how an elemental darkness would work
			expected = TILE;
			preferred = NEUTRAL;
			break;
		case 34:
			effect = PERCENTILE_DAMAGE;
			expected = CREATURE;
			preferred =  ENEMY;
			break;
		case 35:
			effect = RESURRECT;
			expected = CREATURE;
			preferred = NEUTRAL;
			break;
			
		//THESE SPELL EFFECTS NEED SPECIFIC TARGETING TYPES
		case 36:
			effect = TELEPORTAL;
			expected = TILE;
			preferred = NEUTRAL;
			break;
		case 37:
			effect = ALTER_TERRAIN;
			expected = TILE;
			preferred = NEUTRAL;
			break;
		case 38:
			effect = ITEM_CREATION;
			expected = ITEM;
			preferred = NEUTRAL;
			break;
		case 39:
			effect = LINK;
			expected = CREATURE;
			preferred = NEUTRAL;
			break;
		case 40:
			effect = LIFE_TAP;
			expected = CREATURE;
			preferred = NEUTRAL;
			break;
		case 41:
			effect = SUMMON;
			expected = TILE;
			preferred = NEUTRAL;
			break;
	}
	
}

float Spell::setTarget(){
	TCODRandom *rng = TCODRandom::getInstance();
	int low = 0, high = 0, lowest = 0, highest = 0, lowT = 0, highT = 0;
	
	switch (intensity) {
		case MINOR:
			low = rng->getInt(3,4);
			high = rng->getInt(5,6);
		break;
		case NORMAL:
			low = rng->getInt(13,15);
			high = rng->getInt(16,17);
		break;
		case MAJOR:
			low = rng->getInt(43,48);
			high = rng->getInt(49,54);
		break;
	}

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

float Spell::cast(){
	int w = 0;	
	int x = 0;
	int y = 0;
	int z = 0;	
	
	w = coeffs[0] * pow(values[0],powers[0]);
	x = coeffs[1] * pow(values[1],powers[1]);
	y = coeffs[2] * pow(values[2],powers[2]);
	z = coeffs[3] * pow(values[3],powers[3]);
	

	switch(operators[0]){
		case '+':
			switch(operators[1]){
				case '+':
					switch(operators[2]){
						case '+': return 100*((w + x + y + z) / target);
						case '*': return 100*((w + x + y * z) / target);
						default: break;
					}
				case '*':
					switch(operators[2]){
						case '+': return 100*((w + x * y + z) / target);
						case '*': return 100*((w + x * y * z) / target);
						default: break;
					}
				default: break;
			}

		case '*':
			switch(operators[1]){
				case '+':
					switch(operators[2]){
						case '+': return 100*((w * x + y + z) / target);
						case '*': return 100*((w * x + y * z) / target);
						default: break;
					}
				case '*':
					switch(operators[2]){
						case '+': return 100*((w * x * y + z) / target);
						case '*': return 100*((w * x * y * z) / target);
						default: break;
					}
				default: break;
			}
		
		default: break;	
	}	

	return 100*(((w + x + y + z)*1.0) / target);
}


