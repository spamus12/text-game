#include "game.h"
#include <memory>
#include <random>
#include "actor.h"
#include "item.h"


// Static member variable definition
vector<Game::Room> Game::rooms{};
shared_ptr<Player> Game::player{};
int Game::playerX{};
int Game::playerY{};

const Game::Room Game::DEFAULT_ROOM = Game::Room("Error", 0, -20, "You did something wrong.");


/* Room functions */

shared_ptr<Actor> Game::Room::getActor(string id) {
	for (shared_ptr<Actor>& actor : actors)
		if (actor->getID() == id)
			return actor;
	return nullptr;
}


/* World functions */

// Initialize the world
void Game::initGame(string playerName, int x, int y) {
	player = make_shared<Player>(playerName);
	playerX = x;
	playerY = y;

	rooms = {};
}

// Add a room to the world
void Game::addRoom(string id, int x, int y, string desc) {

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

// Spawn an actor in the specified room
void Game::spawnActor(string id, shared_ptr<Actor> actor) {
	if (!isRoom(id)) {
		cout << "Error: Room " << id << " does not exist." << endl;
		return;
	}
	Room* room = getRoomPtr(id);
	room->actors.push_back(actor);
}
void Game::spawnActor(int x, int y, shared_ptr<Actor> actor) {
	if (!isRoom(x, y)) {
		cout << "Error: Room (" << x << "," << y << ") does not exist." << endl;
		return;
	}
	Room* room = getRoomPtr(x, y);
	room->actors.push_back(actor);
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

// Print the specified room's description and list all actors and items
const void Game::look(string id) {
	
	Room room = *getRoomPtr(id);
	cout << "You look around the room." << endl;

	// Describe room
	cout << room.desc << endl;

	// Describe actors
	for (shared_ptr<Actor> actor : room.actors) {

		// If the actor has an initial description, then print it
		if (actor->getInitDesc() != "NOTHING")
			cout << actor->getInitDesc();

		// Otherwise, pick a random one
		else {

			// Change the starting word based on the actor's name
			bool vs = vowelStart(actor->getName());
			string defaultInitDesc[3] = {
				((vs) ? "An " : "A ") + actor->getName() + " stands idly by.",
				((vs) ? "An " : "A ") + actor->getName() + " counts some coins.",
				((vs) ? "An " : "A ") + actor->getName() + " shuffles lazily."
			};

			int choice = randint(1, 3);
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

		int choice = randint(1, 3);
		cout << initDesc[choice] << endl;
	}

}
const void Game::look(int x, int y) {
	look(getRoomPtr(x, y)->ID);
}


/* Player functions */

// Move the player to a space in the world
void Game::moveTo(int x, int y) {
	for (Room r : rooms)
		if (r.x == x && r.y == y) {

			string roomDesc = r.desc;
			playerX = x;
			playerY = y;
			cout << roomDesc << endl;

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

	if (isRoom(newX, newY))
		moveTo(newX, newY);
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
