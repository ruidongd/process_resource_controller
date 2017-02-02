//
//  Manager.cpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/23/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//

#include "Manager.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

Manager::Manager(){
    Process *p = new Process("Init", 0);
    p->getPCB().status.first = "ready";
    p->getPCB().status.second = &RL;
    RL.push_back(p);
    R1 = Resource(1, "free");
    R2 = Resource(2, "free");
    R3 = Resource(3, "free");
    R4 = Resource(4, "free");
    scheduler();
}
Manager::~Manager(){
    RL[0]->destory();
}
void Manager::reset(){
    RL[0]->destory();
    Process *p = new Process("Init", 0);
    p->getPCB().status.first = "ready";
    p->getPCB().status.second = &RL;
    RL.push_back(p);
    R1 = Resource(1, "free");
    R2 = Resource(2, "free");
    R3 = Resource(3, "free");
    R4 = Resource(4, "free");
    scheduler();
}

void Manager::create_proces(std::string name, int priority){
    Process* p = new Process(name, priority);
    RL.push_back(p);
    running->getPCB().tree.children.push_back(p);
    p->getPCB().tree.parent = running;
    p->getPCB().status.first = "ready";
    p->getPCB().status.second = &RL;
    scheduler();
}

void Manager::destory(std::string name){
    
    for(int i = 0; i < RL.size(); ++i){
        if(RL[i]->getPCB().ID == name){
            RL[i]->destory();
            break;
        }
    }
    scheduler();
    
}

Resource::RCB& Manager::getResource(int RID){
    if(RID == 1)
        return R1.getRCB();
    else if (RID == 2)
        return R2.getRCB();
    else if (RID == 3)
        return R3.getRCB();
    else if (RID == 4)
        return R4.getRCB();
    // cannot meet
    else
        return R1.getRCB();
}

void Manager::request(int RID){
    Resource::RCB& temp = getResource(RID);
    if(temp.Status == "free"){
        temp.Status = "Allocated";
        running->getPCB().other_resource.push_back(std::make_pair(RID, temp.remain_unit));
        temp.remain_unit = 0;
    }
    else{
        running->getPCB().status.first = "Block";
        temp.WL.push_back(running);
        RL.erase(std::find(RL.begin(), RL.end(), running));
        running->getPCB().status.second = &temp.WL;
    }
    scheduler();
}

void Manager::request(int RID, int unit){
    Resource::RCB& temp = getResource(RID);
    if(temp.remain_unit >= unit){
        if(temp.remain_unit == unit)
            temp.Status = "Allocated";
        running->getPCB().other_resource.push_back(std::make_pair(RID, unit));
        temp.remain_unit -= unit;
    }
    else{
        running->getPCB().status.first = "Block";
        temp.WL.push_back(running);
        RL.erase(std::find(RL.begin(), RL.end(), running));
        running->getPCB().status.second = &temp.WL;
    }
    scheduler();
}

void Manager::release(int RID){
    
}

void Manager::release(int RID, int unit){
    
}

void Manager::timeout(){
    RL.erase(std::find(RL.begin(), RL.end(), running));
    RL.push_back(running);
    scheduler();
}


void Manager::scheduler(){
    int priority = -1;
    if(running) running->getPCB().status.first = "ready";
    for(int i = 0; i < RL.size(); ++i){
            if(RL[i]->getPCB().priority > priority){
            running = RL[i];
            priority = RL[i]->getPCB().priority;
        }
    }
    running->getPCB().status.first = "running";
}



void Manager::shell(){
    std::string input;
    while(true){
        std::cout << running->getPCB().ID << std::endl;
        std::cout << "Shell >> ";
        if(!std::getline(std::cin, input) || (input.find_first_not_of(' ') == std::string::npos))
            break;
        std::istringstream iss(input);
        std::vector<std::string> inputs{std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{}};
        if(inputs[0] == "init"){
            reset();
        }
        else if(inputs[0] == "cr"){
            create_proces(inputs[1], stoi(inputs[2]));
        }
        else if(inputs[0] == "req"){
            int num = stoi(inputs[1]);
            if(num >= 1 && num <= 4 && inputs.size() == 3)
                request(num, stoi(inputs[2]));
            else if(num >= 1 && num <= 4 && inputs.size() == 2)
                request(num);
        }
        else if(inputs[0] == "rel"){
            int num = stoi(inputs[1]);
            if(num >= 1 && num <= 4 && inputs.size() == 3)
                release(num, stoi(inputs[2]));
            else if(num >= 1 && num <= 4 && inputs.size() == 2)
                release(num);        }
        else if(inputs[0] == "to"){
            timeout();
        }
        else if(inputs[0] == "de"){
            destory(inputs[1]);
        }
    }
}