#pragma once
#include <iostream>
#include <map>
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

// Define game action function type
using CommandFunc = function<void(const string&)>;


enum Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};
enum EnemyType {
	VERMIN = 10,
	STANDARD = 20,
	BOSS = 40
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
		template <typename T>
		shared_ptr<T> getActor(string id);

		// Get the first instance of an item by name
		shared_ptr<Item> getItem(string name);
		
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

	// Determines when the game loop should stop
	static bool gameOver;

	// Determines if any setup is allowed
	// Specific functions require this variable to be true to run
	static bool setup;

	// Stores each command
	map<string, CommandFunc> commandMap;


	/* Command functions */

	// Process the player input and execute the command
	void processCommand(const string& verb, const string& noun);

	void handleGo(const string& noun);		// Moves the player
	void handleTake(const string& noun);	// Takes an item from a room
	void handleLook(const string& noun);	// Examines an item or room


public:

	// Constant dodge chance
	static const int BASE_DODGE{ 10 };


	/* World functions */

	// Initialize world
	static void initSetup();

	// Begin the game
	static void initGame();

	// Add a room to the world
	// Setup function
	static void addRoom(string id, int x, int y, string desc);
	static void addRoom(Room room);

	// Check if a room exists
	static const bool isRoom(string id);
	static const bool isRoom(int x, int y);

	// Spawn an actor in the specified room
	template <typename T>
	static void spawnActor(string id, shared_ptr<T> actor);
	template <typename T>
	static void spawnActor(int x, int y, shared_ptr<T> actor);
	template <typename T>
	static void spawnActor(shared_ptr<T> actor);

	// Remove an actor from the specified room
	// Unspecified room means the current room
	static void removeActor(string rID, string aID);
	static void removeActor(int x, int y, string aID);
	static void removeActor(string aID);

	// Remove a character from the specified room and replace it with a body
	static void kill(string rID, string aID);
	static void kill(int x, int y, string aID);
	static void kill(string aID);

	// Spawn an item in the specified room
	static void spawnItem(string id, shared_ptr<Item> item);
	static void spawnItem(int x, int y, shared_ptr<Item> item);

	// Print the specified room's description and list all actors and items
	static const void look(string id);
	static const void look(int x, int y);

	// Initialize combat with a character
	static void initCombat(shared_ptr<Character> enemy, EnemyType eType);

	// Set the state of the game to end
	static void setGameOver();


	/* Player functions */

	// Move the player to the specified coordinates
	static void moveTo(int x, int y);

	// Move the player in the specified direction
	static void move(Direction dir);

	// Describe the current room
	static void look();


	/* Other functions */

	// A static function for number generation
	static const int randint(int floor, int top);

	// Determines if a word starts with a vowel
	static const bool vowelStart(string s);

	// Wait for a specified amount of seconds
	static const void waitForSeconds(float seconds);

	// Prompt the player for input
	static const void prompt(string& s, bool ignoreCaps = true);

	// Calculate variable damage roll
	static int getRoll(unsigned int damage);


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

	// Get a pointer to the player
	static shared_ptr<Player> getPlayerPtr();

	// Get a reference to the player
	static const shared_ptr<Player>& getPlayerRef();

	// Get the player's location
	static const int getPlayerX();
	static const int getPlayerY();

};


/* Template function definitions */
#include "game-templates.ipp"
