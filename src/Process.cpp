#include "Process.h"
void Process::setPid(int pid)
{
    this->pid = pid;
}
std::string Process::getPid() const 
{
    return this->pid;
}
std::string Process::getProcess()
{
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    std::string res = (this->pid + "   " + this->user + "   " + this->mem.substr(0,5) + "     " + this->cpu.substr(0,5) + "     " + this->upTime.substr(0,5) + "    " + this->cmd.substr(0,30) + "...");
    return res;
}