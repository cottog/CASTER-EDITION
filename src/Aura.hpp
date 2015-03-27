class Aura : public Persistent {	//have to make a factory just like with Pickables, so that you can save and load all the different aura types
public:
	enum StatType {
		NONE,ALL,TOTALSTR,TOTALPOWER,TOTALDEX,TOTALINTEL,TOTALDODGE,TOTALDR,HEALTH,MAXHEALTH,LIGHT,SHIELD
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
	void load(TCODZip &Zip);
	virtual void apply(Actor *target);
	virtual void unApply(Actor *target);
};


class ShieldAura : public Aura {
public:
	ShieldAura(int duration = 0, int bonus = 0);
	void apply(Actor *target);
	void unApply(Actor *target);
};