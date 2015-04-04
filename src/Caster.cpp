#include "main.hpp"

Caster::Caster(float maxMana) : mana(maxMana), maxMana(maxMana), cantus(""){
}

Caster::~Caster(){
	spellBook.clearAndDelete();
}

void Caster::save(TCODZip &zip){
	zip.putFloat(mana);
	zip.putFloat(maxMana);
	zip.putString(cantus.c_str());
	zip.putInt(spellBook.size());

	for (Spell **it = spellBook.begin(); it!=spellBook.end(); it++) {
		(*it)->save(zip);
	}
}

void Caster::load(TCODZip &zip){
	mana = zip.getFloat();
	maxMana = zip.getFloat();
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
}

bool Caster::hasCantus(){
	return !cantus.empty();
}