#include "libtcod.hpp"
#include "main.hpp"

Spell::Spell(float target, SpellIntensity intensity, TargetSystem targeting, SpellEffect effect, 
		ExpectedTarget expected , float cost) :
	target(target), intensity(intensity),targeting(targeting),effect(effect),expected(expected),cost(cost) {
		name = strdup("");
	}


float Spell::setTarget(){
	TCODRandom *rng = TCODRandom::getInstance();
	target = 0;

	for (int x=0; x<4; x++){
		target += (rng->getInt(0,25));
	}
	return target;
}

Spell *Spell::newSpell(Actor *caster){
	Spell *spell = NULL;
	TCODRandom *rng = TCODRandom::getInstance();
	int typeSwitch = rng->getInt(1,45);
	if (typeSwitch <= 38){
		spell = new CreatureSpell();
	} else {
		spell = new TileSpell(); 
	}
	spell->chooseIntensity(caster->xpLevel);
	spell->setTarget();
	spell->chooseTargetSystem();
	spell->chooseEffect();
	spell->setName();

	return spell;
}

Spell *Spell::create(TCODZip &zip){
	Spell *spell = NULL;
	ExpectedTarget spellType = (ExpectedTarget)zip.getInt();

	switch(spellType){
		case CREATURE: spell = new CreatureSpell(); break;
		case TILE: spell = new TileSpell(); break;
		default: break;
	}
	spell->load(zip);

	return spell;
}

void Spell::setName(){
	char buf[64] = "";
	TCODRandom *rng = TCODRandom::getInstance();
	int nameSwitch = rng->getInt(1,17);
	switch (nameSwitch){
		case 1: strcat(buf,"Atlantes'"); break;
		case 2: strcat(buf,"Circe's"); break;
		case 3: strcat(buf,"Merlin's"); break;
		case 4: strcat(buf,"Torgrod's"); break;
		case 5: strcat(buf,"Medea's"); break;
		case 6: strcat(buf,"Alatar's"); break;
		case 7: strcat(buf,"Tom-Tom's"); break;
		case 8: strcat(buf,"Silent's"); break;
		case 9: strcat(buf,"Prospero's"); break;
		case 10: strcat(buf,"Thoth-Amon's"); break;
		case 11: strcat(buf,"Yara's"); break;
		case 12: strcat(buf,"Hecate's"); break;
		case 13: strcat(buf,"Vegoia's"); break;
		case 14: strcat(buf,"Vyasa's"); break;
		case 15: strcat(buf,"Abramelin's"); break;
		case 16: strcat(buf,"Balzak's"); break;
		case 17: strcat(buf,"K'aarna's"); break;
	}
	switch (intensity) {
		case MINOR: strcat(buf," Minor"); break;
		case MAJOR: strcat(buf," Major"); break;
		case EPIC: strcat(buf," Epic"); break;
		case NORMAL: break;
		default: break;
	}
	nameSwitch = rng->getInt(1,2);
	switch (targeting){
		case SELF: strcat(buf,nameSwitch? " Personal":" Secret"); break;
		case ADJACENT_TILE: strcat(buf,nameSwitch? " Short-Range":" Limited"); break;
		case BOLT_SPELL: strcat(buf,nameSwitch? " Bolt of":" Flash of"); break;
		case VISUAL_HOLOCAUST: strcat(buf,nameSwitch? " Discerning":" Astute"); break;
		case LEVEL_WIDE_HOLOCAUST: strcat(buf,nameSwitch? " Whisper of":" Murmur of"); break;
		case RANDOM_IN_LOS: strcat(buf,nameSwitch? " Eye of":" Leer of"); break;
		case RANDOM_IN_LEVEL: strcat(buf,nameSwitch? " Random":" Arbitrary"); break;
		case ALL_ADJACENT_TILES: strcat(buf,nameSwitch? " Aura of":" Ambient"); break;
		case ALL_CREATURES_CHAINING: strcat(buf,nameSwitch? " Chain of":" Arcing"); break;
		case ALL_CREATURES_IN_LEVEL: strcat(buf,nameSwitch? " Shout of":" Roar of"); break;
		case  ALL_CREATURES_IN_LINE: strcat(buf,nameSwitch? " Ray of":" Line of"); break;
		case ALL_CREATURES_IN_RADIUS: strcat(buf,nameSwitch? " Orb of":" Ball of"); break;
		case SINGLE_CREATURE_IN_SIGHT: strcat(buf,nameSwitch? expected==TILE? " Locus of" : " Wink of": expected==TILE? " Point of" : " Glimmer of"); break;
		case X_CREATURES_IN_SIGHT: strcat(buf,nameSwitch? " Thoughts of":" Image of"); break;
		case ALL_CREATURES_IN_LOS: strcat(buf,nameSwitch? " Gaze of":" Visage of"); break;
		default: break;
	}

	switch(effect){
		case STRAIGHT_HEAL: strcat(buf, " Healing"); break;
		case HEAL_OVER_TIME: strcat(buf, " Relaxation"); break;
		case STAT_BOOST: strcat(buf, " Enhancement"); break;
		case CURE_DEBUFFS: strcat(buf, " Revitalization"); break;
		case DETECTION: strcat(buf, " Revelation"); break;
		case POLYMORPH: strcat(buf, " Metamorphosis"); break;
		case SHAPE_SHIFTING: strcat(buf, " Transfiguration"); break;
		case LIGHT: strcat(buf, " Illumination"); break;
		case ILLUSION: strcat(buf, " Illusion"); break;
		case SIMULACRUM: strcat(buf, " Duplication"); break;
		case SHIELD: strcat(buf, " Protection"); break;
		case REFLECTION: strcat(buf, " Reflection"); break;
		case ABSORPTION: strcat(buf, " Consumption"); break;
		case ANTIMAGIC_ZONE: strcat(buf, " Negation"); break;
		case DAMAGING_TELEPORT: strcat(buf, " Harmful Dislocation"); break;
		case WEAPON_ENHANCEMENT: strcat(buf, " Improvement"); break;
		case PROTECT_FROM_HAZARDS: strcat(buf, " Resistance"); break;
		case TELEPORT: strcat(buf, " Dislocation"); break;
		case DAMAGING_AURA: strcat(buf, " Harm"); break;
		case INTELLIGENT_DAMAGING_AURA: strcat(buf, " Selective Harm"); break;
		case CONTROLLED_TELEPORT: strcat(buf, " Translocation"); break;
		case DAMAGING_PULL: strcat(buf, " Harmful Tugging"); break;
		case DAMAGING_PUSH: strcat(buf, " Harmful Nudging"); break;
		case PULL: strcat(buf, " Tugging"); break;
		case PUSH: strcat(buf, " Nudging"); break;
		case STRAIGHT_DAMAGE: strcat(buf, " Destruction"); break;
		case BLEED_DAMAGE: strcat(buf, " Stinging"); break;
		case STAT_DRAIN: strcat(buf, " Weakness"); break;
		case STAT_SAPPING: strcat(buf, " Borrowed Vitality"); break;
		case LIFE_LEECHING: strcat(buf, " Stolen Vitality"); break;
		case MANA_LEECHING: strcat(buf, " Stolen Mentality"); break;
		case INSTAKILL: strcat(buf, " Annihilation"); break;
		case DEBUFFING: strcat(buf, " Devitalization"); break;
		case UNSUMMON: strcat(buf, " Returning"); break;
		case BANISHING: strcat(buf, " Banishing"); break;
		case MANA_DAMAGE: strcat(buf, " Mental Strain"); break;
		case DOOM_TIMER: strcat(buf, " Certain Doom"); break;
		case DARKNESS: strcat(buf, " Obscurity"); break;
		case PERCENTILE_DAMAGE: strcat(buf, " Measured Destruction"); break;
		case RESURRECT: strcat(buf, " Rebirth"); break;
		case TELEPORTAL: strcat(buf, " Stability"); break;
		case ALTER_TERRAIN: strcat(buf, " Terraforming"); break;
		case LINK: strcat(buf, " Union"); break;
		case SUMMON: strcat(buf, " Calling"); break;
		case LIFE_TAP: strcat(buf, " Sacrifice"); break;
		default: break;
	}
	this->name = buf;
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

void CreatureSpell::save(TCODZip &zip){
	zip.putInt(expected);
	zip.putFloat(target);
	zip.putInt(intensity);
	zip.putInt(targeting);
	zip.putInt(effect);
	zip.putFloat(cost);
	zip.putInt(preferred);
	zip.putInt(actual);
}

void CreatureSpell::load(TCODZip &zip){
	target = zip.getFloat();
	intensity = (SpellIntensity)zip.getInt();
	targeting = (TargetSystem)zip.getInt();
	effect = (SpellEffect)zip.getInt();
	cost = zip.getFloat();
	preferred = (TargetType)zip.getInt();
	actual = (TargetType)zip.getInt();
}

void CreatureSpell::chooseEffect(){
	int baseCost = cost;
	TCODRandom *rng = TCODRandom::getInstance();
	int effectSwitch = 0;
	if (targeting == SELF) {
		int fart = rng->getInt(1,500);  //make it very unlikely for a bad spell to become a self-targeting spell
		if (fart <= 499) {
			effectSwitch = rng->getInt(1,18);
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
		case 18:
			effect = CONTROLLED_TELEPORT;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.5;
			break;
		//THESE SPELLS ARE BAD FOR SELF-TARGETING SPELLS 
		case 19: 
			effect = DAMAGING_PULL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.2;
			break;
		case 20:
			effect = DAMAGING_PUSH;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 21:
			effect = STRAIGHT_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 22:
			effect = BLEED_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1;
			break;
		case 23:
			effect = STAT_DRAIN;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 24:
			effect = STAT_SAPPING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 25:
			effect = LIFE_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 26:
			effect = MANA_LEECHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 27:
			effect = INSTAKILL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 2.1;
			break;
		case 28: 
			effect = DEBUFFING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 29:
			effect = UNSUMMON;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.3;
			break;
		case 30:
			effect = BANISHING;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 31:
			effect = MANA_DAMAGE;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		case 32:
			effect = DOOM_TIMER;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.4;
			break;
		case 33:
			effect = PERCENTILE_DAMAGE;
			expected = CREATURE;
			preferred =  ENEMY;
			cost *= 1.3;
			break;
		case 34:
			effect = RESURRECT;
			expected = CREATURE;
			preferred = FRIENDLY;
			cost *= 1.5;
			break;
		case 35:
			effect = PULL;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1;
			break;
		case 36:
			effect = PUSH;
			expected = CREATURE;
			preferred = ENEMY;
			cost *= 1.1;
			break;
		//THESE SPELL EFFECTS NEED SPECIFIC TARGETING TYPES
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
				//if(actor->caster) actor->caster->gainMana(cost*(aura->bonus/100));	//gain mana proportial to cost of spell, dependent on absorption magnitude
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
				Aura *hot = new HealAura(10+((int)intensity),4+3*((int)intensity),Aura::ITERABLE); //this will have to change when auras are changed around
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
			case BLEED_DAMAGE: {
				Aura *hot = new HealAura(10+((int)intensity),-1*(4+3*((int)intensity)),Aura::ITERABLE); 
				actor->auras.push(hot);

				break;
			}
			case STAT_DRAIN: {
				//this will be added once elemental subtypes are all tied to a (stat or stats)
				break;
			}
			case STAT_SAPPING: {
				//this will be added at the same time as the above
				break;
			}
			case LIFE_LEECHING: {
				//damage the target and heal the caster for a portion of that damage
				int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				int damage = rng->getInt(.45*potentialDamage,.555*potentialDamage);	//half the damage of the straight-damage spell
				if (actor->destructible) actor->destructible->takeDamage(actor, caster, damage);

				if (caster->destructible) caster->destructible->heal(.5*damage);

				break;
			}
			case MANA_LEECHING: {
				/*
				int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				int damage = rng->getInt(.45*potentialDamage,.555*potentialDamage);	//half the damage of the straight-damage spell
				if (actor->caster){
					actor->caster->gainMana(-1*damage);
				} else {
					continue;
				}
				if (caster->caster) caster->caster->gainMana(.75*damage);
				*/

				break;
			}
			case INSTAKILL: {
				if (actor->destructible) actor->destructible->takeDamage(actor, caster, 250*((int)intensity)-1);	//should kill most Actors, but might not actually kill very big bosses, as intended
				break;
			}
			case DEBUFFING: {
				//same spiel concerning elemental subtypes and buffs/debuffs
				break;
			}
			case UNSUMMON: {
				//think of the best way to identify whether or not a specific actor is a summoned creature and get rid of them
				//prolly just put a bool in the Actor class, or put a String tag member variable that can be used to identify summons, uniques, regular mobs, etc
				//the tag field will be useful to differentiate between mobs that need "the" in front of their names or not
				break;
			}
			case BANISHING: {
				//right now this simply deletes the target, but in the future, it should push the target to a list of banished creatures, which will be used to populate the Abyss
				engine.actors.remove(actor);
				delete actor;

				break;
			}
			case MANA_DAMAGE:{
				//int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				//int damage = rng->getInt(.45*potentialDamage,.555*potentialDamage);	//half the damage of the straight-damage spell
				//if(actor->caster) actor->caster->gainMana(-1*damage);

				break;
			}
			case DOOM_TIMER: {
				//this adds a DoomAura to the target, which counts down and kills the target at the end of the countdown; the higher the intensity, the lower the timer
				//similar to instakill, this DoomAura simply inflicts a lot of damage to the target, so it may not kill big enemies

				Aura *doom = new DoomAura(13-2*((int)intensity),250*((int)intensity)-1);
				actor->auras.push(doom);
				break;
			}
			case PERCENTILE_DAMAGE: {
				//deal a percentage of the target's health as damage; the higher the intensity, the bigger the percentage

				if (actor->destructible) actor->destructible->takeDamage(actor, caster, actor->destructible->maxHp*(.2*((int)intensity) ));
				break;
			}
			case RESURRECT: {
				if (actor->destructible && actor->destructible->isDead()) {
					actor->destructible->resurrect(actor, 0.2*((int)intensity));	//resurrect the target and give it .2,.4,.6, or .8 of maxHp as health
				}
				break;
			}
			case CONTROLLED_TELEPORT: {
				bool locationFound = false; 

				while (!locationFound) {
					int x = caster->x;
					int y = caster->y;
					
					if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
						if (engine.map->canWalk(x,y)){
							locationFound = true;
							actor->x = x;
							actor->y = y;
						}
					}
				}	
				break;
			}
			case PULL: {
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
				//I thought at first, why not just pass the actual actor here by reference, but there's no way to step back (or peek ahead) in a TCODLine that I know of
				//so once it steps forward and the actor shouldin't be there, its all bad news

				break;
			}
			case PUSH: {
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

				break;
			}
			case LINK: {
				//prolly store an actor pointer in the Caster class to handle Links.
				//since this is restricted to target single creatures, it shouldn't need more than a single pointer
				//however, if an actor casts this on themself once and then does it again, the way I plan to implement it will cause an infinite loop
				//I think I will prevent everyone but the player from doing so
				//the player will be warned about possible "magical instabilities" caused by their action, but they will be allowed to do it
				break;
			}
			case LIFE_TAP: {
				int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				int damage = rng->getInt(.9*potentialDamage,1.11*potentialDamage);
				if (actor->destructible) actor->destructible->takeDamage(actor,NULL,damage);
				// if (caster->caster) caster->caster->gainMana(damage);
				break;
			}
		}
	}	

	targets.clearAndDelete();
	return true;
}

void TileSpell::save(TCODZip &zip){
	zip.putInt(expected);
	zip.putFloat(target);
	zip.putInt(intensity);
	zip.putInt(targeting);
	zip.putInt(effect);
	zip.putFloat(cost);
}

void TileSpell::load(TCODZip &zip){
	target = zip.getFloat();
	intensity = (SpellIntensity)zip.getInt();
	targeting = (TargetSystem)zip.getInt();
	effect = (SpellEffect)zip.getInt();
	cost = zip.getFloat();
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


	return true;
}