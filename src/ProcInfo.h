#include<iostream>
#include "ProcParser.h"
class ProcInfo{
private:
        std::string pid;
        std::string user;
        std::string cmd;
        std::string cpu;
        std::string mem;
        std::string upTime;
public:
    ProcInfo(std::string pid){
        this->pid = pid;
        user = ProcessParser::getProcUser(pid);
        cmd = ProcessParser::getCmd(pid);
        cpu = ProcessParser::getCpuPercent(pid);
        mem = ProcessParser::getVmSize(pid);
        upTime = ProcessParser::getProcUpTime(pid);
    }
        void setPid(int pid);
        std::string getPid() const;
        std::string getProcess();
};