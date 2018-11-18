#include "libtcod.hpp"
#include <string>

class Spell : public Persistent { //abstract class representing a generic spell; note the pure virtual function cast()
public:

	//TODO: create the LastCast object for casting spell's more quickly
	//TODO: consider creating a tiny description of each spell that indicates what its purpose may be; this would be achieved by having each spell contain a string or TCODText object and you appending characters to it that would be defined in a wiki for players to read
	//TODO: (not so much for caster module, but for the whole engine) add a function for an actor to determine whether or not it is seen by the player, to keep messages from being displayed in error
	//TODO: implement a 'mana-scent' system, in which some spells whose elemental subtype otherwise wouldn't matter (regular teleport) can now have an effect. A spell will emit a surge of mana-scent of the elemental subtype it corresponds to, possibly attracting certain creatures based on the subtype
		//maybe a caster with a significantly greater amount of a certain mana will 'smell' strongly of the mana
	
//	int ID; //spellID, unique reference INT to identify a spell based on its attributes, not its name
	float target;	//the target of a spell is the target value that the spellcaster's Cantus should sum out to. for a better treatment on spellcasting, check the readme
	SpellCastingConstants::SpellIntensity intensity; //the intensity or strength of a spell. Allows spells to scale with the caster's level.	
	SpellCastingConstants::TargetSystem targeting; //the range and extent of creatures that this individual spell will target	
	SpellCastingConstants::SpellEffect effect; //the effect of a spell (healing, damage, teleporting, etc). This can (and eventually will) form a useless combination with effect and targetsysem
	SpellCastingConstants::TargetType targetType;
	//SpellCastingConstants::ExpectedTarget expected; //the type of 'thing' the spell will target (items, creatures, floor tiles)
	float cost; //a value used to determine the total cost for casting a spell; this is raised or lowered based on the attributes of the spell (targetSystem, spellEffect)
	TargetingSystems::t_TargetingSystem targetSystem;
	EffectSystems::t_EffectSystem effectSystem;
	
	

	Spell(float target = 0, SpellCastingConstants::SpellIntensity intensity = SpellCastingConstants::NO_INTENSITY, SpellCastingConstants::TargetSystem targeting = SpellCastingConstants::NO_TARGET, 
			SpellCastingConstants::SpellEffect effect = SpellCastingConstants::NO_EFFECT, SpellCastingConstants::TargetType targetType = SpellCastingConstants::NO_TYPE, float cost = 0);

	//TODO: think of what would be best way to handle wands (procedural, not procedural, some of either)
	Spell *newSpell(Actor *caster);	//returns a spell with all aspects randomly generated
	void chooseIntensity(int level);	//choose the Intensity of the spell
	void chooseTargetSystem();	//choose the TargetSystem of the spell
	float setTarget(); //this function sets the targetNumber for a spell; this is the value to which the results of the spell's formula are compared in order to see if a spell is successfully cast
	void chooseEffect();	//choose the SpellEffect of the spell
	bool cast(Actor *caster) const; //this function resolves the effect of the spell;
	void save(TCODZip &zip);
	void load(TCODZip &zip);
	static Spell *create(TCODZip &zip);
	void setName();	//set the name of the spell, depending on its attributes (not a traditional setter)
	const char *getName() {return name.c_str();}	//getter for the spell name

protected:
	std::string name;	//the name of the spell	
};


//now take the idea of a "Cantus," a chant that the caster will mutter in order to cast a spell. The cantus corresponds to 4 characters, which hold integer values from 0 to 26


//this might not be the best way to do it, but to create a completely usable spell,
//create either anSpell, call chooseIntensity, setTarget, chooseTargetSystem, chooseEffect, and then finally setName()