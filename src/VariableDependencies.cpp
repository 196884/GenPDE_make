#include "VariableDependencies.h"

std::ostream& operator<<(std::ostream& stream, const VarDependencies& deps)
{
    stream << "Deps[";
    bool first = true;
    for(size_t i=0; i<deps.mNbSVDeps; ++i)
    {
        if( !first )
            stream << ", ";
        first = false;
        stream << "SV_" << deps.mSVDeps[2*i] << "(" << deps.mSVDeps[2*i+1] << ")";
    }
    for(size_t i=0; i<deps.mNbAVDeps; ++i)
    {
        if( !first )
            stream << ", ";
        first = false;
        stream << "AV_" << deps.mAVDeps[2*i] << "(" << deps.mAVDeps[2*i+1] << ")";
    }
    stream << "]";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const VarMemoryLayout& layout)
{
    stream << ((layout.mIsInnerAV) ? 1 : 0) << std::endl;
    stream << layout.mNbAVDeps << std::endl;
    for(size_t i=0; i<layout.mNbAVDeps; ++i)
        stream << layout.mAVDeps[2*i] << std::endl << layout.mAVDeps[2*i+1] << std::endl;
    stream << layout.mNbSVDeps << std::endl;
    for(size_t i=0; i<layout.mNbSVDeps; ++i)
        stream << layout.mSVDeps[2*i] << std::endl << layout.mSVDeps[2*i+1] << std::endl;
    return stream;
}
