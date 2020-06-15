#pragma once

#include <boost/algorithm/string.hpp>

#include "bbspirit.hpp"

namespace bbspirit
{

struct text_printer;

std::size_t find_seperator_space(std::string_view original)
{
    std::size_t retval = std::string_view::npos; 
    bool done = false;

    std::size_t x = original.size();

    while (x > 0)
    {
        x--;

        if (original.at(x) != ' ')
        {
            if (x < (original.size() - 1))
            {
                if (original.at(x + 1) == ' ')
                {
                    return x + 1;
                }
            }
        }
    }

    if (original.at(x) == ' ') return 0;
    
    return std::string_view::npos;
}

std::vector<std::string_view> splitText(std::string_view original, std::uint16_t maxWidth)
{
    std::vector<std::string_view> retval;

    if (original.size() > maxWidth)
    {
        std::string_view worker{ original };

        //do
        //{ 
        //    std::string_view temp{ original.data(), maxWidth };

        //    auto x = temp.size() - 1;
        //    while (temp.at(x) !=)

        //} 
        //while (worker.size() > 0);
        
        
    }
    else
    {
        retval.push_back(original);
    }

    return retval;
}

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
            out_ << tf_._newline;
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