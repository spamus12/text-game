#pragma once
#include <functional>
#include <iostream>
#include <memory>
using namespace std;


// Forward declaration of actor classes
class Actor;
class Character;
class Player;

// Base class for all items
enum ItemType {
	TRINKET,
	WEAPON,
	ARMOR,
	HEAL,
	ATTACK
};
class Item {

protected:

	// Default use function for all items
	static void DEFAULT_USE(Character&);

	// Name of the item
	string name;

	// Description of the item
	string desc;

	// The type of item
	ItemType itemType{ TRINKET };

	// Use the item
	// Requires a target
	function<void(Character&)> fUse;

public:
	Item(string _name, string _desc, function<void(Character&)> _fUse = DEFAULT_USE) :
		name{ _name },
		desc{ _desc },
		fUse{ _fUse }
	{ }

	// Use the item
	virtual void use(Character& target);

	// Print the item's description
	void printDesc() const;

	// Getters and setters
	string getName() const;
	string getDesc() const;
	ItemType getItemType() const;

	void setName(string n);
	void setDesc(string d);
	void setUse(function<void(Character&)> f);

	// Virtual destructor
	virtual ~Item() = default;

};


// Weapon class
enum DamageType {
	PIERCING,
	SLASHING,
	BLUNT
};
class Weapon : public Item {

protected:

	// Weapon damage
	unsigned int damage;

	// Damage type
	DamageType damageType;

	// Speed effect
	int speedEffect;

	// The type of item
	ItemType itemType{ WEAPON };

public:
	Weapon(string _name, string _desc, unsigned int _damage, DamageType _damageType, int _speedEffect, function<void(Character&)> _fUse = DEFAULT_USE) :
		Item(_name, _desc, _fUse),
		damage{ _damage },
		damageType{ _damageType },
		speedEffect{ _speedEffect }
	{ }


	// Getters and setters
	unsigned int getDamage() const;
	DamageType getDamageType() const;
	int getSpeedEffect() const;

};


// Armor class
class Armor : public Item {

protected:

	// Damage reduction
	unsigned int defense;

	// Speed effect
	int speedEffect;

	// Item type
	ItemType itemType{ ARMOR };

public:
	Armor(string _name, string _desc, unsigned int _defense, int _speedEffect, function<void(Character&)> _fUse = DEFAULT_USE) :
		Item(_name, _desc, _fUse),
		defense{ _defense },
		speedEffect{ _speedEffect }
	{ }


	unsigned int getDefense() const;
	int getSpeedEffect() const;
};


// Support class
class Support : public Item {

protected:
	// The magnitude of the support
	unsigned int effect;

public:
	Support(string _name, string _desc, ItemType _itemType, unsigned int _effect) :
		Item(_name, _desc),
		effect{ _effect }
	{ 
		itemType = _itemType;
	}

	// Use function is unique to support items
	void use(shared_ptr<Character> target);
	void use(shared_ptr<Player> target);

	unsigned int getEffect() const;

};
