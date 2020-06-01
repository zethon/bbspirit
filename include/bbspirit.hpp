#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/object.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace x3 = boost::spirit::x3;

namespace bbspirit
{

struct SimpleElement
{
//    int age;
    std::string tag;
    std::string content;
//    double salary;

//    std::string name;
//    std::string content;

    friend std::ostream& operator<<(std::ostream& os, const SimpleElement& el)
    {
        os << '[' << el.tag << ']' << el.content << "[/" << el.tag << ']';
        return os;
    }
};

} // namespace bbspirit

//BOOST_FUSION_ADAPT_STRUCT
//(
//    bbspirit::SimpleElement, name, content
//)

BOOST_FUSION_ADAPT_STRUCT
(
    bbspirit::SimpleElement,
        /*age,*/ tag, content/*, salary*/
)

namespace bbspirit
{

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;


using x3::int_;
using x3::lit;
using x3::double_;
using x3::lexeme;
using ascii::char_;
using ascii::string;

const x3::rule<class SimpleElementID, SimpleElement> employee = "employee";

auto const quoted_string = lexeme['"' >> +(char_ - '"') >> '"'];

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

auto const employee_def
    = '['
    >> x3::lexeme[+(char_ - ']')][assignTag]
    >> ']'
    >> x3::lexeme[+(char_ - x3::lit("[/"))][assignContent]
    >> "[/"
    >> x3::lexeme[+(char_ - ']')][testTag]
    >> ']'
    ;

BOOST_SPIRIT_DEFINE(employee);

} // namespace bbspirit
