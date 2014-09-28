#include "main.hpp"
#include <stdio.h>
#include <cmath>

Ai *Ai::create(TCODZip &zip) {
	AiType type = (AiType)zip.getInt();
	Ai *ai = NULL;
	switch(type) {
		case PLAYER: ai = new PlayerAi(); break;
		case MONSTER: ai = new MonsterAi(); break;
		case CONFUSED_MONSTER: ai = new ConfusedMonsterAi(0,NULL); break;
	}
	ai->load(zip);
	return ai;
}

PlayerAi::PlayerAi() : xpLevel(1) {
}

void PlayerAi::save(TCODZip &zip) {
	zip.putInt(PLAYER);
	zip.putInt(xpLevel);
}

void PlayerAi::load(TCODZip &zip) {
	xpLevel = zip.getInt();
}


const int LEVEL_UP_BASE = 20;
const int LEVEL_UP_FACTOR = 15;

int PlayerAi::getNextLevelXp() {
	return LEVEL_UP_BASE + (xpLevel-1) * LEVEL_UP_FACTOR;
}

void PlayerAi::update(Actor *owner){
	int levelUpXp = getNextLevelXp();
	if (owner->destructible->xp >= levelUpXp) {
		xpLevel++;
		owner->destructible->xp -= levelUpXp;
		engine.gui->message(TCODColor::yellow,"You feel a strange rush as the enemy falls to the floor!");
		engine.gui->menu.clear();
		engine.gui->menu.addItem(Menu::CONSTITUTION,"+20 HP");
		engine.gui->menu.addItem(Menu::STRENGTH,"+1 Attack");
		engine.gui->menu.addItem(Menu::AGILITY,"+1 Defense");
		
		Menu::MenuItemCode menuItem = Menu::RESUME;
		while (menuItem == Menu::RESUME) {
			menuItem = engine.gui->menu.pick(Menu::PAUSE);
		}
		switch (menuItem) {
			case Menu::CONSTITUTION :
				owner->destructible->maxHp+= 20;
				owner->destructible->hp += 20;
			break;
			case Menu::STRENGTH :
				owner->attacker->basePower += 1;
				owner->attacker->totalPower += 1;
			break;
			case Menu::AGILITY :
				owner->destructible->baseDR += 1;
				owner->destructible->totalDR += 1;
			break;
			default: break;
		}
	}

	if (owner->destructible &&owner->destructible->isDead() ) {
		return;
	}
	int dx = 0, dy = 0;
	switch (engine.lastKey.vk) {
		case TCODK_KP5: 
			engine.gameStatus = Engine::NEW_TURN; 
			break;
		case TCODK_UP:
		case TCODK_KP8:
			dy = -1; 
			break;
		case TCODK_KP7:
			dy = -1;
			dx = -1;
			break;
		case TCODK_KP9:
			dy = -1;
			dx = 1;
			break;
		case TCODK_KP1:
			dy = 1;
			dx = -1;
			break;
		case TCODK_KP3:
			dy = 1;
			dx = 1;
			break;
		case TCODK_DOWN: 
		case TCODK_KP2:
			dy = 1; 
			break;
		case TCODK_LEFT: 
		case TCODK_KP4:	
			dx =-1; 
			break;
		case TCODK_RIGHT: 
		case TCODK_KP6: 
			dx = 1; 
			break;
		case TCODK_CHAR: handleActionKey(owner, engine.lastKey.c); break;
		default: break;
	}
	if ( dx != 0 || dy != 0){
		engine.gameStatus = Engine::NEW_TURN;
		if (moveOrAttack(owner,owner->x+dx,owner->y+dy)){
			engine.map->computeFov();
		}
	}
}

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety){
	if (engine.map->isWall(targetx, targety)) return false;
	//look for actors to attack
	
	for (Actor **iterator = engine.actors.begin();
		iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor->destructible && !actor->destructible->isDead()
			&& actor->x == targetx && actor->y == targety) {
			owner->attacker->attack(owner,actor);
			return false;
		}
	}
	
	if (!engine.map->canWalk(targetx,targety)) {
		return false;
	}
	
	//look for corpses
	for (Actor **iterator=engine.actors.begin();
		iterator!= engine.actors.end(); iterator++){
		Actor *actor = *iterator;
		
		bool corpseOrItem = (actor->destructible && actor->destructible->isDead()) 
			|| actor->pickable;
		if (corpseOrItem
			&& actor->x == targetx && actor->y == targety) {
			engine.gui->message(TCODColor::white,"there's a %s here",actor->getName());
		}
	}
	owner->x = targetx;
	owner->y = targety;
	return true;
}

void PlayerAi::handleActionKey(Actor *owner, int ascii) {
	switch (ascii) {
		case 'g': //pickup item 
		{
			bool found = false;
			for (Actor **iterator = engine.actors.begin();
				iterator != engine.actors.end(); iterator++) {
				Actor *actor = *iterator;
				if (actor->pickable && actor->x == owner->x && actor->y == owner->y) {
					if (actor->pickable->pick(actor,owner)) {
						found = true;
						engine.gui->message(TCODColor::lightGrey,"You pick up the %s.",actor->getName());
						break;
					} else if (!found) {
						found = true;
						engine.gui->message(TCODColor::red,"Your inventory is full.");
					}
				}
			}
			if (!found) {
				engine.gui->message(TCODColor::lightGrey,"There's nothing interesting here.");
			}
			engine.gameStatus = Engine::NEW_TURN;
		}
		break;
		case 'i': //display inventory
		{
			Actor *actor = choseFromInventory(owner);
			if (actor) {
				engine.gui->message(TCODColor::lightGreen,"You use the %s",actor->getName());
				actor->pickable->use(actor,owner);
				engine.gameStatus = Engine::NEW_TURN;
			}
		}
		break;
		case 'd': //drop item
		{
			Actor *actor = choseFromInventory(owner);
			if (actor) {
				actor->pickable->drop(actor,owner);
				engine.gameStatus=Engine::NEW_TURN;
			}
		}
		break;
		case '>': //go down the stairs, if atop them
		{
			if (engine.stairs->x == owner->x && engine.stairs->y == owner->y) {
				engine.player->attacker->lastTarget = NULL;
				engine.nextLevel();
			} else {
				engine.gui->message(TCODColor::lightGrey,"There are no stairs here.");
			}
		}
		break;
		case 'V': //make it fullscreen
		{
			int w,h;
			if (!TCODConsole::isFullscreen()) {
				engine.gui->message(TCODColor::darkerPink,"going big!");
				TCODSystem::getCurrentResolution(&w,&h);
				TCODConsole::initRoot(w/8,h/8,"CASTER EDITION",true);
			} else {
				engine.gui->message(TCODColor::darkerPink,"minimizing");
				TCODConsole::initRoot(engine.screenWidth,engine.screenHeight,"CASTER EDITION",false);
			}
		}
		break;
		case 'l' : //use the 'l'ook function
		{
			engine.gui->renderKeyLook();
		}
		break;
	}
}

Actor *PlayerAi::choseFromInventory(Actor *owner){
	static const int INVENTORY_WIDTH = 50;
	static const int INVENTORY_HEIGHT = 28;
	static TCODConsole con(INVENTORY_WIDTH,INVENTORY_HEIGHT);
	
	//display the inventory frame
	con.setDefaultForeground(TCODColor(0,255,50));
	con.printFrame(0,0,INVENTORY_WIDTH,INVENTORY_HEIGHT,true,
		TCOD_BKGND_DEFAULT,"INVENTORY");
	
	//display the items with their keyboard shortcut
	con.setDefaultForeground(TCODColor::white);
	int shortcut = 'a';
	int y=1;
	for (Actor **it = owner->container->inventory.begin();
		it != owner->container->inventory.end(); it++) {
		Actor *actor = *it;
		if (actor->pickable->stackSize == 1) {
			if (actor->pickable->type == Pickable::EQUIPMENT && ((Equipment*)(actor->pickable))->equipped == true) {
				con.print(2,y,"(%c) %s [E]",shortcut,actor->getName());
			} else {
				con.print(2,y,"(%c) %s",shortcut,actor->getName());	
			}
		} else {
			con.print(2,y,"(%c) %s [%d]",shortcut,actor->getName(),actor->pickable->stackSize);
		}
		y++;
		shortcut++;
	}
	
	//blit the inventory console on the root console
	TCODConsole::blit(&con,0,0,INVENTORY_WIDTH,INVENTORY_HEIGHT,
		TCODConsole::root,engine.screenWidth/2 - INVENTORY_WIDTH/2,
		engine.screenHeight/2-INVENTORY_HEIGHT/2);
	TCODConsole::flush();
	
	//wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL,true);
	if (key.vk == TCODK_CHAR) {
		int actorIndex = key.c - 'a';
		
		if (actorIndex >= 0 && actorIndex < owner->container->inventory.size()){
			return owner->container->inventory.get(actorIndex);
		}
	}
	return NULL;
}


void MonsterAi::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	zip.putInt(moveCount);
}

void MonsterAi::load(TCODZip &zip) {
	moveCount = zip.getInt();
}

//how many turn the monster chases the player
//after losing his sight
static const int TRACKING_TURNS = 3;

void MonsterAi::update(Actor *owner) {
	if (owner->destructible && owner->destructible->isDead()){
		return;
	}
	if (engine.map->isInFov(owner->x,owner->y)){
		//we can see the player and move towards him
		moveCount = TRACKING_TURNS;
	} else {
		moveCount--;
	}
	if (moveCount > 0) {
		moveOrAttack(owner,engine.player->x,engine.player->y);
	}
}

void MonsterAi::moveOrAttack(Actor *owner, int targetx, int targety){
	int dx = targetx - owner->x;
	int dy = targety - owner->y;
	int stepdx = (dx > 0 ? 1:-1);
	int stepdy = (dy > 0 ? 1:-1);
	float distance = sqrtf(dx*dx+dy*dy);
	
	if (distance >= 2) {
		dx = (int)(round(dx/distance));
		dy = (int)(round(dy/distance));
		if (engine.map->canWalk(owner->x+dx,owner->y+dy)){
			owner->x += dx;
			owner->y += dy;
		} else if (engine.map->canWalk(owner->x+stepdx,owner->y)) {
			owner->x += stepdx;
		} else if (engine.map->canWalk(owner->x,owner->y+stepdy)) {
			owner->y += stepdy;
		}
	} else if (owner->attacker) {
		owner->attacker->attack(owner,engine.player);
	}
}

ConfusedMonsterAi::ConfusedMonsterAi(int nbTurns, Ai *oldAi)
	: nbTurns(nbTurns),oldAi(oldAi) {
}

void ConfusedMonsterAi::save(TCODZip &zip) {
	zip.putInt(CONFUSED_MONSTER);
	zip.putInt(nbTurns);
	oldAi->save(zip);
}

void ConfusedMonsterAi::load(TCODZip &zip) {
	nbTurns = zip.getInt();
	oldAi = Ai::create(zip);
}

void ConfusedMonsterAi::update(Actor *owner) {
	TCODRandom *rng = TCODRandom::getInstance();
	int dx = rng->getInt(-1,1);
	int dy = rng->getInt(-1,1);
	if (dx != 0 || dy != 0) {
		int desx = owner->x + dx;
		int desy = owner->y + dy;
		if (engine.map->canWalk(desx,desy)) {
			owner->x = desx;
			owner->y = desy;
		} else {
			Actor *actor = engine.getActor(desx,desy);
			if (actor) {
				owner->attacker->attack(owner,actor);
			}
		}
	}
	nbTurns--;
	if (nbTurns == 0) {
		owner->ai = oldAi;
		delete this;
	}
}

