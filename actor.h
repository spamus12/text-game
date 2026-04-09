#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;


// Forward declaration of item classes
class Item;
class Weapon;
class Armor;

// Forward declaration of classes
class Actor;
class Character;
class Player;

// Base class for all actors
class Actor {

protected:

	// Default examination function
	const function<void()> DEFAULT_EXAMINE;

	// The actor's unique identifier
	string ID;

	// The actor's name
	string name;

	// The actor's initial description
	string initDesc;

	// Store the examine function
	function<void()> fExamine;

public:

	// Constructor
	Actor(string _ID, string _name);


	// Run interaction function
	void examine();


	// Getter and setter functions
	string getID() const;
	string getName() const;
	string getInitDesc() const;

	void setID(string i);
	void setName(string n);
	void setInitDesc(string d);

	// Set examine function
	void setExamine(function<void()> f);

};


// Character class

class Character : public Actor {

private:
	// Default members
	const function<void()> DEFAULT_TALK;
	const function<void(shared_ptr<Player>)> DEFAULT_ATTACK;
	const function<void(shared_ptr<Player>)> DEFAULT_TURN;
	
protected:

	// Max and current HP
	unsigned int maxHP;
	int currentHP;

	// Inventory
	vector<shared_ptr<Item>> inventory;

	// Gear
	shared_ptr<Weapon> weapon;
	shared_ptr<Armor> armor;

	// Store the talk function
	function<void()> fTalk;

	// Store the attack function
	function<void(shared_ptr<Player>)> fAttack;

	// Store the character's combat turn
	function<void(shared_ptr<Player>)> fTurn;

public:

	// Constructor
	Character(
		string _ID, string _name, unsigned int _maxHP, vector<shared_ptr<Item>> _inventory,
		shared_ptr<Weapon> _weapon, shared_ptr<Armor> _armor);

	// Equip gear
	void equip(shared_ptr<Weapon> w);
	void equip(shared_ptr<Armor> a);

	

	// Run talk function
	void talk();

	// Run attack function
	void attack(shared_ptr<Player>);

	// Run turn function
	void turn(shared_ptr<Player>);

	// Return the equipped weapon
	shared_ptr<Weapon> getWeapon() const;

	// Return the quipped armor
	shared_ptr<Armor> getArmor() const;

	// Hurt and heal the character
	void hurt(unsigned int amount);
	void heal(unsigned int amount);

	// Getters and Setters
	unsigned int getMaxHP() const;
	int getCurrentHP() const;
	vector<shared_ptr<Item>> getInventory() const;
	string getInitDesc() const;

	void setMaxHP(unsigned int amount);
	void setCurrentHP(int amount);
	void setInitDesc(string newDesc);

	void setTalk(function<void()> f);
	void setAttack(function<void(shared_ptr<Player>)> f);
	void setTurn(function<void(shared_ptr<Player>)> f);

};


// Player class
class Player : public Actor {

protected:

	// Max and current HP
	unsigned int maxHP;
	int currentHP;

	// Level and experience
	unsigned int level;
	unsigned int exp;

	// Inventory
	vector<shared_ptr<Item>> inventory;

	// Gear
	shared_ptr<Weapon> weapon;
	shared_ptr<Armor> armor;

public:
	Player(string _name) :
		Actor("PLAYER", _name),
		maxHP{ 20 },
		currentHP{ static_cast<int>(maxHP) },
		level{ 1 },
		exp{ 0 },
		inventory{},
		weapon{},
		armor{}
	{ }

	// Check if the player can level up and do so
	void levelUp();

	// Give the player exp
	void giveExp(unsigned int amount);

	// Add an item to the player's inventory
	void give(shared_ptr<Item> item);

	// Equip gear
	void equip(shared_ptr<Weapon> w);
	void equip(shared_ptr<Armor> a);

	// Hurt and heal the player
	void hurt(unsigned int amount);
	void heal(unsigned int amount);

	// Getters and setters
	unsigned int getMaxHP() const;
	int getCurrentHP() const;
	unsigned int getLevel() const;
	unsigned int getExp() const;
	vector<shared_ptr<Item>> getInventory() const;
	shared_ptr<Weapon> getWeapon() const;
	shared_ptr<Armor> getArmor() const;

	void setMaxHP(unsigned int amount);
	void setCurrentHP(int amount);
	void setExp(unsigned int amount);

};
