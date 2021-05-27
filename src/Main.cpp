#include<iostream>
#include"SysInfo.h"
#include"ProcessParser.h"
int main(void){
    std::cout<<ProcessParser::getProcUser("1852")<<std::endl;
}