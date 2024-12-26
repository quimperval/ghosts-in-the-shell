#include<ncurses.h>
#include<iostream>
#include<chrono>
#include<thread>
#include<atomic>
#include<queue>
#include<mutex>
#include<thread>
#include<condition_variable>

std::atomic<bool> running(true);
std::queue<int> eventQueue;
std::condition_variable queueCondition;
std::mutex queueMutex
;
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
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    std::thread inputThread(keyboardEventListener);
    std::thread rendererThread(renderer);

    inputThread.join();
    rendererThread.join();
    endwin();
    return 0;
}
