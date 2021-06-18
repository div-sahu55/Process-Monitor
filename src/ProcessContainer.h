#include "ProcInfo.h"
#include <string>
#include <vector>
class ProcessContainer {
    public:
        ProcessContainer()
        {
            this->refreshList();
        }
        void refreshList();
        std::string printList();
        std::vector<std::string> getList();

    private:
        std::vector<ProcInfo>_list;
};

void ProcessContainer::refreshList()
{
    std::vector<std::string> pids = ProcessParser::getPidList();
    //deleting all the previosly stored pids
    this->_list.clear();
    for(auto pid : pids){
        ProcInfo proc(pid); // passing pid value to the object
        this->_list.push_back(proc);
    }
}

std::string ProcessContainer::printList()
{
    std::string result="";
    for (ProcInfo i : _list) {
        result += i.ProcInfo::getProcess();
    }
    return result;
}

std::vector<std::string> ProcessContainer::getList() 
{
    std::vector<std::string> values;
    for (int i = (this->_list.size()-10); i < this->_list.size(); i++){
        values.push_back(this->_list[i].ProcInfo::getProcess());
    }
    return values;
}