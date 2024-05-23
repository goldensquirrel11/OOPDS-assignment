#ifndef INTERFACE_H
#define INTERFACE_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/// @brief
/// @param character
/// @param numberOfTimes
/// @return a string containing of repeated character the indicated number of times
string repeatChar(const char character, const int numberOfTimes)
{
    string temp = "";
    for (int i = numberOfTimes; i > 0; i--)
    {
        temp += character;
    }

    return temp;
}

#endif