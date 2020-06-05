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
    std::make_tuple("[url]Hello Wo[/rld[/url]", "url", "Hello Wo[/rld"),
};

// --run_test=bbspirit/simpleTagTest
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

const std::tuple<std::string, std::string> openTagData[] =
{
    std::make_tuple("[b]", "b"),
    std::make_tuple("[cat]", "cat"),
};

// --run_test=bbspirit/openTagTest
BOOST_DATA_TEST_CASE(openTagTest, data::make(openTagData), original, expected)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::OpenTag openTag{};

    bool result =
        phrase_parse(start, stop, bbspirit::openTag2, x3::ascii::space, openTag);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(openTag.id, expected);
}

const std::tuple<std::string, std::string> closeTagData[] =
{
    std::make_tuple("[/b]", "b"),
    std::make_tuple("[/cat]", "cat"),
};

// --run_test=bbspirit/closeTagTest
BOOST_DATA_TEST_CASE(closeTagTest, data::make(closeTagData), original, expected)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::CloseTag closeTag{};

    bool result =
        phrase_parse(start, stop, bbspirit::closeTag2, x3::ascii::space, closeTag);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(closeTag.id, expected);
}

struct tag_visitor
    : public boost::static_visitor<std::string>
{
    std::string operator()(const std::string& value) const
    {
        return value;
    }

    std::string operator()(const bbspirit::OpenTag& value) const
    {
        return value.id;
    }

    std::string operator()(const bbspirit::CloseTag& value) const
    {
        return value.id;
    }
};

// 0 - tag
// 1 - which type (0 - OpenTag, 1 - CloseTag, 2 - string)
const std::tuple<std::string, std::uint32_t, std::string> contentData[] =
{
    std::make_tuple("[cat]", 0, "cat"),
    std::make_tuple("[/b]", 1, "b"),
    std::make_tuple("cat", 2, "cat"),
};

// --run_test=bbspirit/contentParserTest
BOOST_DATA_TEST_CASE(contentParserTest, data::make(contentData), original, whichType, innerString)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::Element element{};

    bool result =
        phrase_parse(start, stop, bbspirit::contentParser, x3::ascii::space, element);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(element.get().which(), whichType);
    const auto temp = boost::apply_visitor(tag_visitor(), element.get()); 
    BOOST_REQUIRE_EQUAL(temp, innerString);
}

BOOST_AUTO_TEST_SUITE_END() // arccutils
