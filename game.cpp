#include "game.h"
#include <algorithm>
#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <typeinfo>
#include "actor.h"
#include "item.h"


// Static member variable definition
vector<Game::Room> Game::rooms{};
shared_ptr<Player> Game::player{};
int Game::playerX{};
int Game::playerY{};
bool Game::gameOver{};
bool Game::setup{ false };
map<string, CommandFunc> Game::commandMap{};
map<string, string> Game:: actionSyns{};

const Game::Room Game::DEFAULT_ROOM = Game::Room("Error", 0, -20, "You did something wrong.");


/* Room functions */

// Get first instance of item by name
shared_ptr<Item> Game::Room::getItem(string name) {
	for (shared_ptr<Item>& item : items)
		if (item->getName() == name)
			return item;
	return nullptr;
}


/* World functions */

// Initialize the world
void Game::initSetup() {
	setup = true;
	rooms = {};
	addRoom(DEFAULT_ROOM);
	gameOver = false;

	// Register commands
	commandMap["go"] = [](const string& target) { Game::handleGo(target); };
	commandMap["take"] = [](const string& target) { Game::handleTake(target); };
	commandMap["look"] = [](const string& target) { Game::handleLook(target); };
	//commandMap["search"] = [](const string& target) { Game::handleSearch(target); };
	//commandMap["interact"] = [](const string& target) { Game::handleInteract(target); };
	//commandMap["fight"] = [](const string& target) { Game::handleFight(target); };

	// Register command synonyms
	actionSyns["move"] = "go";
	actionSyns["run"]  = "go";
	actionSyns["walk"] = "go";

	actionSyns["get"]  = "take";
	actionSyns["grab"] = "take";
	actionSyns["pick"] = "take";

	actionSyns["examine"] = "look";
	actionSyns["inspect"] = "look";
	actionSyns["see"]     = "look";
}

// Begin the game
void Game::initGame() {

	// Disallow setup functions
	setup = false;

	// If there is no starting room, then abort
	if (!isRoom(0, 0)) {
		cout << "Error: no room at starting position (0,0)." << endl;
		return;
	}

	// Intro sequence variables and items
	string pName{""};

	auto saber = make_shared<Weapon>(
		"Rusty saber",
		"A mildly rusted sword. It was once a shining brass color, but now that beauty has long passed.",
		5, SLASHING, 1
	);
	auto chainmail = make_shared<Armor>(
		"Old chainmail",
		"A simple, light set of chainmail.",
		3, 0
	);
	auto keepsake = make_shared<Item>(
		"Old locket",
		"A locked necklace from a life you don't remember anymore."
	);

	// Intro sequence
	cout << "Your vision is black, your head is spinning, and you have no idea where you are." << endl;
	waitForSeconds(3.0f);
	cout << "'Who am I?'" << endl;
	waitForSeconds(2.0f);
	while (pName == "" || pName == "\n")
		prompt(pName, false);
	cout << '\'' << pName << "' you say out loud." << endl;
    waitForSeconds(2.0f);
    cout << "You get to your feet." << endl;
    waitForSeconds(2.0f);
    cout << "Your journey begins." << endl;
    waitForSeconds(1.0f);
    look();

	// Initialize the player
	player = make_shared<Player>(pName);

	// Begin input loop
	string input{};
	while (input != "quit") {
		prompt(input);
		processCommand(input);
	}

}

// Add a room to the world
void Game::addRoom(string id, int x, int y, string desc) {

	// Bad use case
	if (!setup) {
		cout << "Error: the game must be set to setup mode to add a room." << endl;
		return;
	}

	// Check if the room already exists
	if (isRoom(id)) {
		cout << "Error: room identifier " << id << " already exists." << endl;
		return;
	}
	if (isRoom(x, y)) {
		cout << "Error: room already exists at point (" << x << "," << y << ")." << endl;
		return;
	}

	// If the room is completely unique, then add it
	rooms.push_back(Room(id, x, y, desc));
}
void Game::addRoom(Room room) {
	addRoom(room.ID, room.x, room.y, room.desc);
}

// Check if a room exists
const bool Game::isRoom(string id) {
	for (Room room : rooms)
		if (room.ID == id)
			return true;
	return false;
}
const bool Game::isRoom(int x, int y) {
	for (Room room : rooms)
		if (room.x == x && room.y == y)
			return true;
	return false;
}


// Remove an actor from the specified room
void Game::removeActor(string rID, string aID) {
	vector<shared_ptr<Actor>> newActors{};
	if (!isRoom(rID)) {
		cout << "Error: Room " << rID << " does not exist." << endl;
		return;
	}
	Room* currentRoom = getRoomPtr();
	vector<shared_ptr<Actor>> rActors = currentRoom->actors;
	bool found = false;
	for (auto it = rActors.begin(); it < rActors.end(); it++) {
		if ((*it)->getID() == aID && !found) {
			found = true;
		}
		else {
			newActors.push_back(*it);
		}
	}
	currentRoom->actors = newActors;
}
void Game::removeActor(int x, int y, string aID) {
	vector<shared_ptr<Actor>> newActors{};
	if (!isRoom(x, y)) {
		cout << "Error: Room at (" << x << "," << y  << ") does not exist." << endl;
		return;
	}
	Room* currentRoom = getRoomPtr();
	vector<shared_ptr<Actor>> rActors = currentRoom->actors;
	bool found = false;
	for (auto it = rActors.begin(); it < rActors.end(); it++) {
		if ((*it)->getID() == aID && !found) {
			found = true;
		}
		else {
			newActors.push_back(*it);
		}
	}
	currentRoom->actors = newActors;
}
void Game::removeActor(string aID) {
	removeActor(playerX, playerY, aID);
}

// Remove a character from the specified room and replace it with a body
void Game::kill(string rID, string aID) {
	Room* room = getRoomPtr(rID);

	// Store the character's information
	shared_ptr<Character> character = room->getActor<Character>(aID);
	string newID = aID + "body";
	string newName = character->getName() + "'s body";
	vector<shared_ptr<Item>> inventory = character->getInventory();

	// Make the body
	auto body = make_shared<Body>(newID, newName, inventory);

	// Remove the actor from the room and add the body
	removeActor(rID, aID);
	spawnActor<Body>(rID, body);
}
void Game::kill(int x, int y, string aID) {
	Room* room = getRoomPtr(x, y);
	kill(room->ID, aID);
}
void Game::kill(string aID) {
	Room* room = getRoomPtr(playerX, playerY);
	kill(room->ID, aID);
}

// Spawn an item in the specified room
void Game::spawnItem(string id, shared_ptr<Item> item) {
	if (!isRoom(id)) {
		cout << "Error: Room " << id << " does not exist." << endl;
		return;
	}
	Room* room = getRoomPtr(id);
	room->items.push_back(item);
}
void Game::spawnItem(int x, int y, shared_ptr<Item> item) {
	if (!isRoom(x, y)) {
		cout << "Error: Room (" << x << "," << y << ") does not exist." << endl;
		return;
	}
	Room* room = getRoomPtr(x, y);
	room->items.push_back(item);
}

// Remove an item from the specified room
void Game::removeItem(Room* room, string iName) {
	vector<shared_ptr<Item>> newItems{};
	bool found{ false };
	for (int i = 0; i < room->items.size(); i++) {
		if (room->items[i]->getName() == iName && !found) {
			found = true;
		}
		else {
			newItems.push_back(room->items[i]);
		}
	}
	if (!found) {
		cout << "Item '" << iName << "' not found." << endl;
		return;
	}
	room->items = newItems;
}

// Print the specified room's description and list all actors and items
const void Game::look(string id) {
	
	Room room = *getRoomPtr(id);

	// Describe room
	cout << room.desc << endl;

	// Describe actors
	for (shared_ptr<Actor> actor : room.actors) {

		// If the actor has an initial description, then print it
		if (actor->getInitDesc() != "NOTHING")
			cout << actor->getInitDesc() << endl;

		// Otherwise, pick a random one
		else {

			// Change the starting word based on the actor's name
			bool vs = vowelStart(actor->getName());
			string defaultInitDesc[3] = {
				((vs) ? "An " : "A ") + actor->getName() + " stands idly by.",
				((vs) ? "An " : "A ") + actor->getName() + " counts some coins.",
				((vs) ? "An " : "A ") + actor->getName() + " shuffles lazily."
			};

			int choice = randint(0, 2);
			cout << defaultInitDesc[choice] << endl;;
		}
	}

	// Describe items
	for (shared_ptr<Item> item : room.items) {

		// Change the starting word based on the item's name
		bool vs = vowelStart(item->getName());
		string initDesc[3] = {
			((vs) ? "An " : "A ") + item->getName() + " is laying on the ground nearby.",
			((vs) ? "An " : "A ") + item->getName() + " sits unnoticed on the floor.",
			((vs) ? "An " : "A ") + item->getName() + " is sitting not too far away from your feet."
		};

		int choice = randint(0, 2);
		cout << initDesc[choice] << endl;
	}

}
const void Game::look(int x, int y) {
	look(getRoomPtr(x, y)->ID);
}

// Combat healthbar forward declaration
string healthbar(int currentHP, unsigned int maxHP, EnemyType eType);
// Dodge calculator forward declaration
bool dodge(int pSpeed, int eSpeed);

// Initialize combat with a character
void Game::initCombat(shared_ptr<Character> enemy, EnemyType eType) {

	// First, determine who goes first
	// The one with the higher speed goes first
	// On a tie, then choose on a coinflip
	bool turn;
	if (player->getSpeed() == enemy->getSpeed())
		turn = player->getSpeed() > enemy->getSpeed();
	else
		turn = randint(0, 1) == 1;


	bool flee = false;

	// Loop through combat until a contestant reaches 0 HP or the player flees
	while (true) {

		// Get calls to data for easier reading
		const string playerName = player->getName();
		const unsigned int playerMaxHP = player->getMaxHP();
		const int playerHP = player->getCurrentHP();
		const unsigned int playerDamage = player->getWeapon()->getDamage();
		const unsigned int playerDefense = player->getArmor()->getDefense();
		const int playerSpeed = player->getSpeed()
					+ player->getArmor()->getSpeedEffect()
					+ player->getWeapon()->getSpeedEffect();

		const string enemyName = enemy->getName();
		const unsigned int enemyMaxHP = enemy->getMaxHP();
		const int enemyHP = enemy->getCurrentHP();
		const unsigned int enemyDamage = enemy->getWeapon()->getDamage();
		const unsigned int enemyDefense = enemy->getArmor()->getDefense();
		const int enemySpeed = enemy->getSpeed()
					+ enemy->getArmor()->getSpeedEffect()
					+ enemy->getWeapon()->getSpeedEffect();

		// Display information
		cout << enemyName << ':' << endl;
		cout << healthbar(enemyHP, enemyMaxHP, eType);
		cout << "\n\n\n";
		cout << playerName << ':' << endl;
		cout << healthbar(playerHP, playerMaxHP, STANDARD);
		cout << playerHP << '/' << playerMaxHP << endl;;
		cout << endl;

		// Prepare to store the player's turn
		function<void()> playerTurn;
		bool playerGuarding = false;

		waitForSeconds(1.0f);

		// Input loop
		while (true) {

			// Display choices and wait for input
			cout << "You may:\n\n" <<
			"-Attack- the enemy.\n" <<
			"-Guard- against the upcoming attack.\n" <<
			"-Use- an item in your inventory.\n" <<
			"-Run- away from the fight.\n" << endl;
			cout << endl;

			string choice;
			prompt(choice);
			
			// Handle input
			if (choice == "attack" || choice == "a") {

				// Attack
				playerTurn = [=]() {
					// Check for dodge
					if (dodge(playerSpeed, enemySpeed)) {
						cout << ((eType == BOSS) ? "" : "The ") << enemyName << " dodges your attack!" << endl;
						waitForSeconds(1.0f);
						cout << endl;
					}
					// If not, then deal damage
					else {
						unsigned int damage = max(
							static_cast<int>(playerDamage) -
							static_cast<int>(enemyDefense), 1);
						damage += getRoll(damage);
						enemy->hurt(damage);
						cout << "You hit " << ((eType == BOSS) ? "" : "the ")
						<< enemyName << " for " << damage << " damage!" << endl;
						waitForSeconds(1.0f);
						cout << endl;
					}
				};

				break;
			}
			else if (choice == "guard" || choice == "g") {
				// Guard
				playerGuarding = true;
				playerTurn = [&playerGuarding]() {
					cout << "You prepare for the next attack." << endl;
					waitForSeconds(1.0f);
				};

				break;
			}
			else if (choice == "use" || choice == "u") {
				// Use an item

				// Make a vector of all usable items in the player's inventory
				vector<shared_ptr<Item>> uItems{};
				for (shared_ptr<Item> item : player->getInventory()) 
					if (item->getItemType() == HEAL || item->getItemType() == ATTACK)
						uItems.push_back(item);
				
				// If the vector is empty, then reset
				if (uItems.size() == 0) {
					cout << "You have no usable items." << endl;
					continue;
				}

				shared_ptr<Item> chosenItem{nullptr};

				// Input loop
				while (true) {

					// Prompt the user to choose an item from the list
					cout << "Usable items:" << endl;
					for (int i = 0; i < uItems.size(); i++) 
						cout << i + 1 << ". " << uItems[i]->getName() << endl;;

					cout << "Choose an item by typing its number in the list." << endl;
					string sChoice;
					prompt(sChoice);

					// Error handling
					try {
						int iChoice = stoi(sChoice);
						if (iChoice > uItems.size() || iChoice < 1) {
							cout << "Please choose an item from the list." << endl;
							continue;
						}
						chosenItem = uItems[iChoice - 1];
						break;
					}
					catch (const invalid_argument& e) {
						cout << "Please type a number." << endl;
						continue;
					}
				}

				playerTurn = [&]() {
					auto supportItem = dynamic_pointer_cast<Support>(chosenItem);
					cout << "You use the " << supportItem->getName() << "." << endl;
					if (supportItem->getItemType() == HEAL) {
						supportItem->use(player);
						cout << "You gained " << supportItem->getEffect() << " HP." << endl;
					}
					else if (supportItem->getItemType() == ATTACK) {
						supportItem->use(enemy);
						cout << "The " << supportItem->getName() << " dealt "
						<< supportItem->getEffect() << " damage to "
						<< ((eType == BOSS) ? "" : "The ") << enemyName << "!" << endl;
					}
					player->removeItem(supportItem->getName());
					waitForSeconds(1.0f);
				};

				break;
			}
			else if (choice == "run" || choice == "r") {
				// Run from battle
				if (eType == BOSS) {
					cout << "There is nowhere to run." << endl;
					continue;
				}

				playerTurn = [&]() {
					cout << "You attempt to flee..." << endl;
					waitForSeconds(2.0f);

					// There is a base run chance of 10 in 20
					int run = 10;

					// Get the difference between the player's and enemy's speeds
					int difference = playerSpeed - enemySpeed;
					run += difference;

					flee = randint(1, 20) <= run;

					if (flee)
						cout << "Success!" << endl;
					else
						cout << "No to avail!" << endl;
				};
				
				break;
			}
			else {
				// Try again
				cout << "Your choice didn't make any sense..." << endl;
				continue;
			}
		}
		
		// Prepare to take turns
		// If the player guarded, ensure they go first
		if (playerGuarding)
			turn = true;

		// Take turns
		if (turn) {
			playerTurn();
			
			// If the player guarded
			if (playerGuarding) {
				cout << "You avoided " << ((eType == BOSS) ? "" : "the ") << enemyName << "'s attack!" << endl;
			}
			else {
				if (dodge(enemySpeed, playerSpeed)) {
					cout << ((eType == BOSS) ? "" : "The ") << enemyName << "'s attack narrowly misses!" << endl;
					waitForSeconds(1.0f);
					cout << endl;
				}
				else
					enemy->turn(player);
			}
		}
		else {
			if (dodge(enemySpeed, playerSpeed)) {
				cout << ((eType == BOSS) ? "" : "The ") << enemyName << "'s attack narrowly misses!" << endl;
				waitForSeconds(1.0f);
				cout << endl;
			}
			else
				enemy->turn(player);
			playerTurn();
		}

		// Check the player's HP
		if (player->getCurrentHP() <= 0) {
			cout << "You are dead." << endl;
			break;
		}

		// Check the enemy HP
		if (enemy->getCurrentHP() <= 0) {
			kill(enemy->getID());
			cout << "You win!" << endl;
			waitForSeconds(1.0f);
			player->giveExp(enemy->getExp());
			cout << "You got " << enemy->getExp() << " experience points." << endl;
			waitForSeconds(1.0f);
			player->levelUp();
			break;
		}

	}

}

// Healthbar definition
string healthbar(int currentHP, unsigned int maxHP, EnemyType eType) {
	string bar = "[";
	float chunk = maxHP / eType;
	for (int i = 0; i < eType; i++) 
		if ((chunk * i) <= currentHP)
			bar += '=';
		else
			bar += ' ';
	bar += "]\n";
	return bar;
}

// Dodge definition
bool dodge(int attackerSpeed, int defenderSpeed) {
	int difference = defenderSpeed - attackerSpeed;
	return Game::randint(1, 100) <= Game::BASE_DODGE + difference;
}

// Set the state of the game to end
void Game::setGameOver() {
	gameOver = true;
}


/* Player functions */

// Move the player to a space in the world
void Game::moveTo(int x, int y) {
	for (Room r : rooms)
		if (r.x == x && r.y == y) {

			playerX = x;
			playerY = y;
			look();

		}
	
	cout << "You can't go that way." << endl;
}

// Move the player in a specified direction
void Game::move(Direction dir) {
	int newX = playerX, newY = playerY;

	switch (dir) {
	case NORTH:
		newY++;
		break;
	case SOUTH:
		newY--;
		break;
	case EAST:
		newX++;
		break;
	case WEST:
		newX--;
		break;
	}

	moveTo(newX, newY);
}

// Describe the current room
void Game::look() {
	const Room& room = getRoomRef();
	cout << room.desc << endl;
}


/* Other functions */

// RNG implementation
random_device rd;
mt19937 gen(rd());
const int Game::randint(int floor, int top) {
	uniform_int_distribution<> distr(floor, top);
	return distr(gen);
}

// Determines if a word starts with a vowel
const bool Game::vowelStart(string s) {
	string vowels = "aeiouAEIOU";
	for (char c : vowels)
		if (s[0] == c)
			return true;
	return false;
}

// Wait for a specified amount of seconds
const void Game::waitForSeconds(float seconds) {
	this_thread::sleep_for(chrono::milliseconds(static_cast<unsigned int>(1000 * seconds)));
}

// Propmpt the user for input and set it to lowercase
const void Game::prompt(string& s, bool ignoreCaps) {
	cout << "> ";
	getline(cin, s);
	if (ignoreCaps)
		std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c){ return std::tolower(c); });
}

// Calculate variable damage roll
int Game::getRoll(unsigned int damage) {
	return Game::randint(1, max(static_cast<int>(damage % 5), 1))
	* (Game::randint(0, 1) == 1) ? -1 : 1;
}

// Return a lowercase version of the passed string
void Game::lowercase(string& s) {
	transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return tolower(c);
        });
}


/* Getter functions */

// Return a pointer to a specific room
Game::Room* Game::getRoomPtr(string id) {
	for (Room& room : rooms)
		if (room.ID == id)
			return &room;
	return nullptr;
}
Game::Room* Game::getRoomPtr(int x, int y) {
	for (Room& room : rooms)
		if (room.x == x && room.y == y)
			return &room;
	return nullptr;
}
Game::Room* Game::getRoomPtr() {
	return getRoomPtr(playerX, playerY);
}

// Return a read-only reference to a specific room
const Game::Room& Game::getRoomRef(string id) {
	for (Room& room : rooms)
		if (room.ID == id)
			return room;
	return DEFAULT_ROOM;
}
const Game::Room& Game::getRoomRef(int x, int y) {
	for (Room& room : rooms)
		if (room.x == x && room.y == y)
			return room;
	return DEFAULT_ROOM;
}
const Game::Room& Game::getRoomRef() {
	return getRoomRef(playerX, playerY);
}

// Return a reference to the rooms vector
const vector<Game::Room>& Game::getRoomsList() {
	return rooms;
}

// Return a pointer to the player
shared_ptr<Player> Game::getPlayerPtr() {
	return player;
}

// Return a reference to the player
const shared_ptr<Player>& Game::getPlayerRef() {
	return player;
}

// Return the player's position
const int Game::getPlayerX() {
	return playerX;
}
const int Game::getPlayerY() {
	return playerY;
}
