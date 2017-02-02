//
//  Process.cpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/23/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//

#include "Process.hpp"

Process::Process(std::string PID, int prioprity){
    p.ID = PID;
    p.priority = prioprity;
}


Process::~Process(){
}

void Process::destory(){
    // release resource
    
    // deallocate children
    if(p.tree.children.size() > 0){
        for(auto c:p.tree.children){
            c->destory();
        }
    }
    p.tree.children.clear();
    // delete from parent
    int index = 0;
    if(p.tree.parent){
        for(; index < p.tree.parent->getPCB().tree.children.size();++index){
            if(p.tree.parent->getPCB().tree.children.at(index)->getPCB().ID == this->getPCB().ID){
                p.tree.parent->getPCB().tree.children.erase(p.tree.parent->getPCB().tree.children.begin()+index);
            }
        }
    }
    // delete from the the other_resource list(RL/WL)
    index = 0;
    for(; index < p.status.second->size(); ++index){
        if(p.status.second->at(index)->getPCB().ID == this->getPCB().ID){
            p.status.second->erase(p.status.second->begin()+index);
            break;
        }
    }
    delete this;
}
Process::PCB& Process::getPCB(){
    return p;
}
