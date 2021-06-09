#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include "ProcessParser.h"
#include "util.h"
#include "constants.h"
//methods defined for parsing data from various files of /proc.
std::string ProcessParser::getCmd(std::string pid){
    std::string line;
    std::ifstream stream = Util::getStream(Path::basePath() + pid + Path::cmdPath());
    std::getline(stream,line);
    return line;
}
std::vector<std::string> ProcessParser::getPidList(){
    DIR* dir;
    std::vector<std::string> container;
    if(!(dir=opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));

        while(dirent* dirp = readdir(dir)){
            //checking if is this a directory
            if(dirp->d_type != DT_DIR)
            continue;
            //is every character of the name a digit?
            if(std::all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c){ return std::isdigit(c); })){
            //above expression, basically iterates over the name of each and every file/folder in the directory
            //and points to the beginnning and the end of the file name and then checks the lambda expression
            // to see whether each character of the name is a digit or not.
                container.push_back(dirp->d_name);
            }
        }
        if((closedir(dir))==-1)
            throw std::runtime_error(std::strerror(errno));
        return container;
}
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
int ProcessParser::getNumberOfCores(){
    std::string line;
    std::string name = "cpu cores";
    int result;
    std::ifstream stream = Util::getStream((Path::basePath()  + Path::cpuInfo()));
   while(std::getline(stream,line)){
        //comparing line-by-line
        if(line.compare(0, name.size(),name)==0){
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf),end;
            std::vector<std::string> values(beg,end);
            //conversion of data from kb to gb
            result = (stoi(values[3]));
            break;
        }
    }
return result;
}
std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber){
    std::string line;
    std::string name = "cpu" + coreNumber;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statpath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            // set of cpu data active and idle times;
            return values;
        }
    }
    return (std::vector<std::string>());
}
float getSysActiveCpuTime(std::vector<std::string> values)
{
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}

float getSysIdleCpuTime(std::vector<std::string>values)
{
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
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
std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2){
//Because CPU stats can be calculated only if you take measures in two different time,
//this function has two parameters: two vectors of relevant values.
//We use a formula to calculate overall activity of processor.
    float activeTime = getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1);
    float idleTime = getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return std::to_string(result);
}
void ProcessParser::split_string(std::string const &str, const char delim, std::vector<std::string> &out){

    // construct a stream from the string
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}
float ProcessParser::getSysRamPercent(){
    std::string line;
    std::string name1 = "MemAvailable:";
    std::string name2 = "MemFree:";
    std::string name3 = "Buffers:";

    std::string value;
    int result;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::memInfoPath()));
    float total_mem = 0;
    float free_mem = 0;
    float buffers = 0;
    while (std::getline(stream, line)) {
        if (total_mem != 0 && free_mem != 0)
            break;
        if (line.compare(0, name1.size(), name1) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            total_mem = stof(values[1]);
        }
        if (line.compare(0, name2.size(), name2) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
           std::vector<std::string> values(beg, end);
            free_mem = stof(values[1]);
        }
        if (line.compare(0, name3.size(), name3) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            buffers = stof(values[1]);
        }
    }
    //calculating usage:
    return float(100.0*(1-(free_mem/(total_mem-buffers))));
}
std::string ProcessParser::getSysKernelVersion(){
    std::string line = "Linux version ";
    std::ifstream stream = Util::getStream(Path::basePath() + Path::versionPath());
    std::getline(stream,line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf),end;
    std::vector<std::string> values(beg,end);
    return values[2];
}
int ProcessParser::getTotalThreads(){
    std::string line;
    int result = 0;
    std::string name = "Threads:";
    std::vector<std::string>_list = ProcessParser::getPidList();
    for (int i=0 ; i<_list.size();i++) {
    std::string pid = _list[i];
    //getting every process and reading their number of their threads
    std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    }
    return result;
}
int ProcessParser::getTotalNumberOfProcesses(){
    std::string line;
    int result = 0;
    std::string name = "processes";
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statpath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}
int ProcessParser::getNumberOfRunningProcesses(){
    std::string line;
    int result = 0;
    std::string name = "procs_running";
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statpath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}
std::string ProcessParser::getOSName(){
    std::string line;
    std::string name = "PRETTY_NAME=";

    std::ifstream stream = Util::getStream(("/etc/os-release"));

    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
              std::size_t found = line.find("=");
              found++;
              std::string result = line.substr(found);
              result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
              return result;
        }
    }
    return "";

}

