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
    {
        cout << "[ERROR] Error opening initial game configuration file" << endl;
        cout << "[EXIT] Exiting..." << endl;
        return 0;
    }

    Game game(configFile);

    while (game.isValidState()) {
        game.nextTurn();
    }

    cout << "\nGAME OVER!\n";

    return 0;
}