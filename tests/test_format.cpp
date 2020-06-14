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

BOOST_AUTO_TEST_SUITE(bbformat)

const std::tuple<std::string, std::string> textTestData[] = 
{
    { "[b]Hello World", "Hello World" },
    { "Hello [i]World[/i]!" , "Hello World!"},
    { 
        "[QUOTE]Hello[/QUOTE]World",
R"(> Hello
World)"
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
    BOOST_TEST(ss.str() == expected);
}

BOOST_AUTO_TEST_SUITE_END() // bbformat