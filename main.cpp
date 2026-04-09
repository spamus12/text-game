#include <iostream>
#include <memory>
#include <vector>
#include "actor.h"
#include "item.h"
#include "game.h"


void setup();

int main() {

	Game::initGame("Dominic");
	setup();

	Game::look("Start");
	Game::getRoomPtr()->getActor("Knight1")->examine();

	return 0;
}


void setup() {

	auto sword = make_shared<Weapon>("Sword", "A knight's blade.", 5, SLASHING);
	auto plateArmor = make_shared<Armor>("Plate Armor", "Heavy armor of thick metal plating.", 4);
	auto keepsake = make_shared<Item>("Keepsake", "An old locket from a time long past.");

	auto knight = make_shared<Character>(
		"Knight1",
		"Old Knight",
		40,
		vector<shared_ptr<Item>>{ sword, plateArmor, keepsake },
		sword,
		plateArmor
	);

	Game::addRoom("Start", 0, 0, "You are in an empty room.");
	Game::spawnActor("Start", knight);

	auto pennies = make_shared<Item>("pile of pennies", "A pile of pennies.");
	Game::spawnItem("Start", pennies);

}
