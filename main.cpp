/*
 *
 * This file contains the High-level program flow in main()
 *
 * --- Summary of code ---
 * The config file is read from user input and the config file data is extracted from it.
 * After that, the main game loop starts and only ends when the game has reached an invalid
 * state (either there is only one robot left OR the game has reached the turn counter limit)
 *
 */

#include <iostream>
#include <fstream>

#include "interface.h"

using namespace std;

int main()   
{
    ifstream configFile;
    string filename;

    cout << "Please enter the name of the initial game configuration file:" << endl;
    cin >> filename;

    configFile.open(filename);
    if (!configFile.is_open())
    { // Error output when the config file could not be opened
        cout << "[ERROR] Error opening initial game configuration file" << endl;
        cout << "[EXIT] Exiting..." << endl;
        return 0;
    }

    // Read in data from config file
    Game game(configFile);

    // main game loop
    while (game.isValidState()) {
        game.nextTurn();
    }

    cout << "\nGAME OVER!\n";

    return 0;
}