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
    string action;
    string target;

}

// Moves the player
void Game::handleGo(const string& noun) {
    ;
}

// Takes an item from a room
void Game::handleTake(const string& noun) {
    ;
}

// Examines an item or room
void Game::handleLook(const string& noun) {
    ;
}
