#include<iostream>
#include"SysInfo.h"
#include"ProcessParser.h"
int main(void){
    std::cout<<ProcessParser::getVmSize("300")<<std::endl;
}