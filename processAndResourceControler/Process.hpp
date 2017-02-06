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
#include <exception>

class Resource;
class Process;

class requestException : public std::exception{
    virtual const char * what() const throw(){
        return "error";
    }
};

class releaseException : public std::exception{
    virtual const char * what() const throw(){
        return "error";
    }
};

class Resource{
public:
    struct RCB{
        int RID;
        std::string Status;
        std::vector<Process*> WL;
        std::vector<int> Waiting_unit;
        int remain_unit;
    };
    RCB r;
    Resource(){};
    Resource(int RID, std::string Status);
    RCB& getRCB();
    ~Resource(){};
};

class Process{
    struct PCB{
        struct creation_tree{
            Process* parent;
            std::vector<Process*> children;
        };
        creation_tree tree;
        std::string ID;
        std::vector<int> other_resource; // name, holding unit
        std::pair<std::string, std::vector<Process*>*> status;
        int priority;
        int wait_unit;
    };
    PCB p;
    public:
    
    Process(){};
    Process(std::string PID, int priority);
    PCB& getPCB();
    void destory(std::vector<std::string>& names, Resource& R1, Resource& R2, Resource& R3, Resource& R4, std::vector<Process*>& RL);
    void printPID();
    void request(Resource::RCB& r, std::vector<Process*>& RL);
    void request(Resource::RCB& r, int unit, std::vector<Process*>& RL);
    void release(Resource::RCB& r, std::vector<Process*>& RL);
    void release(Resource::RCB& r, int n, std::vector<Process*>& RL);
    void destory_release(Resource& R, std::vector<Process*>& RL);
    bool isChildren(std::string name);
    Process* getChildren(std::string name);
    ~Process();
};



#endif /* Process_hpp */
