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

    // Call the appropriate function
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
    
    // Get a pointer to the current room
    Room* room = getRoomPtr();

    // Try to find the specified item
    shared_ptr<Item> item = findItem(room, target);
    if (item == nullptr) {
        cout << "Could not find item '" << target << ".'" << endl;
        return;
    }

    // Give the item to the player
    player->give(item);

    cout << "'" << item->getName() << "' added to inventory." << endl;

    // Remove the item from the room
    removeItem(room, item->getName());

}


// Examines an item or room
void Game::handleLook(const string& target) {
    cout << "Look function!" << endl;
}

// Find an item in a room based on a specific query
shared_ptr<Item> Game::findItem(Room* room, const string& query) {

    // Split the query into mulitple words
    stringstream ss(query);
    string current;
    vector<string> words;
    while (getline(ss, current, ' '))
        words.push_back(current);

    // Go through and find one that contains the query
    for (int item = 0; item < room->items.size(); item++) {
        if (room->items[item]->getName().find(query) != string::npos) {
            return room->items[item];
        }
    }
    return nullptr;

}
