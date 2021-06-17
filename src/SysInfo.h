#include "ProcParser.h"
class SysInfo {
    private:
        std::vector<std::string> lastCpuStats;
        std::vector<std::string> currentCpuStats;
        std::vector<std::string> cores_stats;
        std::vector<std::vector<std::string>> lastCoresStats;
        std::vector<std::vector<std::string>> currentCoresStats;
        std::string cpuPercent;
        float memPercent;
        std::string OSName;
        std::string kernelVer;
        long upTime;
        int totalProc;
        int runningProc;
        int threads;

    public:
     SysInfo() {
    // Getting initial info about system
    // Initial data for individual cores is set
    // System data is set
    getOtherCores(ProcessParser::getNumberOfCores());
    setLastCpuStats();
    setAttributes();
    OSName = ProcessParser::getOSName();
    kernelVer = ProcessParser::getSysKernelVersion();
        }
        void setAttributes();
        void setLastCpuStats();
        std::string getMemPercent() const;
        long long getUpTime() const;
        std::string getThreads() const;
        std::string getTotalProc() const;
        std::string getRunningProc() const;
        std::string getKernelVersion() const;
        std::string getOsName() const;
        std::string getCpuPercent() const;
        void getOtherCores(int _size);
        void setCoresStats();
        std::vector<std::string> getCoresStats() const;
};