#include "PayoutExpression.h"

std::ostream& operator<<(std::ostream& stream, const PayoutExpression& pe)
{
    pe.print(stream);
    return stream;
}
