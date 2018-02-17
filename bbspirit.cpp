#include <iostream>
#include <string>
#include <map>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

namespace bbcpp
{

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using ElementWrapper = boost::recursive_wrapper<SimpleElement>;
using NodeVariant = boost::variant<ElementWrapper, std::string>;

struct SimpleElement
{
    std::string name;
    // std::string content;
    std::vector<ElementWrapper> children;
};

// struct ValueElement
// {
//     std::string name;
//     std::string value;
//     std::string content;
//     char quoteChar;
// };

} // bbcpp namespace

BOOST_FUSION_ADAPT_STRUCT
(
    bbcpp::SimpleElement,
    (std::string, name)
    (std::string, content)
);

// BOOST_FUSION_ADAPT_STRUCT
// (
//     bbcpp::ValueElement,
//     (std::string, name)
//     (std::string, value)
//     (std::string, content)
//     (char, quoteChar)
// );

namespace bbcpp
{

template<typename Iterator>
struct SimpleElementParser
    : qi::grammar<Iterator, SimpleElement()> 
{
    SimpleElementParser()
        : SimpleElementParser::base_type(element)
    {
        using qi::lit;
        using qi::lexeme;
        using ascii::char_;
        using ascii::string;
        using namespace qi::labels;

        using phoenix::at_c;
        using phoenix::push_back;

        end_tag = lit("[/") 
            >> string(_r1)
            >> ']';

        start_tag %= '['
            >> lexeme[+(char_ - ']')]
            >> ']';

        text = lexeme[+(char_ -(lit("[/") >> string(_r1) >> lit("]")))];
        // text = end_tag(_r1);

        element 
            %= start_tag[at_c<0>(_val) = _1]

            // accept everything except the expected closing tag
            //>> lexeme[+(char_ -(lit("[/") >> string(at_c<0>(_val)) >> lit("]")))]
            >> text(at_c<0>(_val)) // lexeme[+(char_ -(lit("[/") >> string(at_c<0>(_val)) >> lit("]")))]

            >> end_tag(at_c<0>(_val));
    }


    qi::rule<Iterator, std::string(std::string)> text;
    qi::rule<Iterator, std::string()> start_tag;
    qi::rule<Iterator, void(std::string)> end_tag;

    qi::rule<Iterator, SimpleElement()> element;
};

} // bbcpp namespace

int main()
{
    using namespace bbcpp;

    using range_t = std::string::const_iterator;

    const std::string test1 = "[B]This [I]is[/I] bold![/B]";

    const SimpleElementParser<range_t> parser;
    SimpleElement element;

    std::string::const_iterator begin = test1.begin();
    std::string::const_iterator end = test1.end();

    if (boost::spirit::qi::parse(begin, end, parser, element))
    {
        std::cout << "Parsed!\n";
        std::cout << "tagname : " << element.name << std::endl;
        std::cout << "tagtext : " << element.content << std::endl;        
    }
    else
    {
        std::cout << "No parsed\n";
    }


    // const std::string test1 = R"([QUOTE="Amaury, post: 790261, member: 48064"]Welcome to XenForo Community![/QUOTE])";
    // // const std::string test1 = R"([QUOTE=Max Power;12345])";

    // std::string::const_iterator begin = test1.begin();
    // std::string::const_iterator end = test1.end();

    // QuoteNode qn;
    // const QuoteParser<std::string::const_iterator> parser;

    // if (boost::spirit::qi::parse(begin, end, parser, qn))
    // {
    //     std::cout << "Parsed!\n";
    //     std::cout << "username : " << qn._author << std::endl;
    //     std::cout << "postid   : " << qn._postid << std::endl;
    // }
    // else
    // {
    //     std::cout << "No parsed\n";
    // }

    //QuoteNode qn;
    //qn._author = "Max Power";
    //qn._postid = "1234565";
    //std::cout << "username: " << qn._author << " postid: " << qn._postid << std::endl;

    return 0;
}