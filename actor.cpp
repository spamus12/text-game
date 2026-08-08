#include "actor.h"
#include "cmath"
#include "game.h"
#include "item.h"


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
	shared_ptr<Weapon> _weapon, shared_ptr<Armor> _armor, int _speed, unsigned int _expValue) :

	Actor(_ID, _name),
	maxHP{ _maxHP },
	currentHP{ static_cast<int>(_maxHP) },
	inventory{ move(_inventory) },
	weapon{ move(_weapon) },
	armor{ move(_armor) },
	speed{ speed },
	expValue{ _expValue }
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
	const function<void()> DEFAULT_ATTACK = [this]() {
		cout << "You initiate combat with the " << name << "." << endl;
		Game::initCombat(shared_ptr<Character>(this), STANDARD);
		};

	// Assign attack function
	fAttack = DEFAULT_ATTACK;


	// Default turn function
	const function<void(shared_ptr<Player>)> DEFAULT_TURN = [this](shared_ptr<Player> player) {

		cout << "The " << name << " hits you with its " << weapon->getName() << "." << endl;

		// Calculate damage
		int damage = weapon->getDamage();
		damage -= player->getArmor()->getDefense();
		damage = max(damage, 1);
		damage += Game::getRoll(damage);
		player->hurt(damage);

		cout << "You take " << damage << " damage." << endl;
		Game::waitForSeconds(1.0f);
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
void Character::attack() {
	fAttack();
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
int Character::getSpeed() const {
	return speed;
}
unsigned int Character::getExp() const {
	return expValue;
}

void Character::setMaxHP(unsigned int amount) {
	maxHP = amount;
}
void Character::setCurrentHP(int amount) {
	currentHP = amount;
}
void Character::setSpeed(int s) {
	speed = s;
}

void Character::setTalk(function<void()> f) {
	fTalk = f;
}
void Character::setAttack(function<void()> f) {
	fAttack = f;
}
void Character::setTurn(function<void(shared_ptr<Player>)> f) {
	fTurn = f;
}


/* Player class */

// Check if the player can level up and do so
void Player::levelUp() {
	// The amount of exp required to level up
	unsigned int expMax = static_cast<unsigned int>(round(20 * pow(1.5f, level)));

	if (exp >= expMax) {
		level++;
		cout << "You leveled up! You are now level " << level << "." << endl;
		Game::waitForSeconds(2.0f);
		maxHP = static_cast<unsigned int>(static_cast<float>(maxHP) * 1.2);
		cout << "New maximum HP: " << maxHP << "." << endl;
		Game::waitForSeconds(2.0f);
		speed++;
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

// Remove the first instance of an item from the player's inventory
void Player::removeItem(string iName) {
	vector<shared_ptr<Item>> newInventory{};
	bool found = false;
	for (shared_ptr<Item>& i : inventory)
		if (i->getName() == iName && !found) {
			found = true;
			continue;
		}
		else
			newInventory.push_back(i);
	inventory = newInventory;
	if (!found)
		cout << "Error: Item " << iName << " does not exist." << endl;
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
int Player::getSpeed() const {
	return speed;
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
void Player::setSpeed(int amount) {
	speed = amount;
}


/* Body class */
Body::Body(string _ID, string _name, vector<shared_ptr<Item>> _inventory) :
	Actor(_ID, _name),
	inventory{ _inventory }
{

	// Define default examine function
	if (inventory.size() == 0)
		const function<void()> DEFAULT_EXAMINE = [this]() {
			cout << "The " << name << " is beginning to rot." << endl;
			};
	else
		const function<void()> DEFAULT_EXAMINE = [this]() {
			cout << "The " << name << " appears to have some items left on it." << endl;
			};

	// Assign examine function
	fExamine = DEFAULT_EXAMINE;

	// Change the initial description of the body
	initDesc = (Game::vowelStart(name) ? "An " : "A ") + name + " lies motionless on the ground.";
}

// Get the first instance of an item by name and remove it
shared_ptr<Item> Body::take(string iName) {

	vector<shared_ptr<Item>> newInventory{};
	shared_ptr<Item> theItem{};
	bool found = false;
	for (auto it = inventory.begin(); it < inventory.end(); it++) {
		if ((*it)->getName() == name && !found) {
			found = true;
		}
		else
			newInventory.push_back(*it);
	}
	if (!found) cout << "Error: item " << iName << " not found." << endl;
	return theItem;
}

// Get a reference to the inventory
vector<shared_ptr<Item>>& Body::getInventory() {
	return inventory;
}


/* Box class */
Box::Box(string _ID, string _name, vector<shared_ptr<Item>> _inventory) :
	Actor(_ID, _name),
	inventory{ _inventory }
{

	// Define default examine function
	if (inventory.size() == 0)
		const function<void()> DEFAULT_EXAMINE = [this]() {
			cout << "It is an empty " << name << '.' << endl;
			};
	else
		const function<void()> DEFAULT_EXAMINE = [this]() {
			cout << "The " << name << " appears to have some items in it." << endl;
			};

	// Assign examine function
	fExamine = DEFAULT_EXAMINE;

	// Change the initial description of the box
	initDesc = "There is ";
	initDesc += (Game::vowelStart(name) ? "An " : "A ") + name + ".";
}

// Get the first instance of an item by name and remove it
shared_ptr<Item> Box::take(string iName) {

	vector<shared_ptr<Item>> newInventory{};
	shared_ptr<Item> theItem{};
	bool found = false;
	for (auto it = inventory.begin(); it < inventory.end(); it++) {
		if ((*it)->getName() == name && !found) {
			found = true;
		}
		else
			newInventory.push_back(*it);
	}
	if (!found) cout << "Error: item " << iName << " not found." << endl;
	return theItem;
}

// Get a reference to the inventory
vector<shared_ptr<Item>>& Box::getInventory() {
	return inventory;
}
