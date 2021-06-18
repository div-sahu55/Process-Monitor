#include "ProcInfo.h"

void Process::setPid(int pid)
{
    this->pid = pid;
}
std::string Process::getPid() const 
{
    return pid;
}
std::string Process::getProcess()
{
    mem = ProcessParser::getVmSize(pid);
    upTime = ProcessParser::getProcUpTime(pid);
    cpu = ProcessParser::getCpuPercent(pid);

    std::string res = (pid + "   " + user + "    " + mem.substr(0,5) + "     " + cpu.substr(0,5) + "     " + upTime.substr(0,5) + "    " + cmd.substr(0,30) + "...");
    return res;
}