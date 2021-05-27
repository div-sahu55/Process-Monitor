#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
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
std::string ProcessParser::getCpuPercent(std::string pid){
    std::string line;
    float utime = stof(ProcessParser::getProcUpTime(pid));
    std::ifstream stream = Util::getStream((Path::basePath()+ pid + "/" + Path::statpath()));
    std::getline(stream,line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf),end;
    std::vector<std::string> values(beg,end);

    float stime = stof(values[14]);        
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);

    float uptime = ProcessParser::getSysUpTime();
    float freq = sysconf(_SC_CLK_TCK);          //using sysconf to get clock ticks of the host machine
    
    //formula for calculating cpu percent
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    float result = 100.0*((total_time/freq)/seconds); 

    return std::to_string(result);
}
std::string ProcessParser::getProcUpTime(std::string pid){
    std::string line;
    std::ifstream stream = Util::getStream(Path::basePath() + pid + "/" + Path::statpath());
    std::getline(stream,line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf),end;
    std::vector<std::string>values(beg,end);

    float result = stof(values[13])/sysconf(_SC_CLK_TCK);
    return std::to_string(result);
}
long int ProcessParser::getSysUpTime(){
    std::string line;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::upTimePath()));
    std::getline(stream,line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf),end;
    std::vector<std::string> values(beg,end);
    
    return stoi(values[0]);
}
std::string ProcessParser::getProcUser(std::string pid){
    std::string line;
    std::string name ="Uid:";
     std::string key;
    std::ifstream stream1 = Util::getStream((Path::basePath()+ pid + Path::statusPath()));
    while (std::getline(stream1,line)){
        if(line.compare(0, name.size(),name)==0){
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf),end;
            std::vector<std::string> values(beg,end);
            key=values[1];
            break;
    }
    }
   std::ifstream stream2 = Util::getStream("/etc/passwd");
   line = "";
   const char delim = ':';
   std::vector<std::string> splits;
   while(std::getline(stream2,line)){
       split_string(line,delim,splits);
   }
   std::string result;
   for(int i=0;i<splits.size();i++){
       if(key==splits[i]){
           result=splits[i-2];
           break;
       }
   }
    return result;
}
void ProcessParser::split_string(std::string const &str, const char delim, std::vector<std::string> &out){

    // construct a stream from the string
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}


