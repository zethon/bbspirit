#include <iostream>

#include <boost/test/unit_test.hpp>

#include "../include/bbspirit.hpp"

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(bbspirit)

// --run_test=bbspirit/simpleTag
BOOST_AUTO_TEST_CASE(simpleTag)
{
    std::string test = "[be]Hello World[/be]"s;

    std::string::const_iterator start = test.begin();
    const std::string::const_iterator stop = test.end();

    bbspirit::SimpleElement element{};

    bool result =
        phrase_parse(start, stop, bbspirit::employee, x3::ascii::space, element);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(element.tag, "b");
    BOOST_REQUIRE_EQUAL(element.content, "Hello World");
}

BOOST_AUTO_TEST_SUITE_END() // arccutils
