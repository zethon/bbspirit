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

    auto db = [](auto& ctx)
    {
        std::cout << "db: " << x3::_attr(ctx) << '\n';
    };

//    auto db2 = [](auto& ctx)
//    {
//        std::cout << "db2: " << x3::_attr(ctx) << '\n';
//    };

    auto assignTag = [](auto& ctx)
    {
        std::cout << "->: " << x3::_attr(ctx) << '\n';
        x3::_val(ctx).tag = x3::_attr(ctx);
    };

    auto testTag = [](auto& ctx)
    {
        auto& element = x3::_val(ctx);
////        element.tag = x3::_attr(ctx);
//        element.content = x3::_attr(ctx);

        std::cout << "a: " << element.tag << '\n';
        std::cout << "b: " << x3::_attr(ctx) << '\n';
        std::cout << "hi";

//        x3::_pass(ctx) = (element.tag == x3::_attr(ctx));

//        if (x3::_val(ctx).tag = x3::_attr(ctx))
//        {
//            std::cout << "hi"
//        }
//        std::string temp = x3::_attr(ctx);
//        x3::_pass(ctx) = x3::_attr(ctx)
//                == x3::_val(ctx).tag;
    };

    auto const employee_def
        = '['
        >> +(char_ - ']')[assignTag]
        >> ']'
        >> +(char_ - x3::lit("[/"))[db]
        >> x3::lit("[/")
        >> +(char_ - ']')[testTag]
        >> ']'
        ;

    BOOST_SPIRIT_DEFINE(employee);

//struct SimpleElementID{};
//const x3::rule<struct SimpleElementID, SimpleElement> simpleTag = "simpleTag";

//const auto simpleTag_def =
//        x3::lexeme['[' >> +(x3::char_ - ']') >> ']' >> +(x3::char_)];

//BOOST_SPIRIT_DEFINE(simpleTag);

} // namespace bbspirit


//namespace bbspirit
//{

//namespace x3 = boost::spirit::x3;

//struct node;

//using node_value_base = x3::variant <
//    std::string,
//    x3::forward_ast<node>
//>;
    
//struct node_value : node_value_base
//{
//    using base_type::base_type;
//    using base_type::operator=;
//};

//using node_vector = std::vector<node_value>;

//struct node
//{
//    node_vector children;
//};

//} // namespace bbspirit

//BOOST_FUSION_ADAPT_STRUCT
//(
//    bbspirit::node, children
//)


