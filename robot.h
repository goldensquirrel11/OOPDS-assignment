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

    Robot(string name, int posX, int posY);

    // Accessors

    string getName() const;
    int getPositionX() const;
    int getPositionY() const;
    int getKillsToNextEvolve() const;
    int getLives() const;
    string getType() const;

    // Modifiers

    void setType(string type);
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

inline void Robot::setType(string type)
{
    this->type = type;
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

class TramplingRobot : public virtual Robot
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
            return;
        }
    }
}

class Cell
{
public:
    // true if the Cell is located in the board, false otherwise
    bool isValid = false;

    // points to the robot occupying the cell, nullptr if there are no robots in the cell
    Robot *occupant = nullptr;

    int relativeX = 0;
    int relativeY = 0;

    Cell(int posX, int posY)
    {
        this->relativeX = posX;
        this->relativeY = posY;
    }

    Cell(bool isValid, Robot *occupant, int relativeX, int relativeY)
    {
        this->isValid = isValid;
        this->occupant = occupant;
        this->relativeX = relativeX;
        this->relativeY = relativeY;
    }

    Cell(const Cell &rval)
    {
        isValid = rval.isValid;
        occupant = rval.occupant;
    }

    Cell(Cell &&rval)
    {
        swap(isValid, rval.isValid);
        swap(occupant, rval.occupant);

        rval.occupant = nullptr;
    }

    Cell &operator=(const Cell &rval)
    {
        if (this != &rval)
        {
            isValid = rval.isValid;
            occupant = rval.occupant;
        }

        return *this;
    }

    Cell &operator=(Cell &&rval)
    {
        if (this != &rval)
        {
            swap(isValid, rval.isValid);
            swap(occupant, rval.occupant);

            rval.occupant = nullptr;
        }

        return *this;
    }

    ~Cell()
    {
        occupant = nullptr;
        delete occupant;
        occupant = nullptr;
    }
};

class LookingRobot : public virtual Robot
{
private:
    int lookRange = 1;

public:
    Cell look(int relativeX, int relativeY);
};

/// @brief checks if there are any robots in a coordinate position
/// @param relativeX relative X position
/// @param relativeY relative Y position
/// @return Cell object
inline Cell LookingRobot::look(int relativeX, int relativeY)
{
    int positionX = this->getPositionX() + relativeX;
    int positionY = this->getPositionY() + relativeY;

    // Checking if the position that is being referenced is a position inside the game board
    if (positionX >= Board::getWidth() || positionY >= Board::getHeight() || positionX < 0 || positionY < 0)
    {
        return Cell(relativeX, relativeY);
    }

    for (int i = 0; i < robotDeque.size(); i++)
    {
        if (robotDeque[i]->getPositionX() == positionX && robotDeque[i]->getPositionY() == positionY)
        {
            // TODO: Log enemy spotted
            return Cell(true, robotDeque[i], relativeX, relativeY);
        }
    }

    return Cell(true, nullptr, relativeX, relativeY);
}

class FiringRobot : public virtual Robot
{
private:
    int fireRange = 1;

public:
    // Exception for when the robot attempts to shoot itself
    class AttemptToShootSelf
    {
    };

    virtual void fire(int relativeX, int relativeY);

    int getFireRange() const;

    void setFireRange(int fireRange);
};

inline void FiringRobot::fire(int relativeX, int relativeY)
{
    if (relativeX == 0 && relativeY == 0)
    {
        throw AttemptToShootSelf();
    }

    int positionX = this->getPositionX() + relativeX;
    int positionY = this->getPositionY() + relativeY;

    if (positionX >= Board::getWidth() || positionY >= Board::getHeight() || positionX < 0 || positionY < 0)
    {
        throw PositionOutsideOfBoard();
    }

    for (int i = 0; i < Robot::robotDeque.size(); i++)
    {
        if (Robot::robotDeque[i]->getPositionX() == positionX && Robot::robotDeque[i]->getPositionY() == positionY)
        {
            this->kill(Robot::robotDeque[i]);
            // TODO: Log fire (kill)
            return;
        }
    }

    // TODO: Log fire (no kill)
}

inline int FiringRobot::getFireRange() const
{
    return this->fireRange;
}

inline void FiringRobot::setFireRange(int fireRange)
{
    this->fireRange = fireRange;
}

class MovingRobot : public virtual Robot
{
private:
    int moveRange = 1;

    // Exception when robot moves to the same position it is currently in
    class RelativePositionIsZero
    {
    };

public:
    bool move(int relativeX, int relativeY);
};

/// @brief Moves the robot object to a specified relative position
/// @param relativeX relative X position
/// @param relativeY relative X position
/// @return true if the robot moves successfully, false if the position is blocked by another robot
/// @exception RelativePositionIsZero When moving to the same spot the robot is currently at
/// @exception PositionOutsideOfBoard When moving to a position outside of the board
inline bool MovingRobot::move(int relativeX, int relativeY)
{
    // TODO: Define move function
    if (relativeX == 0 && relativeY == 0)
    {
        throw RelativePositionIsZero();
    }

    int positionX = this->getPositionX() + relativeX;
    int positionY = this->getPositionY() + relativeY;

    if (positionX >= Board::getWidth() || positionY >= Board::getHeight() || positionX < 0 || positionY < 0)
    {
        throw PositionOutsideOfBoard();
    }

    for (int i = 0; i < Robot::robotDeque.size(); i++)
    {
        if (Robot::robotDeque[i]->getPositionX() == positionX && Robot::robotDeque[i]->getPositionY() == positionY)
        {
            return false;
        }
    }

    this->updatePositionX(positionX);
    this->updatePositionY(positionY);

    // TODO: Log move

    return true;
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