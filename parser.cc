//
// Created by student on 10/16/18.
//

#include <iostream>
#include "parser.h"
#include <cstdlib>

using namespace std;

//Start of Global variables

Scope * currentScope = NULL;

//End of Global variables

void parser::syntax_error() {
    cout << "Syntax Error\n";
    exit(1);
}
Token parser::expect(TokenType expected_type) {
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}
Token parser::peek() {
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}
Token parser::peek2() {
    Token t = lexer.GetToken();
    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    lexer.UngetToken(t);
    return t1;
}

void parser::ParseProgram(){

    parse_scope();
    expect(END_OF_FILE);
}

void parser::parse_scope() {

    //cout<<"Parsing SCOPE\n";




    //Start of parsing

    expect(LBRACE);

    Scope * newScope = new Scope();
    newScope->parentScope = currentScope;
    currentScope = newScope;

    parse_scope_list();
    expect(RBRACE);

    //End of parsing

    Scope * temp = currentScope;
    currentScope = currentScope->parentScope;
    delete temp;

}

void parser::parse_scope_list() {

    //cout<<"Parsing SCOPE_LIST\n";
    Token t1 = peek();
    Token t2 = peek2();

    if((t1.token_type == ID && t2.token_type == COLON) ||(t1.token_type == ID && t2.token_type == COMMA)){

        parse_var_decl();

        Token t1 = peek();
        //Token t2 = peek2();
        if(t1.token_type == ID || t1.token_type == LBRACE || t1.token_type == WHILE){

            parse_scope_list();
            //scope_list -> var_decl scope_list
        }
        else if(t1.token_type == RBRACE){

            //scope_list -> var_decl

        }
        else{

            syntax_error();

        }

    }
    else if ((t1.token_type == ID && t2.token_type == EQUAL) ||(t1.token_type == WHILE)){

        parse_stmt();

        Token t1 = peek();
        //Token t2 = peek2();
        if(t1.token_type == ID || t1.token_type == LBRACE || t1.token_type == WHILE ){

            parse_scope_list();
            //scope_list -> stmt scope_list
        }
        else if(t1.token_type == RBRACE){

            //scope_list -> stmt

        }
        else{

            syntax_error();

        }
    }
    else if(t1.token_type == LBRACE){

        parse_scope();

        Token t1 = peek();

        if(t1.token_type == ID || t1.token_type == LBRACE || t1.token_type == WHILE){

            parse_scope_list();
            // scope_list -> scope scope_list
        }
        else if(t1.token_type == RBRACE){

            //scope_list -> scope
        }
        else{

            syntax_error();

        }
    }
    else{

        syntax_error();
    }

}

void parser::parse_var_decl() {

    //cout<<"Parsing VAR_DECL\n";
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);

    //var_decl -> id_list COLON type_name SEMICOLON
}

void parser::parse_id_list(){

    //CHECK THIS

    //cout <<"Parsing ID_LIST\n";
   Token t1 = peek();
   Token t2 = peek2();

   if(t1.token_type == ID && t2.token_type == COMMA){

       expect(ID);
       expect(COMMA);
       parse_id_list();

       //id_list -> ID COMMA id_list
   }
   else if(t1.token_type == ID ){

       expect(ID);

       //id_list -> ID
   }
   else{
       syntax_error();
   }

}

void parser::parse_type_name() {

    //cout<<"Parsing TYPE_NAME\n";
    Token t = peek();

    if(t.token_type == REAL){

        expect(REAL);
        //type_name -> REAL

    }
    else if(t.token_type == INT){

        expect(INT);
        //type_name -> INT

    }
    else if(t.token_type == BOOLEAN){

        expect(BOOLEAN);
        //type_name -> BOOLEAN

    }
    else if(t.token_type == STRING){

        expect(STRING);
        //type_name -> STRING

    }
    else{

        syntax_error();
    }

}

void parser::parse_stmt_list() {

    // cout<<"Parsing STMT_LIST\n";
    parse_stmt();

    Token t = peek();

    if(t.token_type == ID || t.token_type == WHILE){

        parse_stmt_list();
        //stmt_list -> stmt stmt_list
    }
    else if(t.token_type == RBRACE){

        //stmt_list -> stmt

    }
    else{

        syntax_error();
    }
}

void parser::parse_stmt() {

    //cout<<"Parsing STMT\n";
    Token t = peek();

    if(t.token_type == ID){

        parse_assign_stmt();
        //stmt -> assign_stmt
    }
    else if(t.token_type = WHILE){

        parse_while_stmt();
        //stmt -> while_stmt
    }
    else{

        syntax_error();
    }

}

void parser::parse_assign_stmt() {

    expect(ID);
    expect(EQUAL);
    parse_expr();
    expect(SEMICOLON);
    //assign_stmt -> ID EQUAL expr SEMICOLON
}

void parser::parse_while_stmt() {
    //cout<<"Parsing WHILE_STMT\n";
    expect(WHILE);
    parse_condition();

    Token t = peek();

    if(t.token_type == LBRACE){

        expect(LBRACE);
        parse_stmt_list();
        expect(RBRACE);
        //while_stmt -> WHILE condition LBRACE stmt list RBRACE
    }
    else if(t.token_type == ID || t.token_type == WHILE ){

        parse_stmt();
        //while_stmt -> WHILE condition stmt

    }
    else{

        syntax_error();
    }
}

void parser::parse_expr(){

    Token t = peek();

    if( t.token_type == PLUS ||
        t.token_type == MINUS ||
        t.token_type == MULT ||
        t.token_type == DIV){

        parse_arithmetic_operator();
        parse_expr();
        parse_expr();
        //exp -> arithmetic_operator expr expr
        //FIRST(arithmeticoperator) = { PLUS, MINUS, MULT, DIV }

    }else if  (t.token_type == AND ||
               t.token_type == OR ||
               t.token_type == XOR){

        parse_binary_boolean_operator();
        parse_expr();
        parse_expr();

        //exp -> binary_boolean_operator expr expr
        //FIRST(booleanoperator) = { NOT, AND, OR, XOR }
    }else if ( t.token_type == GREATER ||
               t.token_type == GTEQ ||
               t.token_type == LESS ||
               t.token_type == NOTEQUAL ||
               t.token_type == LTEQ){

        parse_relational_operator();
        parse_expr();
        parse_expr();

        //exp -> relational_operator expr expr
        //FIRST(relationaloperator) = { GREATER, GTEQ, LESS, NOTEQUAL, LTEQ }
    }else if ( t.token_type == ID ||
               t.token_type == NUM ||
               t.token_type == REALNUM ||
               t.token_type == STRING_CONSTANT ||
               t.token_type == TRUE ||
               t.token_type == FALSE){

        parse_primary();

        //expr -> primary
        //FIRST(primary) = { ID, NUM, REALNUM, STRING_CONSTANT, TRUE, FALSE }
    }/*else if(t.token_type == NOT){

        expect(NOT);
        parse_expr();
        //exp -> NOT parse_expr
    }*/
    else{

        syntax_error();
    }


}

void parser::parse_arithmetic_operator(){

    Token t = peek();

    if(t.token_type == PLUS){

        expect(PLUS);
        //arithmetic_operator -> PLUS

    }else if(t.token_type == MINUS){

        expect(MINUS);
        //arithmetic_operator -> MINUS

    }else if(t.token_type == MULT){

        expect(MULT);
        //arithmetic_operator -> MULT

    }else if(t.token_type == DIV){

        expect(DIV);
        //arithmetic_operator -> DIV

    }
    else{

        syntax_error();
    }
}

void parser::parse_binary_boolean_operator() {

    Token t = peek();

    //binary_boolean_operator -> AND | OR | XOR

    if(t.token_type == AND){

        expect(AND);
        //binary_boolean_operator ->  AND

    }else if(t.token_type == OR){

        expect(OR);
        //binary_boolean_operator ->  OR

    }else if(t.token_type == XOR){

        expect(XOR);
        //binary_boolean_operator ->  XOR

    }
    else{

        syntax_error();
    }
}

void parser::parse_relational_operator() {

    Token t = peek();

    //GREATER | GTEQ | LESS | NOTEQUAL | LTEQ

    if(t.token_type == GREATER){

        expect(GREATER);
        //relational_operator -> GREATER

    }else if(t.token_type == GTEQ){

        expect(GTEQ);
        //relational_operator -> GTEQ

    }else if(t.token_type == LESS){

        expect(LESS);
        //relational_operator -> LESS

    }else if(t.token_type == NOTEQUAL){

        expect(NOTEQUAL);
        //relational_operator -> NOTEQUAL

    }else if(t.token_type == LTEQ){

        expect(LTEQ);
        //relational_operator -> LTEQ

    }
    else{

        syntax_error();
    }
}

void parser::parse_primary() {

    Token t = peek();

    //primary -> ID | NUM | REALNUM | STRING_CONSTANT | bool_const

    if(t.token_type == ID){

        expect(ID);
        //primary -> ID

    }else if(t.token_type == NUM){

        expect(NUM);
        //primary -> NUM

    }else if(t.token_type == REALNUM){

        expect(REALNUM);
        //primary -> REALNUM

    }else if(t.token_type == STRING_CONSTANT){

        expect(STRING_CONSTANT);
        //primary -> STRING_CONSTANT

    }else if(t.token_type == TRUE || t.token_type == FALSE ){

        parse_bool_const();
        //primary -> bool_const

    }
    else{

        syntax_error();
    }

}

void parser::parse_bool_const() {

    Token t = peek();

    //bool_const -> TRUE | FALSE

    if(t.token_type == TRUE){

        expect(TRUE);
        //bool_const -> TRUE

    }else if(t.token_type == FALSE){

        expect(FALSE);
        //bool_const -> FALSE

    }
    else{

        syntax_error();
    }
}

void parser::parse_condition() {

    expect(LPAREN);
    parse_expr();
    expect(RPAREN);
    //condition -> LPAREN expr RPAREN
}

