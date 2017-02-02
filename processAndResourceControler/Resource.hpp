//
//  Resource.hpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/24/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//

#ifndef Resource_hpp
#define Resource_hpp
#include "Process.hpp"
class Resource{
    
    
public:
    struct RCB{
        int RID;
        std::string Status;
        std::vector<Process*> WL;
        int remain_unit;
    };
    RCB r;
    Resource(){};
    Resource(int RID, std::string Status);
    RCB& getRCB();
    ~Resource(){};
};



#endif /* Resource_hpp */
