#include "ProcInfo.h"

void ProcInfo::setPid(int pid){
    this->pid = pid;
}
std::string ProcInfo::getPid() const{
    return this->pid;
}
std::string ProcInfo::getProcess(){
    mem = ProcessParser::getVmSize(pid);
    upTime = ProcessParser::getProcUpTime(pid);
    cpu = ProcessParser::getCpuPercent(pid);
    //generate a complete string with all the process info
    std::string res = (pid + "   " + user + "    " + mem.substr(0,5) + "     " + cpu.substr(0,5) + "     " + upTime.substr(0,5) + "    " + cmd.substr(0,30) + "...");
    return res;
}