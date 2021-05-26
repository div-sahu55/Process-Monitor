#include<string>
#include<fstream>

//classic helper function
class Util{
    public:
    static std::string convertToTime(long int input_seconds); // function to convert time read in seconds from the kernel into proper format
    static std::string getProgressBar(std::string percent);
    static std::ifstream getStream(std::string path);
};
std::string Util::convertToTime(long int input_seconds){
    long minutes = input_seconds/60;
    long hours = minutes/60;
    long seconds = int(input_seconds%60); //Converts its real-number argument to an integer, with 
    minutes = int(minutes%60);            //rounding to the integer that is closer to zero.
    std::string result = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return result;
}
std::string Util::getProgressBar(std::string percent){
    std::string result = "0%";
    int _size=50;
    int boundaries = (stof(percent)/100)*_size; //Parses string interpreting its content as a floating-point number

    for(int i=0;i<_size;i++){
        if(i<=boundaries){               //total boundary size is 50 i.e a 50% would mean 25 bars and the rest would be spaces.
            result+="|";
        }
        else{
            result += " ";
        }
    }
    result += " " + percent.substr(0,5) + "/100%"; // starts at the beginning(0) of the string(result) and returns a substring of 5 length
    return result;
}
//wrappper for creating streams 
std::ifstream Util::getStream(std::string path){
    std::ifstream stream(path);                           //stream object for reading ino a file.
    if(!stream){
        throw std::runtime_error("Non - existing PID");
    }
    return stream;
}