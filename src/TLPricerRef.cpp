#include "TLPricerRef.h"

TLPricerRef::TLPricerRef(PricerUid uid)
:mUid(uid)
{}

void TLPricerRef::print(std::ostream &stream) const
{
    stream << "<PricerRef uid=\"" << mUid << "\"/>";
}

std::ostream& operator<<(std::ostream& stream, const TLPricerRef& pricer)
{
    pricer.print(stream);
    return stream;
}
