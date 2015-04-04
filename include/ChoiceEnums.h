#ifndef CHOICE_ENUMS_H
#define CHOICE_ENUMS_H

#include <string>

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

#endif // CHOICE_ENUMS_H
