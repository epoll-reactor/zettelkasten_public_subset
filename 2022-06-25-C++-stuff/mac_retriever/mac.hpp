#ifndef MAC_HPP
#define MAC_HPP

#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_lit.hpp>

#include <boost/spirit/include/phoenix_operator.hpp>

#define BOOST_SPIRIT_USE_PHOENIX_V3 1

using v_mac_t = std::vector<uint8_t>;

template <typename _Iterator>
struct mac_parser : public boost::spirit::qi::grammar<_Iterator, v_mac_t()> {
  mac_parser() : mac_parser::base_type(rule_bytes) {
    using namespace boost::spirit;
    rule_hex_digit = qi::char_("0-9")[qi::_val = qi::_1 - '0'];
    rule_hex_lc    = qi::char_("a-f")[qi::_val = qi::_1 - 'a' + 10];
    rule_hex_uc    = qi::char_("A-F")[qi::_val = qi::_1 - 'A' + 10];
    rule_byte      = (rule_hex >> rule_hex)[qi::_val = (qi::_1 << 4) + qi::_2];
    rule_hex       = rule_hex_digit | rule_hex_lc | rule_hex_uc;
    rule_bytes =
         rule_byte >> qi::lit(':')
      >> rule_byte >> qi::lit(':')
      >> rule_byte >> qi::lit(':')
      >> rule_byte >> qi::lit(':')
      >> rule_byte >> qi::lit(':')
      >> rule_byte;
  }

  boost::spirit::qi::rule<_Iterator, uint8_t()> rule_hex_digit;
  boost::spirit::qi::rule<_Iterator, uint8_t()> rule_hex_lc;
  boost::spirit::qi::rule<_Iterator, uint8_t()> rule_hex_uc;
  boost::spirit::qi::rule<_Iterator, uint8_t()> rule_hex;
  boost::spirit::qi::rule<_Iterator, uint8_t()> rule_byte;
  boost::spirit::qi::rule<_Iterator, v_mac_t()> rule_bytes;
};








#endif // MAC_HPP
