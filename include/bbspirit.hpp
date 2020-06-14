#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace bbspirit
{

struct OpenTag
{
    std::string id;
};

struct CloseTag
{
    std::string id;
};

} // namespace bbspirit

bbspirit::OpenTag operator"" _openTag(const char* str, std::size_t)
{
    return bbspirit::OpenTag { str };
}

bbspirit::CloseTag operator"" _closeTag(const char* str, std::size_t)
{
    return bbspirit::CloseTag { str };
}

BOOST_FUSION_ADAPT_STRUCT
(
    bbspirit::OpenTag, id
)

BOOST_FUSION_ADAPT_STRUCT
(
    bbspirit::CloseTag, id
)

namespace bbspirit
{

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

struct Element : x3::variant<OpenTag, CloseTag, std::string>
{
    using base_type::base_type;
    using base_type::operator=;
};

auto closeTag
    = x3::rule<struct closeTagID2, CloseTag, true> { "closeTag" }
    = x3::no_skip["[/" >> +x3::alnum >> ']'];

auto openTag
    = x3::rule<struct openTagID2, OpenTag, true> { "openTag" }
    = x3::no_skip['[' >> +x3::alnum >> ']'];

const auto contentParser
    = x3::rule<class ContentParserID, Element, true> { "contentParser" }
    = closeTag | openTag |  x3::no_skip[+(x3::char_ - (closeTag | openTag))];

using Elements = std::vector<Element>;
const auto elementsParser
    = x3::rule<class ContentParserID, Elements, true> { "elementsParser" }
    = contentParser >> *(contentParser);

} // namespace bbspirit