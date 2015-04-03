#ifndef FIXINGS_PARSER_BASE_H
#define FIXINGS_PARSER_BASE_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "Framework.h"

#include "GpDate.h"

#include "TradeFixings.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace qi  = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
class FixingsParserBase
{
protected:
};

#endif // FIXINGS_PARSER_BASE_H
