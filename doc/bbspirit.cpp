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

// using ElementWrapper = boost::recursive_wrapper<SimpleElement>;
// using NodeVariant = boost::variant<ElementWrapper, std::string>;

struct SimpleElement
{
    std::string name;
    // std::string content;
    // std::vector<NodeVariant> children;

    friend std::ostream& operator<<(std::ostream& os, const SimpleElement& el)
    {
        os << "SimpleElement[name='" << el.name << "']";
        return os;
    }
};

struct ValueElement
{
    std::string name;
    std::string value;
    char quoteChar;

    friend std::ostream& operator<<(std::ostream& os, const ValueElement& el)
    {
        os << "ValueElement[name='" << el.name << "', value='" << el.value << "', quoteChar='" << el.quoteChar << "']";
        return os;
    }
};

using NodeVariant = boost::variant<std::string, SimpleElement, ValueElement>;
typedef boost::make_recursive_variant<NodeVariant, std::vector<boost::recursive_variant_> >::type var1_t;

} // bbcpp namespace

struct printer : boost::static_visitor<std::string>
{
    template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
    std::string operator()(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& var) const
    {
        return boost::apply_visitor(printer(), var);
    }

    template <typename T>
    std::string operator()(const std::vector<T>& vec) const
    {
        std::ostringstream ost;
        ost << "( ";
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            ost << printer()(*it);
        }
        ost << ") ";

        return ost.str();
    }
   
    template <typename T>
    std::string operator()(const T& operand) const
    {
        std::ostringstream ost;
        ost << operand << ' ';
        return ost.str();
    }

    template <int...> struct indices {};
    template <typename... Ts, int... Is>
    std::string operator()(const std::tuple<Ts...>& tup, indices<Is...>)
    {
        std::ostringstream ost;
        ost << "( ";
        (void) (int []){0, (ost << printer()( std::get<Is>(tup) ), 0)... };
        ost << ") ";
        return ost.str();
    }

    template <int N, int... Is>
        struct make_indices : make_indices<N-1, N-1, Is...> {};
    template <int... Is>
        struct make_indices<0, Is...> : indices<Is...> {};
    template <typename... Ts>
    std::string operator()(const std::tuple<Ts...>& tup) const
    {
        return printer()(tup, make_indices<sizeof...(Ts)>());
    }    
};

int main()
{
    using namespace bbcpp;

    std::vector<var1_t> root;
    
    var1_t b_text = std::string("This is bold text!");

    var1_t b_el = SimpleElement{"B"};
    b_el.push_back(b_text);

    root.push_back(b_el);



    // std::vector<var1_t> vec1;
    // vec1.push_back(SimpleElement{"B"});
    // vec1.push_back(ValueElement{"B", "name", '"'});
    // vec1.push_back(SimpleElement{"I"});
    // vec1.push_back(vec1);

    var1_t var1(root);
    std::string result1(printer()(var1));
    std::cout << "result1: " << result1 << '\n';    


    std::cout << "hi!\n";
}

// BOOST_FUSION_ADAPT_STRUCT
// (
//     bbcpp::SimpleElement,
//     (std::string, name)
//     (std::string, content)
// );

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

// template<typename Iterator>
// struct SimpleElementParser
//     : qi::grammar<Iterator, SimpleElement()> 
// {
//     SimpleElementParser()
//         : SimpleElementParser::base_type(element)
//     {
//         using qi::lit;
//         using qi::lexeme;
//         using ascii::char_;
//         using ascii::string;
//         using namespace qi::labels;

//         using phoenix::at_c;
//         using phoenix::push_back;

//         end_tag = lit("[/") 
//             >> string(_r1)
//             >> ']';

//         start_tag %= '['
//             >> lexeme[+(char_ - ']')]
//             >> ']';

//         text = lexeme[+(char_ -(lit("[/") >> string(_r1) >> lit("]")))];
//         // text = end_tag(_r1);

//         element 
//             %= start_tag[at_c<0>(_val) = _1]

//             // accept everything except the expected closing tag
//             //>> lexeme[+(char_ -(lit("[/") >> string(at_c<0>(_val)) >> lit("]")))]
//             >> text(at_c<0>(_val)) // lexeme[+(char_ -(lit("[/") >> string(at_c<0>(_val)) >> lit("]")))]

//             >> end_tag(at_c<0>(_val));
//     }


//     qi::rule<Iterator, std::string(std::string)> text;
//     qi::rule<Iterator, std::string()> start_tag;
//     qi::rule<Iterator, void(std::string)> end_tag;

//     qi::rule<Iterator, SimpleElement()> element;
// };

} // bbcpp namespace



// int main()
// {
    // using namespace bbcpp;

    // using range_t = std::string::const_iterator;

    // const std::string test1 = "[B]This [I]is[/I] bold![/B]";

    // const SimpleElementParser<range_t> parser;
    // SimpleElement element;

    // std::string::const_iterator begin = test1.begin();
    // std::string::const_iterator end = test1.end();

    // if (boost::spirit::qi::parse(begin, end, parser, element))
    // {
    //     std::cout << "Parsed!\n";
    //     // std::cout << "tagname : " << element.name << std::endl;
    //     // std::cout << "tagtext : " << element.content << std::endl;        
    // }
    // else
    // {
    //     std::cout << "No parsed\n";
    // }


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

//     return 0;
// }