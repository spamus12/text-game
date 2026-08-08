#include "item.h"
#include "actor.h"


// Forward declaration of Actor class
class Actor;
class Character;

/* Item class */

// Default use function for all items
void Item::DEFAULT_USE(Character&) {
	cout << "This cannot be used here." << endl;
}

// Execute the use function
void Item::use(Character& target) {
	fUse(target);
}

// Print the item's description
void Item::printDesc() const {
	cout << desc << endl;
}

// Getters and setters
string Item::getName() const {
	return name;
}
string Item::getDesc() const {
	return desc;
}
ItemType Item::getItemType() const {
	return itemType;
}

void Item::setName(string n) {
	name = n;
}
void Item::setDesc(string d) {
	desc = d;
}
void Item::setUse(function<void(Character&)> f) {
	fUse = f;
}


/* Weapon class */

unsigned int Weapon::getDamage() const {
	return damage;
}
DamageType Weapon::getDamageType() const {
	return damageType;
}
int Weapon::getSpeedEffect() const {
	return speedEffect;
}


/* Armor class */

unsigned int Armor::getDefense() const {
	return defense;
}
int Armor::getSpeedEffect() const {
	return speedEffect;
}


/* Support class */

// When used on an enemy
void Support::use(shared_ptr<Character> target) {
	target->hurt(effect);
}
// When used on the player
void Support::use(shared_ptr<Player> target) {
	target->heal(effect);
}

unsigned int Support::getEffect() const {
	return effect;
}
