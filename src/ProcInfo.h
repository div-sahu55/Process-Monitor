#include<iostream>
#include "ProcParser.h"
class Process{
private:
        std::string pid;
        std::string user;
        std::string cmd;
        std::string cpu;
        std::string mem;
        std::string upTime;
public:
    Process(std::string pid){
        this->pid = pid;
        user = ProcessParser::getProcUser(pid);
        cmd = ProcessParser::getCmd(pid);
        cpu = ProcessParser::getCpuPercent(pid);
        mem = ProcessParser::getVmSize(pid);
        upTime = ProcessParser::getProcUpTime(pid);
    }
        void setPid(int pid);
        std::string getPid() const;
        std::string getUser() const;
        std::string getCmd() const;
        int getCpu() const;
        int getMem() const;
        std::string getUpTime() const;
        std::string getProcess();
};