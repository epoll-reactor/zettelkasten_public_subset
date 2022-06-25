#include "mac.hpp"

int main() {

  const std::string sMac = "10:a0:CD:34:5e:f0";

  v_mac_t vResult;
  vResult.reserve( 6 );

  using iterator_type = std::string::const_iterator;
  using mac_parser_string_t = mac_parser<iterator_type>;

  mac_parser_string_t parser;

  std::string::const_iterator iter = sMac.begin();
  std::string::const_iterator end  = sMac.end();

  auto result = boost::spirit::qi::parse( iter, end, parser, vResult );

  if ( iter == end ) std::cout << "full string matched" << std::endl;

  std::cout << "result=(" << result << "):'" << std::hex;
  for ( uint16_t ch: vResult ) std::cout << ch;
  std::cout << "'" << std::endl;

  return 0;
}
