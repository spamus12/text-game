/* Template function definitions */
#include "game.h"
#include <memory>
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
