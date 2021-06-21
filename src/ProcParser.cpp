#include <vector>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include "ProcParser.h"
#include "HelperFunc.h"
#include "Const.h"
//methods defined for parsing data from various files of /proc.
std::string ProcParser::getCmd(std::string pid){
    std::string str;
    std::ifstream stream = HelperFunc::getStream(Paths::basePath() + pid + Paths::cmdPath());
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
void ProcParser::split_string(std::string const &str, const char delim, std::vector<std::string> &out){
// construct a stream from the string
    std::stringstream ss(str);
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}
std::vector<std::string> ProcParser::getPidList(){
    DIR* dir = opendir(Paths::basePath().c_str());
    struct dirent* dir_ptr;
    std::vector<std::string> list_;
    if(!dir)
        throw std::runtime_error(errors::procOpen().c_str());
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
                else {
                    s+=ch;
                   }
                }
            if(!flag){
                continue;
            }
            else{
                list_.push_back(s);
            }
        }
            closedir(dir);
        return list_;
    }
}
std::string ProcParser::getVmSize(std::string pid){
  
    std::string name = Names::Vm(),line;   //declaring search attribute for file
    std::vector<std::string> res;
    std::ifstream stream = HelperFunc::getStream((Paths::basePath() + pid + Paths::statusPath()));   //opening stream for specific file
    while(std::getline(stream,line)){
       switch(line.compare(0, name.size(),name)==0){
           case 1:  
            res = string_sept(line);
            return std::to_string(stold(res[1])/float(1024));  //conversion of data from kb to gb
           case 0: continue;
           }
    }
    return "";
}
uint8_t ProcParser::getNumberOfCores(){
    std::string name = Names::cores(),line;
    uint8_t result;
    std::vector<std::string> res;
    std::ifstream stream = HelperFunc::getStream((Paths::basePath()  + Paths::cpuInfo()));
   while(std::getline(stream,line)){
        switch(line.compare(0, name.size(),name)==0){
            case 1:
            res = string_sept(line);
            result = stoi(res[3]);
            case 0: continue;
        }
    }
    return result;
}
std::vector<std::string> ProcParser::getSysCpuPercent(std::string coreNumber){
    std::string line;
    std::vector<std::string> res;
    std::string name = Names::cpu() + coreNumber;
    std::ifstream stream = HelperFunc::getStream((Paths::basePath() + Paths::statpath()));
    while (std::getline(stream, line)) {
        switch(line.compare(0, name.size(),name) == 0) {
            case 1:
        res = string_sept(line);
         return res;   // set of cpu data active and idle times;
         case 0: continue;
        }
    }
    return (std::vector<std::string>());
}
double getSysActiveCpuTime(std::vector<std::string> values)
{
    return (stof(values[S_USER]) + stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) + stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) + stof(values[S_STEAL]) +
            stof(values[S_GUEST]) + stof(values[S_GUEST_NICE]));
}
double getSysIdleCpuTime(std::vector<std::string>values)
{
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}
long long int ProcParser::getSysUpTime(){
    std::string line;
    std::ifstream stream = HelperFunc::getStream((Paths::basePath() + Paths::upTimePath()));
    std::getline(stream,line);
    std::vector<std::string> res = string_sept(line);
    return stoi(res[0]);
}
std::string ProcParser::getProcUname(std::string pid){
    std::string line,name = Names::uid(),str;
    std::vector<std::string> res;
    std::ifstream stream1 = HelperFunc::getStream((Paths::basePath()+ pid + Paths::statusPath()));
    while (std::getline(stream1,line)){
        switch(line.compare(0, name.size(),name)==0){
            case 1:
            res = string_sept(line);
            str=res[1];
            break;
            case 0: continue;
        }
    }
   std::ifstream stream2 = HelperFunc::getStream(Paths::getPass());
   line = "";
   const char delim = ':';
   std::vector<std::string> splits;
   while(std::getline(stream2,line)){
       split_string(line,delim,splits);
   }
   std::string result;
   for(int i=0;i<splits.size();i++){
       if(str==splits[i]){
           result=splits[i-2];
           break;
       }
   }
    return result;
}
std::string ProcParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2){ // here the values1 and 2 will be the last and current core stats 
//Because CPU stats can be calculated only if you take measures in two different time,
//this function has two parameters: two vectors of relevant values.
//formula to calculate overall activity of processor:
    double activeTime = getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1);
    double idleTime = getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1);
    double totalTime = activeTime + idleTime;
    double result = 100.0*(activeTime / totalTime);
    return std::to_string(result);
}
double ProcParser::getSysRamPercent(){
    std::string name1 = Names::ram1(),name2 = Names::ram2(),name3 = Names::ram3(),line,value;
    std::ifstream stream = HelperFunc::getStream((Paths::basePath() + Paths::memInfoPath()));
    double total_mem = 0,buffers = 0,free_mem = 0;
    while (std::getline(stream, line)){
        if(total_mem != 0 && free_mem != 0)
            break;
        if(line.compare(0, name1.size(), name1) == 0) {
    std::vector<std::string> res = string_sept(line);
            total_mem = stold(res[1]);
        }
        else if(line.compare(0, name2.size(), name2) == 0) {
    std::vector<std::string> res = string_sept(line);
            free_mem = stold(res[1]);
        }
        else if(line.compare(0, name3.size(), name3) == 0) {
        std::vector<std::string> res = string_sept(line);
            buffers = stold(res[1]);
        }
    }
    //calculating usage:
    return double(100.0*(1-(free_mem/(total_mem-buffers))));
}
std::string ProcParser::getKernelVer(){
    std::string line = "";
    std::ifstream stream = HelperFunc::getStream(Paths::basePath() + Paths::versionPath()); // first line of /proc/version
    std::getline(stream,line);
    std::vector<std::string> res = string_sept(line);
    return res[2];
}
uint32_t ProcParser::getTotalNumberOfProcesses(){
    std::string line,name = Names::procs1();
    uint32_t result = 0;
    std::vector<std::string> res;
    std::ifstream stream = HelperFunc::getStream((Paths::basePath() + Paths::statpath()));
    while (std::getline(stream, line)) {
        switch(line.compare(0, name.size(), name) == 0) {
            case 1:
            res = string_sept(line);
            result += stoi(res[1]);
            break;
            case 0: continue;
        }
    }
    return result;
}
uint16_t ProcParser::getNumberOfRunningProcesses(){
    std::string line,name = Names::procs2();
    uint16_t result = 0;
    std::vector<std::string> res;
    std::ifstream stream = HelperFunc::getStream((Paths::basePath() + Paths::statpath()));
    while (std::getline(stream, line)) {
        switch (line.compare(0, name.size(), name) == 0) {
            case 1:
             res = string_sept(line);
            result += stoi(res[1]);
            break;
            case 0: continue;
        }
    }
    return result;
}
std::string ProcParser::getOS(){
    std::string line,name = Names::os(),result="";
    unsigned long long int indx=0;
    std::ifstream stream = HelperFunc::getStream(Paths::OSinfo()); // this file contains the os name in this format: PRETTY_NAME="Ubuntu 20.04.2 LTS"
    while (std::getline(stream, line)) {
        switch(line.compare(0, name.size(), name) == 0) {
              case 1:
              indx = line.find("=");
              indx++;
              result = line.substr(indx);  //parsing the string to get the complete os name
              result.erase(std::remove(result.begin(), result.end(), '"'), result.end()); //complexity: O(n^2)
              case 0: continue;
        }
    }
    return result;
}
