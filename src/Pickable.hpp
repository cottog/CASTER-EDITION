class Pickable : public Persistent {
public:
	enum PickableType {
		NONE,HEALER, LIGHTNING_BOLT, CONFUSER, POISON, FIREBALL, EQUIPMENT, WEAPON
	};
	PickableType type;
	bool stacks; //can this item be stacked?
	int stackSize; //if it can, how big is the current stack?
	int value; //how much is this item worth (in currency)
	
	Pickable(PickableType type = NONE,bool stacks = 1, int stackSize = 1, int value = 0);
	bool pick(Actor *owner, Actor *wearer);
	virtual bool use(Actor *owner, Actor *wearer);
	void drop(Actor *owner,Actor *wearer);
	static Pickable *create(TCODZip &zip);
};

class Healer: public Pickable {
public:
	float amount; //how much HP it restores
	
	Healer(float amount);
	bool use(Actor *owner, Actor *wearer);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};

class LightningBolt: public Pickable {
public:
	float range, damage;
	
	LightningBolt(float range, float damage);
	bool use(Actor *owner, Actor *wearer);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};

class Fireball : public LightningBolt {
public:
	float aoe;
	
	Fireball(float range, float aoe, float damage);
	bool use(Actor *owner, Actor *wearer);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
};

class Confuser : public Pickable {
public:
	int nbTurns;
	float range;
	Confuser(int nbTurns, float range);
	bool use(Actor *owner, Actor *wearer);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};

class Poison : public Pickable {
public:
	float tick; //how much damage does this do each turn?
	int duration; //how long does this last?
	
	Poison(float tick, int duration);
	bool use(Actor *owner, Actor *wearer);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};

class ItemBonus : public Persistent {
public:
	enum BonusType {
		NOBONUS,HEALTH,DODGE,DR,STRENGTH,DEXTERITY,INTELLIGENCE,LIGHT
	};
	BonusType type;
	float bonus;
	
	ItemBonus(BonusType type, float bonus);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};

class ItemReq : public Persistent {
public:
	enum ReqType {
		NOREQ,STRENGTH,DEXTERITY,INTELLIGENCE
	};
	ReqType type;
	float requirement;
	
	ItemReq(ReqType type, float requirement);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};

class Equipment : public Pickable {
public:
	enum SlotType {
		NOSLOT, HEAD, CHEST, LEGS, FEET, HAND1, GLOVES,
		RING, HAND2, RANGED
	};

	bool equipped;
	SlotType slot;
	TCODList<ItemBonus *> bonus;
	ItemReq *requirement;
	
	Equipment(SlotType slot = NOSLOT, 
		TCODList<ItemBonus *> = new TCODList<ItemBonus *>(), ItemReq *requirement = NULL);
	bool use(Actor *owner, Actor *wearer);
	bool requirementsMet(Actor *owner, Actor *wearer);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};

class Weapon : public Equipment {
public:
	enum WeaponType {
		NOTYPE,LIGHT,HEAVY,RANGED
	};
	
	int diceNum;   //the number of dice to be rolled
	int diceType;  //the type of dice to be rolled      "[diceNum]d[diceType]"
	int critMult;
	int critRange;
	WeaponType wType;
	
	Weapon(int diceNum = 0, int diceType = 0, int critMult = 2, int critRange = 20, 
		WeaponType wType = NOTYPE, SlotType slot = NOSLOT, 
		TCODList<ItemBonus *> bonus = new TCODList<ItemBonus *>(), ItemReq *requirement = NULL);
	bool use(Actor *owner, Actor *wearer);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};
