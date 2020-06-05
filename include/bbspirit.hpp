#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace bbspirit
{

struct SimpleElement
{
    std::string tag;
    std::string content;
};

struct OpenTag
{
    std::string id;
};

struct CloseTag
{
    std::string id;
};

} // namespace bbspirit

BOOST_FUSION_ADAPT_STRUCT
(
    bbspirit::SimpleElement, tag, content
)

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

auto closeTag2
    = x3::rule<struct closeTagID2, CloseTag, true> {"closeTag2"}
    = ("[/" >> x3::lexeme[+(x3::char_ - ']')] >> ']');

auto openTag2
    = x3::rule<struct openTagID2, OpenTag, true> { "openTag2" }
    = ('[' >> x3::lexeme[+(x3::char_ - ']')] >> ']');

const auto contentParser
    = x3::rule<class ContentParserID, Element, true> {"contentParser"}
    = closeTag2 | openTag2 | x3::lexeme[+(x3::char_)];

} // namespace bbspirit

namespace bbspirit
{

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using x3::lit;
using x3::lexeme;
using ascii::char_;

struct TagName{};
auto openTag
    = x3::rule<struct openTagID, std::string, true> { "openTag" }
    = ('[' >> x3::lexeme[+(char_ - ']')] >> ']')
        [([](auto& ctx) { x3::get<TagName>(ctx) = _attr(ctx); })];

auto closeTag
    = x3::rule<struct closeTagID, std::string, true> {"closeTag"}
    = ("[/" >> x3::lexeme[+(char_ - ']')] >> ']')
        [([](auto& ctx) { _pass(ctx) = (x3::get<TagName>(ctx) == _attr(ctx)); })];

auto tagContents
    = x3::rule<struct openTagID, std::string> {"tagContents"}
    = x3::lexeme[ +(char_ - closeTag) ];

auto const simpleTag
    = x3::rule<class SimpleElementID, SimpleElement, true> {"simpleTag"}
    = x3::with<TagName>(std::string())
    [
        openTag
        >> tagContents
        >> x3::omit [ closeTag ]
    ];

} // namespace bbspirit
