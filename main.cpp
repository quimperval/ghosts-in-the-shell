#include<ncurses.h>
#include<iostream>
#include<chrono>
#include<thread>
#include<atomic>
#include<queue>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<vector>
#include<utility>
//dependencies of this project
#include "cell.h"
#include "ghost.h"

std::atomic<bool> running(true);
std::queue<int> eventQueue;
std::condition_variable queueCondition;
std::mutex queueMutex;
float score=0;
int rows=-1;
int cols=-1;
std::vector<std::vector<Cell>> cells;
Ghost mg; //mg for.Main Ghost
std::vector<std::vector<char>> pattern = {
        {'#', 's', 's', 's', '#', '#', '#', '#', 's', '#'},
        {'s', 's', '#', 's', 's', 's', 's', 's', 's', '#'},
        {'#', 's', '#', 's', '#', '#', '#', 's', '#', '#'},
        {'s', 's', '#', 's', '#', 's', '#', 's', '#', '#'},
        {'#', 's', 's', 's', '#', 's', 's', 's', 's', '#'},
        {'s', 's', '#', '#', '#', '#', '#', '#', 's', 's'},
        {'#', 's', 's', 's', 's', 's', '#', 's', '#', '#'},
        {'s', 's', 's', '#', '#', 's', '#', 's', 's', 's'},
        {'#', 's', '#', 's', 's', 's', 's', '#', 's', '#'},
        {'#', '#', '#', 's', '#', '#', '#', '#', 's', '#'}
    };

void keyboardEventListener(){
    while(running)
    {
        int ch = getch();//get the keyboard typed value
	if(ch!=ERR)
	{
            std::lock_guard<std::mutex> lock(queueMutex);
	    eventQueue.push(ch);
	    queueCondition.notify_one();
	    if (ch == 'q') {
                running = false; // Signal to stop the program
            }
	}
    }
}


void renderer() {
    while (running) {
	std::string strScore = "Score: ";
        strScore += std::to_string(score);
	std::size_t loc = strScore.find(".", 0);
	if(loc!= std::string::npos)
	{
         strScore=strScore.substr(0, loc);   
	}
	int xScorePos = cols-strScore.size()-1;
	mvprintw(0,xScorePos,"%s", strScore.data());
        std::unique_lock<std::mutex>lock(queueMutex);
        queueCondition.wait(lock, [] { return !eventQueue.empty() || !running; });

        while (!eventQueue.empty()) {
            int event = eventQueue.front();
            eventQueue.pop();

            lock.unlock();
            
            // Render or process the event
            clear();
            clrtoeol();
	    refresh();
	    mvprintw(0, 0, "Key pressed: %c", event);

            refresh();

            lock.lock();
        }

	    for(int i=0; i< cells.size(); i++)
	    {
		std::string line = "";
		for(Cell c : cells[i])
		{
		    char v = c.getValue();
		    line += v;
		}
		mvprintw(i+1,0, "%s", line.data());
	    }
	    //render the main ghost
	    mvprintw(mg.getYPos(), mg.getXPos(),"@");
    }
}

void initializeMainGhost()
{
    //start on the middle vector, and get the middle cell.
    bool validPosition = false;
    int middleRow = cells.size()/2;
    int middleCol = cells[middleRow].size()/2;
    int rows = cells.size();
    int cols = cells[middleRow].size();
    if(middleRow%2 == 0)
    {
        middleRow--;
    }

    if(middleCol%2 == 0)
    {
        middleCol--;
    }

    bool searchLeft = false;
    bool searchRight = false;
    bool searchBottom = false;
    bool searchTop = false;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    Cell mCell = cells[middleRow][middleCol];
    std::vector<std::pair<int, int>> directions{{0,1}, {1,0},{0,-1},{-1,0}};

    int totalElements = rows*cols;
    
    int row = middleRow;
    int col = middleCol;

    int currentDirection =0;
    int layer =0;
    	
    while(mCell.getValue()!='*' && totalElements> 0)
    {
        if(row>=0 && row < rows && col>=0 && col< cols && !visited[row][col])
	{
            visited[row][col] = true;
	    totalElements--;
	}
        int nextRow = row+directions[currentDirection].first;
	int nextCol = row+directions[currentDirection].second;

	//check if the next position is valid and whithin
	//the current spiral layer
	if(nextRow >= middleRow - layer && nextRow <= middleRow+layer && 
	    nextCol >= middleCol -layer && nextCol <= middleCol +layer && nextRow >= 0 && 
	    nextRow < rows && nextCol >=0 && nextCol < cols && 
	    !visited[nextRow][nextCol])
	{
            row= nextRow;
	    col= nextCol;
	} else
	{
            currentDirection = (currentDirection +1)% 4;
	    if(currentDirection==0)
	    {
                layer++;
	    }

	}
	
        mCell = cells[row][col];
	
	
    }
    mg.setXPos(col);
    mg.setYPos(row);
}

int main(){
    Cell c;
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    getmaxyx(stdscr,rows,cols);
    std::cout << "cols:" << cols << std::endl;
    std::cout << "rows: " << rows << std::endl;
    for(int i=0; i< pattern.size(); i++)
    {
        //create a vector
	
	std::vector<Cell>  v;

	for(char c : pattern[i])
	{
            //create a cell and put it in the vector of cells7
	    Cell cell;
	    if(c=='#')
	    {
                cell.setValue(c);
	    } 
	    else if(c=='s')
	    {
                cell.setValue('*');
	    }
	    else
	    {
	        //
	    }

	    v.push_back(cell);
	}

	//push the vector of cells to the vector of vectors
	cells.push_back(v);

    }

    initializeMainGhost();
    std::cout << "mg.x: " << mg.getXPos() << "; mg.y: " << mg.getYPos();
    std::thread inputThread(keyboardEventListener);
    std::thread rendererThread(renderer);

    inputThread.join();
    rendererThread.join();
    endwin();
    return 0;
}

