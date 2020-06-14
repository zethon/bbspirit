#pragma once

#include <boost/algorithm/string.hpp>

#include "bbspirit.hpp"

namespace bbspirit
{

class TextFormatter
{

std::int8_t     _quoteLevel = 0;
std::string     _quoter = "> ";

struct text_printer
{
    std::ostream& out_;
    std::uint16_t indent_ = 0;

    text_printer(std::ostream& out)
        : out_{ out }
    {}

    void operator()(const std::string& text)
    {
        for (auto i = 0u; i < indent_; i++)
        {
            out_ << "> ";
        }

        if (indent_ > 0) out_ << ' ';

        out_ << text;
    }

    void operator()(const OpenTag& tag)
    {
        if (boost::iequals(tag.id, "QUOTE"))
        {
            indent_++;
        }
    }

    void operator()(const CloseTag& tag)
    {
        if (boost::iequals(tag.id, "QUOTE"))
        {
            indent_--;
        }
    }
};

public:

void setQuoteLevel(std::uint8_t level) { _quoteLevel = level; }
std::uint8_t quoteLevel() const { return _quoteLevel; }

void format(const Elements& elements, std::ostream& out)
{
    text_printer visitor{ out };

    for (const auto& element : elements)
    {
        boost::apply_visitor(visitor, element);
    }
}

};

} // namespace bbspirit