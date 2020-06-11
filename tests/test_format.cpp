#include <iostream>
#include <ostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include "../include/bbspirit.hpp"

using namespace std::string_literals;

namespace data = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(bbformat)

// --run_test=bbformat/textFormatterTest
BOOST_AUTO_TEST_CASE(textFormatterTest)
{
    using el = bbspirit::Element;

    bbspirit::Elements text = 
    {
        el{ "b"_openTag }, el{ "Hello World"s }, el { "c"_closeTag }
    };
}

BOOST_AUTO_TEST_SUITE_END() // bbformat