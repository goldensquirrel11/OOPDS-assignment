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
            Log::trample(this->getName(), Robot::robotDeque[i]->getName());
            this->kill(Robot::robotDeque[i]);
            return;
        }
    }
}

inline bool TramplingRobot::canTrample() const
{
    return true;
}

/// @brief A datatype that holds information about a specified cell in the game board
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
        relativeX = rval.relativeX;
        relativeY = rval.relativeY;
    }

    Cell(Cell &&rval)
    {
        swap(isValid, rval.isValid);
        swap(occupant, rval.occupant);
        swap(relativeX, rval.relativeX);
        swap(relativeY, rval.relativeY);

        rval.occupant = nullptr;
    }

    Cell &operator=(const Cell &rval)
    {
        if (this != &rval)
        {
            isValid = rval.isValid;
            occupant = rval.occupant;
            relativeX = rval.relativeX;
            relativeY = rval.relativeY;
        }

        return *this;
    }

    Cell &operator=(Cell &&rval)
    {
        if (this != &rval)
        {
            swap(isValid, rval.isValid);
            swap(occupant, rval.occupant);
            swap(relativeX, rval.relativeX);
            swap(relativeY, rval.relativeY);

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
            Log::fireHit(this->getName(), Robot::robotDeque[i]->getName());
            this->kill(Robot::robotDeque[i]);
            return;
        }
    }
}

/// @param fireRange if the fireRange >= 0 : indicates the fire range value
/// @param fireRange if the fireRange == -1 : indicates an unlimited fire range value
inline int FiringRobot::getFireRange() const
{
    return this->fireRange;
}

/// @param fireRange if the fireRange >= 0 : indicates the fire range value
/// @param fireRange if the fireRange == -1 : indicates an unlimited fire range value
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

    if (this->canTrample() == false) {
        for (int i = 0; i < Robot::robotDeque.size(); i++)
        {
            if (Robot::robotDeque[i]->getPositionX() == positionX && Robot::robotDeque[i]->getPositionY() == positionY)
            {
                return false;
            }
        }
    }

    Log::move(this->getName(), positionX, positionY);

    this->updatePositionX(positionX);
    this->updatePositionY(positionY);

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

            scannedCells.push_back(Cell());

            scannedCells.back() = look(i, j);

            if (!scannedCells.back().isValid || scannedCells.back().occupant != nullptr)
            {
                scannedCells.pop_back();
                continue;
            }
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

    setNextTurn(getNextTurn() + 1);
}

inline void RoboCop::evolve()
{
    // TODO: RoboCop Evolve
}


class Terminator : public LookingRobot, public MovingRobot, public TramplingRobot
{
public:
    Terminator(string name, int posX, int posY) : Robot(name, posX, posY)
    {
        setType("Terminator");
    };

    void executeTurn();
    void evolve();
};

inline void Terminator::executeTurn()
{
    Deque<Cell> scannedCells;
    int enemyIndex = -1; // Index of found enemy position

    // Looking at all adjacent cells
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            scannedCells.push_back(Cell());

            scannedCells.back() = look(i, j);

            if (!scannedCells.back().isValid)
            {
                scannedCells.pop_back();
                continue;
            }

            if (scannedCells.back().occupant != nullptr) {
                enemyIndex = scannedCells.size() - 1;
            }
        }
    }

    // If an enemy is found, move to enemy position
    if (enemyIndex != -1) {
        move(scannedCells[enemyIndex].relativeX, scannedCells[enemyIndex].relativeY);
        trample();
    }
    // If no enemy is found, move to a random cell if any are valid
    else {
        if (scannedCells.size() != 0)
        {
            int cellIndex = RNG::integer(0, scannedCells.size() - 1);

            move(scannedCells[cellIndex].relativeX, scannedCells[cellIndex].relativeY);
        }
    }
    
    setNextTurn(getNextTurn() + 1);
}

inline void Terminator::evolve()
{
    // TODO: Terminator Evolve
}

class TerminatorRoboCop : public LookingRobot, public MovingRobot, public TramplingRobot, public FiringRobot
{
public:
    TerminatorRoboCop(string name, int posX, int posY) : Robot(name, posX, posY)
    {
        setType("TerminatorRoboCop");
    };

    void executeTurn();
    void evolve();
};

inline void TerminatorRoboCop::executeTurn()
{
    Deque<Cell> scannedCells;
    int enemyIndex = -1; // Index of found enemy position

    // Looking at all adjacent cells
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            scannedCells.push_back(Cell());

            scannedCells.back() = look(i, j);

            if (!scannedCells.back().isValid)
            {
                scannedCells.pop_back();
                continue;
            }

            if (scannedCells.back().occupant != nullptr)
            {
                enemyIndex = scannedCells.size() - 1;
            }
        }
    }

    // If an enemy is found, move to enemy position
    if (enemyIndex != -1)
    {
        move(scannedCells[enemyIndex].relativeX, scannedCells[enemyIndex].relativeY);
        trample();
    }
    // If no enemy is found, move to a random cell if any are valid
    else
    {
        if (scannedCells.size() != 0)
        {
            int cellIndex = RNG::integer(0, scannedCells.size() - 1);

            move(scannedCells[cellIndex].relativeX, scannedCells[cellIndex].relativeY);
        }
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

    setNextTurn(getNextTurn() + 1);
}

inline void TerminatorRoboCop::evolve()
{
    // TODO: TerminatorRoboCop Evolve
}


class UltimateRobot : public LookingRobot, public MovingRobot, public TramplingRobot, public FiringRobot
{
public:
    UltimateRobot(string name, int posX, int posY) : Robot(name, posX, posY)
    {
        setType("UltimateRobot");
        setFireRange(-1);
    };

    void executeTurn();
    void evolve();
};

inline void UltimateRobot::executeTurn()
{
    Deque<Cell> scannedCells;

    // Looking at all adjacent cells
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            scannedCells.push_back(Cell());

            scannedCells.back() = look(i, j);

            if (!scannedCells.back().isValid)
            {
                scannedCells.pop_back();
                continue;
            }
        }
    }

    // Moving to a random cell if any are valid
    if (scannedCells.size() != 0)
    {
        int cellIndex = RNG::integer(0, scannedCells.size() - 1);

        move(scannedCells[cellIndex].relativeX, scannedCells[cellIndex].relativeY);

        trample(); // tramples any robot that is occupying the same position
    }

    // Fire 3 times at random positions
    int shotsLeft = 3;
    while (shotsLeft > 0)
    {
        // TODO: Rewrite this to generate positions relative to UltimateRobot
        int relativeX = RNG::posX();
        int relativeY = RNG::posY();

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

    setNextTurn(getNextTurn() + 1);
}

/// @brief UltimateRobot does not evolve into anything so this function will not do anything
inline void UltimateRobot::evolve()
{
    return;
}


class BlueThunder : public FiringRobot
{
private:
    int firePositionX = 0;
    int firePositionY = -1;

    void setNextFirePosition();

public:
    BlueThunder(string name, int posX, int posY) : Robot(name, posX, posY)
    {
        setType("BlueThunder");
        setFireRange(1);
    };

    void executeTurn();
    void evolve();
};

inline void BlueThunder::setNextFirePosition()
{
    if (firePositionX != 1 && firePositionY == -1)
        firePositionX++;
    else if (firePositionX == 1 && firePositionY != 1)
        firePositionY++;
    else if (firePositionX != -1 && firePositionY == 1)
        firePositionX--;
    else
        firePositionY--;
}

inline void BlueThunder::executeTurn()
{
    // Fires at the next valid position in a clockwise fashion
    bool hasShot = false;
    while (!hasShot)
    {
        try
        {
            fire(firePositionX, firePositionY);
        }
        catch (Robot::PositionOutsideOfBoard)
        {
            setNextFirePosition();
            continue;
        }

        hasShot = true;
        setNextFirePosition();
    }

    setNextTurn(getNextTurn() + 1);
}

inline void BlueThunder::evolve()
{
    // TODO: BlueThunder Evolve
}


class Madbot : public FiringRobot
{
public:
    Madbot(string name, int posX, int posY) : Robot(name, posX, posY)
    {
        setType("Madbot");
        setFireRange(1);
    };

    void executeTurn();
    void evolve();
};

inline void Madbot::executeTurn()
{
    // Fire at a random valid adjacent position
    bool hasShot = false;
    while (!hasShot)
    {
        int relativeX = RNG::integer(-1, 1);
        int relativeY = RNG::integer(-1, 1);

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

        hasShot = true;
    }

    setNextTurn(getNextTurn() + 1);
}

inline void Madbot::evolve()
{
    // TODO: Madbot Evolve
}


class RoboTank : public FiringRobot
{
public:
    RoboTank(string name, int posX, int posY) : Robot(name, posX, posY)
    {
        setType("RoboTank");
        setFireRange(-1);
    };

    void executeTurn();
    void evolve();
};

inline void RoboTank::executeTurn()
{
    bool hasShot = false;
    while (!hasShot)
    {
        int relativeX = RNG::integer(-getPositionX(), Board::getWidth() - getPositionX() - 1);
        int relativeY = RNG::integer(-getPositionY(), Board::getHeight() - getPositionY() - 1);

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

        hasShot = true;
    }

    setNextTurn(getNextTurn() + 1);
}

inline void RoboTank::evolve()
{
    // TODO: BlueThunder Evolve
}

#endif