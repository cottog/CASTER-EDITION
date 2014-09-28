#include <stdio.h>
#include <stdarg.h>
#include "main.hpp"

static const int PANEL_HEIGHT = 27;
static const int BAR_WIDTH = 20;
static const int MSG_X = BAR_WIDTH+2;
static const int MSG_HEIGHT = PANEL_HEIGHT-1;

Gui::Gui() {
	con = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
	sidebar = new TCODConsole(MSG_X, engine.screenHeight - PANEL_HEIGHT);
}

Gui::~Gui() {
	delete con;
	delete sidebar;
	clear();
}

void Gui::clear() {
	log.clearAndDelete();
}

void Gui::save(TCODZip &zip) {
	zip.putInt(log.size());
	for (Message **it = log.begin(); it != log.end(); it++) {
		zip.putString((*it)->text);
		zip.putColor(&(*it)->col);
	}
}

void Gui::load(TCODZip &zip) {
	int nbMessages = zip.getInt();
	while (nbMessages > 0) {
		const char *text = zip.getString();
		TCODColor col = zip.getColor();
		message(col,text);
		nbMessages--;
	}
}

void Gui::render(){
	//clear the off-screen consoles
	con->setDefaultBackground(TCODColor::black);
	con->clear();
	sidebar->setDefaultBackground(TCODColor::black);
	sidebar->clear();
	
	sidebar->setDefaultForeground(TCODColor(100,180,250));
	sidebar->printFrame(0,0,MSG_X,engine.screenHeight - con->getHeight(),
		true, TCOD_BKGND_ALPHA(50),"CHARACTER INFO");
	
	//draw the health bar
	renderBar(1,5,BAR_WIDTH,"HP",engine.player->destructible->hp,
		engine.player->destructible->maxHp,
		TCODColor::lightRed,TCODColor::darkerRed);
		
	//draw the XP bar
	PlayerAi *ai = (PlayerAi *)engine.player->ai;
	char xpTxt[128];
	sprintf(xpTxt,"XP(%d)",ai->xpLevel);
	renderBar(1,7,BAR_WIDTH,xpTxt,engine.player->destructible->xp,
		ai->getNextLevelXp(),TCODColor::lightViolet,TCODColor::darkerViolet);
	
	//draw the last target's hp bar
	if ((engine.player->attacker->lastTarget != NULL)&&(engine.player->attacker->lastTarget!=engine.player)&&(!engine.player->attacker->lastTarget->destructible->isDead())) {
		renderBar(1,13,BAR_WIDTH, "target's HP",engine.player->attacker->lastTarget->destructible->hp,
			engine.player->attacker->lastTarget->destructible->maxHp,TCODColor::lightRed, TCODColor::darkerRed);
    }
	
	//draw the message log
	int y=0;
	float colorCoef = 0.4f;
	for (Message **it = log.begin(); it != log.end(); it++){
		Message *message = *it;
		con->setDefaultForeground(message->col * colorCoef);
		con->print(0,y,message->text);
		y++;
		if (colorCoef < 1.0f) {
			colorCoef+=0.3f;
		}
	}
	
	//dungeon level
	sidebar->setDefaultForeground(TCODColor::white);
	sidebar->print(3,9,"Dungeon level %d", engine.level);
	
	//character name
	sidebar->print(3,3,"Name: %s",engine.player->getName(true));
	
	//blit the GUI  consoles on the root console
	TCODConsole::blit(con,0,0,engine.screenWidth,PANEL_HEIGHT,
		TCODConsole::root,0,engine.screenHeight-PANEL_HEIGHT);
	TCODConsole::blit(sidebar,0,0,MSG_X,engine.screenHeight-PANEL_HEIGHT,
		TCODConsole::root,0,0);
}

void Gui::renderBar(int x, int y, int width, const char *name,
	float value, float maxValue, const TCODColor &barColor,
	const TCODColor &backColor) {

	//fill the background
	sidebar->setDefaultBackground(backColor);
	sidebar->rect(x,y,width,1,false,TCOD_BKGND_SET);
	
	int barWidth = (int)(value / maxValue * width);
	if (barWidth > 0) {
		//draw the bar
		sidebar->setDefaultBackground(barColor);
		sidebar->rect(x,y,barWidth,1,false,TCOD_BKGND_SET);
	}
	//print text on top of the bar
	sidebar->setDefaultForeground(TCODColor::white);
	sidebar->printEx(x+width/2,y,TCOD_BKGND_NONE,TCOD_CENTER,
		"%s : %g/%g",name, value, maxValue);
}

Gui::Message::Message(const char *text, const TCODColor &col):
	text(strdup(text)),col(col){
}

Gui::Message::~Message() {
	free(text);
}

void Gui::message(const TCODColor &col, const char *text,...){
	//build the text
	va_list ap;
	char buf[128];
	va_start(ap,text);
	vsprintf(buf,text,ap);
	va_end(ap);
	
	char *lineBegin=buf;
	char *lineEnd;
	
	do {
		//make room for the new message
		if (log.size() == MSG_HEIGHT) {
			Message *toRemove = log.get(0);
			log.remove(toRemove);
			delete toRemove;
		}
		//detect end of the line
		lineEnd = strchr(lineBegin,'\n');
		
		if (lineEnd) {
			*lineEnd = '\0';
		}
		
		//add a new message to the log
		Message *msg = new Message(lineBegin,col);
		log.push(msg);
		
		//go to next line
		lineBegin = lineEnd+1;
	} while(lineEnd);
}

Menu::~Menu() {
	clear();
}

void Menu::clear() {
	items.clearAndDelete();
}

void Menu::addItem(MenuItemCode code, const char *label) {
	MenuItem *item = new MenuItem();
	item->code = code;
	item->label = label;
	items.push(item);
}

const int PAUSE_MENU_WIDTH = 30;
const 	int PAUSE_MENU_HEIGHT = 20;
Menu::MenuItemCode Menu::pick(DisplayMode mode) {
	//the image stuff is supposed to be here
	//no image off the website, so ignore the image stuff for now
	
	int selectedItem = 0;
	int menux,menuy;
	
	if (mode == PAUSE) {
		menux = engine.screenWidth/2 - PAUSE_MENU_WIDTH/2;
		menuy = engine.screenHeight/2 - PAUSE_MENU_HEIGHT/2;
		TCODConsole::root->setDefaultForeground(TCODColor(200,180,50));
		TCODConsole::root->printFrame(menux,menuy,PAUSE_MENU_WIDTH,PAUSE_MENU_HEIGHT,true,
			TCOD_BKGND_ALPHA(70),"PAUSE");
		menux += 2;
		menuy += 3;
	} else {
		//static TCODImage img("menu_background1.png");
		//img.blit2x(TCODConsole::root,0,0);
		menux = 10;
		menuy = TCODConsole::root->getHeight()/3;
	}
	while(!TCODConsole::isWindowClosed()) {
		
		int currentItem = 0;
		for (MenuItem **it = items.begin(); it != items.end(); it++) {
			if (currentItem == selectedItem) {
				TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
			} else {
				TCODConsole::root->setDefaultForeground(TCODColor::blue);
			}
			TCODConsole::root->print(menux,menuy+currentItem*3,(*it)->label);
			currentItem++;
		}
		TCODConsole::flush();
		
		//check key presess
		TCOD_key_t key;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
		switch(key.vk) {
			case TCODK_UP:
				selectedItem--;
				if (selectedItem < 0) {
					selectedItem = items.size()-1;
				}
			break;
			case TCODK_DOWN:
				selectedItem = (selectedItem + 1) % items.size();
			break;
			case TCODK_ENTER: return items.get(selectedItem)->code;
			case TCODK_ESCAPE: 
				return RESUME;
			break;
			default: break;
		}
		
	}
	return NONE;
}

void Gui::renderKeyLook() {
	
	int x = engine.player->x;
	int y = engine.player->y;
	if (engine.pickATile(&x,&y)){
		
		if (!engine.map->isExplored(x,y)) {
			message(TCODColor::lightGrey,"Having never visited this area, you may only guess at what secrets it holds.\nLikely none.");
			return;
		}
		
		char buf[128] = ""; 
		if (engine.map->isInFov(x,y)){
			strcat(buf,"You see: ");
		}else {
			strcat(buf,"You remember seeing: ");
		}
		bool first = true;
		bool found = false;
		for (Actor **it = engine.actors.begin(); it != engine.actors.end(); it++) {
			Actor *actor = *it;
			//find actors under the mouse cursor
			if (actor->x == x && actor->y == y) {
				if (!first) {
					strcat(buf, ", ");
				} else {
					found = true;
					first = false;
				}
					strcat(buf,actor->getName());
				/* if (actor->attacker && !actor->destructible->isDead() && engine.map->isInFov(x,y)) {
					engine.player->attacker->lastTarget = actor;
				} */
			}
		}
		if (found) {
			message(TCODColor::lightGrey,buf);
		} else {
			message(TCODColor::lightGrey,"There's nothing interesting here.");
		}
	}
}
