#pragma once
#include <functional>
#include <iostream>
using namespace std;


// Forward declaration of actor classes
class Actor;
class Character;
class Player;

// Base class for all items
class Item {

protected:

	// Default use function for all items
	const static void DEFAULT_USE(Character&);

	// Name of the item
	string name;

	// Description of the item
	string desc;

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
	void use(Character& target);

	// Getters and setters
	string getName() const;
	string getDesc() const;

	void setName(string n);
	void setDesc(string d);
	void setUse(function<void(Character&)> f);

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

public:
	Weapon(string _name, string _desc, unsigned int _damage, DamageType _damageType, function<void(Character&)> _fUse = DEFAULT_USE) :
		Item(_name, _desc, _fUse),
		damage{ _damage },
		damageType{ _damageType }
	{ }


	// Getters and setters
	unsigned int getDamage() const;
	DamageType getDamageType() const;

};


// Armor class
class Armor : public Item {

protected:

	// Damage reduction
	unsigned int defense;

public:
	Armor(string _name, string _desc, unsigned int _defense, function<void(Character&)> _fUse = DEFAULT_USE) :
		Item(_name, _desc, _fUse),
		defense{ _defense }
	{ }


	unsigned int getDefense() const;
};
