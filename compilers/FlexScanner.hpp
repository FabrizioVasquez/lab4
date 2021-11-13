#pragma once

#include <FlexLexer.h>

#include "Token.hpp"

namespace utec {
namespace compilers {
std::vector<Token>vec{};
std::vector<std::string>clean_tokens{};
int global_p = 1;
class FlexScanner : public yyFlexLexer {
 public:
  FlexScanner(std::istream& arg_yyin, std::ostream& arg_yyout)
      : yyFlexLexer(arg_yyin, arg_yyout) {}
  FlexScanner(std::istream* arg_yyin = nullptr,
              std::ostream* arg_yyout = nullptr)
      : yyFlexLexer(arg_yyin, arg_yyout) {}

  // 34 - 3 * 42

  Token query_token(){
    if(last_token._lexema == "END")
      last_token = get_token();
    return last_token;
  }

  Token get_token() {
    if(last_token._lexema != "END"){
      Token token_copy = last_token;
      //std::cout<<last_token;
      last_token = Token();
      return token_copy;
    }
    int value;
    if ((value = yylex()) == 0) return Token();
    //vec.push_back(yytext);
    return Token(yytext, (Categoria)value);
  }

  private:
    Token last_token;
};

}  // namespace compilers
}  // namespace utec