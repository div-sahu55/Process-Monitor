#pragma once
#include<string>

class Path{
    public:
    static std::string basePath(){
        return "/proc/";
    }
    static std::string cmdPath(){
        return "/cmdline";
    }
    static std::string statusPath(){
        return "/status";
    }
    static std::string statpath(){
        return "stat";
    }
    static std::string upTimePath(){
        return "uptime";
    }
    static std::string memInfoPath(){
        return "meminfo";
    }
    static std::string versionPath(){
        return "version";
    }
};