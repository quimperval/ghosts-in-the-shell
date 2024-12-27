#include<ncurses.h>
#include<iostream>
#include<chrono>
#include<thread>
#include<atomic>
#include<queue>
#include<mutex>
#include<thread>
#include<condition_variable>

//dependencies of this project
#include "cell.h"

std::atomic<bool> running(true);
std::queue<int> eventQueue;
std::condition_variable queueCondition;
std::mutex queueMutex;
float score=0;
int rows=-1;
int cols=-1;
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
	mvprintw(5,5,"%s", strScore.data());
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
    }
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
    std::thread inputThread(keyboardEventListener);
    std::thread rendererThread(renderer);

    inputThread.join();
    rendererThread.join();
    endwin();
    return 0;
}
