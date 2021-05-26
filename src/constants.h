#pragma once
#include<string>

class Path{
    public:
    static std::string basePath(){ 
        return "/proc/";         // /proc is a Virtual File System. Contained within the proc file system are information about processes and other system information.
         }                        //It is mapped to /proc and mounted at boot time.
    static std::string cmdPath(){
        return "/cmdline";        // /proc/[pid]/cmdline - a read-only file holds the complete command line for
                                // the process, unless the process is a zombie.
    }
    static std::string statusPath(){
        return "/status";           //Provides much of the information in /proc/[pid]/stat and
     }                               // /proc/[pid]/statm in a format that's easier for humans to  parse. 
                              
    static std::string statpath(){
        return "stat";              // /proc/[pid]/stat provides information about memory usage, measured in pages.
    }
    static std::string upTimePath(){
        return "uptime";            //  /proc/uptime contains two numbers (values in seconds): the
         }                           // uptime of the system (including time spent in suspend) and
                                    // the amount of time spent in the idle process.
    
    static std::string memInfoPath(){
        return "meminfo";         // /proc/meminfo contains a bunch of information about your system’s memory, including the
    }                              //  total amount available (in kb) and the amount free on the top two lines.
    static std::string versionPath(){
        return "version";           //This string identifies the kernel version that is
                                    // currently running.
    }
};