#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace bbspirit
{

namespace x3 = boost::spirit::x3;

struct node;

using node_value_base = x3::variant <
    std::string,
    x3::forward_ast<node>
>;
    
struct node_value : node_value_base
{
    using base_type::base_type;
    using base_type::operator=;
};

using node_vector = std::vector<node_value>;

struct node
{
    node_vector children;
};

} // namespace bbspirit

BOOST_FUSION_ADAPT_STRUCT
(
    bbspirit::node, children
)


