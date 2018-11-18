#include "main.hpp"

namespace TargetingSystems
{	
	namespace
	{
			TCODRandom *rng = TCODRandom::getInstance();
	}

	TCODList<Actor *> SelfTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		targets.push(caster);
		return targets;
	}
	
	TCODList<Actor *> AdjCrtTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();	
		engine.getAllActorsInRadius(targets, caster->x, caster->y, 1.6,targetType,caster->hostile);
		if (targets.isEmpty()) {
			targets.clear();
			return false;
		}
		while (targets.size() > ((int)intensity)) {
			int elNum = 1 /*rng->getInt(0,targets.size()-1)*/;
			Actor *toRev = targets.get(elNum);
			targets.remove(toRev);
		}
		return targets;
	}
	
	TCODList<Actor *> BoltTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		int x = caster->x;
		int y = caster->y;
		int stepX = caster->x;
		int stepY = caster->y;
		
		TCODList<Actor *> targets = new TCODList<Actor *>();
		
		if (engine.pickATile(&x,&y,4+2*((int)intensity))){
			TCODLine::init(caster->x, caster->y, x, y);
			do {
				if ( !engine.map->canWalk(stepX,stepY)) {
					Actor *actor = engine.getActor(stepX,stepY);
					if (!actor) {
						break;
					} else if ( ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ) {
						targets.push(actor);
						break;
					}
				}
			} while (TCODLine::step(&stepX,&stepY));
		} else {
			targets.clear();
		}
		
		return targets;		
	}
	
	TCODList<Actor *> VisHolocaustTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		int x = caster->x;
		int y = caster->y;
		
		TCODList<Actor *> targets = new TCODList<Actor *>();		
		
		if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
			Actor *actor2 = engine.getActor(x,y);
			if (!actor2) {
				targets.clear();
				return false;
			} else {
				SpellCastingConstants::ElementalSubtype targetedElement = actor2->destructible->element; //we know the actor has a destructible since getActor checks for it
				targets.push(actor2);
				for (Actor **iterator = engine.actors.begin();iterator != engine.actors.end(); iterator++) {
				Actor *actor = *iterator;
					if (actor != caster && actor->destructible && !actor->destructible->isDead() && actor->isVisible() && actor->destructible->element == targetedElement && ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ){
						targets.push(actor);
					}
				}
			}
		} else {
			targets.clear();
		}
		
		return targets;
	}
	
	TCODList<Actor *> LvlHolocaustTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){		
		int x = caster->x;
		int y = caster->y;
		
		TCODList<Actor *> targets = new TCODList<Actor *>();
		
		if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
			Actor *actor2 = engine.getActor(x,y);
			if (!actor2) {
				targets.clear();
			} else {
				SpellCastingConstants::ElementalSubtype targetedElement = actor2->destructible->element; //we know the actor has a destructible since getActor checks for it
				targets.push(actor2);
				for (Actor **iterator = engine.actors.begin();iterator != engine.actors.end(); iterator++) {
				Actor *actor = *iterator;
					if (actor != caster && actor->destructible && !actor->destructible->isDead() && actor->destructible->element == targetedElement && ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ){  //same as above, but we don't check if its visible
						targets.push(actor);
					}
				}
			}
		} else {
			targets.clear();
		}
		return targets;
	}
		
	TCODList<Actor *> RdmLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		
		engine.getAllActorsInRadius(targets, caster->x, caster->y, engine.fovRadius,targetType,caster->hostile);
		if (targets.isEmpty()) {
			targets.clear();
		}
		while (targets.size() > 1) {
			int elNum = TargetingSystems::rng->getInt(0,targets.size()-1);
			Actor *toRev = targets.get(elNum);
			targets.remove(toRev);
		}
		
		return targets;
	}
	
	TCODList<Actor *> RdmLvlTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();

		engine.getAllActorsInRadius(targets, caster->x, caster->y, 0.0,targetType,caster->hostile);
		if (targets.isEmpty()) {
			targets.clear();
			return false;
		}
		while (targets.size() > 1) {
			int elNum = TargetingSystems::rng->getInt(0,targets.size()-1);
			Actor *toRev = targets.get(elNum);
			targets.remove(toRev);
		}
		
		return targets;
	}
	
	TCODList<Actor *> AllAdjCrtTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		
		engine.getAllActorsInRadius(targets, caster->x, caster->y, 1.6*intensity,targetType, caster->hostile);
		if (targets.isEmpty()) {
			targets.clear();
		}
		
		return targets;
	}
	
	TCODList<Actor *> AllCrtrChainTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		TCODList<Actor *> possibleJumps = new TCODList<Actor *>(); //this TCODList holds the actors to which this spell could possibly jump
		int x = caster->x;
		int y = caster->y;		
		
		if (engine.pickATile(&x,&y,4+2*((int)intensity))){
			Actor *actor = engine.getActor(x,y);
			if (!actor) {
				targets.clear();
			} else if ( ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ) {
				targets.push(actor);
			}
		}

		if (targets.isEmpty()) {
			possibleJumps.clear();
			targets.clear();
		}

		while ( targets.size() > 0 && targets.size() < 1+2*((int)intensity) ) {
			engine.getAllActorsInRadius(possibleJumps, targets.get(targets.size()-1)->x, targets.get(targets.size()-1)->y, 1+2*((int)intensity),targetType, caster->hostile);

			if (possibleJumps.isEmpty()) {
				possibleJumps.clear();
			} else {
				int jumpTarget = TargetingSystems::rng->getInt(0, possibleJumps.size() - 1);
				targets.push(possibleJumps.get(jumpTarget));
				possibleJumps.clear();
			}

		}
		possibleJumps.clear();
		
		return targets;
	}
	
	TCODList<Actor *> AllCrtrLvlTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		
		engine.getAllActorsInRadius(targets,caster->x, caster->y, 0.0, targetType, caster->hostile);
		if (targets.isEmpty()) {
			targets.clear();		
		}
		
		return targets;
	}
	
	TCODList<Actor *> AllCrtrLineTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();		
		int x = caster->x;
		int y = caster->y;
		int stepX = caster->x;
		int stepY = caster->y;
		
		if (engine.pickATile(&x,&y,4+2*((int)intensity))){
			TCODLine::init(stepX, stepY, x, y);
			do {
				if ( !engine.map->canWalk(stepX,stepY)) {
					Actor *actor = engine.getActor(stepX,stepY);
					if ( actor && ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ) {
						targets.push(actor);
					}
				}
			} while (TCODLine::step(&stepX,&stepY));
		} else {
			targets.clear();
		}
		
		return targets;
	}
	
	TCODList<Actor *> AllCrtrRadTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		int x = caster->x;
		int y = caster->y;

		if (engine.pickATile(&x,&y,4+2*((int)intensity))){
			engine.getAllActorsInRadius(targets,caster->x, caster->y, 2+2*((int)intensity), targetType, caster->hostile);

			if (targets.isEmpty()){
				targets.clear();		
			}

		} else {
			targets.clear();
		}
		
		return targets;
	}
	
	TCODList<Actor *> OneCrtrLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();		
		int x = caster->x;
		int y = caster->y;
		
		if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
			Actor *actor = engine.getActor(x,y);
			if (!actor) {
				targets.clear();				
			} else {
				if ( ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ){
					targets.push(actor);
				}
			}
		} else {
			targets.clear();			
		}
		
		return targets;
	}
	
	TCODList<Actor *> XCrtrLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		
		while ( targets.size() < 2+((int)intensity) ) {
			
			int x = caster->x;
			int y = caster->y;
			if (engine.pickATile(&x,&y,4+2*((int)intensity))){	
				Actor *actor = engine.getActor(x,y);
				if (!actor) {
					break;
				} else if ( ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ) {
					targets.push(actor);
				}
			} else {
				break;	
			}
		}
		if (targets.isEmpty()) {
			targets.clear();
		}
		
		return targets;
	}
	
	TCODList<Actor *> AllCrtrLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity){
		TCODList<Actor *> targets = new TCODList<Actor *>();
		
		for (Actor **iterator = engine.actors.begin();iterator != engine.actors.end(); iterator++) {
			Actor *actor = *iterator;
			if (actor != caster && actor->isVisible() && actor->destructible && !actor->destructible->isDead() && ((targetType == SpellCastingConstants::NEUTRAL) || (targetType == SpellCastingConstants::ENEMY && actor->hostile != caster->hostile) || (targetType == SpellCastingConstants::FRIENDLY && actor->hostile == caster->hostile)) ){ 
				targets.push(actor);
			}
		}

		return targets;
	}
	
}
