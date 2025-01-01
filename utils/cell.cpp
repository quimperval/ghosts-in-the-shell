#include "cell.h"
#include <iostream>

Cell::Cell(){
    //std::cout<< "cell constructor\n";
}

Cell::~Cell(){
    //std::cout << "Cell destructor\n";
}

char Cell::getValue()
{
    return c;
}

void Cell::setValue(char val)
{
    c = val;
}

