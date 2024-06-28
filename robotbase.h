#ifndef ROBOTBASE_H
#define ROBOTBASE_H

#include "deque.h"

using namespace std;

class Robot
{
private:
    string name;
    string type;
    /// @brief The next turn number this robot should execute it's actions
    int nextTurn = 1;
    int lives = 3;
    int killsToNextEvolve = 3;
    int posX = 0;
    int posY = 0;

    // Exception for when lives is decremented past 0
    class NoLivesLeft
    {
    };

    // Exception for adding negative number of kills
    class AddingNegativeKills
    {
    };

public:
    // Exception when referencing a position outside of the game board
    class PositionOutsideOfBoard
    {
    };

    static Deque<Robot *> robotDeque;
    static Deque<Robot *> reviveDeque;

    Robot(string name, int posX, int posY);

    // Accessors

    string getName() const;
    int getPositionX() const;
    int getPositionY() const;
    int getKillsToNextEvolve() const;
    int getLives() const;
    string getType() const;
    int getNextTurn() const;
    virtual int getLookRange() const;
    virtual int getFireRange() const;
    virtual int getMoveRange() const;
    virtual bool canTrample() const;

    // Modifiers

    void setType(string type);
    void setNextTurn(int turn);
    void updatePositionX(int newPosX);
    void updatePositionY(int newPosY);
    void minusOneLife();
    void addKill(int killsToAdd);

    virtual void kill(Robot *robotToKill);
    virtual void executeTurn() = 0;
    virtual void evolve() = 0;
};

Deque<Robot *> Robot::robotDeque;
Deque<Robot *> Robot::reviveDeque;

inline Robot::Robot(string name, int posX, int posY)
{
    this->name = name;
    this->posX = posX;
    this->posY = posY;
}

inline string Robot::getName() const
{
    return this->name;
}

inline int Robot::getPositionX() const
{
    return this->posX;
}

inline int Robot::getPositionY() const
{
    return this->posY;
}

inline int Robot::getKillsToNextEvolve() const
{
    return this->killsToNextEvolve;
}

inline int Robot::getLives() const
{
    return this->lives;
}

inline string Robot::getType() const
{
    return this->type;
}

inline int Robot::getNextTurn() const
{
    return this->nextTurn;
}

inline int Robot::getLookRange() const
{
    return 0;
}

inline int Robot::getFireRange() const
{
    return 0;
}

inline int Robot::getMoveRange() const
{
    return 0;
}

inline bool Robot::canTrample() const
{
    return false;
}

inline void Robot::setType(string type)
{
    this->type = type;
}

inline void Robot::setNextTurn(int turn)
{
    this->nextTurn = turn;
}

inline void Robot::updatePositionX(int newPosX)
{
    this->posX = newPosX;
}

inline void Robot::updatePositionY(int newPosY)
{
    this->posY = newPosY;
}

inline void Robot::minusOneLife()
{
    if (this->lives == 0)
        throw NoLivesLeft();
    else
        this->lives--;
}

inline void Robot::addKill(int killsToAdd)
{
    if (killsToAdd < 0)
        throw AddingNegativeKills();

    killsToNextEvolve -= killsToAdd;

    if (killsToNextEvolve <= 0)
    {
        killsToNextEvolve += 3;
        evolve();
    }
}

inline void Robot::kill(Robot *robotToKill)
{
    robotToKill->minusOneLife();
    this->addKill(1);
    int IndexOfRobotToKill = 0;

    while (Robot::robotDeque[IndexOfRobotToKill] != robotToKill)
    {
        IndexOfRobotToKill++;
    }

    if (robotToKill->getLives() > 0)
    {
        Robot::reviveDeque.push_back(robotToKill);
    }

    Robot::robotDeque.erase(IndexOfRobotToKill);
}

#endif