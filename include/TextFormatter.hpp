#pragma once

#include <boost/algorithm/string.hpp>

#include "bbspirit.hpp"

namespace bbspirit
{

    struct text_printer;


class TextFormatter
{

friend text_printer;

std::uint16_t   _maxQuoteLevel = 1;
std::uint16_t   _maxWidth = 80;

std::string     _quoteStr = "> ";
std::string     _newline = "\n";


public:

void format(const Elements& elements, std::ostream& out)
{
    text_printer visitor{ *this, out };

    for (const auto& element : elements)
    {
        boost::apply_visitor(visitor, element);
    }
};

struct text_printer
{
    TextFormatter&  tf_;

    std::ostream& out_;
    std::uint16_t indent_ = 0;

    text_printer(TextFormatter& tf, std::ostream& out)
        : tf_{ tf },
        out_{ out }
    {}

    void operator()(const std::string& text)
    {
        for (auto i = 0u; i < indent_; i++)
        {
            out_ << tf_._quoteStr;
        }

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

    void operator()(const Whitespace& ws)
    {
        switch (ws)
        {
            default:
            break;

            case Whitespace::NEWLINE:
                out_ << '\n';
            break;

            case Whitespace::TAB:
                out_ << '\t';
            break;
        }
    }
};

};

} // namespace bbspirit