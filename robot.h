#ifndef ROBOT_H
#define ROBOT_H

#include "random.h"

#include <string>
using namespace std;


class Robot
{
private:
    int lives = 3;
    int killToNextEvolve = 3;
    int posX = 0;
    int posY = 0;
    string statusLog = "";
public:

    int getPositionX();
    int getPositionY();
    void updatePositionX(int newPosX);
    void updatePositionY(int newPosY);
    void evolve();
    void die();
    void addKill(int killsToAdd);
    bool isAlive();
};

class SteppingRobot
{
private:
    /* data */
public:
    SteppingRobot(/* args */);
    ~SteppingRobot();
};

SteppingRobot::SteppingRobot(/* args */)
{
}

SteppingRobot::~SteppingRobot()
{
}

class SeeingRobot
{
private:
    /* data */
public:
    SeeingRobot(/* args */);
    ~SeeingRobot();
};

SeeingRobot::SeeingRobot(/* args */)
{
}

SeeingRobot::~SeeingRobot()
{
}

class ShootingRobot
{
private:
    /* data */
public:
    ShootingRobot(/* args */);
    ~ShootingRobot();
};

ShootingRobot::ShootingRobot(/* args */)
{
}

ShootingRobot::~ShootingRobot()
{
}

class MovingRobot
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

#endif