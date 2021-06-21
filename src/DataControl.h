#include<iostream>
#include <string>
#include <vector>
#include "ProcParser.h"
class ProcData{
        
        std::string pID,uname,cmd,ram;
public:
    ProcData(){
    };
    ProcData(std::string pid){                   //constructor for initializing values
        pID = pid;
        uname = ProcParser::getProcUname(pid);
        cmd = ProcParser::getCmd(pid);
        ram = ProcParser::getVmSize(pid);
    }   
        void setPid(int pid);
        std::vector<std::string> getList();
        std::string getPid() const;
        std::string getProcess();
};
class SysInfo {

        std::vector<std::string> lastCpuStats,currentCpuStats,cores_stats;
        std::vector<std::vector<std::string>> lastCoresStats,currentCoresStats;
        std::string cpuPercent;
        double memPercent;
        unsigned long long int upTime;
        uint16_t runningProcs;
        uint32_t totalProcs;
    public:
     SysInfo() {
    // Getting initial info about system
    getOtherCores(ProcParser::getNumberOfCores());
    setLastCpuStats();
        }
        void setAttr();
        void setLastCpuStats();
        std::string getMemPercent() const;
        unsigned long long int getUpTime() const;
        std::string getTotalProc() const;
        std::string getRunningProc() const;
        
        std::string getCpuPercent() const;
        void getOtherCores(int _size);
        void setCoresStats();
        std::vector<std::string> getCoresStats() const;
};
class DataInit : public ProcData, public SysInfo{
  
     std::vector<ProcData> list_;
     std::string OSName,kernelVer;
    public:
        DataInit(){
            this->reloadList();
            setAttr();
            OSName = ProcParser::getOS();
            kernelVer = ProcParser::getKernelVer();
        }
     void reloadList();
     std::string getKernelVer() const;
     std::string getOs() const;
     std::string printList();
     std::vector<std::string> getList();
};