#include <boost/test/unit_test.hpp>

#include "../include/bbspirit.hpp"

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(misc)

BOOST_AUTO_TEST_CASE(CommandHistory)
{
    std::string test = "[b]Hello World[/b]"s;

    std::string::const_iterator start = test.begin();
    const std::string::const_iterator stop = test.end();

    bbspirit::SimpleElement element{};

    bool result =
        phrase_parse(start, stop, bbspirit::employee, x3::ascii::space, element);

}

BOOST_AUTO_TEST_SUITE_END() // arccutils
