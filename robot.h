#ifndef ROBOT_H
#define ROBOT_H

#include "random.h"

#include <string>
using namespace std;

class Robot
{
private:
    string name;
    int lives = 3;
    int killToNextEvolve = 3;
    int posX = 0;
    int posY = 0;
    string statusLog = "";

public:
    // TODO: Robot constructor
    // - set name

    string getName();
    int getPositionX();
    int getPositionY();
    void updatePositionX(int newPosX);
    void updatePositionY(int newPosY);
    void evolve();
    void die();
    void addKill(int killsToAdd);
    bool isAlive();
};

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