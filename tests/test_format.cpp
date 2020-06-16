#include <iostream>
#include <ostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include "../include/bbspirit.hpp"
#include "../include/TextFormatter.hpp"

using namespace std::string_literals;

namespace data = boost::unit_test::data;

namespace std
{

template <typename T>
inline std::ostream&
    operator<<(std::ostream& out, std::vector<T> const& item)
{
    out << '[';
    bool first = true;
    for (auto const& element : item) 
    {
        out << (!first ? "," : "") << element;
        first = false;
    }
    return out << ']';
}

inline std::ostream& 
    operator<<(std::ostream& out, const std::tuple<std::string, std::uint16_t>& info)
{
    const auto&[text, width] = info;
    out << text << ',' << width;
    return out;
}

}

BOOST_AUTO_TEST_SUITE(bbformat)

const std::tuple<std::string, std::size_t> findSeperatorSpaceTestData[] = 
{
    { "test ", 4 },
    { "test    ", 4 },
    { "test", std::string_view::npos },
    { " test", 0 },
    { "t st x", 4 },
    { "t st   xyz", 4 },
    { "    ", 0 },
    { "    x", 0 },
    { "x    x", 1 },
    { "ab cd ", 5}
};

// --run_test=bbformat/findSeperatorSpaceTest
BOOST_DATA_TEST_CASE(findSeperatorSpaceTest, data::make(findSeperatorSpaceTestData), str, offset)
{
    using namespace bbspirit;
    BOOST_TEST(find_seperator_space(str) == offset);
}

using TestInfo = std::tuple<std::string, std::uint16_t>;
const std::tuple<TestInfo, std::vector<std::string>> textSplitterTestData[] =
{
    {
        { "0123456789"s, 10 },
        { { "0123456789"s } }
    },
    {
        { "0123456789"s, 80 },
        { { "0123456789"s } }
    },
    {
        { "0123456789"s, 5 },
        { { "01234"s}, {"56789"s} }
    },
    {
        { "0123456789abc"s, 5 },
        { { "01234"s}, {"56789"s}, {"abc"s} }
    },
    {
        { "ab cd  efghi jklm"s, 5 },
        { { "ab cd"s}, {" efghi"s}, {"jklm8"s} }
    },
};

// --run_test=bbformat/textSplitterTest
BOOST_DATA_TEST_CASE(textSplitterTest, data::make(textSplitterTestData), data, expected)
{
    const auto& [original, maxwidth] = data;

    auto results = bbspirit::splitText(original, maxwidth);
    
    BOOST_TEST(results.size() == expected.size());

    for (const auto& expected_item : expected | boost::adaptors::indexed())
    {
        auto expected_str = expected_item.value();
        auto result_str = std::string{ results.at(expected_item.index()) };
        BOOST_TEST(expected_str == result_str);
    }
}

const std::tuple<std::string, std::string> textTestData[] = 
{
    { "[b]Hello World", "Hello World" },
    { "Hello [i]World[/i]!" , "Hello World!"},
    { 
        "[QUOTE]Hello[/QUOTE]World",
R"(> Hello
World)"
    },
    {
        "[QUOTE]Hello[/QUOTE]World[QUOTE]How are you?[/QUOTE]LOL",
R"(> Hello
World
> How are you?
LOL)"
    }

};

// --run_test=bbformat/textFormatterTest
BOOST_DATA_TEST_CASE(textFormatterTest, data::make(textTestData), original, expected)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::Elements elements{};

    bool result =
        phrase_parse(start, stop, bbspirit::elementsParser, x3::ascii::space, elements);

    bbspirit::TextFormatter formatter;

    std::stringstream ss;
    formatter.format(elements, ss);
    BOOST_TEST_MESSAGE(ss.str());
    BOOST_TEST(ss.str() == expected);
}

BOOST_AUTO_TEST_SUITE_END() // bbformat