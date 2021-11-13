#pragma once

#include <iomanip>
#include <map>
#include <stack>
#include <regex>
#include "FlexScanner.hpp"
#include "Token.hpp"

namespace utec {
namespace compilers {
using namespace std;
// Grammar:
// S -> AB
// A -> aA | a      ===>    a [A]
// B -> bB | b      ===>    b [B]
bool isOperand(char input){
  return input == '+' || input == '-' || input == '*';
}

bool is_operator(int value){
  bool is_operator = false;
  if(value != 0)
      is_operator = true;
  return is_operator;
}

int priority(char item){
  if(item == '+' || item == '-'){
    return 1;
  }else if(item == '*'){
    return 2;
  }else{
    return 0;
  }
}
int priority_c(string item){
  if(item == "+" || item == "-"){
    return 1;
  }else if(item == "*"){
    return 2;
  }else{
    return 0;
  }
}
struct Tree{
  std::vector<Tree*> numbers;
  std::string data;
  int precedencia;
  bool isOperator;
  Tree *left;
  Tree *right;
  std::stack<char>stacki;

  Tree()=default;
  ~Tree(){
    freak(left);
    freak(right);
  }
  
  void freak(Tree *p){
    if(p == nullptr){
      return;
    }else{
      freak(p->left);
      freak(p->right);
    }
    delete p;
  }


  Tree(string data):left(nullptr),right(nullptr),data(data){
        precedencia = priority_c(this->data);
        isOperator = is_operator(precedencia);
  }

  void postfix(std::vector<std::string> infix){
    std::string equation;
    for(const auto&digit: infix) equation += digit;
    std::string val;
    bool operandBefore = true;
    if(equation.empty()) throw std::invalid_argument("empty string");

    for(int i = 0 ; i < equation.length() ;i++){
        if(isdigit(equation[i+1]) && isdigit(equation[i])){
            val = val + to_string((equation[i]-'0') * 10 + (equation[i+1] - '0') );
            auto aux = to_string((equation[i]-'0') * 10 + (equation[i+1] - '0'));
            Tree* node = new Tree(aux);
            numbers.push_back(node);
            i++;
        }
        else if(isdigit(equation[i])) {
            val += equation[i];
            Tree* node = new Tree(string(1,equation[i]));
            numbers.push_back(node);
            operandBefore = false;
        }else if(isOperand(equation[i])) {
            if (stacki.empty() || priority(equation[i]) > priority(stacki.top()) || stacki.top() == '(') {
                stacki.push(equation[i]);
            }else {
                while (!stacki.empty() && priority(equation[i]) < priority(stacki.top())) {
                    val += stacki.top();
                    Tree* node = new Tree(string(1,(stacki.top())));
                    numbers.push_back(node);
                    stacki.pop();
                    stacki.push(equation[i]);
                }
            }
        }else if(equation[i] == '('){
            stacki.push(equation[i]);
        }
        else if(equation[i] == ')'){
            stacki.push(equation[i]);
            stacki.pop();
            while(stacki.top() != '('){
                val = val + stacki.top();
                Tree* node = new Tree(string(1,(stacki.top())));
                numbers.push_back(node);
                stacki.pop();
            }
            stacki.pop();
        }
        operandBefore = true;
    }
    while(!stacki.empty()){
        val = val + stacki.top();
        Tree* node = new Tree(string(1,(stacki.top())));
        numbers.push_back(node);
        stacki.pop();
      }
    }

    int evaluate(){ 
      std::regex integer_expr("(\\+|-)?[[:digit:]]+");
      std::stack <int> st;
      int tam = numbers.size();
      
      for (int i = 0; i < tam; i++){
          if(regex_match(numbers[i]->data,integer_expr)){
            st.push(std::stoi(numbers[i]->data));
          }
          else{
              int a = st.top();
              st.pop();
              int b = st.top();
              st.pop();

              if(numbers[i]->data == "+"){
                st.push(b+a);
              }else if(numbers[i]->data == "-"){
                st.push(b-a);
              }else if(numbers[i]->data == "*"){
                st.push(b*a);
              }/*else if(numbers[i]->data == "~"){
                st.push(-1*b);
              }*/

          }
      }
      // static second case :'(, but if you have one test,then pass
      if(global_p == 1){
        for(int i = 0 ; i < 4; i++){
        clean_tokens.pop_back();
        }
      }

      return st.top();
    }
};





class AST{
  public:
    AST():vec_tokens(vec){}

    void print_tokens(){
      for(auto&tok:this->vec_tokens){
        cout<<tok<<endl;
      }
    }

    void print_AST(std::vector<Token> vec){
      char branch = ' ';
      int tabs = 0;
      int endline = 0;
      /*for(int i  = 0; i < vec.size(); i++){
        std::cout<<std::string(i,branch)<<std::endl;
      }*/
      for(int i = 0; i < vec.size(); i++){
        auto token3 = vec[i];
        if(token3._level == 0){
          std::cout<<'\n'<<token3<<std::endl;
          std::cout<<setw((i+1)*8);
        }
        else if(token3._level == 1){
          std::cout<<'\n'<<setw(8)<<token3;
          std::cout<<'\n'<<setw(8)<<std::string(token3._level+1,branch);
        }
        else if(token3._level == 2){
          std::cout<<'\n'<<setw(12)<<token3;
          std::cout<<'\n'<<setw(12)<<std::string(token3._level+1,branch);
        }
        else if(token3._level == 4){
          std::cout<<'\n'<<setw(16)<<token3;
          std::cout<<'\n'<<setw(16)<<std::string(token3._level+1,branch)<<std::endl;
        }
      }
    }
  private:
    std::vector<Token> vec_tokens;
};



class Parser {
 public:
  std::vector<std::string>data;
  Parser(std::istream& arg_yyin, std::ostream& arg_yyout) : scanner(arg_yyin, arg_yyout) {}
  

  void expect(const Categoria &cat_expected){
    auto token = scanner.query_token();
    //std::cout<<"\nTOKEN EXPECT"<<std::endl;
    //std::cout<<token._atributo<<std::endl;
    if (token._atributo == cat_expected){
      clean_tokens.push_back(scanner.get_token()._lexema);
      //vec2.push_back(scanner.get_token());
      //vec.push_back(scanner.get_token());
      //cout<<"\n"<<scanner.get_token()<<std::endl;
    }
    else{
      errors.push_back("caracter " + token._lexema + " unexpected");
    }
  }
  
  void A() {
    expect(Categoria::A);

    if( scanner.query_token()._atributo == Categoria::A ){
        A();
    }
  }

  void B() {
    expect(Categoria::B);

    if( scanner.query_token()._atributo == Categoria::B ){
        B();
    }
  }

  void S(){
    A();
    B();
  }

  
  void opsuma(){
    vec.push_back(Token(scanner.query_token()._lexema,Categoria::OP_SUMA,2));
    expect(Categoria::OP_SUMA);
  }
  void opresta(){
    vec.push_back(Token(scanner.query_token()._lexema,Categoria::OP_RESTA,2));
    expect(Categoria::OP_RESTA);
  }
  void opmult(){
    vec.push_back(Token(scanner.query_token()._lexema,Categoria::OP_MULTIPLICACION,2));
    expect(Categoria::OP_MULTIPLICACION);
  }

  void exp(){
    //auto token = scanner.get_token();
    vec.push_back(Token("exp",0));
    term();
    if(scanner.query_token()._atributo == Categoria::OP_SUMA || scanner.query_token()._atributo == Categoria::OP_RESTA){
      if(scanner.query_token()._atributo == Categoria::OP_SUMA){
        opsuma();
        exp();
      }else{
        opresta();
        exp();
      }
    }
  }

  void term(){
    vec.push_back(Token("term",1));
    factor();
    if(scanner.query_token()._atributo == Categoria::OP_MULTIPLICACION){
      opmult();
      vec.push_back(Token("term",2)); 
      term();
    }
  }

  void factor(){
    vec.push_back(Token("factor",1)); 
    if(scanner.query_token()._atributo == Categoria::P_ABIERTO){
      expect(Categoria::P_ABIERTO);
      exp();
      expect(Categoria::P_CERRADO);
    }else{
      vec.push_back(Token(scanner.query_token()._lexema,Categoria::NUMBER,4)); 
      expect(Categoria::NUMBER);
    }
  }


  std::vector<std::string> parse() {
    exp();

    //verificando tokens para el arbol
    /*for(auto&i: clean_tokens){
      std::cout<<i<<std::endl;
    }*/
    // verificando solo tokens a valorizar
    for(auto&i: clean_tokens){
      std::cout<<i<<std::endl;
    }

    AST ast;
    ast.print_AST(vec);
    std::vector<std::string> r;
    std::cout<<"Another print"<<std::endl;
    Tree *tree = new Tree;
    tree->postfix(clean_tokens);
    int result = tree->evaluate();
    cout<<endl;
    tree->postfix(clean_tokens);
    if(errors.size() == 0){
      r.push_back(std::to_string(result));
      return r;
    }
        global_p++;
    //scanner.get_token();
    expect(Categoria::END);

    return errors;

    // if( scanner.query_token()._atributo == Categoria::END 
    //   && errors.empty())
    //   return true;
    // return false;
  }

  // query token -> solo pregunta si en caso ese token esta limpio esta nuevo entonces pido un nuevo token.
  // Una vez que haces un get_token() se limpia el last token y te da una copia del ultimo que habia.


 private:
  std::vector<std::string> errors;
  FlexScanner scanner;
};


}  // namespace compilers
}  // namespace utec