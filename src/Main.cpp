#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include "Const.h"
#include "HelperFunc.h"
#include "DataControl.h"
void DisplaySys(DataInit sys, WINDOW* systemw);
void DisplayProc(DataInit procs,WINDOW* win);

int main(int argc, char *argv[] ){

if(!has_colors()){
        std::runtime_error(errors::noColors().c_str());
    }
     //Object which contains list of current processes, Container for Process Class
    DataInit proc;
    // Object which containts relevant methods and attributes regarding system details
    DataInit sys;
        initscr(); // start curses mode
    noecho(); // not printing input values
    cbreak(); // Terminating on ctrl + c
    curs_set(0);
    start_color(); // Enabling color change of text
    int yMax,xMax;
    getmaxyx(stdscr,yMax,xMax); // getting size of window measured in lines and columns(column one char length)

    WINDOW *systemw = newwin(16,xMax-1,0,0); //window 1 for displaying the system info
    WINDOW *processw = newwin(17,xMax-1,17,0); //window 2 for displaying process info

    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_YELLOW,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_BLACK);
    init_pair(4,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(5,COLOR_CYAN,COLOR_BLACK);
    while (1){
        char c = '*';
        box(systemw,int(c),int(c)); //box containing first window
        box (processw,int(c),int(c)); //box containing second window
        DisplaySys(sys,systemw);
        DisplayProc(proc,processw);
        wrefresh(systemw);
        wrefresh(processw);
        refresh();
        sleep(1);
    }
    endwin();
    return 0;
}
/*---------------------------------------Functions for terminal display ---------------------------------------*/

//function for displaying system information (top window)
void DisplaySys(DataInit sys, WINDOW* systemw){
    
    //initializing system info to the variables
    sys.setAttr();
    //temprorary string variables to contain complete string.
    std::string temp1 = ( "|| OS: " + sys.getOs() + "  ||");
    std::string temp2 = ( "Kernel version: " + sys.getKernelVer() + "  ||");
    std::string temp3 =  "CPU[%]: ";
    std::string temp4 = HelperFunc::getProgressBar(sys.getCpuPercent());
    std::string temp5 = ( "Other cores:");
    std::string temp7 = ( "Memory[%]: ");
    std::string t1 = ( "Total Processes:" + sys.getTotalProc());
    std::string t2 = ( "Running Processes:" + sys.getRunningProc());
    std::string t3 = ( "Up Time: " + HelperFunc::getTime(sys.getUpTime()));

    //converting the temp string variables to const char * using c_str() to match the arguments of print functions
     wattron(systemw,COLOR_PAIR(2));
    mvwprintw(systemw,2,20,temp1.c_str());
    mvwprintw(systemw,2,50,temp2.c_str());
     wattroff(systemw,COLOR_PAIR(2));
    mvwprintw(systemw,5,2,temp3.c_str());
    wattron(systemw,COLOR_PAIR(3));
    wprintw(systemw,temp4.c_str());
    wattroff(systemw,COLOR_PAIR(1));
    mvwprintw(systemw,6,2,temp5.c_str());
    wattron(systemw,COLOR_PAIR(1));
    std::vector<std::string> val = sys.getCoresStats();
    for (int i = 0; i < val.size(); i++) {
        std::string temp6;
        temp6 = val[i]; 
        mvwprintw(systemw,(7+i),2,temp6.c_str());
    }
    wattroff(systemw,COLOR_PAIR(3));
    mvwprintw(systemw,10,2,temp7.c_str());
    wattron(systemw,COLOR_PAIR(1));
    std::string temp8 = HelperFunc::getProgressBar(sys.getMemPercent());
    wprintw(systemw,temp8.c_str());
    wattroff(systemw,COLOR_PAIR(1));

    mvwprintw(systemw,11,2,t1.c_str());
    mvwprintw(systemw,12,2,t2.c_str());
    mvwprintw(systemw,13,2,t3.c_str());

}
//function for displaying process info (bottom window)
void DisplayProc(DataInit procs,WINDOW* win){

    procs.reloadList();
    int yMax,xMax;
    getmaxyx(stdscr,yMax,xMax);
    wattron(win,COLOR_PAIR(2));
    mvwprintw(win,1,20,"<----------------Currently Running Processes---------------->");
    wattroff(win,COLOR_PAIR(2));
    wattron(win,COLOR_PAIR(4));
    mvwprintw(win,3,2,"PID:");
    mvwprintw(win,3,9,"User:");
    mvwprintw(win,3,16,"RAM[MB]:");
    mvwprintw(win,3,30,"CMD:");
    wattroff(win, COLOR_PAIR(4));
    std::vector<std::string> pr = procs.getList();
    wattron(win,COLOR_PAIR(5));
    for(int i = 0; i < 10; i++) {
        std::string temp;
        temp = pr[i];
        mvwprintw(win,5+i,2,temp.c_str());
    }
    wattroff(win,COLOR_PAIR(5));
}
