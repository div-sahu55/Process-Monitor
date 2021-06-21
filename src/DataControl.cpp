#include "DataControl.h"
#include "util.h"
void ProcData::setPid(int pid){
    this->pids = pid;
}
std::string ProcData::getPid() const{
    return this->pids;
}
std::string ProcData::getProcess(){
    mem = ProcParser::getVmSize(pids);
    //generate a complete string with all the process info
    std::string res = (pids + "   " + user + "    " + mem.substr(0,5)  + "    " + cmd.substr(0,40) + "...");
    return res;
}
void DataInit::refreshList(){
    std::vector<std::string> pids = ProcParser::getPidList();
    //deleting all the previosly stored pids
    this->list_.clear();
    for(auto pid : pids){
        ProcData proc(pid); // passing pid value to the object
        this->list_.push_back(proc);
    }
}
std::string DataInit::printList(){
    std::string result="";
    for (ProcData p : list_) {
        result += p.getProcess();
    }
    return result;
}
std::vector<std::string> DataInit::getList(){
    std::vector<std::string> values;
    for (int i = (this->list_.size()-10); i < this->list_.size(); i++){
        values.push_back(this->list_[i].getProcess());
    }
    return values;
}
// getters and setters:
void SysInfo::setAttributes(){
    // getting parsed data
    memPercent = ProcParser::getSysRamPercent();
    upTime = ProcParser::getSysUpTime();
    totalProc = ProcParser::getTotalNumberOfProcesses();
    runningProc = ProcParser::getNumberOfRunningProcesses();
    currentCpuStats = ProcParser::getSysCpuPercent();
    cpuPercent = ProcParser::PrintCpuStats(lastCpuStats,currentCpuStats);
    lastCpuStats = currentCpuStats;
    setCoresStats();
}
void SysInfo::getOtherCores(int len){
    //when number of cores is detected, vectors are modified to fit incoming data
    cores_stats = std::vector<std::string>();
    cores_stats.resize(len);
    lastCoresStats = std::vector<std::vector<std::string>>();
    lastCoresStats.resize(len);
    currentCoresStats = std::vector<std::vector<std::string>>();
    currentCoresStats.resize(len);
    for (int i = 0; i < len; i++) {
        lastCoresStats[i] = ProcParser::getSysCpuPercent(std::to_string(i));
    }
}
void SysInfo::setCoresStats(){
    // Getting data from files (previous data is required)
    for(int i = 0; i < currentCoresStats.size(); i++) {
        currentCoresStats[i] = ProcParser::getSysCpuPercent(std::to_string(i));
    }
    for(int i = 0; i < currentCoresStats.size(); i++) {
        // after acquirement of data we are calculating every core percentage of usage
        cores_stats[i] = ProcParser::PrintCpuStats(lastCoresStats[i],currentCoresStats[i]);
    }
    //updating the last cpu cores stats
    lastCoresStats = currentCoresStats;
}
// Constructing a string for every core data display
std::vector<std::string> SysInfo::getCoresStats() const{
    std::vector<std::string> result = std::vector<std::string>();
    for (int i = 0; i < cores_stats.size();i++) {
        std::string temp = ("cpu" + std::to_string(i) +": ");
        float check = stof(cores_stats[i]);
        if (!check || cores_stats[i] == "nan") {
            return std::vector<std::string>();
        }
        temp += Util::getProgressBar(cores_stats[i]);
        result.push_back(temp);
    }
    return result;
}
std::string SysInfo::getCpuPercent() const{
    return cpuPercent;
}
std::string SysInfo::getMemPercent() const{
    return std::to_string(memPercent);
}
long long int SysInfo::getUpTime() const{
    return upTime;
}
std::string SysInfo::getKernelVersion() const{
    return kernelVer;
}
std::string SysInfo::getTotalProc() const{
    return std::to_string(totalProc);
}
std::string SysInfo::getRunningProc() const{
    return std::to_string(runningProc);
}
std::string SysInfo::getOsName() const{
    return OSName;
}
void SysInfo::setLastCpuStats(){
    lastCpuStats = ProcParser::getSysCpuPercent();
}