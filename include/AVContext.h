#ifndef AVCONTEXT_H
#define AVCONTEXT_H

#include <vector>
#include <map>

#include "Framework.h"
#include "AuxiliaryVariable.h"

class CEValues;

class AVContext
{
public:
    typedef boost::shared_ptr<const CEValues>            CEVConstPtr;
    typedef std::map< GenPDE::VariableUID, CEVConstPtr > AVValuesMap;

public:
    AVContext();
    ~AVContext();
    void setAVDiscretizationValues( GenPDE::VariableUID av_uid, const std::vector<double>& values );
    void setAVDiscretizationValues( GenPDE::VariableUID av_uid, const CEVConstPtr&         values );
    CEVConstPtr evalCE(GenPDE::VariableUID uid) const;
    void clear();

protected:
    AVValuesMap m_avValuesMap;
};  

#endif // AVCONTEXT_H
