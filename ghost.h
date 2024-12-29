#ifndef GHOST_H
#define GHOST_H

class Ghost
{
    public:
	Ghost();
	int getXPos();
	int getYPos();
	void setXPos(int val);
        void setYPos(int val);
	char getValue();
	void setValue(char val);
    private:
	int xPos=-1;
	int yPos=-1;
	char c;
};
#endif
