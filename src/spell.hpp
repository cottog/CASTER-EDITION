#include "libtcod.hpp"
class Spell{ //abstract class representing a generic spell; note the pure virtual function cast()
public:
	enum SpellIntensity { //the "intensity" of the spell is a means to scale the spell with caster level; higher-intensity spells cost more and have greater or longer-lasting effects
		NO_INTENSITY = 0,MINOR,MAJOR,NORMAL,EPIC
	};
	enum ExpectedTarget {    //whether this particular spell is targeting items, creatures, or people
		NO_EXPECT, ITEM, CREATURE, TILE   //item is as yet unused
	};
	enum TargetType {   //who the spell would prefer to target
		NO_TYPE,NEUTRAL, ENEMY, FRIENDLY
	};
	enum TargetSystem {   //this determines how the spell will select a target
		NO_TARGET,
		SELF, //target the caster
		ADJACENT_TILE, //one random tile next to the caster
		BOLT_SPELL, //travels in a line from caster to enemy and hits the first one in the line
		VISUAL_HOLOCAUST, //choose an enemy, all enemies of that type (in sight) take damage; so, if you pick an orc, all orcs in sight take damage
		LEVEL_WIDE_HOLOCAUST, //exactly as the above, but level-wide instead of restricted to sight
		RANDOM_IN_LOS,  //single random target in LOS
		RANDOM_IN_LEVEL, //single random target in level
		ALL_ADJACENT_TILES, //chooses creatures from the 8 tiles immediately adjacent to the caster
		ALL_CREATURES_CHAINING,  //like chain lightning
		ALL_CREATURES_IN_LEVEL, //chooses all creatures in the level
		ALL_CREATURES_IN_LINE, //penetrating spell; penetrates targets, ignores creatures that aren't targets.
		ALL_CREATURES_IN_RADIUS, //ball spell
		SINGLE_CREATURE_IN_SIGHT, //single chosen target in sight. not random like the above one
		X_CREATURES_IN_SIGHT, //chooses X random creatures in LoS of the caster
		ALL_CREATURES_IN_LOS //chooses all creatures in line of sight
	};
	enum SpellEffect {  //this is the effect of the spell (pretty straight-forward)
		NO_EFFECT,
		STRAIGHT_HEAL, //1 heal damage from the caster (restore HP)
		HEAL_OVER_TIME, //2 restore hp a small amount for a set number of turns
		STAT_BOOST, //3 increase a stat or stats
		CURE_DEBUFFS, //4 remove harmful status effects from the caster
		DETECTION, //5 detect items or creatures on the specified tiles
		POLYMORPH,      //6 permanent changing of a creature's form
		SHAPE_SHIFTING, //7 temporary changing of a creature's form
		LIGHT,          //8 not sure how an elemental subtype of a light or darkness spell would work
		ILLUSION, //9 create an illusory version of the caster in the specified tiles
		SIMULACRUM, //10 create a copy of the target creature a few tiles away from it, that is friendly towards the caster
		SHIELD, //11 shield the caster from a set amount (and subtype) of damage before disappearing
		REFLECTION, //12 reflect a set amount (and type) of damage back towards the attacker
		ABSORPTION, //13 absorb damage done by to the caster by spells of a certain type, refunding mana to the caster and preventing damage
		ANTIMAGIC_ZONE, //14 create a zone in which magic (or perhaps a certain elemental subtype of magic) cannot be cast (even by the caster) for a certain amount of time
		DAMAGING_TELEPORT, //15 teleports the target(s) and damages those around the target(s)'s starting location in a radius
		WEAPON_ENHANCEMENT, //16 gives an elemental brand (and perhaps other stat bonuses) to the equipped weapon(s) of the selected target for a limited amount of time
		PROTECT_FROM_HAZARDS, //17 protects the target(s) from elemental hazards (lava, glass shards, radiation, etc...), as determined by the elemental subtype of the casting
		TELEPORT, //18 teleports the target(s) to a random location
		DAMAGING_AURA, //19 places a damaging circular aura around the target(s) that damages surrounding creatures indiscriminately (enemies, neutrals, and friendlies could be damaged)
		INTELLIGENT_DAMAGING_AURA, //20 places a damaging circular aura around the the target(s) that damages only the enemies of the target
		CONTROLLED_TELEPORT, //21 teleports target(s) to a location chosen by the caster; 
		
		//these ones aren't great for self-targeting spells
		DAMAGING_PULL, //22 pulls target(s) strictly towards the caster, while also doing elemental damage to them
		DAMAGING_PUSH, //23 pushes target(s) strictly away from the caster and damages them. perhaps does extra damage if they hit a wall and cannot be pushed the full distance away
		PULL, //24 pull target(s) towards the caster
		PUSH, //25 push target(s) away from the caster
		STRAIGHT_DAMAGE, //26 does HP damage to the target(s)
		BLEED_DAMAGE, //27 does small HP damage to the target(s)
		STAT_DRAIN, //28 reduce the target(s) stat(s)
		STAT_SAPPING, //29 reduce the target(s) stat(s) and increase the caster's corresponding stat(s)
		LIFE_LEECHING, //30 do bleed damage to the target(s) and do a corresponding amount of healing over time to the caster
		MANA_LEECHING, //31 do bleed damage to the target(s)'s mana resource(s) and restore a corresponding amount of the same resource(s) to the caster [do nothing if they have none]
		INSTAKILL, //32 instantly reduce the target(s) HP to zero
		DEBUFFING, //33 lower the stats of a target, or inflicts them with certain other status effects (blinded, slowed, confused, etc) for a certain amount of time
		UNSUMMON,       //34 would seem to be incompatible with most targeting types (or at least it will be usually useless), but it could attempted to completely destroy a summoned target and just weaken non-summoned targets
		BANISHING, //35 send the target(s) to another dimension, removing them from the map, but not necessarily killing them; hopefully, I will get around to making this "other dimension" an accessible location, so that the player could be sent here (or send himself here) and have to fight the enemies he sends away
		MANA_DAMAGE, //36 inflict damage to the target(s) mana resource(s) [do nothing if they have none]
		DOOM_TIMER, //37 set a timer of a set length on the target(s), which decrements each turn; after the timer reaches zero, they are instakilled
		DARKNESS,       //38 not sure how an elemental subtype of a light or darkness spell would work, but perhaps hide or highly increase the stealth value of a creature that would be masked by the elemental darkness (water creature hide in water-darkness)
		PERCENTILE_DAMAGE, //39 deals a set percentage of the target(s) max HP
		RESURRECT, //40 resurrect dead target(s), does nothing to living target(s)
		
		
		//these ones aren't so compatible with all targeting types
		TELEPORTAL, //41 creates a point on the ground to which the caster will return when he casts a teleportation spell, if he's in range; otherwise, his location upon teleportation will be random
		ALTER_TERRAIN, //42 adds or removes hazards from the ground (lava pools, shards of glass, etc)
		LINK, //43 "links" the caster and the target. they will share damage or debuffs or something
		SUMMON, //44 summons a random creature near the caster
		LIFE_TAP //45 sacrifices some of the caster's life in exchange for mana
	};
	enum ElementalSubtype {  //these are the possible subtypes of a spell, determined by how much of each resource is spent on it
		NO_SUBTYPE = 0,	//0000	NOTHING 				
		FIRE,			//0001	FIRE 					Intelligence
		AIR,			//0010	AIR 					Dexterity (ranged accuracy)
		LIGHTNING,		//0011	FIRE+AIR 				Speed
		WATER,			//0100	WATER 					Mana
		STEAM,			//0101	WATER+FIRE 				
		ICE,			//0110	WATER+AIR 				
		RADIATION,		//0111	FIRE+AIR+WATER 			Mix stats around
		EARTH,			//1000	EARTH 					Power (bonus damage)
		LAVA,			//1001	FIRE+EARTH 				lower speed and small DoT
		DUST,			//1010	AIR+EARTH 				Strength (melee accuracy)
		GLASS,			//1011	FIRE+AIR+EARTH 			
		MUD,			//1100	WATER+EARTH 			fovRadius
		METAL,			//1101	FIRE+WATER+EARTH 		lower weapon damage	
		POISON,			//1110	EARTH+AIR+WATER 		maxHP
		FORCE			//1111	FIRE+AIR+WATER+EARTH 	
	};	//originally I planned for some elemental subtypes to have shared resistances, but I figure why not let each elemental subtype have its own resistance? I might implement a system such that gaining resistance in a base element propagates to a lesser extent to its derivative elements, as outlined above
	
	//TODO: create the LastCast object for casting spell's more quickly
	//TODO: consider creating a tiny description of each spell that indicates what its purpose may be; this would be achieved by having each spell contain a string or TCODText object and you appending characters to it that would be defined in a wiki for players to read
	//TODO: (not so much for caster module, but for the whole engine) add a function for an actor to determine whether or not it is seen by the player, to keep messages from being displayed in error
	//TODO: implement a 'mana-scent' system, in which some spells whose elemental subtype otherwise wouldn't matter (regular teleport) can now have an effect. A spell will emit a surge of mana-scent of the elemental subtype it corresponds to, possibly attracting certain creatures based on the subtype
		//maybe a caster with a significantly greater amount of a certain mana will 'smell' strongly of the mana
	
//	int ID; //spellID, unique reference INT to identify a spell based on its attributes, not its name
	float target;	//the target of a spell is the target value that the spellcaster's Cantus should sum out to. for a better treatment on spellcasting, check the readme
	SpellIntensity intensity;	//the intensity or strength of a spell. Allows spells to scale with the caster's level. Minor, Normal, Major, or Epic
	TargetSystem targeting; //the range and extent of creatures that this individual spell will target
	SpellEffect effect; //the effect of a spell (healing, damage, teleporting, etc). This can (and eventually will) form a useless combination with effect and targetsysem
	ExpectedTarget expected; //the type of 'thing' the spell will target (items, creatures, floor tiles)
	float cost; //a value used to determine the total cost for casting a spell; this is raised or lowered based on the attributes of the spell (targetSystem, spellEffect)
	
	Spell(float target = 0, SpellIntensity intensity = NO_INTENSITY, TargetSystem targeting = NO_TARGET, SpellEffect effect = NO_EFFECT, 
		ExpectedTarget expected = NO_EXPECT, float cost = 0);
	
	//TODO: think of what would be best way to handle wands (procedural, not procedural, some of either)
	Spell *newSpell(Actor *caster);	//returns a spell with all aspects randomly generated
	void chooseIntensity(int level);	//choose the Intensity of the spell
	void chooseTargetSystem();	//choose the TargetSystem of the spell
	float setTarget(); //this function sets the targetNumber for a spell; this is the value to which the results of the spell's formula are compared in order to see if a spell is successfully cast
	virtual void chooseEffect() = 0;	//choose the SpellEffect of the spell
	virtual bool cast(Actor *caster) = 0; //this function resolves the effect of the spell;
	virtual void save(TCODZip &zip) = 0;
	virtual void load(TCODZip &zip) = 0;
	static Spell *create(TCODZip &zip);
	void setName();	//set the name of the spell, depending on its attributes (not a traditional setter)
	const char *getName() {return name;}	//getter for the spell name

protected:
	const char *name;	//the name of the spell	
};

class CreatureSpell : public Spell { //spell class that represents spells that target other Actor objects
public:
	TargetType preferred; //the type of enemy (neutral, enemy, or ally) the spell would optimally target; of course, the spell could potentially do the exact opposite
	TargetType actual; //the type of enemy (neutral, enemy, or ally) that the spell actually targets, as opposed to the preferred type

	void chooseEffect();
	CreatureSpell( float target = 0, SpellIntensity intensity = NO_INTENSITY, TargetSystem targeting = NO_TARGET, SpellEffect effect = NO_EFFECT, 
		ExpectedTarget expected = CREATURE,  float cost = 0, TargetType preferred = NO_TYPE, TargetType actual = NO_TYPE);
	//use the above constructor to create an empty CreatureSpell object, then use chooseIntensity, then chooseTargetSystem, and then chooseEffect to create a non-empty spell
	bool cast(Actor *caster);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
};

class TileSpell : public Spell { //spell class that represents spells that target map tiles
public:
	void chooseEffect();
	TileSpell( float target = 0, SpellIntensity intensity = NO_INTENSITY, TargetSystem targeting = NO_TARGET, SpellEffect effect = NO_EFFECT, 
		ExpectedTarget expected = TILE, float cost = 0 );
	//use the above constructor to create an empty TileSpell object, then use chooseIntensity, then chooseTargetSystem, and then chooseEffect to create a non-empty spell
	bool cast(Actor *caster);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
};


//now take the idea of a "Cantus," a chant that the caster will mutter in order to cast a spell. The cantus corresponds to 4 characters, which hold integer values from 0 to 26


//this might not be the best way to do it, but to create a completely usable spell,
//create either an instance of TileSpell or CreatureSpell, call chooseIntensity, setTarget, chooseTargetSystem, chooseEffect, and then finally setName()