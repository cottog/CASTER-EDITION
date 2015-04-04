#include <string>

class Caster : public Persistent {
public:
	float mana;
	float maxMana;
	TCODList<Spell *> spellBook;

	Caster(float maxMana);
	~Caster();
	void save(TCODZip &zip); //save the Caster class
	void load(TCODZip &zip); //load a Caster class from save file
	const char *getCantus();

	bool cast(Spell *spell); //handles determining the success of the cantus for a particular spell, as well as subtracting the mana cost and all that
	void enterCantus();	// allows the user to input a string of 4 alphabetical characters to set as their new cantus
	bool hasCantus(); //returns true if the Caster's Cantus is not the empty string
	void learnSpell(Actor *owner); //adds a new spell to the spellbook
protected:
	std::string cantus;	
};
