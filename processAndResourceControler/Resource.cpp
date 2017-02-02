//
//  Resource.cpp
//  processAndResourceControler
//
//  Created by DaiRuidong on 1/24/17.
//  Copyright © 2017 RuidongDai. All rights reserved.
//

#include "Resource.hpp"

Resource::Resource(int RID, std::string Status){
    r.RID = RID;
    r.Status = Status;
    r.remain_unit = RID;
}

Resource::RCB& Resource::getRCB(){
    return r;
}