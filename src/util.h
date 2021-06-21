#include<string>
#include<fstream>
//classic helper function
class Util{
    public:
    Util();
    // function to convert time read in seconds from the kernel into proper format
    static std::string getTime(unsigned long int seconds); 
    static std::string getProgressBar(std::string percent);
    //wrappper for creating streams 
    static std::ifstream getStream(std::string path);
};
