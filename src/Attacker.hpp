class Attacker : public Persistent {
public:
	float basePower; //attack power
	float totalPower; //attack power
	float baseStr; //melee accuracy
	float totalStr;
	float baseDex; //ranged accuracy
	float totalDex;
	Actor *lastTarget; //last target attacked by this attacker
	
	Attacker (float power, float str = 0, float dex = 0);
	Attacker(const Attacker &attacker);
	void attack(Actor *owner, Actor *target);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};