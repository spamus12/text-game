#include "game.h"
#include <memory>
#include "actor.h"
#include "item.h"


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

}