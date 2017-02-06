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
    p.other_resource.push_back(0);
    p.other_resource.push_back(0);
    p.other_resource.push_back(0);
    p.other_resource.push_back(0);
}


Process::~Process(){
}

Process* Process::getChildren(std::string name){
    if(p.ID == name)
        return this;
    for(auto temp: p.tree.children){
        if(temp->isChildren(name))
            return temp;
    }
    return nullptr;
}
bool Process::isChildren(std::string name){
    if(p.ID == name)
        return true;
    for(auto temp: p.tree.children){
        if(temp->isChildren(name))
            return true;
    }
    return false;
}


void Process::request(Resource::RCB& r ,std::vector<Process*>& RL){
    if(r.Status == "free"){
        r.Status = "Allocated";
        p.other_resource[r.RID-1] = r.remain_unit;
        r.remain_unit = 0;
    }
    else{
        r.WL.push_back(this);
        RL.erase(std::find(RL.begin(), RL.end(), this));
        p.wait_unit = r.RID;
        p.status.first = "Blocked";
        p.status.second = &r.WL;
    }
    
}
void Process::request(Resource::RCB& r, int unit, std::vector<Process*>& RL){
    if(unit > r.RID || unit <= 0){
        throw requestException();
    }
    if(p.other_resource[r.RID-1] + unit > r.RID)
        throw requestException();
    if(r.remain_unit >= unit){
        r.Status = "Allocated";
        p.other_resource[r.RID-1] += unit;
        r.remain_unit -= unit;
    }
    else{
        r.WL.push_back(this);
        RL.erase(std::find(RL.begin(), RL.end(), this));
        p.status.first = "Blocked";
        p.status.second = &r.WL;
        p.wait_unit = unit;
    }
}
void Process::release(Resource::RCB &r, std::vector<Process *> &RL){
    if(p.other_resource[r.RID-1] == 0)
        throw releaseException();
    r.remain_unit += p.other_resource[r.RID-1];
    if(r.remain_unit == r.RID && r.WL.size() == 0)
        r.Status = "free";
    p.other_resource[r.RID-1] = 0;
    while(r.WL.size() > 0 && r.WL[0]->getPCB().wait_unit <= r.remain_unit){
        r.WL[0]->getPCB().status.first = "ready";
        r.WL[0]->getPCB().status.second = &RL;
        r.WL[0]->getPCB().other_resource[r.RID-1] += r.WL[0]->getPCB().wait_unit;
        r.remain_unit -= r.WL[0]->getPCB().wait_unit;
        r.WL[0]->getPCB().wait_unit = 0;
        RL.push_back(r.WL[0]);
        r.WL.erase(r.WL.begin()+0);
    }
}
void Process::release(Resource::RCB &r, int n, std::vector<Process *> &RL){
    if(n <= 0|| p.other_resource[r.RID-1] < n){
        std::cout << p.other_resource[r.RID-1] << std::endl;
        throw releaseException();
    }
    r.remain_unit += n;
    if(r.remain_unit == r.RID && r.WL.size() == 0)
        r.Status = "free";
    p.other_resource[r.RID-1] -= n;
    while(r.WL.size() > 0 && r.WL[0]->getPCB().wait_unit <= r.remain_unit){
        r.WL[0]->getPCB().status.first = "ready";
        r.WL[0]->getPCB().status.second = &RL;
        r.WL[0]->getPCB().other_resource[r.RID-1] += r.WL[0]->getPCB().wait_unit;
        r.remain_unit -= r.WL[0]->getPCB().wait_unit;
        r.WL[0]->getPCB().wait_unit = 0;
        RL.push_back(r.WL[0]);
        r.WL.erase(r.WL.begin()+0);
    }
    
}

void Process::destory_release(Resource &R, std::vector<Process *> &RL){
    R.getRCB().remain_unit += p.other_resource[R.getRCB().RID-1];
    if(R.getRCB().remain_unit == R.getRCB().RID && R.getRCB().WL.size() == 0)
        R.getRCB().Status = "free";
    while(R.getRCB().WL.size() > 0 && R.getRCB().WL[0]->getPCB().wait_unit <= R.getRCB().remain_unit){
        R.getRCB().WL[0]->getPCB().status.first = "ready";
        R.getRCB().WL[0]->getPCB().status.second = &RL;
        R.getRCB().WL[0]->getPCB().other_resource[R.getRCB().RID-1] += R.getRCB().WL[0]->getPCB().wait_unit;
        R.getRCB().remain_unit -= R.getRCB().WL[0]->getPCB().wait_unit;
        R.getRCB().WL[0]->getPCB().wait_unit = 0;
        RL.push_back(R.getRCB().WL[0]);
        R.getRCB().WL.erase(R.getRCB().WL.begin()+0);
    }
}
void Process::destory(std::vector<std::string>& names, Resource& R1, Resource& R2, Resource& R3, Resource& R4,std::vector<Process*>& RL){
    // delete name from names
    auto it = std::find(names.begin(), names.end(), p.ID);
    if(it != names.end())
       names.erase(it);
    // release resource
    if(p.ID != "init"){
        for(int i = 1; i <= 4; ++i){
            if(i == 1)
                destory_release(R1, RL);
            else if(i == 2)
                destory_release(R2, RL);
            else if(i == 3)
                destory_release(R3, RL);
            else
                destory_release(R4, RL);
        }
    }
    
    // deallocate children
    while(p.tree.children.size() > 0){
        p.tree.children[0]->destory(names, R1, R2, R3, R4, RL);
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
    p.status.second->erase(std::find(p.status.second->begin(), p.status.second->end(),this));
    delete this;
}
Process::PCB& Process::getPCB(){
    return p;
}

Resource::Resource(int RID, std::string Status){
    r.RID = RID;
    r.Status = Status;
    r.remain_unit = RID;
}

Resource::RCB& Resource::getRCB(){
    return r;
}
