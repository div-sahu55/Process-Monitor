#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include "ProcessParser.h"
#include "util.h"
#include "constants.h"
//methods defined for parsing data from various files of /proc.
std::string ProcessParser::getVmSize(std::string pid){
    std::string line;
    //declaring search attribute for file
    std::string name = "VmData";
    std::string value;
    float result;
    //opening stream for specific file
    std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while(std::getline(stream,line)){
        //comparing line-by-line
        if(line.compare(0, name.size(),name)==0){
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf),end;
            std::vector<std::string> values(beg,end);
            //conversion of data from kb to gb
            result = (stof(values[1])/float(1024));
            break;
        }
    }
    return std::to_string(result);
}