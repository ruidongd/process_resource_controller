//
//  Manager.hpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/23/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//

#ifndef Manager_hpp
#define Manager_hpp
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <exception>
#include "Process.hpp"
struct invalid_manipulate_exception : public std::exception{
    virtual const char * what() const throw(){
        return "error";
    }
};

class Manager{
    std::vector<Process*> RL;
    void reset();
    Resource R1, R2, R3, R4;
    Process* running;
    std::vector<std::string> names;
public:
    Manager();
    void shell();
    void scheduler();
    void create_process(std::string name, int priority);
    void request(std::string RID);
    void request(std::string RID, int unit);
    void release(std::string RID);
    void release(std::string RID, int unit);
    void destory(std::string name);
    void timeout();
    Resource::RCB& getResource(std::string RID);
    ~Manager();
};

#endif /* Manager_hpp */
