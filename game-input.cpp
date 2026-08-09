#include "game.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


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

        // Make it lowercase
        transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
            return tolower(c);
        });

        // Check if the word is a filler word
        if (find(begin(badWords), end(badWords), word) != end(badWords))
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
    for (int i = 1; i < words.size(); i++)
        target += words[i] + ' ';

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
void Game::handleGo(const string& noun) {
    
    // Determine the direction
    dirs['n'] = NORTH;
    dirs['s'] = SOUTH;
    dirs['e'] = EAST;
    dirs['w'] = WEST;

    // Check it and move in it
    char d = noun[0];
    if (dirs.find(d) != dirs.end())
        move(dirs[d]);
    else 
        cout << "Please input a direction." << endl;
    
}

// Takes an item from a room
void Game::handleTake(const string& noun) {
    cout << "Take function!" << endl;
}

// Examines an item or room
void Game::handleLook(const string& noun) {
    cout << "Look function!" << endl;
}
