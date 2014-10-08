class Caster : public Persistent {
public:
	float sanguine;
	float totalSanguine;
	float yellow;
	float totalYellow;
	float black;
	float totalBlack;
	float phlegm;
	float totalPhlegm;
	
	bool cast(Spell spell);
	
};
