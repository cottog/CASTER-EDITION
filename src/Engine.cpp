#include <math.h>
#include "main.hpp"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP),
	player(NULL),stairs(NULL),map(NULL),fovRadius(5),
	screenWidth(screenWidth),screenHeight(screenHeight),
	mapWidth(100),mapHeight(100),level(1){
	TCODConsole::initRoot(screenWidth,screenHeight,"CASTER EDITION",false);
	mapcon = new TCODConsole(mapWidth, mapHeight);
	gui = new Gui();
}

Engine::~Engine(){
	term();
	delete gui;
	delete mapcon;
}

void Engine::init() {
	TCODText text(30,25,21,1, 20);
    TCOD_key_t key;
	TCODConsole::root->clear();
	bool goodName = false;
	
	while (!goodName) {
		do {
			TCODConsole::root->setDefaultForeground(TCODColor::white);
			TCODConsole::root->print(30,24, "Enter name (20 chars limit):");
			text.render(TCODConsole::root);
			TCODConsole::root->flush();
			key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
			
		} while(text.update(key));
		
		char buf[128] = "";
		strcat(buf,"saves/");
		strcat(buf,text.getText());
		strcat(buf,".sav");
		if (!TCODSystem::fileExists(buf)) {
			goodName = true;
		} else {
			text.reset();
			TCODConsole::root->setDefaultForeground(TCODColor::red);
			TCODConsole::root->print(30,27,"Please choose a name that hasn't already been used.");
			TCODConsole::root->print(30,28,"Or delete the file containing this name");
		}
		if (strncmp(text.getText(),"",1) == 0) {
			goodName = false;
			text.reset();
			TCODConsole::root->setDefaultForeground(TCODColor::red);
			TCODConsole::root->print(30,26,"Invalid name.");
		}
    }
	
	player = new Actor(40,25,'@',text.getText(),TCODColor::white, false);
	player->ID = 000000;
	if (strncmp(text.getText(),"Harold",6) == 0) {
		player->destructible = new PlayerDestructible(130,2,"your cadaver");
	} else {
		player->destructible = new PlayerDestructible(30,2,"your cadaver");
	}
	
	Aura *shield = new ShieldAura(10,10);
	engine.player->auras.push(shield);
	
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	player->container = new Container(26);
	actors.push(player);
	stairs = new Actor(0,0,'>',"stairs",TCODColor::white);
	stairs->ID = 000001;
	stairs->blocks = false;
	actors.push(stairs);
	map = new Map(mapWidth,mapHeight);
	map->init(true);
	gui->message(TCODColor::red,"Welcome, Sorcerer! This is CASTER EDITION!");
	gameStatus = STARTUP;
}

void Engine::term() {
	actors.clearAndDelete();
	if (map) delete map;
	gui->clear();
}

void Engine::save() {
	if (player->destructible->isDead()) {
		char buf[128] = "";
		strcat(buf,"saves/");
		strcat(buf,player->getName(true));
		strcat(buf,".sav");
		TCODSystem::deleteFile(buf);
	} else {
		TCODZip zip;
		//save the level first
		zip.putInt(level);
		//then the map
		zip.putInt(map->width);
		zip.putInt(map->height);
		map->save(zip);
		//then the player
		player->save(zip);
		//then the stairs
		stairs->save(zip);
		//then all the other actors
		zip.putInt(actors.size()-2);
		for (Actor **it = actors.begin(); it!= actors.end(); it++) {
			if (*it != player && *it != stairs) {
				(*it)->save(zip);
			}
		}
		//finally the message log
		gui->save(zip);
		
		char buf[128] = "";
		strcat(buf,"saves/");
		strcat(buf,player->getName(true));
		strcat(buf,".sav");
		zip.saveToFile(buf);
	}
}

void Engine::load(bool pause) {
	engine.gui->menu.clear();
	if (pause) {
		engine.gui->menu.addItem(Menu::RESUME,"RESUME GAME");
	} else {
		engine.gui->menu.addItem(Menu::NEW_GAME,"NEW GAME");
	}
	if (pause) {
		engine.gui->menu.addItem(Menu::MAIN_MENU, "MAIN MENU");
		engine.gui->menu.addItem(Menu::SAVE, "SAVE");
	}
	TCODList<const char *> savepaths = TCODSystem::getDirectoryContent("saves/","*.sav");
	if (savepaths.size() != 0) {
		if (pause) {
			engine.gui->menu.addItem(Menu::CONTINUE,"LOAD");
		} else {
			engine.gui->menu.addItem(Menu::CONTINUE,"CONTINUE");
		}
	}
	engine.gui->menu.addItem(Menu::EXIT,"EXIT");
	 
	Menu::MenuItemCode menuItem = engine.gui->menu.pick(pause ? Menu::PAUSE : Menu::MAIN);
	

	if (menuItem == Menu::EXIT || menuItem == Menu::NONE) {
		//exit or window closed
		save();
		exit(0);
	} else if (menuItem == Menu::NEW_GAME) {
		//new game
		engine.term();
		engine.init();
	} else if (menuItem == Menu::SAVE) {
		//save game
		save();
	} else if (menuItem == Menu::RESUME) { 
		
	} else if (menuItem == Menu::MAIN_MENU) {
		save();
		TCODConsole::root->clear();
		load(false);
	} else {
		char buf[128] = "";
		strcat(buf,"saves/");
		strcat(buf,chooseSaveFile(savepaths));
		bool fileChosen = true;
		if (strcmp(buf,"saves/") == 0) {
			if (pause){
				return;
			} else {
				fileChosen = false;
				TCODConsole::root->clear();
				load(false);
			}
		}
		if (fileChosen){
			TCODZip zip;
			engine.term();
			zip.loadFromFile(buf);
			level = zip.getInt();
			//load the map
			int width = zip.getInt();
			int height = zip.getInt();
			map = new Map(width,height);
			map->load(zip);
			//then the player
			player = new Actor(0,0,0,NULL,TCODColor::white);
			player->load(zip);
			actors.push(player);
			//then the stairs
			stairs = new Actor(0,0,0,NULL,TCODColor::white);
			stairs->load(zip);
			actors.push(stairs);
			//then all the other actors
			int nbActors = zip.getInt();
			while (nbActors > 0) {
				Actor *actor = new Actor(0,0,0,NULL,TCODColor::white);
				actor->load(zip);
				actors.push(actor);
				nbActors--;
			}
			//finally the message log
			gui->load(zip);
			gameStatus = STARTUP;
		}
	}
	savepaths.clearAndDelete();
}

void Engine::update(){
	if (gameStatus == STARTUP) map->computeFov();
	gameStatus = IDLE;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&lastKey,NULL);
	if (lastKey.vk == TCODK_ESCAPE) {
		save();
		load(true);
	}
	if (lastKey.vk == TCODK_ENTER && lastKey.ralt) {
		fullscreen();
	}
	player->update();
	if (gameStatus == NEW_TURN){
		player->updateAuras();
		for (Actor **iterator=actors.begin(); 
			iterator != actors.end(); iterator++){
			Actor *actor = *iterator;
			if (actor != player && actor->isVisible()){
				actor->update();
				actor->updateAuras();
			}
		}
	}
}

void Engine::render(){
	TCODConsole::root->clear();
	mapcon->clear();
	//draw the map
	map->render();
	//draw the actors
	for (Actor **iterator = actors.begin();
		iterator!= actors.end(); iterator++){
		Actor *actor = *iterator;
		if ( actor != player){
			actor->render();
		}
	}
	player->render();
	//show the player's stats
	gui->render();
	
	//render the scrolling map
	int mapx1 = 0, mapy1 = 0, mapy2 = 0, mapx2 = 0;
	
	mapx1 = player->x - ((screenWidth - gui->sidebar->getWidth())/2);
	mapy1 = player->y - ((screenHeight - gui->con->getHeight())/2)-1;
	mapx2 = player->x + ((screenWidth - gui->sidebar->getWidth())/2);
	mapy2 = player->y + ((screenHeight - gui->con->getHeight())/2);
	
	if (mapx1 < 0) {
		mapx2 += (0-mapx1);
		mapx1 = 0;
	}
	
	if (mapy1 < 0) {
		mapy2 += (0-mapy1);
		mapy1=0;
	}
	
	if (mapx2 > mapWidth) {
		mapx1 += (mapWidth-mapx2);
		mapx2 = mapWidth;
	}
	
	if (mapy2 > mapHeight) {
		mapy1 += (mapHeight-mapy2);
		mapy2 = mapHeight;
	}
	
	if (mapy2 > TCODConsole::root->getHeight() - gui->con->getHeight()) mapy2 = TCODConsole::root->getHeight() - gui->con->getHeight();
	
	TCODConsole::blit(mapcon,mapx1,mapy1,mapx2,mapy2,
		TCODConsole::root,gui->sidebar->getWidth(),0);
}

void Engine::sendToBack(Actor *actor){
	actors.remove(actor);
	actors.insertBefore(actor,0);
}

Actor *Engine::getClosestMonster(int x, int y, float range) const {
	Actor *closest = NULL;
	float bestDistance = 1E6f;
	for (Actor **iterator = actors.begin();
		iterator != actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor != player&&actor->destructible && !actor->destructible->isDead()){
			float distance = actor->getDistance(x,y);
			if (distance < bestDistance && (distance <= range || range == 0.0f)) {
				bestDistance = distance;
				closest = actor;
			}
		}
	}
	return closest;
}

/* bool Engine::pickATile(int *x, int *y, float maxRange) {
	while (!TCODConsole::isWindowClosed()) {
		int dx = 0, dy = 0;
		render();
		
		if(player->getDistance(*x,*y) > maxRange && maxRange != 0) {
			mapcon->setCharBackground(*x,*y,TCODColor::desaturatedPink);
		} else {
			mapcon->setCharBackground(*x,*y,TCODColor::pink);
		}
		
	int mapx1 = 0, mapy1 = 0, mapy2 = 0, mapx2 = 0;
	
	mapx1 = *x - ((screenWidth - gui->sidebar->getWidth())/2);
	mapy1 = *y - ((screenHeight - gui->con->getHeight())/2);
	mapx2 = *x + ((screenWidth - gui->sidebar->getWidth())/2);
	mapy2 = *y + ((screenHeight - gui->con->getHeight())/2);
	
	if (mapx1 < 0) {
		mapx2 += (0-mapx1);
		mapx1 = 0;
	}
	
	if (mapy1 < 0) {
		mapy2 += (0-mapy1);
		mapy1=0;
	}
	
	if (mapx2 > mapWidth) {
		mapx1 += (mapWidth-mapx2);
		mapx2 = mapWidth;
	}
	
	if (mapy2 > mapHeight) {
		mapy1 += (mapHeight-mapy2);
		mapy2 = mapHeight;
	}
	
	if (mapy2 > TCODConsole::root->getHeight() - gui->con->getHeight()) mapy2 = TCODConsole::root->getHeight() - gui->con->getHeight();

	TCODConsole::blit(mapcon,mapx1,mapy1,mapx2,mapy2,
		TCODConsole::root,gui->sidebar->getWidth(),0);
		TCODConsole::flush();
		
		//let the player pick a tile with the keyboard
		TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&lastKey,NULL,true);
		switch(lastKey.vk){
			case TCODK_UP: dy = -1; break;
			case TCODK_DOWN: dy = 1; break;
			case TCODK_LEFT: dx = -1; break;
			case TCODK_RIGHT: dx = 1; break;
			case TCODK_ENTER:
			{
				gui->message(TCODColor::white,"chose %d %d",*x,*y);
				if ((player->getDistance(*x,*y) > maxRange && maxRange != 0)) {
					gui->message(TCODColor::pink,"This tile is out of range!");
					return false;
				} else {
					return true;
				}
			} break;
			case TCODK_ESCAPE: return false; break;
			default: break;
		}
		gui->message(TCODColor::white,"change %d %d",dx,dy);
		*x+=dx;
		*y+=dy;	
		
		if (*x > mapWidth) *x = mapWidth;
		if (*y > mapHeight) *y = mapWidth;
		if (*x < 0) *x = 0;
		if (*y < 0) *x = 0;
		
	}
	return false;
} */

bool Engine::pickATile(int *x, int *y, float maxRange, float AOE) {
	while (!TCODConsole::isWindowClosed()) {
		int dx = 0, dy = 0;
		render();
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&lastKey,NULL);
		switch (lastKey.vk) {
		case TCODK_UP:
		case TCODK_KP8:
			dy = -1; 
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
		case TCODK_ENTER: 
		{
			if ((player->getDistance(*x,*y) > maxRange && maxRange != 0)) {
				gui->message(TCODColor::pink,"this tile is out of range!");
				return false;
			} else {
				return true;
			}
		}
		case TCODK_ESCAPE: return false;
		default: break;
		}
		*x += dx;
		*y += dy;
		
		if (*x >= mapWidth) *x = mapWidth;
		if (*x < 0) *x = 0;
		if (*y >= mapHeight) *y = mapHeight;
		if (*y < 0) *y = 0;
		
		if (AOE == 0.0f) {
			if(player->getDistance(*x,*y) > maxRange && maxRange != 0) {
				mapcon->setCharBackground(*x,*y,TCODColor::desaturatedPink);
			} else {
				mapcon->setCharBackground(*x,*y,TCODColor::pink);
			}
		} else {
			for (int j = 0; j < map->width; j++) {
				for (int i = 0; i < map->height; i++) {
					if (distance(*x,j,*y,i) <= AOE) {
						if (player->getDistance(*x,*y) > maxRange && maxRange != 0) {
							mapcon->setCharBackground(j,i,TCODColor::desaturatedPink);
						} else {
							mapcon->setCharBackground(j,i,TCODColor::pink);
						}
					}
				}
			}
		}
	int mapx1 = 0, mapy1 = 0, mapy2 = 0, mapx2 = 0;
	
	mapx1 = *x - ((screenWidth - gui->sidebar->getWidth())/2);
	mapy1 = *y - ((screenHeight - gui->con->getHeight())/2)-1;
	mapx2 = *x + ((screenWidth - gui->sidebar->getWidth())/2);
	mapy2 = *y + ((screenHeight - gui->con->getHeight())/2);
	
	if (mapx1 < 0) {
		mapx2 += (0-mapx1);
		mapx1 = 0;
	}
	
	if (mapy1 < 0) {
		mapy2 += (0-mapy1);
		mapy1=0;
	}
	
	if (mapx2 > mapWidth) {
		mapx1 += (mapWidth-mapx2);
		mapx2 = mapWidth;
	}
	
	if (mapy2 > mapHeight) {
		mapy1 += (mapHeight-mapy2);
		mapy2 = mapHeight;
	}
	
	if (mapy2 > TCODConsole::root->getHeight() - (gui->con->getHeight()-2)) mapy2 = TCODConsole::root->getHeight() -gui->con->getHeight();
	
	
	TCODConsole::blit(mapcon,mapx1,mapy1,mapx2,mapy2,
		TCODConsole::root,gui->sidebar->getWidth(),0);
		
	TCODConsole::flush();
		
	} 
	return false;
}

Actor *Engine::getActor(int x, int y) const {
	for (Actor **iterator = actors.begin();
		iterator != actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor->x == x && actor->y == y && actor->destructible
			&& !actor->destructible->isDead()) {
				return actor;
		}
	}
	return NULL;
}

void Engine::nextLevel() {
	level++;
	gui->message(TCODColor::lightViolet,"You take a moment to rest, and recover your strength.");
	player->destructible->heal(player->destructible->maxHp/2);
	gui->message(TCODColor::red,"After a rare moment of peace, you descend\ndeeper into the heart of the dungeon...");

	delete map;
	//delete all actors but player and stairs
	for (Actor **it = actors.begin(); it != actors.end(); it++) {
		if (*it != player && *it != stairs) {
			delete *it;
			it = actors.remove(it);
		}
	}
	//create a new map
	map = new Map(mapWidth,mapHeight);
	map->init(true);
	gameStatus = STARTUP;
}

float Engine::distance(int x1, int x2, int y1, int y2) {
	int dx = x1 - x2;
	int dy = y1 - y2;
	return sqrt(dx*dx+dy*dy);
}

const char *Engine::chooseSaveFile(TCODList<const char *> savepaths) {
	
	//print a prompt that can hold savegame list
	TCODConsole::root->clear();
	TCODConsole::root->setDefaultForeground(TCODColor(255,50,0));
	TCODConsole::root->printFrame(0,0,screenWidth,screenHeight,true,TCOD_BKGND_DEFAULT,"SAVED GAMES");
	
	TCODConsole::root->setDefaultForeground(TCODColor::white);
	TCODConsole::root->print(2,1,"Please choose the save file whose name corresponds to the proper character.");
	
	int y = 3;
	int shortcut = 'a';
	//print a list of savegames with a corresponding letter for the player to press
	for (int i = 0; i < savepaths.size(); i++) {
		TCODConsole::root->print(2,y,"(%c) %s",shortcut,savepaths.get(i));
		y++;
		shortcut++;
	}
	TCODConsole::flush();
	
	//wait for a keypress
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL,true);
	if (key.vk == TCODK_CHAR) {
		int fileIndex = key.c - 'a';
		
		if (fileIndex >= 0 && fileIndex < savepaths.size()) {
			return savepaths.get(fileIndex);
		}
	}
	return "";
}

void Engine::fullscreen() {
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

Actor *Engine::chooseFromList(TCODList<Actor *> &list,const char *title){
	static const int PANEL_WIDTH = 50;
	static const int PANEL_HEIGHT = 28;
	static TCODConsole con(PANEL_WIDTH,PANEL_HEIGHT);
	
	//display the inventory frame
	con.setDefaultForeground(TCODColor(0,255,50));
	con.printFrame(0,0,PANEL_WIDTH,PANEL_HEIGHT,true,
		TCOD_BKGND_DEFAULT,title);
	
	//display the items with their keyboard shortcut
	con.setDefaultForeground(TCODColor::white);
	int shortcut = 'a';
	int y=1;
	for (Actor **it = list.begin();
		it != list.end(); it++) {
		Actor *actor = *it;
		if (actor->pickable->stackSize == 1) {
			if ((actor->pickable->type == Pickable::EQUIPMENT || actor->pickable->type == Pickable::WEAPON)  && ((Equipment*)(actor->pickable))->equipped == true) {
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
	TCODConsole::blit(&con,0,0,PANEL_WIDTH,PANEL_HEIGHT,
		TCODConsole::root,engine.screenWidth/2 - PANEL_WIDTH/2,
		engine.screenHeight/2-PANEL_HEIGHT/2);
	TCODConsole::flush();
	
	//wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL,true);
	if (key.vk == TCODK_CHAR) {
		int actorIndex = key.c - 'a';
		
		if (actorIndex >= 0 && actorIndex < list.size()){
			return list.get(actorIndex);
		}
	}
	return NULL;
}

void Engine::getAllActorsInRadius(TCODList<Actor *> &targets, int x, int y, float radius, Spell::TargetType targetType, bool casterHostility) {
	for (Actor **iterator = engine.actors.begin();iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor->destructible && !actor->destructible->isDead()){
			float distance = actor->getDistance(x,y);
			if ((distance < radius || radius == 0.0) && ((targetType == Spell::NO_TYPE || targetType == Spell::NEUTRAL) || (targetType == Spell::ENEMY && actor->hostile != casterHostility) || (targetType == Spell::FRIENDLY && actor->hostile == casterHostility)) ) {	//just to make sure diagonals are captured
				targets.push(actor);
			}
		}
	}
}

bool Engine::findNearbyOpenTile(int *x, int *y){
	for (int i = -1; i < 2; i++){		//check the 9 (3x3) tiles immediately adjacent to the given coordinates
		for (int j = -1; j < 2; j++){	//I might try and put two shuffle bags here for i and j so that the order of tile-checking is more random.
			if ( map->canWalk(*x+i,*y+j) )  {
				*x += i;
				*y += j;
				return true;
			}	
		}
	}
	return false;
}
