class Caster : public Persistent {
public:
	float mana;
	float totalMana;
	

	bool cast(Spell spell);
protected:
	std::String *cantus;	
};
