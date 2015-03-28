#include "main.hpp"

Pickable::Pickable(PickableType type,bool stacks, int stackSize, int value) :
	type(type),stacks(stacks),stackSize(stackSize),value(value) {
}

Pickable *Pickable::create(TCODZip &zip) {
	PickableType type = (PickableType)zip.getInt();
	Pickable *pickable = NULL;
	switch(type) {
		case NONE: break;
		case HEALER: pickable = new Healer(0); break;
		case LIGHTNING_BOLT: pickable = new LightningBolt(0,0); break;
		case CONFUSER: pickable = new Confuser(0,0); break;
		case POISON: pickable = new Poison(0,0); break;
		case FIREBALL: pickable = new Fireball(0,0,0); break;
		case EQUIPMENT: pickable = new Equipment(); break;
		case WEAPON: pickable = new Weapon(); break;
	}
	pickable->load(zip);
	return pickable;
}

bool Pickable::pick(Actor *owner, Actor *wearer){
	if (wearer->container && wearer->container->add(owner)) {
		engine.actors.remove(owner);
		return true;
	}
	return false;
}

bool Pickable::use(Actor *owner, Actor *wearer) {
	if (wearer->container && owner->pickable->stackSize < 2) {
		wearer->container->remove(owner);
		delete owner;
		return true;
	} else {
		owner->pickable->stackSize -= 1;
		return true;
	}
	return false;
}

void Pickable::drop(Actor *owner,Actor *wearer) {
	if (wearer->container) {
		if ((owner->pickable->type == EQUIPMENT || owner->pickable->type == WEAPON ) && ((Equipment*)(owner->pickable))->equipped) {
			((Equipment*)(owner->pickable))->use(owner,wearer);
		}
		int nbDropped = 1;
	
		if (nbDropped >= owner->pickable->stackSize) {
			wearer->container->remove(owner);
			engine.actors.push(owner);
			owner->x = wearer->x;
			owner->y = wearer->y;
			engine.gui->message(TCODColor::lightGrey,"%s drops a %s.",wearer->getName(),owner->getName());
		} else {
			Pickable *pick = owner->pickable;
			Actor *droppy = new Actor(wearer->x,wearer->y,owner->ch, owner->getName(), owner->col);
			droppy->blocks = false;
			droppy->ID = owner->ID;
			droppy->pickable = pick;
			
			owner->pickable->stackSize -= nbDropped;
			droppy->pickable->stackSize = nbDropped;
			engine.actors.push(droppy);
			engine.sendToBack(droppy);
		}
	}
}

Healer::Healer(float amount) : Pickable(HEALER),amount(amount) {
}

void Healer::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putFloat(amount);
	zip.putInt(stacks);
	zip.putInt(stackSize);
	zip.putInt(value);
}

void Healer::load(TCODZip &zip) {
	amount = zip.getFloat();
	stacks = zip.getInt();
	stackSize = zip.getInt();
	value = zip.getInt();
}

bool Healer::use(Actor *owner, Actor *wearer) {
	if (wearer->destructible) {
		float amountHealed = wearer->destructible->heal(amount);
		if (amountHealed > 0) {
			return Pickable::use(owner,wearer);
		}
	}
	return false;
}

LightningBolt::LightningBolt(float range, float damage) 
	: Pickable(LIGHTNING_BOLT),range(range),damage(damage){
}

void LightningBolt::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putFloat(range);
	zip.putFloat(damage);
	zip.putInt(stacks);
	zip.putInt(stackSize);
	zip.putInt(value);
}

void LightningBolt::load(TCODZip &zip) {
	range = zip.getFloat();
	damage = zip.getFloat();
	stacks = zip.getInt();
	stackSize = zip.getInt();
	value = zip.getInt();
}

bool LightningBolt::use(Actor *owner, Actor *wearer) {
	Actor *closestMonster = engine.getClosestMonster(wearer->x,wearer->y,range);
	if (!closestMonster) {
		engine.gui->message(TCODColor::lightGrey,"No enemy is in range!");
		return false;
	}
	
	//hit closest monster for <damage> hp
	engine.gui->message(TCODColor::lightBlue,
		"A lightning bolt strikes the %s with a deafening crack!",
		closestMonster->getName());
	closestMonster->destructible->takeDamage(closestMonster,owner,damage);
	return Pickable::use(owner,wearer);
}

Fireball::Fireball(float range, float aoe, float damage) : 
	LightningBolt(range, damage),aoe(aoe) {
	type = FIREBALL;
}

void Fireball::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putFloat(range);
	zip.putFloat(aoe);
	zip.putFloat(damage);
	zip.putInt(stacks);
	zip.putInt(stackSize);
	zip.putInt(value);
}

void Fireball::load(TCODZip &zip) {
	range = zip.getFloat();
	aoe = zip.getFloat();
	damage = zip.getFloat();
	stacks = zip.getInt();
	stackSize = zip.getInt();
	value = zip.getInt();
}

bool Fireball::use(Actor *owner, Actor *wearer) {
	int x = engine.player->x;
	int y = engine.player->y;
	if (!engine.pickATile(&x,&y,range,aoe)) {
		return false;
	}
	//burn everything in <range> including the player
	engine.gui->message(TCODColor::orange,"The fireball burns everything within %g tiles",range);
	for (Actor **iterator = engine.actors.begin();
		iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor->destructible && !actor->destructible->isDead()
			&& actor->getDistance(x,y) <= range) {
			engine.gui->message(TCODColor::orange,"The %s howls with pain as its skin begins to sizzle!",actor->getName());
			actor->destructible->takeDamage(actor,owner,damage);
		}
	}
	return Pickable::use(owner,wearer);
}

Confuser::Confuser(int nbTurns, float range) 
	: Pickable(CONFUSER),nbTurns(nbTurns),range(range) {
}

void Confuser::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putInt(nbTurns);
	zip.putFloat(range);
	zip.putInt(stacks);
	zip.putInt(stackSize);
	zip.putInt(value);
}

void Confuser::load(TCODZip &zip) {
	nbTurns = zip.getInt();
	range = zip.getFloat();
	stacks = zip.getInt();
	stackSize = zip.getInt();
	value = zip.getInt();
}

bool Confuser::use(Actor *owner, Actor *wearer) {
	engine.gui->message(TCODColor::cyan,"Please choose an enemy to target");
	int x = engine.player->x;
	int y = engine.player->y;
	engine.gui->message(TCODColor::white,"start %d %d",x,y);
	if (!engine.pickATile(&x,&y,range)) {
		return false;
	}
	Actor *actor = engine.getActor(x,y);
	if (!actor) {
		return false;
	}
	if (actor == engine.player) {
		engine.gui->message(TCODColor::grey,"You try to read the scroll, but you temporarily forget how\nto read!");
		return false;
	}
	//confuse the enemy for <nbTurns> turns
	Ai *confusedAi = new ConfusedMonsterAi(nbTurns, actor->ai);
	actor->ai = confusedAi;
	engine.gui->message(TCODColor::lightGreen,"The eyes of the %s glaze over.",actor->getName());
	return Pickable::use(owner,wearer);
}

Poison::Poison(float tick, int duration) : Pickable(POISON),
	tick(tick),duration(duration) {
}

void Poison::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putFloat(tick);
	zip.putInt(duration);
	zip.putInt(stacks);
	zip.putInt(stackSize);
	zip.putInt(value);
}

void Poison::load(TCODZip &zip) {
	tick = zip.getFloat();
	duration = zip.getInt();
	stacks = zip.getInt();
	stackSize = zip.getInt();
	value = zip.getInt();
}

bool Poison::use(Actor *owner, Actor *wearer) {
	Aura *poison = new HealAura(duration,-1*tick,Aura::ITERABLE);
	wearer->auras.push(poison);
	engine.gui->message(TCODColor::lightGreen,"%s begins to look a little queasy!",wearer->getName());
	return Pickable::use(owner,wearer);
}

ItemBonus::ItemBonus(BonusType type, float bonus) :
	type(type), bonus(bonus) {
}

void ItemBonus::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putFloat(bonus);
}

void ItemBonus::load(TCODZip &zip) {
	type = (BonusType)zip.getInt();
	bonus = zip.getFloat();
}

ItemReq::ItemReq(ReqType type, float requirement) :
	type(type), requirement(requirement) {
}

void ItemReq::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putInt(requirement);
}

void ItemReq::load(TCODZip &zip) {
	type = (ReqType)zip.getInt();
	requirement = zip.getFloat();
}

Equipment::Equipment(SlotType slot, TCODList<ItemBonus *> bonus,ItemReq *requirement) :
	Pickable(EQUIPMENT,false),equipped(false),slot(slot),bonus(bonus),requirement(requirement) {
}

void Equipment::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putInt(equipped);
	zip.putInt(slot);
	zip.putInt(stacks);
	zip.putInt(stackSize);
	zip.putInt(value);
	zip.putInt(bonus.size());
	for (int i = 0; i < bonus.size(); i++) {
		bonus.get(i)->save(zip);
	}
	requirement->save(zip);
}

void Equipment::load(TCODZip &zip) {
	equipped = zip.getInt();
	slot = (SlotType)zip.getInt();
	stacks = zip.getInt();
	stackSize = zip.getInt();
	value = zip.getInt();
	int nbBonus = zip.getInt();
	for (int i = 0; i <nbBonus; i++) {
		ItemBonus *bon = new ItemBonus(ItemBonus::NOBONUS,0);
		bon->load(zip);
		bonus.push(bon);
	}
	ItemReq *req = new ItemReq(ItemReq::NOREQ,0);
	req->load(zip);
	requirement = req;
}

bool Equipment::use(Actor *owner, Actor *wearer) {
	if (!equipped) {
		switch(slot) {
			case HEAD:
					if(requirementsMet(owner,wearer)){
						if (wearer->container->head) {
							engine.gui->message(TCODColor::orange,"You swap out your head item.");
							wearer->container->head->pickable->use(wearer->container->head,wearer);
						}
						
						wearer->container->head = owner;
					}else{
						}
				break;
			case CHEST:
					if(requirementsMet(owner,wearer)){
						if (wearer->container->chest) {
							engine.gui->message(TCODColor::orange,"You swap out your chest item.");
							wearer->container->chest->pickable->use(wearer->container->chest,wearer);
						}
						
						wearer->container->chest = owner;
					}else{
					}
				break;
			case LEGS:
					if(requirementsMet(owner,wearer)){
						if (wearer->container->legs) {
							engine.gui->message(TCODColor::orange,"You swap out your leg item.");
							wearer->container->legs->pickable->use(wearer->container->legs,wearer);
						}
						
						wearer->container->legs = owner;
					}else{
						}
				break;
			case FEET:
					if(requirementsMet(owner,wearer)){
						if (wearer->container->feet) {
							engine.gui->message(TCODColor::orange,"You swap out your feet item.");
							wearer->container->feet->pickable->use(wearer->container->feet,wearer);
						}
						
						wearer->container->feet = owner;
					}else{
						}
				break; 
			case HAND1:
					if(requirementsMet(owner,wearer)){
						if (wearer->container->hand1) {
							engine.gui->message(TCODColor::orange,"You swap out your main hand item.");
							wearer->container->hand1->pickable->use(wearer->container->hand1,wearer);
						} 
						if((((Weapon*)owner->pickable)->wType == Weapon::HEAVY) && (wearer->container->hand2 != NULL)){
							engine.gui->message(TCODColor::orange,"You swap out your off hand item and heft this heavy weapon.");
							wearer->container->hand2->pickable->use(wearer->container->hand2,wearer);
						}
						wearer->container->hand1 = owner;
					}else{
						}
				break;
			case HAND2:
					if(requirementsMet(owner,wearer)){
						if (wearer->container->hand2) {
							engine.gui->message(TCODColor::orange,"You swap out your off hand item.");
							wearer->container->hand2->pickable->use(wearer->container->hand2,wearer);
						}
						if((wearer->container->hand1) && ((Weapon*)wearer->container->hand1->pickable)->wType == Weapon::HEAVY){
							engine.gui->message(TCODColor::orange,"You swap out your heavy main hand item.");
							wearer->container->hand1->pickable->use(wearer->container->hand1,wearer);
						}
						
						wearer->container->hand2 = owner;
					}else{
						}
				break;
			case RANGED:
					if(requirementsMet(owner,wearer)){
						if (wearer->container->ranged) {
							engine.gui->message(TCODColor::orange,"You swap out your ranged item.");
							wearer->container->ranged->pickable->use(wearer->container->ranged,wearer);
						} 
						
						wearer->container->ranged = owner;
					}else{
						}
				break;
			case NOSLOT: break;
			default: break;
		}
		equipped = true;
		for(int i = 0; i < bonus.size(); i++){
			ItemBonus *thisBonus = bonus.get(i);
			switch(thisBonus->type) {
				case ItemBonus::NOBONUS: break;
				case ItemBonus::HEALTH: wearer->destructible->maxHp += thisBonus->bonus; break;
				case ItemBonus::DODGE: wearer->destructible->totalDodge += thisBonus->bonus; break;
				case ItemBonus::DR: wearer->destructible->totalDR += thisBonus->bonus; break;
				case ItemBonus::STRENGTH: wearer->attacker->totalStr += thisBonus->bonus; break;
				case ItemBonus::DEXTERITY: wearer->attacker->totalDex += thisBonus->bonus; break;
				//case ItemBonus::INTELLIGENCE: wearer->totalIntel += thisBonus->bonus; break;
				case ItemBonus::LIGHT: engine.fovRadius += thisBonus->bonus; break;
				default: break;
			}
		}
		wearer->container->sendToBegin(owner);
		return true;
	} else {
		equipped = false;
		switch(slot) {
			case HEAD: wearer->container->head = NULL; break;
			case CHEST: wearer->container->chest = NULL; break;
			case LEGS: wearer->container->legs = NULL; break;
			case FEET: wearer->container->feet = NULL; break;
			case HAND1: wearer->container->hand1 = NULL; break;
			case HAND2: wearer->container->hand2 = NULL; break;
			case RANGED: wearer->container->ranged = NULL; break;
			case NOSLOT: break;
			default: break;
		}
		for(int i = 0; i < bonus.size(); i++){///////
			ItemBonus *thisBonus = bonus.get(i);
			switch(thisBonus->type) {
				case ItemBonus::NOBONUS: break;
				case ItemBonus::HEALTH: 
					wearer->destructible->maxHp -= thisBonus->bonus;
					if (wearer->destructible->hp > wearer->destructible->maxHp) {
						wearer->destructible->hp = wearer->destructible->maxHp;
					}
					break;
				case ItemBonus::DODGE: wearer->destructible->totalDodge -= thisBonus->bonus; break;
				case ItemBonus::DR: wearer->destructible->totalDR -= thisBonus->bonus; break;
				case ItemBonus::STRENGTH: wearer->attacker->totalStr -= thisBonus->bonus; break;
				case ItemBonus::DEXTERITY: wearer->attacker->totalDex -= thisBonus->bonus; break;
				//case ItemBonus::INTELLIGENCE: wearer->totalIntel -= thisBonus->bonus; break;
				case ItemBonus::LIGHT: engine.fovRadius -= thisBonus->bonus; break;
				default: break;
			}
		}
		wearer->container->inventory.remove(owner);
		wearer->container->inventory.push(owner);
		return true;
	}
	return false;
}

bool Equipment::requirementsMet(Actor *owner, Actor *wearer){
	switch(requirement->type){
		case ItemReq::NOREQ:
			return true;
		break;
		case ItemReq::STRENGTH:
			if(wearer->attacker->totalStr >= requirement->requirement){
				return true;
			}else{
				engine.gui->message(TCODColor::orange,"You need %g strength to equip this item!",requirement->requirement);
				return false;
			}
		break;
		case ItemReq::DEXTERITY:
			if(wearer->attacker->totalDex >= requirement->requirement){
				return true;
			}else{
				engine.gui->message(TCODColor::orange,"You need %g dexterity to equip this item!",requirement->requirement);
				return false;
			}
		break;
		case ItemReq::INTELLIGENCE:
			if(wearer->attacker->totalDex >= requirement->requirement){
				return true;
			}else{
				engine.gui->message(TCODColor::orange,"You need %g intelligence to equip this item!",requirement->requirement);
				return false;
			}
		break;
		default: break;
	}
	return false;
}

Weapon::Weapon(int diceNum, int diceType, int critMult, int critRange, 
	WeaponType wType, SlotType slot, TCODList<ItemBonus *> bonus, ItemReq *requirement)
	: Equipment(slot, bonus, requirement), diceNum(diceNum),diceType(diceType),
	critMult(critMult),critRange(critRange),wType(wType) {
	type = WEAPON;
}

void Weapon::save(TCODZip &zip) {
	zip.putInt(type);
	zip.putInt(equipped);
	zip.putInt(slot);
	zip.putInt(stacks);
	zip.putInt(stackSize);
	zip.putInt(value);
	zip.putInt(bonus.size());
	for(int i = 0; i < bonus.size(); i++){
		bonus.get(i)->save(zip);
	}
	requirement->save(zip);
	zip.putInt(diceNum);
	zip.putInt(diceType);
	zip.putInt(critMult);
	zip.putInt(critRange);
	zip.putInt(wType);
}

void Weapon::load(TCODZip &zip) {
	equipped = zip.getInt();
	slot = (SlotType)zip.getInt();
	stacks = zip.getInt();
	stackSize = zip.getInt();
	value = zip.getInt();
	int numBonus = zip.getInt();
	for(int i = 0; i < numBonus; i++){
		ItemBonus *bon = new ItemBonus(ItemBonus::NOBONUS,0);
		bon->load(zip);
		bonus.push(bon);
	}
	ItemReq *req = new ItemReq(ItemReq::NOREQ,0);
	req->load(zip);
	requirement = req;
	diceNum = zip.getInt();
	diceType = zip.getInt();
	critMult = zip.getInt();
	critRange = zip.getInt();
	wType = (WeaponType)zip.getInt();
}

bool Weapon::use(Actor *owner, Actor *wearer) {
	return Equipment::use(owner,wearer);
}

