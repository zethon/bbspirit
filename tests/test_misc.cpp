#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "../include/bbspirit.hpp"

using namespace std::string_literals;

namespace data = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(bbspirit)

const std::tuple<std::string, std::string, std::string> simpleTagData[] =
{
    std::make_tuple("[i]Hello World[/i]", "i", "Hello World"),
    std::make_tuple("[url]Hello World[/url]", "url", "Hello World"),
    std::make_tuple("[url]Hello Wo[rld[/url]", "url", "Hello Wo[rld"),
    std::make_tuple("[url]Hello Wo[/rld[/url]", "url", "Hello Wo[rld"),
};

// --run_test=bbspirit/simpleTag
BOOST_DATA_TEST_CASE(simpleTagTest, data::make(simpleTagData), original, tag, text)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::SimpleElement element{};

    bool result =
        phrase_parse(start, stop, bbspirit::simpleTag, x3::ascii::space, element);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(element.tag, tag);
    BOOST_REQUIRE_EQUAL(element.content, text);
}

BOOST_AUTO_TEST_SUITE_END() // arccutils
