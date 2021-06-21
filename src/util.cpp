#include <cstring>
#include <fstream>
#include "util.h"
std::string Util::getTime(unsigned long int seconds){
    unsigned long int minutes = seconds/60;
    unsigned long int hours = minutes/60;
    uint8_t secs = uint8_t(seconds%60);
    minutes = uint8_t(minutes%60);            
    return (std::to_string(hours) + " hours : " + std::to_string(minutes) + " minutes");
}
std::string Util::getProgressBar(std::string percent){
    std::string bar;
    bar.erase();
    uint8_t bounds = (stof(percent)/100)*50; //Parses string to get the percent as a float

    for(int i=0;i<50;i++){
        if(i<=bounds){               //total boundary size is 50 i.e a 50% would mean 25 bars and the rest would be spaces.
            bar = bar + "|";
        }
        else{
            bar = bar + " ";
        }
    }
    bar = bar + " " + percent.substr(0,5) + "/100%"; // starts at the beginning(0) of the string(result) and returns a substring of 5 length
    return bar;
}
std::ifstream Util::getStream(std::string path){
    std::ifstream stream(path);                           //stream object for reading into a file.
    if(!stream){
        throw std::runtime_error("Unknown PID, please try re-running the program."); // runtime error in case the path fails to open       
    }
    return stream;
}