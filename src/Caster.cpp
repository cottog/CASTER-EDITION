#include "main.hpp"
#include <cctype>
#include <cmath>
#include <iostream>

Caster::Caster(float maxMana) : mana(maxMana), maxMana(maxMana),cantusValue(0),element(Spell::NO_SUBTYPE), cantus(""){
}

Caster::~Caster(){
	spellBook.clearAndDelete();
}

void Caster::save(TCODZip &zip){
	zip.putFloat(mana);
	zip.putFloat(maxMana);
	zip.putInt(cantusValue);
	zip.putInt(element);
	zip.putString(cantus.c_str());
	zip.putInt(spellBook.size());

	for (Spell **it = spellBook.begin(); it!=spellBook.end(); it++) {
		(*it)->save(zip);
	}
}

void Caster::load(TCODZip &zip){
	mana = zip.getFloat();
	maxMana = zip.getFloat();
	cantusValue = zip.getInt();
	element = (Spell::ElementalSubtype)zip.getInt();
	cantus = zip.getString();
	int nbSpells = zip.getInt();

	while (nbSpells > 0){
		Spell *spell = Spell::create(zip);	//need a spell to store what create() returns
		spellBook.push(spell);	//put what create returns into the spellbook
		nbSpells--;
	}
}

const char *Caster::getCantus(){
	return cantus.c_str();
}

static const int CANTUS_MENU_WIDTH = 21;
static const int CANTUS_MENU_HEIGHT = 7;

void Caster::enterCantus(){
	TCODConsole *cantusMenu = new TCODConsole(CANTUS_MENU_WIDTH,CANTUS_MENU_HEIGHT);
	TCODText text(15,4,5,1, 5);
    TCOD_key_t key;

    int max = 0;
    int index = 0;
    int subtype = 0;

    cantusValue = 0;

	do {
		cantusMenu->setDefaultForeground(TCODColor::white);
		cantusMenu->print(1,4, "Enter Cantus:");
		text.render(cantusMenu);
		cantusMenu->flush();
		TCODConsole::blit(cantusMenu,0,0,0,0,
			TCODConsole::root,(engine.screenWidth - CANTUS_MENU_WIDTH)/2,(engine.screenHeight - CANTUS_MENU_HEIGHT)/2);
		key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
		
	} while(text.update(key));

	cantus = text.getText();

	std::string::iterator it;
  	for ( it = cantus.begin() ; it < cantus.end(); it++, index++)
  	{
        *it = tolower(*it);
        if ( !(*it >= 'a' && *it <= 'z') ) {
        	cantus = "";
        	cantusValue = 0;
        	element = Spell::NO_SUBTYPE;
        	engine.gui->message(TCODColor::lightRed,"You could not possibly pronounce that!");
        	return;
        }
        int charValue = ((int)*it-'a'+1);
        cantusValue += charValue;
        if ( charValue == max ) {
        	subtype += pow(2,index);
        } else if ( charValue > max ) {
        	subtype = pow(2,index);
        	max = charValue;
        }
  	}
  	element = (Spell::ElementalSubtype)subtype;
}

bool Caster::hasCantus(){
	return !cantus.empty();
}

TCODColor Caster::cantusColor(){
	switch (element){
		default: return TCODColor::darkGrey;
		case Spell::FIRE: return TCODColor::red;
		case Spell::AIR: return TCODColor::yellow;
		case Spell::LIGHTNING: return TCODColor::lighterBlue;
		case Spell::WATER: return TCODColor::blue;
		case Spell::STEAM: return TCODColor::lightGrey;
		case Spell::ICE: return TCODColor::lightCyan;
		case Spell::RADIATION: return TCODColor::lightPurple;
		case Spell::EARTH: return TCODColor::darkYellow;
		case Spell::LAVA: return TCODColor::darkRed;
		case Spell::DUST: return TCODColor::desaturatedYellow;
		case Spell::GLASS: return TCODColor::lightSea;
		case Spell::MUD: return TCODColor::darkerOrange;
		case Spell::METAL: return TCODColor::cyan;
		case Spell::POISON: return TCODColor::lightGreen;
		case Spell::FORCE: return TCODColor::white;
	}	
}

void Caster::learnSpell(Actor *owner){
	Spell *spell = NULL;
	spell = spell->newSpell(owner);
	engine.gui->message(TCODColor::white,"%f %d %d %d %d %f %s",spell->target,spell->intensity,spell->targeting,spell->effect,spell->expected,spell->cost,spell->getName());

	spellBook.push(spell);
}

bool Caster::cast(Actor *owner, Spell *spell){
	if (spell->cost > owner->caster->mana){
		engine.gui->message(TCODColor::red,"Insufficient mana!");
		return false;
	}
//	std::cout << spell->cost << std::endl;

	float percentageOfTarget = (owner->caster->cantusValue / spell->target) * 100.0f;

	if (percentageOfTarget > 80.0f && percentageOfTarget < 120.0f){ //is the current cantus compatible with the spell?

		if ( spell->cast(owner) ) {	//does the caster choose a target and all that, or is the spell canceled?
//			std::cout << "got to before mana cost" << std::endl;
//			std::cout << spell->cost << std::endl;
			owner->caster->mana -= spell->cost;	//subtract the mana cost of the spell from the caster's mana
//			std::cout << "got to after mana cost" << std::endl;
			return true;
		} else {
			return false;
		}
	} else if (cantusValue == 0) {
		engine.gui->message(TCODColor::red,"With no cantus to recite, you quickly lose focus.");
		return false;
	} else {
		engine.gui->message(TCODColor::red,"The spell fizzles. Perhaps reciting a different cantus could help.");
		return false;
	}
}

Spell *Caster::chooseFromSpellBook(){
	static const int PANEL_WIDTH = 60;
	static const int PANEL_HEIGHT = 29;
	static TCODConsole con(PANEL_WIDTH,PANEL_HEIGHT);
	
	int skipSpells = 0; //this will allow the left and right arrows to navigate to pages in the spell book

	while (true) {
		con.clear();	//clear screen so new spells can be displayed

		//display the inventory frame
		con.setDefaultForeground(TCODColor(0,255,50));
		con.printFrame(0,0,PANEL_WIDTH,PANEL_HEIGHT,true,
		TCOD_BKGND_DEFAULT,"Spellbook");

		//display the items with their keyboard shortcut
		con.setDefaultForeground(TCODColor::white);
		int shortcut = 'a';
		int y=1;

		for (int index = 0+skipSpells; (index <= 25+skipSpells && index < spellBook.size()) ; index++) {
			
			if ( index <= spellBook.size() - 1  ){
				Spell *spell = spellBook.get(index);
				con.print(2,y,"(%c) %s",shortcut,spell->getName());	
				y++;
				shortcut++;
			}
			
		}
		con.setDefaultForeground(TCODColor(0,255,50));
		con.print((PANEL_WIDTH-6)/2,27,"Page %d",(int)((skipSpells/26)+1));

		//blit the inventory console on the root console
		TCODConsole::blit(&con,0,0,PANEL_WIDTH,PANEL_HEIGHT,
			TCODConsole::root,engine.screenWidth/2 - PANEL_WIDTH/2,
			engine.screenHeight/2-PANEL_HEIGHT/2);
		TCODConsole::flush();
		
		//wait for a key press
		TCOD_key_t key;
		TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL,true);
		if (key.vk == TCODK_CHAR) {
			int spellIndex = key.c - 'a';
			
			if (spellIndex >= 0 && spellIndex < spellBook.size()){
				return spellBook.get(spellIndex+skipSpells);
			}
		} else if (key.vk == TCODK_LEFT || key.vk == TCODK_KP4 ){
			if (skipSpells > 0){
				skipSpells -= 26;
			}
		} else if (key.vk == TCODK_RIGHT || key.vk == TCODK_KP6){
			if (skipSpells < spellBook.size() - 26) {
				skipSpells += 26;
			}
		} else if (key.vk == TCODK_ESCAPE){
			return NULL;
		}
	}
}