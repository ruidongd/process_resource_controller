//
//  Manager.hpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/23/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//

#ifndef Manager_hpp
#define Manager_hpp
#include <sstream>

#include "Resource.hpp"
#include "Process.hpp"

class Manager{
    std::vector<Process*> RL;
    void reset();
    Resource R1, R2, R3, R4;
    Process* running;
public:
    Manager();
    void shell();
    void scheduler();
    void create_proces(std::string name, int priority);
    void request(int RID);
    void request(int RID, int unit);
    void release(int RID);
    void release(int RID, int unit);
    void destory(std::string name);
    void timeout();
    Resource::RCB& getResource(int RID);
    ~Manager();
};

#endif /* Manager_hpp */
