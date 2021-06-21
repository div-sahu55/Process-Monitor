#pragma once
#include<vector>
class ProcParser{
    public:
    ProcParser();
    static std::string getCmd(std::string pid);
    static std::vector<std::string> getPidList();  
    static std::string getVmSize(std::string pid);
    static std::string getCpuPercent(std::string pid);
    static long long int getSysUpTime();
    static std::string getProcUpTime(std::string pid);
    static std::string getProcUname(std::string pid);
    static std::vector<std::string> getSysCpuPercent(std::string coreNumber="");
    static uint8_t getNumberOfCores();
    static double getSysRamPercent();
    static std::string getKernelVer();
    static uint32_t getTotalNumberOfProcesses();
    static uint16_t getNumberOfRunningProcesses();
    static std::string getOS();
    static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2);
    // this method is for splitting the string into smaller parts according to the delimiter
    static void split_string(std::string const &str, const char delim, std::vector<std::string> &out);
};

