#include "libtcod.hpp"
#include "main.hpp"

Spell::Spell(float target, SpellCastingConstants::SpellIntensity intensity, SpellCastingConstants::TargetSystem targeting, SpellCastingConstants::SpellEffect effect, 
	SpellCastingConstants::TargetType targetType, float cost) :
	target(target),intensity(intensity),targeting(targeting),effect(effect),targetType(targetType),cost(cost) {
		name = "";
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
	spell->chooseIntensity(caster->xpLevel);
	spell->setTarget();
	spell->chooseTargetSystem();
	spell->chooseEffect();
	spell->setName();
	engine.gui->message(TCODColor::lightGrey,"%s",spell->getName());
	engine.gui->message(TCODColor::white,"%f %d %f %s",spell->target,spell->targeting,spell->cost,spell->getName());
	return spell;
}

Spell *Spell::create(TCODZip &zip){
	Spell *spell = NULL;
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
		case SpellCastingConstants::MINOR: strcat(buf," Minor"); break;
		case SpellCastingConstants::MAJOR: strcat(buf," Major"); break;
		case SpellCastingConstants::EPIC: strcat(buf," Epic"); break;
		case SpellCastingConstants::NORMAL: break;
		default: break;
	}
	nameSwitch = rng->getInt(1,2);
	switch (targeting){
		case SpellCastingConstants::SELF: strcat(buf,nameSwitch? " Personal":" Secret"); break;
		case SpellCastingConstants::ADJACENT_TILE: strcat(buf,nameSwitch? " Short-Range":" Limited"); break;
		case SpellCastingConstants::BOLT_SPELL: strcat(buf,nameSwitch? " Bolt of":" Flash of"); break;
		case SpellCastingConstants::VISUAL_HOLOCAUST: strcat(buf,nameSwitch? " Discerning":" Astute"); break;
		case SpellCastingConstants::LEVEL_WIDE_HOLOCAUST: strcat(buf,nameSwitch? " Whisper of":" Murmur of"); break;
		case SpellCastingConstants::RANDOM_IN_LOS: strcat(buf,nameSwitch? " Eye of":" Leer of"); break;
		case SpellCastingConstants::RANDOM_IN_LEVEL: strcat(buf,nameSwitch? " Random":" Arbitrary"); break;
		case SpellCastingConstants::ALL_ADJACENT_TILES: strcat(buf,nameSwitch? " Aura of":" Ambient"); break;
		case SpellCastingConstants::ALL_CREATURES_CHAINING: strcat(buf,nameSwitch? " Chain of":" Arcing"); break;
		case SpellCastingConstants::ALL_CREATURES_IN_LEVEL: strcat(buf,nameSwitch? " Shout of":" Roar of"); break;
		case SpellCastingConstants:: ALL_CREATURES_IN_LINE: strcat(buf,nameSwitch? " Ray of":" Line of"); break;
		case SpellCastingConstants::ALL_CREATURES_IN_RADIUS: strcat(buf,nameSwitch? " Orb of":" Ball of"); break;
		case SpellCastingConstants::SINGLE_CREATURE_IN_SIGHT: strcat(buf,nameSwitch? " Locus of":" Wink of"); break;
		case SpellCastingConstants::X_CREATURES_IN_SIGHT: strcat(buf,nameSwitch? " Thoughts of":" Image of"); break;
		case SpellCastingConstants::ALL_CREATURES_IN_LOS: strcat(buf,nameSwitch? " Gaze of":" Visage of"); break;
		default: break;
	}

	switch(effect){
		case SpellCastingConstants::STRAIGHT_HEAL: strcat(buf, " Healing"); break;
		case SpellCastingConstants::HEAL_OVER_TIME: strcat(buf, " Relaxation"); break;
		case SpellCastingConstants::STAT_BOOST: strcat(buf, " Enhancement"); break;
		case SpellCastingConstants::CURE_DEBUFFS: strcat(buf, " Revitalization"); break;
		case SpellCastingConstants::DETECTION: strcat(buf, " Revelation"); break;
		case SpellCastingConstants::POLYMORPH: strcat(buf, " Metamorphosis"); break;
		case SpellCastingConstants::SHAPE_SHIFTING: strcat(buf, " Transfiguration"); break;
		case SpellCastingConstants::LIGHT: strcat(buf, " Illumination"); break;
		case SpellCastingConstants::ILLUSION: strcat(buf, " Illusion"); break;
		case SpellCastingConstants::SIMULACRUM: strcat(buf, " Duplication"); break;
		case SpellCastingConstants::SHIELD: strcat(buf, " Protection"); break;
		case SpellCastingConstants::REFLECTION: strcat(buf, " Reflection"); break;
		case SpellCastingConstants::ABSORPTION: strcat(buf, " Consumption"); break;
		case SpellCastingConstants::ANTIMAGIC_ZONE: strcat(buf, " Negation"); break;
		case SpellCastingConstants::DAMAGING_TELEPORT: strcat(buf, " Harmful Dislocation"); break;
		case SpellCastingConstants::WEAPON_ENHANCEMENT: strcat(buf, " Improvement"); break;
		case SpellCastingConstants::PROTECT_FROM_HAZARDS: strcat(buf, " Resistance"); break;
		case SpellCastingConstants::TELEPORT: strcat(buf, " Dislocation"); break;
		case SpellCastingConstants::DAMAGING_AURA: strcat(buf, " Harm"); break;
		case SpellCastingConstants::INTELLIGENT_DAMAGING_AURA: strcat(buf, " Selective Harm"); break;
		case SpellCastingConstants::CONTROLLED_TELEPORT: strcat(buf, " Translocation"); break;
		case SpellCastingConstants::DAMAGING_PULL: strcat(buf, " Harmful Tugging"); break;
		case SpellCastingConstants::DAMAGING_PUSH: strcat(buf, " Harmful Nudging"); break;
		case SpellCastingConstants::PULL: strcat(buf, " Tugging"); break;
		case SpellCastingConstants::PUSH: strcat(buf, " Nudging"); break;
		case SpellCastingConstants::STRAIGHT_DAMAGE: strcat(buf, " Destruction"); break;
		case SpellCastingConstants::BLEED_DAMAGE: strcat(buf, " Stinging"); break;
		case SpellCastingConstants::STAT_DRAIN: strcat(buf, " Weakness"); break;
		case SpellCastingConstants::STAT_SAPPING: strcat(buf, " Borrowed Vitality"); break;
		case SpellCastingConstants::LIFE_LEECHING: strcat(buf, " Stolen Vitality"); break;
		case SpellCastingConstants::MANA_LEECHING: strcat(buf, " Stolen Mentality"); break;
		case SpellCastingConstants::INSTAKILL: strcat(buf, " Annihilation"); break;
		case SpellCastingConstants::DEBUFFING: strcat(buf, " Devitalization"); break;
		case SpellCastingConstants::UNSUMMON: strcat(buf, " Returning"); break;
		case SpellCastingConstants::BANISHING: strcat(buf, " Banishing"); break;
		case SpellCastingConstants::MANA_DAMAGE: strcat(buf, " Mental Strain"); break;
		case SpellCastingConstants::DOOM_TIMER: strcat(buf, " Certain Doom"); break;
		case SpellCastingConstants::DARKNESS: strcat(buf, " Obscurity"); break;
		case SpellCastingConstants::PERCENTILE_DAMAGE: strcat(buf, " Measured Destruction"); break;
		case SpellCastingConstants::RESURRECT: strcat(buf, " Rebirth"); break;
		case SpellCastingConstants::TELEPORTAL: strcat(buf, " Stability"); break;
		case SpellCastingConstants::ALTER_TERRAIN: strcat(buf, " Terraforming"); break;
		case SpellCastingConstants::LINK: strcat(buf, " Union"); break;
		case SpellCastingConstants::SUMMON: strcat(buf, " Calling"); break;
		case SpellCastingConstants::LIFE_TAP: strcat(buf, " Sacrifice"); break;
		default: break;
	}
	engine.gui->message(TCODColor::red,"%s",buf);
	this->name = buf;
}

void Spell::chooseTargetSystem(){
	TCODRandom *rng = TCODRandom::getInstance();
	int targetSwitch = rng->getInt(1,15);
	targeting = (SpellCastingConstants::TargetSystem)(targetSwitch);
	targetSystem = SpellFactories::TargetingSystemFactory::Build(targeting);
}

void Spell::chooseIntensity(int level){
	TCODRandom *rng = TCODRandom::getInstance();
	int dingle = rng->getInt(1,100);

	//determine the possible intensities based on character level
	if (level <= 11) {
		intensity = SpellCastingConstants::MINOR;
		cost = 3;
	} else if (level <= 25) {
		if (dingle <= 55) {
			intensity = SpellCastingConstants::NORMAL;
			cost = 9;
		} else {
			intensity = SpellCastingConstants::MINOR;
			cost = 3;
		}
	} else if (level <= 35) {
		if (dingle <= 30) {
			intensity = SpellCastingConstants::MAJOR;
			cost = 26;
		} else {
			intensity = SpellCastingConstants::NORMAL;
			cost = 9;
		}
	} else if (level <= 45) {
		if (dingle <= 60) {
			intensity = SpellCastingConstants::MAJOR;
			cost = 26;
		} else {
			intensity = SpellCastingConstants::NORMAL;
			cost = 9;
		}
	} else {
		intensity = SpellCastingConstants::EPIC;
		cost = 54;
	}
}

void Spell::save(TCODZip &zip){
	zip.putFloat(target);
	zip.putInt(intensity);
	zip.putInt(targeting);
	zip.putInt(effect);
	zip.putFloat(cost);
	zip.putInt(targetType);
	zip.putString(name.c_str());
}

void Spell::load(TCODZip &zip){
	target = zip.getFloat();
	intensity = (SpellCastingConstants::SpellIntensity)zip.getInt();
	
	targeting = (SpellCastingConstants::TargetSystem)zip.getInt();
	targetSystem = SpellFactories::TargetingSystemFactory::Build(targeting);
	
	effect = (SpellCastingConstants::SpellEffect)zip.getInt();
	cost = zip.getFloat();
	targetType = (SpellCastingConstants::TargetType)zip.getInt();
	name = zip.getString();
}

void Spell::chooseEffect(){
	int baseCost = cost;
	TCODRandom *rng = TCODRandom::getInstance();
	int effectSwitch = 0;
	if (targeting == SpellCastingConstants::SELF) {
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
			effect = SpellCastingConstants::STRAIGHT_HEAL;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1;
			break;
		case 2:
			effect = SpellCastingConstants::HEAL_OVER_TIME;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= .8;
			break;
		case 3:
			effect = SpellCastingConstants::STAT_BOOST;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1;
			break;
		case 4:
			effect = SpellCastingConstants::CURE_DEBUFFS;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.1;
			break;
		case 5: 
			effect = SpellCastingConstants::DETECTION;
			targetType = SpellCastingConstants::NEUTRAL;
			cost *= 1.1;
			break;
		case 6:
			effect = SpellCastingConstants::POLYMORPH;
			targetType = SpellCastingConstants::NEUTRAL;
			cost *= 1.5;
			break;
		case 7: 
			effect = SpellCastingConstants::SHAPE_SHIFTING;
			targetType = SpellCastingConstants::NEUTRAL;
			cost *= 1.1;
			break;
		case 8:
			effect = SpellCastingConstants::SIMULACRUM; //NOTE: simulacrum is similar to illusion in that it copies a creature, but this spell targets a creature and creates a simulacrum of it a short distance away on a random tile
			targetType = SpellCastingConstants::ENEMY;      //NOTE: not actually harmful, but it should prefer 
			cost *= 1.4;
			break;               //simulacrum success should be dependent on how close the resources spent resemble the relative mixture of each element a creature possesses (a glass creature should not be copied by a simulacrum spell of the radiation subtype)  
		case 9:
			effect = SpellCastingConstants::SHIELD;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.1;
			break;
		case 10:
			effect = SpellCastingConstants::REFLECTION;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.2;
			break;
		case 11:
			effect = SpellCastingConstants::ABSORPTION;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.2;
			break;
		case 12:
			effect = SpellCastingConstants::DAMAGING_TELEPORT;
			targetType = SpellCastingConstants::NEUTRAL;
			cost *= 1.4;
			break;
		case 13:
			effect = SpellCastingConstants::WEAPON_ENHANCEMENT;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.3;
			break;
		case 14:
			effect = SpellCastingConstants::PROTECT_FROM_HAZARDS;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.2;
			break;
		case 15:
			effect = SpellCastingConstants::TELEPORT;
			targetType = SpellCastingConstants::NEUTRAL;
			cost *= 1.2;
			break;
		case 16:
			effect = SpellCastingConstants::DAMAGING_AURA;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.3;
			break;
		case 17:
			effect = SpellCastingConstants::INTELLIGENT_DAMAGING_AURA;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.4;
			break;
		case 18:
			effect = SpellCastingConstants::CONTROLLED_TELEPORT;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.5;
			break;
		//THESE SPELLS ARE BAD FOR SELF-TARGETING SPELLS 
		case 19: 
			effect = SpellCastingConstants::DAMAGING_PULL;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.2;
			break;
		case 20:
			effect = SpellCastingConstants::DAMAGING_PUSH;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.3;
			break;
		case 21:
			effect = SpellCastingConstants::STRAIGHT_DAMAGE;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.1;
			break;
		case 22:
			effect = SpellCastingConstants::BLEED_DAMAGE;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1;
			break;
		case 23:
			effect = SpellCastingConstants::STAT_DRAIN;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.1;
			break;
		case 24:
			effect = SpellCastingConstants::STAT_SAPPING;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.3;
			break;
		case 25:
			effect = SpellCastingConstants::LIFE_LEECHING;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.4;
			break;
		case 26:
			effect = SpellCastingConstants::MANA_LEECHING;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.4;
			break;
		case 27:
			effect = SpellCastingConstants::INSTAKILL;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 2.1;
			break;
		case 28: 
			effect = SpellCastingConstants::DEBUFFING;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.3;
			break;
		case 29:
			effect = SpellCastingConstants::UNSUMMON;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.3;
			break;
		case 30:
			effect = SpellCastingConstants::BANISHING;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.4;
			break;
		case 31:
			effect = SpellCastingConstants::MANA_DAMAGE;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.1;
			break;
		case 32:
			effect = SpellCastingConstants::DOOM_TIMER;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.4;
			break;
		case 33:
			effect = SpellCastingConstants::PERCENTILE_DAMAGE;
			targetType =  SpellCastingConstants::ENEMY;
			cost *= 1.3;
			break;
		case 34:
			effect = SpellCastingConstants::RESURRECT;
			targetType = SpellCastingConstants::FRIENDLY;
			cost *= 1.5;
			break;
		case 35:
			effect = SpellCastingConstants::PULL;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1;
			break;
		case 36:
			effect = SpellCastingConstants::PUSH;
			targetType = SpellCastingConstants::ENEMY;
			cost *= 1.1;
			break;
		//THESE SPELL EFFECTS NEED SPECIFIC TARGETING TYPES
		case 37:
			effect = SpellCastingConstants::LINK;
			targetType = SpellCastingConstants::ENEMY;
			targeting = SpellCastingConstants::SINGLE_CREATURE_IN_SIGHT;
			cost = baseCost;
			cost *= 1.1;
			break;
		case 38:
			effect = SpellCastingConstants::LIFE_TAP;
			targetType = SpellCastingConstants::NEUTRAL;
			targeting = SpellCastingConstants::SELF;
			cost = 0;
			break;
		
		default: effect = SpellCastingConstants::NO_EFFECT; break;
	}

	if (targeting != SpellCastingConstants::SELF) { //we will roll some dice and see if the targets its preferred type, or something else (which could be better or worse)
		int roll = rng->getInt(1,100);
		if (roll <= 5) { //5 percent chance to actually target something opposite to what's preferred
			if (targetType == SpellCastingConstants::ENEMY) {
				targetType = SpellCastingConstants::FRIENDLY;
			} 
			if (targetType == SpellCastingConstants::FRIENDLY) {
				targetType = SpellCastingConstants::ENEMY;
			}
		} else if (roll <= 25) { //20% chance to go from neutral targeting to something more discriminating
			if (targetType == SpellCastingConstants::NEUTRAL) {
				int roll2 = rng->getInt(0,1);
				if (roll2) {
					targetType = SpellCastingConstants::ENEMY;
				} else {
					targetType = SpellCastingConstants::FRIENDLY;
				}
			}
		} else { //75% chance to target the preferred targetd
			targetType = targetType;
		}
	}
	cost = (int)cost;
}

bool Spell::cast(Actor *caster) const{

	TCODRandom *rng = TCODRandom::getInstance();
	TCODList<Actor *> targets;
	
	targets = targetSystem(caster, this->targetType, this->intensity);
	
	//now all possible TargetSystems have been accounted for, and the TargetType should be accounted for as well
	//check if targets is empty, and print a message so you know its empty here
	if (targets.isEmpty()) {
		engine.gui->message(TCODColor::lightPurple,"targets is empty");
		targets.clear();
		return false;
	}

	for (Actor **iterator = targets.begin();
		iterator != targets.end(); iterator++) {
		
		Actor *actor = *iterator;
		//engine.gui->message(TCODColor::white,"%s",actor->getName());
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
			default: targets.clear(); return false; break;
			case SpellCastingConstants::STRAIGHT_HEAL: {

				int heal = rng->getInt( (5+10*((int)intensity))*0.8, (5+10*((int)intensity))*1.25); //choose a random amount of HP to heal
				actor->destructible->heal(heal);	//assume that all actors here are Destructible and !isDead(); if it crashes, fix it above, where they are added to targets

				break;
			}
			case SpellCastingConstants::HEAL_OVER_TIME: {
				Aura *hot = new HealAura(10+((int)intensity),4+3*((int)intensity),Aura::ITERABLE); //this will have to change when auras are changed around
				actor->auras.push(hot);
				break;
			}
			case SpellCastingConstants::STAT_BOOST: {
				//determine what elements correspond to what stat and then add this functionality
//				std::cout << "got to stat boost part" << std::endl;
				break;
			}
			case SpellCastingConstants::CURE_DEBUFFS: {
				//once you figure out what buffs correspond to what element, do the inverse here

				break;
			}
			case SpellCastingConstants::DETECTION: {
				engine.map->exploreTile(actor->x,actor->y);
				break;
			}
			case SpellCastingConstants::POLYMORPH: {
				//once you have an enemyFactory class, use that here to generate a new body for the target actor
				//this new body will be determined by the element of the spell

				break;
			}
			case SpellCastingConstants::SHAPE_SHIFTING: {
				//do the smae as polymorph, but make an Aura for shape_shifting so they can go back to their old body

				break;
			}
			case SpellCastingConstants::SIMULACRUM: {
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
			case SpellCastingConstants::SHIELD: {
				int shieldAmount = rng->getInt( (10*((int)intensity))*0.8, (10*((int)intensity))*1.25);	//find a random amount to shield the target by
				Aura *shield = new ShieldAura(10+3*((int)intensity),shieldAmount);
				actor->auras.push(shield);
				break;
			}
			case SpellCastingConstants::REFLECTION: {
				//essentially, this should add a ReflectionAura to the target, which simply holds a duration and an elemental subtype
				//above, where we iterate through the list of targets, we will then check if they have an aura of type REFLECTION, and if that Aura's elemental subtype matches that of this spell
				//if both of the above conditions hold, target is instead changed to caster, effectively reflecting the spell
				//this functionality is partially implemented; the elemental subtype functionality isn't there yet
				Aura *reflection = new ReflectionAura(5+2*((int)intensity));
				actor->auras.push(reflection);
				break;
			}
			case SpellCastingConstants::ABSORPTION: {
				//this will function similarly to the above case, where it will be handled by an aura
				//for an AbsorptionAura, however, instead of adding the caster as a new target, it instead gives the target a portion of the mana cost of the spell, depending on the intensity of the absorption
				//spells absorbed in this way have no other effect on the target
				//this functionality is partially implemented; the elemental subtype functionality isn't there yet
				Aura *absorption = new AbsorptionAura(10+((int)intensity),20*((int)intensity));
				actor->auras.push(absorption);
				break;
			}
			case SpellCastingConstants::DAMAGING_TELEPORT:{
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

				inRadius.clear();
				break;
			}
			case SpellCastingConstants::WEAPON_ENHANCEMENT: {
				//implement a means to add additional damage dice to a weapon, with an elemental subtype associated with it
				//this will most likely be a struct and the weapon stores a TCODList of them
				break;
			}
			case SpellCastingConstants::PROTECT_FROM_HAZARDS: {
				//I'm not sure how to implement this; possibly just an Aura that a hazard object searches for before it damages you
				//not sure if that's the best way
				break;
			}
			case SpellCastingConstants::TELEPORT: {
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
			case SpellCastingConstants::DAMAGING_AURA: {
				Aura *damaging = new DamagingAura(10+((int)intensity), 2+2*((int)intensity), 3+((int)intensity), false);
				actor->auras.push(damaging);
				break;
			}
			case SpellCastingConstants::INTELLIGENT_DAMAGING_AURA: {
				Aura *damaging = new DamagingAura(10+((int)intensity), 2+2*((int)intensity), 3+((int)intensity), true);
				actor->auras.push(damaging);
				break;
			}
			case SpellCastingConstants::DAMAGING_PULL: {
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
			case SpellCastingConstants::DAMAGING_PUSH: {
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
			case SpellCastingConstants::STRAIGHT_DAMAGE: {
				int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				int damage = rng->getInt(.9*potentialDamage,1.11*potentialDamage);
				if (actor->destructible) actor->destructible->takeDamage(actor, caster, damage);
				
				break;
			}
			case SpellCastingConstants::BLEED_DAMAGE: {
				Aura *hot = new HealAura(10+((int)intensity),-1*(4+3*((int)intensity)),Aura::ITERABLE); 
				actor->auras.push(hot);

				break;
			}
			case SpellCastingConstants::STAT_DRAIN: {
				//this will be added once elemental subtypes are all tied to a (stat or stats)
				break;
			}
			case SpellCastingConstants::STAT_SAPPING: {
				//this will be added at the same time as the above
				break;
			}
			case SpellCastingConstants::LIFE_LEECHING: {
				//damage the target and heal the caster for a portion of that damage
				int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				int damage = rng->getInt(.45*potentialDamage,.555*potentialDamage);	//half the damage of the straight-damage spell
				if (actor->destructible) actor->destructible->takeDamage(actor, caster, damage);

				if (caster->destructible) caster->destructible->heal(.5*damage);

				break;
			}
			case SpellCastingConstants::MANA_LEECHING: {
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
			case SpellCastingConstants::INSTAKILL: {
				if (actor->destructible) actor->destructible->takeDamage(actor, caster, 250*((int)intensity)-1);	//should kill most Actors, but might not actually kill very big bosses, as intended
				break;
			}
			case SpellCastingConstants::DEBUFFING: {
				//same spiel concerning elemental subtypes and buffs/debuffs
				break;
			}
			case SpellCastingConstants::UNSUMMON: {
				//think of the best way to identify whether or not a specific actor is a summoned creature and get rid of them
				//prolly just put a bool in the Actor class, or put a String tag member variable that can be used to identify summons, uniques, regular mobs, etc
				//the tag field will be useful to differentiate between mobs that need "the" in front of their names or not
				break;
			}
			case SpellCastingConstants::BANISHING: {
				//right now this simply deletes the target, but in the future, it should push the target to a list of banished creatures, which will be used to populate the Abyss
				engine.actors.remove(actor);
				delete actor;

				break;
			}
			case SpellCastingConstants::MANA_DAMAGE:{
				//int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				//int damage = rng->getInt(.45*potentialDamage,.555*potentialDamage);	//half the damage of the straight-damage spell
				//if(actor->caster) actor->caster->gainMana(-1*damage);

				break;
			}
			case SpellCastingConstants::DOOM_TIMER: {
				//this adds a DoomAura to the target, which counts down and kills the target at the end of the countdown; the higher the intensity, the lower the timer
				//similar to instakill, this DoomAura simply inflicts a lot of damage to the target, so it may not kill big enemies

				Aura *doom = new DoomAura(13-2*((int)intensity),250*((int)intensity)-1);
				actor->auras.push(doom);
				break;
			}
			case SpellCastingConstants::PERCENTILE_DAMAGE: {
				//deal a percentage of the target's health as damage; the higher the intensity, the bigger the percentage

				if (actor->destructible) actor->destructible->takeDamage(actor, caster, actor->destructible->maxHp*(.2*((int)intensity) ));
				break;
			}
			case SpellCastingConstants::RESURRECT: {
				if (actor->destructible && actor->destructible->isDead()) {
					actor->destructible->resurrect(actor, 0.2*((int)intensity));	//resurrect the target and give it .2,.4,.6, or .8 of maxHp as health
				}
				break;
			}
			case SpellCastingConstants::CONTROLLED_TELEPORT: {
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
			case SpellCastingConstants::PULL: {
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
			case SpellCastingConstants::PUSH: {
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
			case SpellCastingConstants::LINK: {
				//prolly store an actor pointer in the Caster class to handle Links.
				//since this is restricted to target single creatures, it shouldn't need more than a single pointer
				//however, if an actor casts this on themself once and then does it again, the way I plan to implement it will cause an infinite loop
				//I think I will prevent everyone but the player from doing so
				//the player will be warned about possible "magical instabilities" caused by their action, but they will be allowed to do it
				break;
			}
			case SpellCastingConstants::LIFE_TAP: {
				int potentialDamage = ((2/3)*((int)intensity)*((int)intensity)*((int)intensity))+(6.5*((int)intensity)*((int)intensity))+(-13.1666*((int)intensity))+16;
				int damage = rng->getInt(.9*potentialDamage,1.11*potentialDamage);
				if (actor->destructible) actor->destructible->takeDamage(actor,NULL,damage);
				// if (caster->caster) caster->caster->gainMana(damage);
				break;
			}
		}
	}	
//	std::cout << "got before list is cleared" << std::endl;
	targets.clear();
//	std::cout << "got after list is cleared" << std::endl;
	return true;
}
