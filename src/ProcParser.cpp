#include <vector>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include "ProcParser.h"
#include "util.h"
#include "constants.h"
//methods defined for parsing data from various files of /proc.
std::string ProcessParser::getCmd(std::string pid){
    std::string str;
    std::ifstream stream = Util::getStream(Path::basePath() + pid + Path::cmdPath());
    std::getline(stream,str);
    return str;
}
std::vector<std::string> string_sept(std::string str){
    std::istringstream ss(str);
    std::vector<std::string> res;
    std::string del;
  for(std::string s ; ss >> s;)
                res.push_back(s);
    return res;
}
void ProcessParser::split_string(std::string const &str, const char delim, std::vector<std::string> &out){
// construct a stream from the string
    std::stringstream ss(str);
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}
std::vector<std::string> ProcessParser::getPidList(){
    DIR* dir = opendir("/proc");
    struct dirent* dir_ptr;
    std::vector<std::string> list_;
    if(!dir)
        throw std::runtime_error("Unable to open directory: /proc");
    else{
        while((dir_ptr = readdir (dir)) != NULL){
                 bool flag=true;
            //checking if this is a directory
            if(dir_ptr->d_type != DT_DIR)
            continue;
            char str = *dir_ptr->d_name;
            std::string s="";
            for(auto it = dir_ptr->d_name;it!=(dir_ptr->d_name + strlen(dir_ptr->d_name));++it){
                        char ch=*it;
                        if(!isdigit(ch)){
                        flag=false;
                        break;
                    }
                        else
                         s+=ch;
                 }
            if(!flag)continue;
            else 
            list_.push_back(s);
}
    if((closedir(dir))==-1)
            throw std::runtime_error("Unable to close directory: /proc");
        return list_;
    }
}
std::string ProcessParser::getVmSize(std::string pid){
    //declaring search attribute for file
    std::string name = "VmData",value,line;
    float result;
    //opening stream for specific file
    std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while(std::getline(stream,line)){
        //comparing line-by-line
        if(line.compare(0, name.size(),name)==0){
            std::vector<std::string> res = string_sept(line);
            return std::to_string(stof(res[1])/float(1024));  //conversion of data from kb to gb
           }
    }
    return "";
}
int ProcessParser::getNumberOfCores(){
    std::string name = "cpu cores",line;
    int result;
    std::ifstream stream = Util::getStream((Path::basePath()  + Path::cpuInfo()));
   while(std::getline(stream,line)){
        //comparing line-by-line
        if(line.compare(0, name.size(),name)==0){
            std::vector<std::string> res = string_sept(line);
            //conversion of data from kb to gb
            return (stoi(res[3]));
        }
    }
    return 0;
}
std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber){
    std::string line;
    std::string name = "cpu" + coreNumber;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statpath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
        std::vector<std::string> res = string_sept(line);
         return res;   // set of cpu data active and idle times;
        }
    }
    return (std::vector<std::string>());
}
float getSysActiveCpuTime(std::vector<std::string> values)
{
    return (stof(values[S_USER]) + stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) + stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) + stof(values[S_STEAL]) +
            stof(values[S_GUEST]) + stof(values[S_GUEST_NICE]));
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
    std::vector<std::string> res = string_sept(line);

    float stime = stof(res[14]);        
    float cutime = stof(res[15]);
    float cstime = stof(res[16]);
    float starttime = stof(res[21]);

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
    std::vector<std::string> res = string_sept(line);

    float result = stof(res[13])/sysconf(_SC_CLK_TCK);
    return std::to_string(result);
}
long int ProcessParser::getSysUpTime(){
    std::string line;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::upTimePath()));
    std::getline(stream,line);
    std::vector<std::string> res = string_sept(line);
    
    return stoi(res[0]);
}
std::string ProcessParser::getProcUser(std::string pid){
    std::string line;
    std::string name ="Uid:";
     std::string key;
    std::ifstream stream1 = Util::getStream((Path::basePath()+ pid + Path::statusPath()));
    while (std::getline(stream1,line)){
        if(line.compare(0, name.size(),name)==0){
    std::vector<std::string> res = string_sept(line);
            key=res[1];
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
std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2){ // here the values1 and 2 will be the last and current core stats 
//Because CPU stats can be calculated only if you take measures in two different time,
//this function has two parameters: two vectors of relevant values.
//formula to calculate overall activity of processor:
    float activeTime = getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1);
    float idleTime = getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return std::to_string(result);
}
float ProcessParser::getSysRamPercent(){
    std::string name1 = "MemAvailable:",name2 = "MemFree:",name3 = "Buffers:",line,value;
    int result;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::memInfoPath()));
    float total_mem = 0,buffers = 0,free_mem = 0;
    while (std::getline(stream, line)){
        if (total_mem != 0 && free_mem != 0)
            break;
        if (line.compare(0, name1.size(), name1) == 0) {
    std::vector<std::string> res = string_sept(line);
            total_mem = stof(res[1]);
        }
        if (line.compare(0, name2.size(), name2) == 0) {
    std::vector<std::string> res = string_sept(line);
            free_mem = stof(res[1]);
        }
        if (line.compare(0, name3.size(), name3) == 0) {
        std::vector<std::string> res = string_sept(line);
            buffers = stof(res[1]);
        }
    }
    //calculating usage:
    return float(100.0*(1-(free_mem/(total_mem-buffers))));
}
std::string ProcessParser::getSysKernelVersion(){
    std::string line = "";
    std::ifstream stream = Util::getStream(Path::basePath() + Path::versionPath()); // first line of /proc/version
    std::getline(stream,line);
    std::vector<std::string> res = string_sept(line);
    return res[2];
}
int ProcessParser::getTotalNumberOfProcesses(){
    std::string line,name = "processes";
    int result = 0;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statpath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
    std::vector<std::string> res = string_sept(line);
            result += stoi(res[1]);
            break;
        }
    }
    return result;
}
int ProcessParser::getNumberOfRunningProcesses(){
    std::string line,name = "procs_running";
    int result = 0;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statpath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::vector<std::string> res = string_sept(line);
            result += stoi(res[1]);
            break;
        }
    }
    return result;
}
//size_t : It is guaranteed to be big enough to contain the size of the biggest object the host system can handle
std::string ProcessParser::getOSName(){
    std::string line,name = "PRETTY_NAME=";
    std::ifstream stream = Util::getStream(("/etc/os-release")); // this file contains the os name in this format: PRETTY_NAME="Ubuntu 20.04.2 LTS"

    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
              std::size_t indx = line.find("=");indx++;
              std::string result = line.substr(indx);
              //parsing the string to get the complete os name
              result.erase(std::remove(result.begin(), result.end(), '"'), result.end()); //complexity: O(n^2)
              return result;
        }
    }
    return "";
}
