#pragma once
#include<vector>
class ProcessParser{
    public:
    ProcessParser();
    static std::string getCmd(std::string pid);
    static std::vector<std::string> getPidList();  
    static std::string getVmSize(std::string pid);
    static std::string getCpuPercent(std::string pid);
    static long int getSysUpTime();
    static std::string getProcUpTime(std::string pid);
    static std::string getProcUser(std::string pid);
    static std::vector<std::string> getSysCpuPercent(std::string coreNumber="");
    static int getNumberOfCores();
    static float getSysRamPercent();
    static std::string getSysKernelVersion();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static std::string getOSName();
    static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2);
    // this method is for splitting the string into smaller parts according to the delimiter
    static void split_string(std::string const &str, const char delim, std::vector<std::string> &out);
};

