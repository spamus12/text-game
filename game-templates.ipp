/* Template function definitions */
#include "game.h"
#include <memory>
#include <sstream>
#include "actor.h"


// Get first instance of actor by id
template <typename T>
shared_ptr<T> Game::Room::getActor(string id) {
	for (const auto& actor : actors)
		if (actor->getID() == id) {
			auto casted = dynamic_pointer_cast<T>(actor);
			if (casted)
				return casted;
		}
	return nullptr;
}

// Spawn an actor in the specified room
template <typename T>
void Game::spawnActor(string id, shared_ptr<T> actor) {
	if (!isRoom(id)) {
		cout << "Error: Room " << id << " does not exist." << endl;
		return;
	}
	Room* room = getRoomPtr(id);
	room->actors.push_back(actor);
}
template <typename T>
void Game::spawnActor(int x, int y, shared_ptr<T> actor) {
	if (!isRoom(x, y)) {
		cout << "Error: Room (" << x << "," << y << ") does not exist." << endl;
		return;
	}
	Room* room = getRoomPtr(x, y);
	room->actors.push_back(actor);
}
template <typename T>
void Game::spawnActor(shared_ptr<T> actor) {
	spawnActor(playerX, playerY, actor);
}

// Find an actor by type
template <typename T>
shared_ptr<T> Game::findActorType(Room* room, const string& query, int its) {

	// Split the query into mulitple words
    stringstream ss(query);
    string current;
    vector<string> words;
    while (getline(ss, current, ' '))
        words.push_back(current);

    // Further specify the query if needed
    string newQuery{ "" };
    for (int i = 0; i < its; i++) {
        newQuery += words[i];
        if (i != its - 1) newQuery += ' ';
    }

    // Go through and find one that contains the query
    for (int actor = 0; actor < room->actors.size(); actor++) {

		// First, check if the actor is of the correct type
		auto casted = dynamic_pointer_cast<T>(room->actors[actor]);
		if (!casted) continue;

		// Then check its name
        string name = room->actors[actor]->getName();
        lowercase(name);        // Make the name lowercase
        if (name.find(newQuery) != string::npos) {
            return casted;
        }
    }
    
    // If at the end of the recursion, then abort
    if (its == words.size()) {
        return nullptr;
    }

    // Otherwise, recurse
    return findActorType<T>(room, query, its+1);

}
