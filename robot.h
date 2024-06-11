#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <cmath>

#include "deque.h"
#include "log.h"
#include "robotbase.h"

using namespace std;

class TramplingRobot : public virtual Robot
{
public:
    void trample();

    bool canTrample() const;
};

inline void TramplingRobot::trample()
{
    for (int i = 0; i < Robot::robotDeque.size(); i++)
    {
        if (Robot::robotDeque[i]->getPositionX() == this->getPositionX() && Robot::robotDeque[i]->getPositionY() == this->getPositionY() && Robot::robotDeque[i] != this)
        {
            this->kill(Robot::robotDeque[i]);
            Log::trample(this->getName(), Robot::robotDeque[i]->getName());
            return;
        }
    }
}

inline bool TramplingRobot::canTrample() const
{
    return true;
}

class Cell
{
public:
    /// @brief true if the Cell is located in the board, false otherwise
    bool isValid = false;

    /// @brief points to the robot occupying the cell, nullptr if there are no robots in the cell
    Robot *occupant = nullptr;

    int relativeX = 0;
    int relativeY = 0;

    Cell()
    {
    }

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

    int getLookRange() const;
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
            Log::enemySpotted(this->getName(), robotDeque[i]->getName(), positionX, positionY);
            return Cell(true, robotDeque[i], relativeX, relativeY);
        }
    }

    return Cell(true, nullptr, relativeX, relativeY);
}

inline int LookingRobot::getLookRange() const
{
    return this->lookRange;
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

    Log::fire(this->getName(), positionX, positionY);

    for (int i = 0; i < Robot::robotDeque.size(); i++)
    {
        if (Robot::robotDeque[i]->getPositionX() == positionX && Robot::robotDeque[i]->getPositionY() == positionY)
        {
            this->kill(Robot::robotDeque[i]);
            Log::fireHit(this->getName(), Robot::robotDeque[i]->getName());
            return;
        }
    }
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

    int getMoveRange() const;
};

/// @brief Moves the robot object to a specified relative position
/// @param relativeX relative X position
/// @param relativeY relative X position
/// @return true if the robot moves successfully, false if the position is blocked by another robot
/// @exception RelativePositionIsZero When moving to the same spot the robot is currently at
/// @exception PositionOutsideOfBoard When moving to a position outside of the board
inline bool MovingRobot::move(int relativeX, int relativeY)
{
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

    Log::move(this->getName(), positionX, positionY);

    return true;
}

inline int MovingRobot::getMoveRange() const
{
    return this->moveRange;
}

class RoboCop : public LookingRobot, public MovingRobot, public FiringRobot
{
public:
    RoboCop(string name, int posX, int posY) : Robot(name, posX, posY)
    {
        setType("RoboCop");
        setFireRange(10);
    };

    void executeTurn();
    void evolve();
};

inline void RoboCop::executeTurn()
{
    Deque<Cell> scannedCells;

    // Looking at all adjacent cells
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            Cell thisCell = look(i, j);

            if (!thisCell.isValid || thisCell.occupant != nullptr)
                continue;

            scannedCells.push_back(thisCell);
        }
    }

    // Moving to a random cell if any are valid
    if (scannedCells.size() != 0)
    {
        int cellIndex = RNG::integer(0, scannedCells.size() - 1);

        move(scannedCells[cellIndex].relativeX, scannedCells[cellIndex].relativeY);
    }

    // Fire 3 times at random positions
    int shotsLeft = 3;
    while (shotsLeft > 0)
    {
        int offset = getFireRange();

        int relativeX = RNG::integer(-offset, offset);

        offset = offset - abs(relativeX);

        int relativeY = RNG::integer(-offset, offset);

        try
        {
            fire(relativeX, relativeY);
        }
        catch (FiringRobot::AttemptToShootSelf)
        {
            continue;
        }
        catch (Robot::PositionOutsideOfBoard)
        {
            continue;
        }

        shotsLeft--;
    }
}

inline void RoboCop::evolve()
{
    // TODO: RoboCop Evolve
}

// class Terminator : public LookingRobot, public MovingRobot, public TramplingRobot
// {
// private:
//     /* data */
// public:
//     Terminator(/* args */);
//     ~Terminator();
// };

// Terminator::Terminator(/* args */)
// {
// }

// Terminator::~Terminator()
// {
// }

// class TerminatorRoboCop : public LookingRobot, public MovingRobot, public TramplingRobot, public FiringRobot
// {
// private:
//     /* data */
// public:
//     TerminatorRoboCop(/* args */);
//     ~TerminatorRoboCop();
// };

// TerminatorRoboCop::TerminatorRoboCop(/* args */)
// {
// }

// TerminatorRoboCop::~TerminatorRoboCop()
// {
// }

// class UltimateRobot : public LookingRobot, public MovingRobot, public TramplingRobot, public FiringRobot
// {
// private:
//     /* data */
// public:
//     UltimateRobot(/* args */);
//     ~UltimateRobot();
// };

// UltimateRobot::UltimateRobot(/* args */)
// {
// }

// UltimateRobot::~UltimateRobot()
// {
// }

// class BlueThunder : public FiringRobot
// {
// private:
//     /* data */
// public:
//     BlueThunder(/* args */);
//     ~BlueThunder();
// };

// BlueThunder::BlueThunder(/* args */)
// {
// }

// BlueThunder::~BlueThunder()
// {
// }

// class Madbot : public FiringRobot
// {
// private:
//     /* data */
// public:
//     Madbot(/* args */);
//     ~Madbot();
// };

// Madbot::Madbot(/* args */)
// {
// }

// Madbot::~Madbot()
// {
// }

// class RoboTank : public FiringRobot
// {
// private:
//     /* data */
// public:
//     RoboTank(/* args */);
//     ~RoboTank();
// };

// RoboTank::RoboTank(/* args */)
// {
// }

// RoboTank::~RoboTank()
// {
// }

#endif