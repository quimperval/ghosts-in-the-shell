#include "ghost.h"

Ghost::Ghost()
{
    //
}

void Ghost::setXPos(int val)
{
    xPos = val;
}

int Ghost::getXPos()
{
    return xPos;
}

void Ghost::setYPos(int val)
{
    yPos=val;
}

int Ghost::getYPos()
{
    return yPos;
}

void Ghost::setValue(char val)
{
    c = val;
}

char Ghost::getValue()
{
    return c;
}
