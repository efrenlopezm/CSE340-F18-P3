//
// Created by student on 10/16/18.
//

#ifndef PROJECT_3_PARSER_H
#define PROJECT_3_PARSER_H

#include <string>
#include <set>
#include <vector>
#include "lexer.h"

using namespace std;


struct Variable{

    string type;
    int lineNumber;
    string name;

};

struct Scope{

    vector<Variable *> variables;
    Scope * parentScope;

};


class parser {


private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();
    Token peek2();


    void parse_scope();
    void parse_scope_list();
    void parse_var_decl();
    void parse_stmt();
    void parse_id_list();
    void parse_type_name();
    void parse_stmt_list();
    void parse_assign_stmt();
    void parse_while_stmt();
    void parse_expr();
    void parse_arithmetic_operator();
    void parse_binary_boolean_operator();
    void parse_relational_operator();
    void parse_primary();
    void parse_bool_const();
    void parse_condition();


public:
    void ParseProgram();

};


#endif //PROJECT_3_PARSER_H
