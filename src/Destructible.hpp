class Destructible : public Persistent{
public:
	float maxHp; //maximum health points
	float hp; //current health points
	float baseDodge; //miss bonus to enemy miss chance
	float totalDodge;
	float baseDR;    //hit points deflected
	float totalDR;
	char *corpseName; //the actor's name once dead/destroyed
	int xp;
	Spell::ElementalSubtype element;

	Destructible(float maxHp, float dodge, const char *corpseName, int xp=0, float DR=0, Spell::ElementalSubtype element = Spell::FORCE);
	~Destructible();
	inline bool isDead() {return hp <= 0; }
	float takeDamage(Actor *owner, Actor *attacker, float damage);
	virtual void die(Actor *owner);
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
	PlayerDestructible(float maxHp, float defense, const char *corpseName);
	void die(Actor *owner);
	void save(TCODZip &zip);
};




