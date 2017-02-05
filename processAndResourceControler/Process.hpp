//
//  Process.hpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/23/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//

#ifndef Process_hpp
#define Process_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

class Process{
    struct PCB{
        struct creation_tree{
            Process* parent;
            std::vector<Process*> children;
        };
        creation_tree tree;
        std::string ID;
        std::vector<std::pair<std::string, int>> other_resource;
        std::pair<std::string, std::vector<Process*>*> status;
        int priority;
    };
    PCB p;
    public:
    
    Process(){};
    Process(std::string PID, int priority);
    PCB& getPCB();
    void destory();
    void printPID();
    void request(std::string RID);
    void request(std::string RID, int n);
    void release(std::string RID);
    ~Process();
};

#endif /* Process_hpp */
