#include<iostream>>
class Process{
    private:
    int PID;
    std::string User;
    std::string CMD;
    double CPU;
    std::string Memory;
    std::string Time;
    public:
    Process();
    void setPID(int);
    int getPID();
    void setUser(std::string);
    std::string getUser();
    void setCPU(double);
    double getCPU();
    void setMemory(std::string);
    void setTime(std::string);
    std::string getMemory();
    std::string getTime();
};