#pragma once
#include <iostream>
#include <memory>
#include <vector>
using namespace std;


// Forward definition
class Item;
class Weapon;
class Armor;
class Actor;
class Character;
class Player;


enum Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};
// A static object for game management
class Game {

private:

	// Struct to define a room
	struct Room {
		// Room's unique identifier
		string ID;

		// Position in the world
		int x, y;

		// Description of the room
		string desc;

		// A set of all actors in the room
		vector<shared_ptr<Actor>> actors;

		// A set of all items in the room
		vector<shared_ptr<Item>> items;

		// Constructor
		Room(string _ID, int _x, int _y, string _desc) :
			ID{ _ID },
			x{ _x },
			y{ _y },
			desc{ _desc },
			actors{},
			items{}
		{ }

		// Get actor by ID
		shared_ptr<Actor> getActor(string id);
		
	};

	// Store the rooms in a vector
	static vector<Room> rooms;

	// The player
	static shared_ptr<Player> player;

	// The player's position
	static int playerX, playerY;

	// Default room
	// Used if a function tries to return a reference to a nonexistant room
	static const Room DEFAULT_ROOM;

public:
	/* World functions */

	// Initialize world
	static void initGame(string playerName, int x = 0, int y = 0);

	// Add a room to the world
	static void addRoom(string id, int x, int y, string desc);

	// Check if a room exists
	static const bool isRoom(string id);
	static const bool isRoom(int x, int y);

	// Spawn an actor in the specified room
	static void spawnActor(string id, shared_ptr<Actor> actor);
	static void spawnActor(int x, int y, shared_ptr<Actor> actor);

	// Spawn an item in the specified room
	static void spawnItem(string id, shared_ptr<Item> item);
	static void spawnItem(int x, int y, shared_ptr<Item> item);

	// Print the specified room's description and list all actors and items
	static const void look(string id);
	static const void look(int x, int y);


	/* Player functions */

	// Move the player to the specified coordinates
	static void moveTo(int x, int y);

	// Move the player in the specified direction
	static void move(Direction dir);


	/* Other functions */

	// A static function for number generation
	static const int randint(int floor, int top);

	// Determines if a word starts with a vowel
	static const bool vowelStart(string s);


	/* Getter functions */

	// Get a pointer to a specific room
	// If no arguments are passed, return the current room
	static Room* getRoomPtr(string id);
	static Room* getRoomPtr(int x, int y);
	static Room* getRoomPtr();

	// Get a reference to a specific room
	// Returns a specified default room if it doesn't exist
	static const Room& getRoomRef(string id);
	static const Room& getRoomRef(int x, int y);
	static const Room& getRoomRef();

	// Get a reference to the rooms vector
	static const vector<Room>& getRoomsList();

	// Get a reference to the player
	static const shared_ptr<Player>& getPlayerRef();

	// Get the player's location
	static const int getPlayerX();
	static const int getPlayerY();

};
