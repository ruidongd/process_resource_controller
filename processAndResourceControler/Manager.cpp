//
//  Manager.cpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/23/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//
// temp input: /Users/Ruidong/Desktop/input.txt
//TODO:
// CR:同名(A)、无效Priority(A)
// REQ:R不存在(A)、太多(A)、太少(A)、INIT(A)
// REL:R不存在(A)、太多(A)、太少(A)、INIT(A)
// DE:不存在(A)、INIT(A)、非自己(A)、非子辈(A)
#include "Manager.hpp"
//
Manager::Manager(){
    Process *p = new Process("init", 0);
    p->getPCB().status.first = "ready";
    p->getPCB().status.second = &RL;
    RL.push_back(p);
    R1 = Resource(1, "free");
    R2 = Resource(2, "free");
    R3 = Resource(3, "free");
    R4 = Resource(4, "free");
    running = nullptr;
    scheduler();
}

Manager::~Manager(){
    RL[0]->destory(names, R1, R2, R3, R4, RL);
}
void Manager::reset(){
    RL[0]->destory(names, R1, R2, R3, R4, RL);
    Process *p = new Process("init", 0);
    p->getPCB().status.first = "ready";
    p->getPCB().status.second = &RL;
    RL.push_back(p);
    R1 = Resource(1, "free");
    R2 = Resource(2, "free");
    R3 = Resource(3, "free");
    R4 = Resource(4, "free");
    running = nullptr;
    scheduler();
}

void Manager::create_process(std::string name, int priority){
    if( name == "init" ||std::find(names.begin(), names.end(), name) != names.end())
        throw invalid_manipulate_exception();
    else if (priority > 2 || priority <= 0)
        throw invalid_manipulate_exception();
    Process* p = new Process(name, priority);
    RL.push_back(p);
    running->getPCB().tree.children.push_back(p);
    p->getPCB().tree.parent = running;
    p->getPCB().status.first = "ready";
    p->getPCB().status.second = &RL;
    names.push_back(name);
    scheduler();
}

void Manager::destory(std::string name){
    // check the PID is running or its children
    if(name == "init"|| !running->isChildren(name))
        throw invalid_manipulate_exception();
    running->getChildren(name)->destory(names, R1, R2, R3, R4, RL);
    if(name == running->getPCB().ID)
        running = nullptr;
    scheduler();
    
}

Resource::RCB& Manager::getResource(std::string RID){
    if(RID == "R1")
        return R1.getRCB();
    else if (RID == "R2")
        return R2.getRCB();
    else if (RID == "R3")
        return R3.getRCB();
    else if (RID == "R4")
        return R4.getRCB();
    else
        throw invalid_manipulate_exception();
}

void Manager::request(std::string RID){
    if(running->getPCB().ID == "init")
        throw invalid_manipulate_exception();
    Resource::RCB& r = getResource(RID);
    running->request(r, RL);
    scheduler();
}

void Manager::request(std::string RID, int unit){
    if(running->getPCB().ID == "init")
        throw invalid_manipulate_exception();
    Resource::RCB& r = getResource(RID);
    running->request(r, unit, RL);
    scheduler();
}
void Manager::release(std::string RID){
    if(running->getPCB().ID == "init")
        throw invalid_manipulate_exception();
    Resource::RCB& r = getResource(RID);
    running->release(r, RL);
    scheduler();
}

void Manager::release(std::string RID, int unit){
    if(running->getPCB().ID == "init")
        throw invalid_manipulate_exception();
    Resource::RCB& r = getResource(RID);
    running->release(r, unit, RL);
    scheduler();
}

void Manager::timeout(){
    RL.erase(std::find(RL.begin(), RL.end(), running));
    running->getPCB().status.first = "ready";
    RL.push_back(running);
    scheduler();
}


void Manager::scheduler(){
    // find highest priority q
    int priority = -1;
    Process* temp = nullptr;
    for(auto p: RL){
        if(p->getPCB().priority > priority){
            priority = p->getPCB().priority;
            temp = p;
        }
    }
    if(!running || running->getPCB().status.first != "running"){
        running = temp;
    }
    else if (running->getPCB().priority < temp->getPCB().priority){
        running->getPCB().status.first = "ready";
        running = temp;
    }
    running->getPCB().status.first = "running";
}

/*Test shell
void Manager::shell(){
    std::string input;
    std::cout << running->getPCB().ID << " "; // change
    while(true){ // change
                std::cout << std::endl;
                std::cout << "R1:" << R1.getRCB().Status << " " <<  R1.getRCB().remain_unit << std::endl;
                std::cout << "R2:" << R2.getRCB().Status << " " <<  R2.getRCB().remain_unit << std::endl;
                std::cout << "R3:" << R3.getRCB().Status << " " <<  R3.getRCB().remain_unit << std::endl;
                std::cout << "R4:" << R4.getRCB().Status << " " <<  R4.getRCB().remain_unit << std::endl;
        try{
            if(!std::getline(std::cin, input))
                break;
            if((input.find_first_not_of(' ') == std::string::npos)){
                std::cout << std::endl; // change
                if(!std::getline(std::cin, input)) // change
                    break;
            }
            std::istringstream iss(input);
            std::vector<std::string> inputs{std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>{}};
            if(inputs[0] == "init"){
                reset();
                std::cout << running->getPCB().ID << " "; // change
            }
            else if(inputs.size() == 3 && inputs[0] == "cr"){
                create_process(inputs[1], stoi(inputs[2]));
                std::cout << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "req" && (inputs.size() == 3 || inputs.size() == 2)){
                if(inputs.size() == 3)
                    request(inputs[1], stoi(inputs[2]));
                else if(inputs.size() == 2)
                    request(inputs[1]);
                std::cout << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "rel" && (inputs.size() == 3 || inputs.size() == 2)){
                if(inputs.size() == 3)
                    release(inputs[1], stoi(inputs[2]));
                else if(inputs.size() == 2)
                    release(inputs[1]);
                std::cout << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "to" && inputs.size() ==1 ){
                timeout();
                std::cout << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "de" && inputs.size() == 2){
                destory(inputs[1]);
                std::cout << running->getPCB().ID << " ";// change
            }
            else if (inputs[0] == "rl" && inputs.size() == 1){
                std::cout << "RL: ";
                for(auto p:RL){
                    std::cout << p->getPCB().ID << " ";
                }
                std::cout << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "wl" && inputs.size() == 1){
                std::cout << "R1: ";
                for(auto temp : R1.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << std::endl;
                std::cout << "R2: ";
                for(auto temp : R2.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << std::endl;
                std::cout << "R3: ";
                for(auto temp : R3.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << std::endl;
                std::cout << "R4: ";
                for(auto temp : R4.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << running->getPCB().ID << " ";
            }
            else if(inputs[0] == "re"){
                for(auto temp:running->getPCB().other_resource){
                    std::cout << "Kepping " << temp << std::endl;
                }
            }
            else
                throw invalid_manipulate_exception();
        }
        catch(invalid_manipulate_exception& e){
            std::cout << "error ";// change
        }
        catch(requestException& e){
            std::cout << "error ";// change
        }
        catch(releaseException& e){
            std::cout << "error ";// change
        }
    }
}
*/
std::string SplitFilename (const std::string& str)
{
    size_t found;
    found=str.find_last_of("/\\");
    return str.substr(0,found);
}

void Manager::shell(){
    std::cout << "File Path:";
    std::string file_path;
    std::getline(std::cin, file_path);
    std::ifstream in;
    std::ofstream out;
    in.open(file_path);
    out.open(SplitFilename(file_path)+"/69229912.txt");
    std::string input;
    out << running->getPCB().ID << " "; // change
    while(std::getline(in, input)){ // change
        try{
            if((input.find_first_not_of(' ') == std::string::npos)){
                out << std::endl; // change
                if(!std::getline(in, input)) // change
                    break;
            }
            std::istringstream iss(input);
            std::vector<std::string> inputs{std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>{}};
            if(inputs[0] == "init"){
                reset();
                out << running->getPCB().ID << " "; // change
            }
            else if(inputs.size() == 3 && inputs[0] == "cr"){
                create_process(inputs[1], stoi(inputs[2]));
                out << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "req" && (inputs.size() == 3 || inputs.size() == 2)){
                if(inputs.size() == 3)
                    request(inputs[1], stoi(inputs[2]));
                else if(inputs.size() == 2)
                    request(inputs[1]);
                out << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "rel" && (inputs.size() == 3 || inputs.size() == 2)){
                if(inputs.size() == 3)
                    release(inputs[1], stoi(inputs[2]));
                else if(inputs.size() == 2)
                    release(inputs[1]);
                out << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "to" && inputs.size() ==1 ){
                timeout();
                out << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "de" && inputs.size() == 2){
                destory(inputs[1]);
                out << running->getPCB().ID << " ";// change
            }
            else if (inputs[0] == "rl" && inputs.size() == 1){
                std::cout << "RL: ";
                for(auto p:RL){
                    std::cout << p->getPCB().ID << " ";
                }
                std::cout << running->getPCB().ID << " ";// change
            }
            else if(inputs[0] == "wl" && inputs.size() == 1){
                std::cout << "R1: ";
                for(auto temp : R1.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << std::endl;
                std::cout << "R2: ";
                for(auto temp : R2.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << std::endl;
                std::cout << "R3: ";
                for(auto temp : R3.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << std::endl;
                std::cout << "R4: ";
                for(auto temp : R4.getRCB().WL)
                    std::cout << temp->getPCB().ID << " " << temp->getPCB().status.first << " " << temp->getPCB().wait_unit << "unit ";
                std::cout << running->getPCB().ID << " ";
            }
            else if(inputs[0] == "re"){
                for(auto temp:running->getPCB().other_resource){
                    std::cout << "Kepping " << temp << std::endl;
                }
            }
            else
                throw invalid_manipulate_exception();
        }
        catch(invalid_manipulate_exception& e){
            out << "error ";// change
        }
        catch(requestException& e){
            out << "error ";// change
        }
        catch(releaseException& e){
            out << "error ";// change
        }
    }
    std::cout << "Output finished" << std::endl;
    in.close();
    out.close();
}
