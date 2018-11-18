class Destructible : public Persistent{
public:
	float maxHp; //maximum health points
	float hp; //current health points
	float maxShield;	//maximum shield, which is essentially a secondary, non-fatal health pool
	float shield;		//current shield value
	float baseDodge; //miss bonus to enemy miss chance
	float totalDodge;
	float baseDR;    //hit points deflected
	float totalDR;
	char *corpseName; //the actor's name once dead/destroyed
	int xp;
	SpellCastingConstants::ElementalSubtype element;

	Destructible(float maxHp, float dodge, const char *corpseName, int xp=0, float DR=0, float maxShield = 0, SpellCastingConstants::ElementalSubtype element = SpellCastingConstants::FORCE);
	~Destructible();
	inline bool isDead() {return hp <= 0; }
	float takeDamage(Actor *owner, Actor *attacker, float damage);
	virtual void die(Actor *owner);
	void resurrect(Actor *owner, float percentage);	//resets the character's blocking bool, character, and color; the character's hp is equal to percentage*maxHp; it will only go up to maxHp
	float heal(float amount);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
	static Destructible *create(TCODZip &zip);
	
protected:
	enum DestructibleType {
		MONSTER, PLAYER
	};
};

class MonsterDestructible : public Destructible{
public:
	MonsterDestructible(float maxHp, float defense, const char *corpseName, int xp);
	void die(Actor *owner);
	void save(TCODZip &zip);
};

class PlayerDestructible : public Destructible {
public:
	PlayerDestructible(float maxHp, float defense, const char *corpseName,float maxShield = 0);
	void die(Actor *owner);
	void save(TCODZip &zip);
};




