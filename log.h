#ifndef LOG_H
#define LOG_H

#include <string>
#include <sstream>
using namespace std;

class Log
{
private:
    static string logBuffer;

    static const string ENEMY_SPOTTED;
    static const string MOVE;
    static const string TRAMPLE;
    static const string FIRE;
    static const string FIRE_HIT;
    static const string EVOLVE;

    static void inputAtNextPosition(istringstream &inputStream, string input);
    static void inputAtNextPosition(istringstream &inputStream, int input);

public:
    string getLog() const;

    void resetLog();

    static void enemySpotted(string robotName, string enemyRobot, int posX, int posY);
    static void move(string robotName, int posX, int posY);
    static void trample(string robotName, string enemyRobot);
    static void fire(string robotName, int posX, int posY);
    static void fireHit(string robotName, string enemyRobot);
    static void evolve(string robotName, string robotType);
};

string Log::logBuffer = "";
const string Log::ENEMY_SPOTTED = "${robotName} spotted ${enemyRobot} at ${posX}, ${posY}";
const string Log::MOVE = "${robotName} moved to ${posX}, ${posY}";
const string Log::TRAMPLE = "${robotName} trampled ${trampledRobot}";
const string Log::FIRE = "${robotName} fires at ${posX}, ${posY}";
const string Log::FIRE_HIT = "${robotName} shot ${shotRobot}";
const string Log::EVOLVE = "${robotName} evolved into a ${robotType}";

/// @brief reads input from istringstream object until the next input position and appends to the log buffer, then appends the input string to the log buffer
/// @param inputStream istringstream object of the log template string
/// @param input input string
inline void Log::inputAtNextPosition(istringstream &inputStream, string input)
{
    string temp;

    getline(inputStream, temp, '$');
    logBuffer += temp;
    logBuffer += input;

    // ignore characters until a '}' is found
    getline(inputStream, temp, '}');
}

/// @brief reads input from istringstream object until the next input position and appends to the log buffer, then appends the input int to the log buffer
/// @param inputStream istringstream object of the log template string
/// @param input input int
inline void Log::inputAtNextPosition(istringstream &inputStream, int input)
{
    string temp;

    getline(inputStream, temp, '$');
    logBuffer += temp;
    logBuffer += to_string(input);

    // ignore characters until a '}' is found
    getline(inputStream, temp, '}');
}

inline string Log::getLog() const
{
    return logBuffer;
}

inline void Log::resetLog()
{
    logBuffer = "";
}

inline void Log::enemySpotted(string robotName, string enemyRobot, int posX, int posY)
{
    istringstream logTemplate(ENEMY_SPOTTED);
    string temp;

    inputAtNextPosition(logTemplate, robotName);
    inputAtNextPosition(logTemplate, enemyRobot);
    inputAtNextPosition(logTemplate, posX);
    inputAtNextPosition(logTemplate, posY);
}

inline void Log::move(string robotName, int posX, int posY)
{
    istringstream logTemplate(ENEMY_SPOTTED);
    string temp;

    inputAtNextPosition(logTemplate, robotName);
    inputAtNextPosition(logTemplate, posX);
    inputAtNextPosition(logTemplate, posY);
}

inline void Log::trample(string robotName, string enemyRobot)
{
    istringstream logTemplate(ENEMY_SPOTTED);
    string temp;

    inputAtNextPosition(logTemplate, robotName);
    inputAtNextPosition(logTemplate, enemyRobot);
}

inline void Log::fire(string robotName, int posX, int posY)
{
    istringstream logTemplate(ENEMY_SPOTTED);
    string temp;

    inputAtNextPosition(logTemplate, robotName);
    inputAtNextPosition(logTemplate, posX);
    inputAtNextPosition(logTemplate, posY);
}

inline void Log::fireHit(string robotName, string enemyRobot)
{
    istringstream logTemplate(ENEMY_SPOTTED);
    string temp;

    inputAtNextPosition(logTemplate, robotName);
    inputAtNextPosition(logTemplate, enemyRobot);
}

inline void Log::evolve(string robotName, string robotType)
{
    istringstream logTemplate(ENEMY_SPOTTED);
    string temp;

    inputAtNextPosition(logTemplate, robotName);
}

#endif