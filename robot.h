#ifndef ROBOT_H
#define ROBOT_H

#include <string>

#include "deque.h"

using namespace std;

class Robot
{
private:
    string name;
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

    // Accessors

    string getName() const;
    int getPositionX() const;
    int getPositionY() const;
    int getKillsToNextEvolve() const;

    // Modifiers

    void updatePositionX(int newPosX);
    void updatePositionY(int newPosY);
    virtual void evolve() = 0;
    void minusOneLife();
    void addKill(int killsToAdd);
};

Deque<Robot *> Robot::robotDeque;
Deque<Robot *> Robot::reviveDeque;

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

class TramplingRobot : public Robot
{
private:
    /* data */
public:
    TramplingRobot(/* args */);
    ~TramplingRobot();
};

TramplingRobot::TramplingRobot(/* args */)
{
}

TramplingRobot::~TramplingRobot()
{
}

class LookingRobot : public Robot
{
private:
    /* data */
public:
    LookingRobot(/* args */);
    ~LookingRobot();
};

LookingRobot::LookingRobot(/* args */)
{
}

LookingRobot::~LookingRobot()
{
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