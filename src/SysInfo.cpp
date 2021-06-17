#include<iostream>
#include "SysInfo.h"
#include "util.h"
// getters:

std::string SysInfo::getCpuPercent() const 
{
    return cpuPercent;
}

std::string SysInfo::getMemPercent() const 
{
    return std::to_string(memPercent);
}

long long SysInfo::getUpTime() const 
{
    return upTime;
}

std::string SysInfo::getKernelVersion() const 
{
    return kernelVer;
}

std::string SysInfo::getTotalProc() const 
{
    return std::to_string(totalProc);
}

std::string SysInfo::getRunningProc() const 
{
    return std::to_string(runningProc);
}

std::string SysInfo::getThreads() const 
{
    return std::to_string(threads);
}

std::string SysInfo::getOsName() const 
{
    return OSName;
}

// setters:

void SysInfo::setLastCpuStats()
{
    lastCpuStats = ProcessParser::getSysCpuPercent();
}

void SysInfo::getOtherCores(int len)
{
    //when number of cores is detected, vectors are modified to fit incoming data
    cores_stats = std::vector<std::string>();
    cores_stats.resize(len);
    lastCoresStats = std::vector<std::vector<std::string>>();
    lastCoresStats.resize(len);
    currentCoresStats = std::vector<std::vector<std::string>>();
    currentCoresStats.resize(len);
    for (int i = 0; i < len; i++) {
        lastCoresStats[i] = ProcessParser::getSysCpuPercent(std::to_string(i));
    }
}

void SysInfo::setCoresStats()
{
    // Getting data from files (previous data is required)
    for(int i = 0; i < currentCoresStats.size(); i++) {
        currentCoresStats[i] = ProcessParser::getSysCpuPercent(std::to_string(i));
    }
    for(int i = 0; i < currentCoresStats.size(); i++) {
        // after acquirement of data we are calculating every core percentage of usage
        cores_stats[i] = ProcessParser::PrintCpuStats(lastCoresStats[i],currentCoresStats[i]);
    }
    //updating the last cpu cores stats
    lastCoresStats = currentCoresStats;
}

void SysInfo::setAttributes() 
{
    // getting parsed data
    memPercent = ProcessParser::getSysRamPercent();
    upTime = ProcessParser::getSysUpTime();
    totalProc = ProcessParser::getTotalNumberOfProcesses();
    runningProc = ProcessParser::getNumberOfRunningProcesses();
    threads = ProcessParser::getTotalThreads();
    currentCpuStats = ProcessParser::getSysCpuPercent();
    cpuPercent = ProcessParser::PrintCpuStats(lastCpuStats,currentCpuStats);
    lastCpuStats = currentCpuStats;
    setCoresStats();
}

// Constructing a string for every core data display
std::vector<std::string> SysInfo::getCoresStats() const
{
    std::vector<std::string> result = std::vector<std::string>();
    for (int i = 0; i < cores_stats.size() ;i++) {
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
