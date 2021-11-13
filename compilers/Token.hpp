#pragma once

#include <iostream>

#include "Enums.h"


class Token {
 public:
  std::string _lexema;
  Categoria _atributo;
  unsigned int _level;
  unsigned int _line;

  Token(std::string lexema, Categoria atributo)
      : _lexema(lexema), _atributo(atributo) {
        _level = 0;
        _line = 0;
      }
  Token(std::string lexema, Categoria atributo, const unsigned int &level):
  _lexema(lexema),_atributo(atributo),_level(level),_line(0){}

  Token(std::string lexema,const unsigned int &level):_lexema(lexema),_level(level),_line(0){}

  Token() : _lexema("END"), _atributo(END) {}

  bool operator==(const Token& in) const {
    return in._lexema == this->_lexema && in._atributo == this->_atributo;
  }
};

//Allows printing Tokens on the Google Test failing reports
std::ostream& operator<<(std::ostream& os, Token token) {
  os << "<";
  switch (token._atributo) {
    case Categoria::ID:
      os << "ID "<<token._level;
      break;
    /*case Categoria::A:
      os << "a";
      break;*/
    case Categoria::B:
      os << "b";
      break;
    case Categoria::ERROR:
      os << "Error";
      break;
    case Categoria::NUMBER:
      os << "Number "<<token._level;
      break;
    case Categoria::OP_SUMA:
      os << "Op_suma "<<token._level;
      break;
    case Categoria::OP_RESTA:
      os << "Op_resta "<<token._level;
      break;
    case Categoria::P_ABIERTO:
      os << "P_abierto "<<token._level;
      break;
    case Categoria::P_CERRADO:
      os << "P_cerrado "<<token._level;
      break;
    case Categoria::FACTOR:
      os << "Factor "<<token._level;
      break;
    case Categoria::OP_MULTIPLICACION:
      os << "Op_multiplicacion "<<token._level;
      break;
    case Categoria::ENDL:
      os << "Salto de linea :'D";
      break;
    default:
      if(token._lexema == "exp"){
        os << "Exp "<< token._level;
      }else if(token._lexema == "term"){
        os << "Term "<< token._level;
      }else if(token._lexema == "factor"){
        os << "Factor "<< token._level;
      }
      //os << "Sin Categoria";
      break;
  }
  os << ", " << token._lexema << ">";
  return os;
}
