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
        this-> user = ProcessParser::getProcUser(pid);
        this->cmd = ProcessParser::getCmd(pid);
        this->cpu = ProcessParser::getCpuPercent(pid);
        this->mem = ProcessParser::getVmSize(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
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