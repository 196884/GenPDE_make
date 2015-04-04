//
//  ChoiceEnums.h
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef CHOICEENUMS_H_
#define CHOICEENUMS_H_

namespace Choice
{
    typedef std::string Uid;
    enum Chooser {
        Chooser_Us        = 0,
        Chooser_Client    = 1
    };
    enum Choice {
        Choice_None  = -1,
        Choice_Leg0  = 0,
        Choice_Leg1  = 1
    };
}

#endif // CHOICEENUMS_H_
