#include "actor.h"
#include "game.h"


/* Actor class */

Actor::Actor(string _ID, string _name) :
	ID{ _ID },
	name{ _name },
	initDesc{ "NOTHING" }
{

	// Define default examine function
	const function<void()> DEFAULT_EXAMINE = [this]() {
		cout << "It is a simple " << name << "." << endl;
		};

	// Assign examine function
	fExamine = DEFAULT_EXAMINE;
}

// Execute examine function
void Actor::examine() {
	fExamine();
}

// Getter and setter functions
string Actor::getID() const {
	return ID;
}
string Actor::getName() const {
	return name;
}
string Actor::getInitDesc() const {
	return initDesc;
}

void Actor::setID(string i) {
	ID = i;
}
void Actor::setName(string n) {
	name = n;
}
void Actor::setInitDesc(string d) {
	initDesc = d;
}

void Actor::setExamine(function<void()> f) {
	fExamine = f;
}


/* Character class */

// Constructor definition
// Initialize default functions in the constructor
Character::Character(
	string _ID, string _name, unsigned int _maxHP, vector<shared_ptr<Item>> _inventory,
	shared_ptr<Weapon> _weapon, shared_ptr<Armor> _armor) :

	Actor(_ID, _name),
	maxHP{ _maxHP },
	currentHP{ static_cast<int>(_maxHP) },
	inventory{ move(_inventory) },
	weapon{ move(_weapon) },
	armor{ move(_armor) }
{

	// Default examine function
	const function<void()> DEFAULT_EXAMINE = [this]() {

		int choice = Game::randint(1, 3);
		switch (choice) {
		case 1:
			cout << "The " << name << " is pacing around the room." << endl;
			break;
		case 2:
			cout << "The " << name << " seems to be deep in thought about something." << endl;
			break;
		case 3:
			cout << "The " << name << " appears to be waiting for someone." << endl;
			break;
		}
		};

	// Assign examine function
	fExamine = DEFAULT_EXAMINE;


	// Default talk function
	const function<void()> DEFAULT_TALK = [this]() {

		int choice = Game::randint(1, 3);
		switch (choice) {
		case 1:
			cout << "The " << name << " doesn't seem to want to talk." << endl;
			break;
		case 2:
			cout << "The " << name << " appears to be too busy to be interested in conversation." << endl;
			break;
		case 3:
			cout << "The " << name << " ignores your attempts at starting conversation." << endl;
			break;
		}
		};

	// Assign talk function
	fTalk = DEFAULT_TALK;


	// Default attack function
	const function<void(shared_ptr<Player>)> DEFAULT_ATTACK = [this](shared_ptr<Player> player) {
		;
		};

	// Assign attack function
	fAttack = DEFAULT_ATTACK;


	// Default turn function
	const function<void(shared_ptr<Player>)> DEFAULT_TURN = [this](shared_ptr<Player> player) {
		;
		};

	// Assign turn function
	fTurn = DEFAULT_TURN;

}





// Equip gear
void Character::equip(shared_ptr<Weapon> w) {
	weapon = w;
}
void Character::equip(shared_ptr<Armor> a) {
	armor = a;
}

// Execute actions
void Character::talk() {
	fTalk();
}
void Character::attack(shared_ptr<Player> player) {
	fAttack(player);
}
void Character::turn(shared_ptr<Player> player) {
	fTurn(player);
}

// Get gear
shared_ptr<Weapon> Character::getWeapon() const {
	return weapon;
}
shared_ptr<Armor> Character::getArmor() const {
	return armor;
}

// Hurt and heal the character
void Character::hurt(unsigned int amount) {
	currentHP -= amount;
}
void Character::heal(unsigned int amount) {
	currentHP += amount;
	if (currentHP > static_cast<int>(maxHP))
		currentHP = amount;
}

// Getters and setters
unsigned int Character::getMaxHP() const {
	return maxHP;
}
int Character::getCurrentHP() const {
	return currentHP;
}
vector<shared_ptr<Item>> Character::getInventory() const {
	return inventory;
}

void Character::setMaxHP(unsigned int amount) {
	maxHP = amount;
}
void Character::setCurrentHP(int amount) {
	currentHP = amount;
}
void Character::setTalk(function<void()> f) {
	fTalk = f;
}
void Character::setAttack(function<void(shared_ptr<Player>)> f) {
	fAttack = f;
}
void Character::setTurn(function<void(shared_ptr<Player>)> f) {
	fTurn = f;
}


/* Player class */

// Check if the player can level up and do so
void Player::levelUp() {
	// The amount of exp required to level up
	unsigned int expMax = 10 + (2 * level);

	if (exp >= expMax) {
		level++;
		maxHP = static_cast<unsigned int>(static_cast<float>(maxHP) * 1.2);
		exp -= expMax;
	}
}

// Increase exp
void Player::giveExp(unsigned int amount) {
	exp += amount;
}

// Add an item to the player's inventory
void Player::give(shared_ptr<Item> item) {
	inventory.push_back(item);
}

// Equip gear
void Player::equip(shared_ptr<Weapon> w) {
	weapon = w;
}
void Player::equip(shared_ptr<Armor> a) {
	armor = a;
}

// Hurt and heal the player
void Player::hurt(unsigned int amount) {
	currentHP -= amount;
}
void Player::heal(unsigned int amount) {
	currentHP += amount;
	if (currentHP > static_cast<int>(maxHP))
		currentHP = maxHP;
}

// Getters and setters
unsigned int Player::getMaxHP() const {
	return maxHP;
}
int Player::getCurrentHP() const {
	return currentHP;
}
unsigned int Player::getLevel() const {
	return level;
}
unsigned int Player::getExp() const {
	return exp;
}
vector<shared_ptr<Item>> Player::getInventory() const {
	return inventory;
}
shared_ptr<Weapon> Player::getWeapon() const {
	return weapon;
}
shared_ptr<Armor> Player::getArmor() const {
	return armor;
}

void Player::setMaxHP(unsigned int amount) {
	maxHP = amount;
}
void Player::setCurrentHP(int amount) {
	currentHP = amount;
}
void Player::setExp(unsigned int amount) {
	exp = amount;
}
