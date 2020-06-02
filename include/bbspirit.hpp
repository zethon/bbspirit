#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace bbspirit
{

struct SimpleElement
{
    std::string tag;
    std::string content;
};

} // namespace bbspirit

BOOST_FUSION_ADAPT_STRUCT
(
    bbspirit::SimpleElement, tag, content
)

namespace bbspirit
{

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using x3::lit;
using x3::lexeme;
using ascii::char_;

const x3::rule<class SimpleElementID, SimpleElement> simpleTag = "simpleTag";

auto assignTag = [](auto& ctx)
{
    x3::_val(ctx).tag = x3::_attr(ctx);
};

auto testTag = [](auto& ctx)
{
    x3::_pass(ctx) = 
        (x3::_val(ctx).tag == x3::_attr(ctx));
};

auto assignContent = [](auto& ctx)
{
    x3::_val(ctx).content = x3::_attr(ctx);
};

auto const simpleTag_def
    = '['
    >> x3::lexeme[+(char_ - ']')][assignTag]
    >> ']'
    >> x3::lexeme[
        +(char_ - x3::lit("[/"))]
            [assignContent]
    >> "[/"
    >> x3::lexeme[+(char_ - ']')][testTag]
    >> ']'
    ;

BOOST_SPIRIT_DEFINE(simpleTag);

} // namespace bbspirit
