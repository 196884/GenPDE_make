#include "PricingInstruction.h"

std::ostream& operator<<(std::ostream& stream, const PricingInstruction& pi)
{
    pi.print(stream);
    return stream;
}
