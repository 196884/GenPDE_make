#include "TradeLeg.h"

std::ostream& operator<<(std::ostream& stream, const TradeLeg& trade_leg)
{
    trade_leg.print(stream);
    return stream;
}
