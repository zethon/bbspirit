#include <iostream>
#include <string>
#include <map>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp> 
#include <boost/spirit/include/qi_raw.hpp>

#include <boost/variant/recursive_variant.hpp>

struct SimpleElement
{
    std::string name;
};

BOOST_FUSION_ADAPT_STRUCT
(
    SimpleElement,
    (std::string, name)
);

struct ValueElement
{
    std::string name;
    std::string value;
    char quoteChar;
};

BOOST_FUSION_ADAPT_STRUCT
(
    ValueElement,
    (std::string, name)
    (std::string, value)
    (char, quoteChar)
);

template<typename Iterator>
struct BBCodeParser
{

};

//    struct mini_xml;

// using mini_xml_node = boost::variant<boost::recursive_wrapper<mini_xml>, std::string>;

// struct mini_xml
// {
//     std::string name;                           // tag name
//     std::vector<mini_xml_node> children;        // children
// };


// struct QuoteNode
// {
//     using Attributes = std::multimap<std::string, std::string>;

//     std::string     _author;
//     std::string     _postid;
//     Attributes      _attributes;
// };

// BOOST_FUSION_ADAPT_STRUCT
// (
//     QuoteNode,
//     (std::string, _author)
//     (std::string, _postid)
// );

// template<typename Iterator>
// struct QuoteParser
//     : boost::spirit::qi::grammar<Iterator, QuoteNode()>
// {
//     QuoteParser()
//         : QuoteParser::base_type(query)
//     {
//         namespace bsq = boost::spirit::qi;

//         vbquote = bsq::lit("[QUOTE=") >> +(bsq::char_ - ';') >> ';' >> +(bsq::char_ - ']') >> ']';
//         xfquote = bsq::lit(R"([QUOTE=")") >> +(bsq::char_ - ',') >> ',' >> +(bsq::char_ - bsq::lit(R"("])")) >> bsq::lit(R"("])");

//         query = xfquote | vbquote;
//     }

//     boost::spirit::qi::rule<Iterator, QuoteNode()> vbquote;
//     boost::spirit::qi::rule<Iterator, QuoteNode()> xfquote;

//     boost::spirit::qi::rule<Iterator, QuoteNode()> query;
// };

int main()
{
    using range_t = std::string::const_iterator;

    const std::string test1 = R"([QUOTE="Amaury, post: 790261, member: 48064"]Welcome to XenForo Community![/QUOTE])";
    // const std::string test1 = R"([QUOTE=Max Power;12345])";

    std::string::const_iterator begin = test1.begin();
    std::string::const_iterator end = test1.end();

    QuoteNode qn;
    const QuoteParser<std::string::const_iterator> parser;

    if (boost::spirit::qi::parse(begin, end, parser, qn))
    {
        std::cout << "Parsed!\n";
        std::cout << "username : " << qn._author << std::endl;
        std::cout << "postid   : " << qn._postid << std::endl;
    }
    else
    {
        std::cout << "No parsed\n";
    }

    //QuoteNode qn;
    //qn._author = "Max Power";
    //qn._postid = "1234565";
    //std::cout << "username: " << qn._author << " postid: " << qn._postid << std::endl;

    return 0;
}