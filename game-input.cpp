#include "game.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "actor.h"
#include "item.h"


// Define game action function type
using CommandFunc = function<void(const string&)>;


/* Command functions */

// Process the player input and execute the command
void Game::processCommand(const string& command) {

    // An array of useless filler words
    const string badWords[8] { "the", "a", "to", "at" };

    // Split the command into a vector of lowercase words
    stringstream ss(command);
    string word;
    vector<string> words;

    while (getline(ss, word, ' ')) {

        // Check if the word is a filler word
        if (std::find(std::begin(badWords), std::end(badWords), word) != std::end(badWords))
            continue;

        // Add it to the vector of words
        words.push_back(word);
    }

    // Determine the action and target
    string action{""};
    string target{""};

    // The action should be the first word
    // If it's a synonym, then normalize it
    action = normAction(words[0]);

    // The rest of the statement should be the target
    for (int i = 1; i < words.size(); i++) {
        target += words[i];
        if (i != words.size() - 1) target += ' ';
    }

    // Check specific cases
    if (action == "look" && words[1] == "in") {
        action = "search";
    }

    // Call the appropriate function
    cout << endl;
    if (commandMap.find(action) != commandMap.end())
        commandMap[action](target);
    else
        cout << "I don't know what you mean by '" << action << ".'" << endl;;

}

// Check if the passed action is a synonym
string Game::normAction(const string& action) {

    // Search the map
    auto it = actionSyns.find(action);
    if (it != actionSyns.end()) {
        return it->second;  // Return the primary action
    }
    return action;  // Either unrecognized or already a primary action

}


// Moves the player
map<char, Direction> dirs{};
void Game::handleGo(const string& target) {
    
    // Determine the direction
    dirs['n'] = NORTH;
    dirs['s'] = SOUTH;
    dirs['e'] = EAST;
    dirs['w'] = WEST;

    // Check it and move in it
    char d = target[0];
    if (dirs.find(d) != dirs.end())
        move(dirs[d]);
    else 
        cout << "Please input a direction." << endl;
    
}


// Takes an item from a room
void Game::handleTake(const string& target) {
    bool fromRoom{ true };

    // If the target is empty, then abort
    if (target == "") {
        cout << "Take what?" << endl;
        return;
    }
    
    // Get a pointer to the current room
    Room* room = getRoomPtr();

    // Try to find the specified item in the room
    shared_ptr<Item> item = findItem(room, target);
    if (item == nullptr) {
        fromRoom = false;

        // If the item isn't in the room, then start by searching each searched box and body
        for (int a = 0; a < room->actors.size(); a++) {

            auto cActor = room->actors[a];
            auto cast1 = dynamic_pointer_cast<Body>(cActor);
            if (cast1 && cast1->searched) {
                item = findItem(cast1->getInventory(), target);
                if (item != nullptr)
                    removeItem(cast1->getInventory(), item->getName());
            }

            auto cast2 = dynamic_pointer_cast<Box>(cActor);
            if (cast2 && cast2->searched) {
                item = findItem(cast2->getInventory(), target);
                if (item != nullptr)
                    removeItem(cast2->getInventory(), item->getName());
            }

        }

        // If it still isn't found, then kill it
        if (item == nullptr) {
            cout << "Could not find item '" << target << ".'" << endl;
            return;
        }
    }

    // Give the item to the player
    player->give(item);

    cout << "The " << item->getName() << " was added to your inventory." << endl;

    // Remove the item
    if (fromRoom) {
        removeItem(room, item->getName());
    }

}


// Examines an item, actor, or room
void Game::handleLook(const string& target) {
    
    // If the target is blank or room, then just call look again
    if (target == "" || target == "room" || target == "around") {
        look();
        return;
    }

    // Get a pointer to the room
    Room* room = getRoomPtr();

    // Start by searching the actors in the room
    shared_ptr<Actor> actor = findActor(room, target);
    if (actor != nullptr) {

        // Call the actor's examine function
        actor->examine();
        return;

    }

    // End by searching the items in the room
    shared_ptr<Item> item = findItem(room, target);
    if (item != nullptr) {

        // Print the item's description
        item->printDesc();
        return;

    }

    // If it made it this far then there was nothing found
    cout << "I couldn't find a '" << target << ".'" << endl;

}


// View the inventory a body or box in the room
// Also prompt to take any items
void Game::handleSearch(const string& target) {
    bool found{ false };
    ItemSet inventory{};

    // First, search the box actors for a name that matches the target
    auto box = findActorType<Box>(getRoomPtr(), target);
    if (box != nullptr) {
        box->searched = true;
        inventory = box->getInventory();
        found = true;
        
        // Print its contents
        cout << "You open the " << box->getName() << " and look inside..." << endl;
        waitForSeconds(1.0f);
        cout << "..." << endl;
        waitForSeconds(1.0f);

        // If the box is empty
        if (inventory.size() == 0) {
            cout << "There's nothing but dust inside." << endl << endl;;
            return;
        }

        cout << "Contents of the " << box->getName() << ":" << endl;
        printInventory(inventory);
        cout << endl;

    }

    // Second, if it wasn't already found, then search the body actors
    auto body = findActorType<Body>(getRoomPtr(), target);
    if (!found && body != nullptr) {
        body->searched = true;
        inventory = body->getInventory();
        found = true;

        // Print its contents
        cout << "You rummage through the body's things..." << endl;
        waitForSeconds(1.0f);
        cout << "..." << endl;
        waitForSeconds(1.0f);

        // If the body is empty
        if (inventory.size() == 0) {
            cout << "You couldn't manage to find anything worth the effort." << endl << endl;;
            return;
        }

        cout << "Here's what you could find:" << endl;
        
        printInventory(inventory);
        cout << endl;

    }

    // If no actor was found, then return
    if (!found) {
        cout << "I don't know what you meant by '" << target << ".'" << endl;
        return;
    }

}


// Find an item in a room based on a specific query
shared_ptr<Item> Game::findItem(const ItemSet& inventory, const string& query, int its) {

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
    for (int item = 0; item < inventory.size(); item++) {
        string name = inventory[item]->getName();
        lowercase(name);        // Make the name lowercase
        if (name.find(newQuery) != string::npos) {
            return inventory[item];
        }
    }
    
    // If at the end of the recursion, then abort
    if (its == words.size()) {
        return nullptr;
    }

    // Otherwise, recurse
    return findItem(inventory, query, its+1);

}
shared_ptr<Item> Game::findItem(Room* room, const string& query) {
    return findItem(room->items, query);
}

// Find an actor in a room based on a specific query
shared_ptr<Actor> Game::findActor(Room* room, const string& query, int its) {

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
        string name = room->actors[actor]->getName();
        lowercase(name);        // Make the name lowercase
        if (name.find(newQuery) != string::npos) {
            return room->actors[actor];
        }
    }
    
    // If at the end of the recursion, then abort
    if (its == words.size()) {
        return nullptr;
    }

    // Otherwise, recurse
    return findActor(room, query, its+1);

}

// Print a passed inventory
void Game::printInventory(const ItemSet& inventory) {
    for (int item = 0; item < inventory.size(); item++) {
        cout << "> " << inventory[item]->getName() << endl;
        cout << " - " << inventory[item]->getDesc() << endl;
    }
}
