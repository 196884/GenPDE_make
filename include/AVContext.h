//
//  AVContext.h
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AVCONTEXT_H
#define AVCONTEXT_H

#include <vector>
#include <map>

#include "Framework.h"

#include "AuxiliaryVariable.h"

class CEValues;

class AVContext
{
protected:
    typedef boost::shared_ptr<const CEValues>            CEVConstPtr;
    typedef std::map< GenPDE::VariableUID, CEVConstPtr > AVValuesMap;

public:
    void setAVDiscretizationValues(GenPDE::VariableUID av_uid, const std::vector<double>& values);
    CEVConstPtr evalCE(GenPDE::VariableUID uid) const;

protected:
    AVValuesMap m_avValuesMap;
};  

#endif // AVCONTEXT_H
