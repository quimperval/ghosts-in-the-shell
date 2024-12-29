#ifndef CELL_H
#define CELL_H

#include<iostream>

class Cell
{
    public:
        Cell();
	~Cell();
	char getValue();
	void setValue(char val);
    private:
	char c=' ';
};

#endif
