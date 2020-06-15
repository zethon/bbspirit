#include <iostream>
#include <ostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include "../include/bbspirit.hpp"

using namespace std::string_literals;

namespace data = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(bbspirit)

constexpr std::uint8_t OPENTAG = 0;
constexpr std::uint8_t CLOSETAG = 1;
constexpr std::uint8_t WHITESPACE = 2;
constexpr std::uint8_t STRING = 3;

const std::tuple<std::string, std::string, bool> openTagData[] =
{
    std::make_tuple("[b]", "b", true),
    std::make_tuple("[cat]", "cat", true),
    std::make_tuple("[cat ]", "", false),
};

// --run_test=bbspirit/openTagTest
BOOST_DATA_TEST_CASE(openTagTest, data::make(openTagData), original, expected, parsed)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::OpenTag openTag{};

    bool result =
        parse(start, stop, bbspirit::openTag, openTag);

    BOOST_REQUIRE_EQUAL(result, parsed);

    if (!parsed) return;
    
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(openTag.id, expected);
}

const std::tuple<std::string> openTagFailData[] =
{
    "[b ]",
    "[cat%$&]",
    "[ cat]",
};

// --run_test=bbspirit/openTagFailTest
BOOST_DATA_TEST_CASE(openTagFailTest, data::make(openTagFailData), original)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::OpenTag openTag{};

    bool result =
        parse(start, stop, bbspirit::openTag, openTag);

    BOOST_TEST(!result);
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
        parse(start, stop, bbspirit::closeTag, closeTag);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(closeTag.id, expected);
}

const std::tuple<std::string> closeTagFailData[] =
{
    "[/b ]",
    "[/cat%$&]",
    "[/ cat]",
};

// --run_test=bbspirit/closeTagFailTest
BOOST_DATA_TEST_CASE(closeTagFailTest, data::make(closeTagFailData), original)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::OpenTag openTag{};

    bool result =
        parse(start, stop, bbspirit::openTag, openTag);

    BOOST_TEST(!result);
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

    std::string operator()(const bbspirit::Whitespace& ws) const
    {
        switch (ws)
        {
            case Whitespace::NEWLINE:
                return "\n"s;
            case Whitespace::TAB:
                return "\t"s;
            default:
            break;
        }

        return std::string{};
    }
};

// 0 - tag
// 1 - which type (0 - OpenTag, 1 - CloseTag, 2 - string)
const std::tuple<std::string, std::uint32_t, std::string> contentData[] =
{
    std::make_tuple("[cat]", OPENTAG, "cat"),
    std::make_tuple("[/b]", CLOSETAG, "b"),
    std::make_tuple("cat", STRING, "cat"),
    std::make_tuple(" cat", STRING, " cat"),
    std::make_tuple("cat ", STRING, "cat "),
    std::make_tuple(" cat ", STRING, " cat "),
};

// --run_test=bbspirit/contentParserTest
BOOST_DATA_TEST_CASE(contentParserTest, data::make(contentData), original, whichType, innerString)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::Element element{};

    bool result =
        parse(start, stop, bbspirit::contentParser, element);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(element.get().which(), whichType);
    const auto temp = boost::apply_visitor(tag_visitor(), element.get()); 
    BOOST_REQUIRE_EQUAL(temp, innerString);
}

using InfoTuple = std::tuple<std::string, std::uint32_t>;
using InfoVector = std::vector<InfoTuple>;
const std::tuple<std::string, InfoVector> elementsData[] =
{
    std::tuple<std::string, InfoVector>
    { 
        "[cat]", 
        { { "cat", OPENTAG } }
    },
    std::tuple<std::string, InfoVector>
    {   
        "[/b]", 
        { { "b", CLOSETAG } }
    },
    std::tuple<std::string, InfoVector>
    {
        "cat", 
        { { "cat", STRING } }
    },
    std::tuple<std::string, InfoVector>
    {
        "[b]hello world[/b]", 
        { { "b", OPENTAG }, { "hello world", STRING }, { "b", CLOSETAG } }
    },
    std::tuple<std::string, InfoVector>{
        "foo [b]hello world[/b] bar", 
        { { "foo ", STRING }, { "b", OPENTAG }, { "hello world", STRING }, { "b", CLOSETAG }, { " bar", STRING } }
    },
    std::tuple<std::string, InfoVector>
    {
        "foo[ [b]hello wo[/rld[/b] bar", 
        { { "foo[ ", STRING }, { "b", OPENTAG }, { "hello wo[/rld", STRING }, { "b", CLOSETAG }, { " bar", STRING } }
    },
    std::tuple<std::string, InfoVector>
    {
        " dogs \t cats ",
        { 
            {" dogs ", STRING},
            {"\t", WHITESPACE},
            {" cats ", STRING},
        }
    },
    std::tuple<std::string, InfoVector>
    {
        "Here is a list:\n\tdogs\n\tcats ",
        {
            {"Here is a list:", STRING},
            {"\n", WHITESPACE},
            {"\t", WHITESPACE},
            {"dogs", STRING},
            {"\n", WHITESPACE},
            {"\t", WHITESPACE},
            {"cats ", STRING},
        }
    },
    std::tuple<std::string, InfoVector>
    {
        "[b]Here is a list:[/b]\n[li]dogs[li]cats ",
        {
            {"b", OPENTAG},
            {"Here is a list:", STRING},
            {"b", CLOSETAG },
            {"\n", WHITESPACE},
            {"li", OPENTAG},
            {"dogs", STRING},
            {"li", OPENTAG},
            {"cats ", STRING},
        }
    },
};

BOOST_AUTO_TEST_SUITE_END() // bbspirit

namespace std
{
    std::ostream& operator<<(std::ostream& out, const bbspirit::InfoVector& indexPairs)
    {
        out << "{ ";
        for (const auto& [tag, type] : indexPairs)
        {
            out << "{" << tag << "," << type << "}";
        }
        out << " }";
        return out;
    }
}

BOOST_AUTO_TEST_SUITE(bbspirit)

// --run_test=bbspirit/elementsParserTest
BOOST_DATA_TEST_CASE(elementsParserTest, data::make(elementsData), original, infoVec)
{
    std::string::const_iterator start = std::begin(original);
    const std::string::const_iterator stop = std::end(original);

    bbspirit::Elements elements{};

    bool result =
        parse(start, stop, bbspirit::elementsParser, elements);

    BOOST_REQUIRE(result);

    auto x = std::string{ start, stop };
    BOOST_REQUIRE(start == stop);
    BOOST_REQUIRE_EQUAL(elements.size(), infoVec.size());

    for (const auto& datum : infoVec | boost::adaptors::indexed())
    {
        const auto& [extag, extype] = datum.value();
        const auto& parsedVar = elements.at(datum.index());
            
        BOOST_REQUIRE_EQUAL(elements.at(datum.index()).get().which(), extype);
        const auto temp = boost::apply_visitor(tag_visitor(), elements.at(datum.index()).get()); 
        BOOST_REQUIRE_EQUAL(temp, extag);
    }
}

BOOST_AUTO_TEST_SUITE_END() // bbspirit
