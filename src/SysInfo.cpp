#include<iostream>
#include "SysInfo.h"
#include "util.h"
// getters:

std::string SysInfo::getCpuPercent() const 
{
    return this->cpuPercent;
}

std::string SysInfo::getMemPercent() const 
{
    return std::to_string(this->memPercent);
}

long SysInfo::getUpTime() const 
{
    return this->upTime;
}

std::string SysInfo::getKernelVersion() const 
{
    return this->kernelVer;
}

std::string SysInfo::getTotalProc() const 
{
    return std::to_string(this->totalProc);
}

std::string SysInfo::getRunningProc() const 
{
    return std::to_string(this->runningProc);
}

std::string SysInfo::getThreads() const 
{
    return std::to_string(this->threads);
}

std::string SysInfo::getOsName() const 
{
    return this->osName;
}
// setters:

void SysInfo::setLastCpuMeasures()
{
    this->lastCpuStats = ProcessParser::getSysCpuPercent();
}

void SysInfo::getOtherCores(int _size)
{
    //when number of cores is detected, vectors are modified to fit incoming data
    this->cores_stats = std::vector<std::string>();
    this->cores_stats.resize(_size);
    this->lastCpuCoresStats = std::vector<std::vector<std::string>>();
    this->lastCpuCoresStats.resize(_size);
    this->currentCpuCoresStats = std::vector<std::vector<std::string>>();
    this->currentCpuCoresStats.resize(_size);
    for (int i = 0; i < _size; i++) {
        this->lastCpuCoresStats[i] = ProcessParser::getSysCpuPercent(std::to_string(i));
    }
}

void SysInfo::setCpuCoresStats()
{
    // Getting data from files (previous data is required)
    for(int i = 0; i < this->currentCpuCoresStats.size(); i++) {
        this->currentCpuCoresStats[i] = ProcessParser::getSysCpuPercent(std::to_string(i));
    }
    for(int i = 0; i < this->currentCpuCoresStats.size(); i++) {
        // after acquirement of data we are calculating every core percentage of usage
        this->cores_stats[i] = ProcessParser::PrintCpuStats(this->lastCpuCoresStats[i],this->currentCpuCoresStats[i]);
    }
    this->lastCpuCoresStats = this->currentCpuCoresStats;
}

void SysInfo::setAttributes() 
{
    // getting parsed data
    this->memPercent = ProcessParser::getSysRamPercent();
    this->upTime = ProcessParser::getSysUpTime();
    this->totalProc = ProcessParser::getTotalNumberOfProcesses();
    this->runningProc = ProcessParser::getNumberOfRunningProcesses();
    this->threads = ProcessParser::getTotalThreads();
    this->currentCpuStats = ProcessParser::getSysCpuPercent();
    this->cpuPercent = ProcessParser::PrintCpuStats(this->lastCpuStats,this->currentCpuStats);
    this->lastCpuStats = this->currentCpuStats;
    this->setCpuCoresStats();
}

// Constructing std::string for every core data display
std::vector<std::string> SysInfo::getCoresStats() const
{
    std::vector<std::string> result = std::vector<std::string>();
    for (int i = 0; i < this->cores_stats.size() ;i++) {
        std::string temp = ("cpu" + std::to_string(i) +": ");
        float check = stof(this->cores_stats[i]);
        if (!check || this->cores_stats[i] == "nan") {
            return std::vector<std::string>();
        }
        temp += Util::getProgressBar(this->cores_stats[i]);
        result.push_back(temp);
    }
    return result;
}
