class Aura : public Persistent {	//have to make a factory just like with Pickables, so that you can save and load all the different aura types
public:
	enum StatType {
		NONE,TOTALSTR,TOTALPOWER,TOTALDEX,TOTALINTEL,TOTALDODGE,
		TOTALDR,
		HEALTH,
		MAXHEALTH,
		LIGHT,
		SHIELD
	};
	enum LifeStyle {
		CONTINUOUS,    // buff/debuff that lasts for a certain time
		ITERABLE       // buff/debuff that progressively gets more intense over a certain time, disappears afterwards
		//,WASTING     // some kind of debuff that lasts a long time, gets more intense every step_count of turns, and then stays at some peak value for a long time
	};
	
	int totalDuration; //how many turns should this last?
	int duration; //how many turns are left?
	int bonus; //what is the magnitude of this aura's effect?
	StatType stat; //what stat(s) should this aura affect?
	LifeStyle life; //how should this aura act during its duration?
	
	Aura(int duration = 0, int bonus = 0, StatType stat = NONE, LifeStyle life = CONTINUOUS);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
	virtual void apply(Actor *target) = 0;
	virtual void unApply(Actor *target) = 0;
	static Aura *create(TCODZip &zip);
};


class ShieldAura : public Aura {
public:
	ShieldAura(int duration = 0, int bonus = 0, LifeStyle life = CONTINUOUS);
	void apply(Actor *target);
	void unApply(Actor *target);
};

class Heal : public Aura {
public:
	Heal(int duration, int tick, LifeStyle life = ITERABLE);	//tick is the amount healed per turn (or once)
	void apply(Actor *target);
	void unApply(Actor *target);
};

class HealthBoost : public Aura {
public:
	HealthBoost(int duration, int boost, LifeStyle life = CONTINUOUS);	//boost is the amount your maxHealth is boosted by
	void apply(Actor *target);
	void unApply(Actor *target);
};

class LightBoost : public Aura {
public:
	LightBoost(int duration, int boost, LifeStyle life = CONTINUOUS);
	void apply(Actor *target);
	void unApply(Actor *target);
};

class DRBoost : public Aura {
public:
	DRBoost(int duration, int boost, LifeStyle life = CONTINUOUS);
	void apply(Actor *target);
	void unApply(Actor *target);
};