#ifndef ROBOT_H
#define ROBOT_H

#include <string>

#include "deque.h"

using namespace std;

class Robot
{
private:
    string name;
    string type;
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

    Robot(string name, string type, int posX, int posY);

    // Accessors

    string getName() const;
    int getPositionX() const;
    int getPositionY() const;
    int getKillsToNextEvolve() const;
    int getLives() const;

    // Modifiers

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

inline Robot::Robot(string name, string type, int posX, int posY)
{
    this->name = name;
    this->type = type;
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
    
    if (killsToNextEvolve <= 0) {
        killsToNextEvolve += 3;
        evolve();
    }
}

inline void Robot::kill(Robot *robotToKill)
{
    robotToKill->minusOneLife();
    this->addKill(1);
    int i = 0;

    if (robotToKill->getLives() > 0)
    {
        for (; i < Robot::robotDeque.size(); i++)
        {
            if (Robot::robotDeque[i] == robotToKill)
            {
                Robot::reviveDeque.push_back(robotToKill);
            }
        }
    }

    Robot::robotDeque.erase(i);
}

class TramplingRobot : public Robot
{
public:
    void trample();
};

inline void TramplingRobot::trample()
{
    for (int i = 0; i < Robot::robotDeque.size(); i++)
    {
        if (Robot::robotDeque[i]->getPositionX() == this->getPositionX() && Robot::robotDeque[i]->getPositionY() == this->getPositionY() && Robot::robotDeque[i] != this)
        {
            this->kill(Robot::robotDeque[i]);
        }
    }
}

class LookingRobot : public Robot
{
private:
    int lookRange = 1;

public:
    Robot* look(int relativeX, int relativeY);
};

inline Robot* LookingRobot::look(int relativeX, int relativeY)
{
    int positionX = this->getPositionX() + relativeX;
    int positionY = this->getPositionY() + relativeY;

    // Checking if the position that is being referenced is a position inside the game board
    if (positionX >= Board::getWidth() || positionY >= Board::getHeight() || positionX < 0 || positionY < 0)
    {
        throw PositionOutsideOfBoard();
    }

    for (int i = 0; i < robotDeque.size(); i++)
    {
        if (robotDeque[i]->getPositionX() == positionX && robotDeque[i]->getPositionY() == positionY) {
            return robotDeque[i];
        }
    }
}

class FiringRobot : public Robot
{
private:
    /* data */
public:
    FiringRobot(/* args */);
    ~FiringRobot();
};

FiringRobot::FiringRobot(/* args */)
{
}

FiringRobot::~FiringRobot()
{
}

class MovingRobot : public Robot
{
private:
    /* data */
public:
    MovingRobot(/* args */);
    ~MovingRobot();
};

MovingRobot::MovingRobot(/* args */)
{
}

MovingRobot::~MovingRobot()
{
}

class RoboCop : public LookingRobot, public MovingRobot, public FiringRobot
{
private:
    /* data */
public:
    RoboCop(/* args */);
    ~RoboCop();
};

RoboCop::RoboCop(/* args */)
{
}

RoboCop::~RoboCop()
{
}

class Terminator : public LookingRobot, public MovingRobot, public TramplingRobot
{
private:
    /* data */
public:
    Terminator(/* args */);
    ~Terminator();
};

Terminator::Terminator(/* args */)
{
}

Terminator::~Terminator()
{
}

class TerminatorRoboCop : public LookingRobot, public MovingRobot, public TramplingRobot, public FiringRobot
{
private:
    /* data */
public:
    TerminatorRoboCop(/* args */);
    ~TerminatorRoboCop();
};

TerminatorRoboCop::TerminatorRoboCop(/* args */)
{
}

TerminatorRoboCop::~TerminatorRoboCop()
{
}

class UltimateRobot : public LookingRobot, public MovingRobot, public TramplingRobot, public FiringRobot
{
private:
    /* data */
public:
    UltimateRobot(/* args */);
    ~UltimateRobot();
};

UltimateRobot::UltimateRobot(/* args */)
{
}

UltimateRobot::~UltimateRobot()
{
}

class BlueThunder : public FiringRobot
{
private:
    /* data */
public:
    BlueThunder(/* args */);
    ~BlueThunder();
};

BlueThunder::BlueThunder(/* args */)
{
}

BlueThunder::~BlueThunder()
{
}

class Madbot : public FiringRobot
{
private:
    /* data */
public:
    Madbot(/* args */);
    ~Madbot();
};

Madbot::Madbot(/* args */)
{
}

Madbot::~Madbot()
{
}

class RoboTank : public FiringRobot
{
private:
    /* data */
public:
    RoboTank(/* args */);
    ~RoboTank();
};

RoboTank::RoboTank(/* args */)
{
}

RoboTank::~RoboTank()
{
}

#endif