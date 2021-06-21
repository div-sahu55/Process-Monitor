#include<iostream>
#include <string>
#include <vector>
#include "ProcParser.h"
class ProcData{
        
        std::string pids,user,cmd,mem;
public:
    ProcData(){

    }
    ProcData(std::string pid){                   //constructor for initializing values
        pids = pid;
        user = ProcParser::getProcUser(pid);
        cmd = ProcParser::getCmd(pid);
        mem = ProcParser::getVmSize(pid);
    }   
        void setPid(int pid);
        std::vector<std::string> getList();
        std::string getPid() const;
        std::string getProcess();
};
class SysInfo {

        std::vector<std::string> lastCpuStats,currentCpuStats,cores_stats;
        std::vector<std::vector<std::string>> lastCoresStats,currentCoresStats;
        std::string cpuPercent,OSName,kernelVer;
        float memPercent;
        long upTime,runningProc,totalProc;
    public:
     SysInfo() {
    // Getting initial info about system
    getOtherCores(ProcParser::getNumberOfCores());
    setLastCpuStats();
    setAttributes();
    OSName = ProcParser::getOSName();
    kernelVer = ProcParser::getSysKernelVersion();
        }
        void setAttributes();
        void setLastCpuStats();
        std::string getMemPercent() const;
        long long int getUpTime() const;
        std::string getTotalProc() const;
        std::string getRunningProc() const;
        std::string getKernelVersion() const;
        std::string getOsName() const;
        std::string getCpuPercent() const;
        void getOtherCores(int _size);
        void setCoresStats();
        std::vector<std::string> getCoresStats() const;
};
class DataInit : public ProcData, public SysInfo{
  
     std::vector<ProcData> list_;
    public:
        DataInit(){
            this->refreshList();
        }
     void refreshList();
     std::string printList();
     std::vector<std::string> getList();
};


