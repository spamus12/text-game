#include "item.h"
#include "actor.h"


// Forward declaration of Actor class
class Actor;
class Character;

/* Item class */

// Default use function for all items
const void Item::DEFAULT_USE(Character&) {
	cout << "This cannot be used here." << endl;
}

// Execute the use function
void Item::use(Character& target) {
	fUse(target);
}

// Getters and setters
string Item::getName() const {
	return name;
}
string Item::getDesc() const {
	return desc;
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


/* Armor class */
unsigned int Armor::getDefense() const {
	return defense;
}
