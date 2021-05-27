#include<iostream>
#include"SysInfo.h"
#include"ProcessParser.h"
int main(void){
    for(auto e : ProcessParser::getPidList())
    std::cout<<e<<std::endl;
}