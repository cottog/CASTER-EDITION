class Ai : public Persistent {
public:
	float waitTime;	//how much time must elapse until this actor's next turn?
	float attackTime; //how much time does attacking take up?
	float walkTime; //how much time does walking take up?

	Ai(float waitTime = 0, float attackTime = 1, float walkTime = 1);
	virtual void update(Actor *owner)=0;
	static Ai *create(TCODZip &zip);
protected:
	enum AiType {
		MONSTER, CONFUSED_MONSTER, PLAYER
	};
};


class PlayerAi:public Ai {
public:
	PlayerAi(float waitTime = 0, float attackTime = 1, float walkTime = 1);
	int getNextLevelXp(Actor *owner);
	void update(Actor *owner);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
protected:
	bool moveOrAttack(Actor *owner, int targetx, int targety);
	void handleActionKey(Actor *owner, int ascii, bool control, bool alt);
	Actor *choseFromInventory(Actor *owner);
};

class MonsterAi : public Ai {
public:
	MonsterAi(float waitTime = 0, float attackTime = 1, float = 1);
	void update(Actor *owner);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
protected:
	int moveCount;
	
	void moveOrAttack(Actor *owner, int targetx, int targety);
};

class ConfusedMonsterAi : public Ai {
public:
	ConfusedMonsterAi(int nbTurns, Ai *oldAi,float waitTime = 0, float attackTime = 1, float = 1);
	void update(Actor *owner);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
protected:
	int nbTurns;
	Ai *oldAi;
};


