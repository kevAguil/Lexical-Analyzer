//
//  lex.cpp
//  PA1_K_Aguilar
//
//  Created by Kevin Aguilar on 3/5/23.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <map>
#include <regex>
#include <istream>
#include "lex.h"
#include <list>


LexItem getNextToken (istream& in, int& linenumber){
    enum TokState { START, INID, ININT, INREAL, INSTRING, COMMENT} lexstate = START;
    string lexeme = "";
    char ch;
    while(in.get(ch)) {
        switch( lexstate ) {
            case START:{
                
                if(isspace(ch)){
                    if(ch == '\n'){
                        linenumber++;
                    }
                    break;
                }
                
                if(ch == '+'){
                    lexeme += ch;
                    return LexItem(PLUS,lexeme,linenumber);
                }
                else if(ch == '-'){
                    lexeme += ch;
                    if(in.peek() == 'E' || in.peek() == 'e'){
                        in.get(ch);
                        if(in.peek() == 'Q' || in.peek() == 'q'){
                            lexeme += ch;
                            in.get(ch);
                            lexeme += ch;
                            return LexItem(SEQ, lexeme, linenumber);
                        }
                    }
                    else if(in.peek() == 'L' || in.peek() == 'l'){
                        in.get(ch);
                        if(in.peek() == 'T' || in.peek() == 't'){
                            lexeme += ch;
                            in.get(ch);
                            lexeme += ch;
                            return LexItem(SLTHAN, lexeme, linenumber);
                        }
                    }
                    else if(in.peek() == 'G' || in.peek() == 'g'){
                        in.get(ch);
                        if(in.peek() == 'T' || in.peek() == 't'){
                            lexeme += ch;
                            in.get(ch);
                            lexeme += ch;
                            return LexItem(SGTHAN, lexeme, linenumber);
                        }
                    }
                    else{
                        return LexItem(MINUS, lexeme, linenumber);
                    }
                }
                else if(ch == '*'){
                    lexeme += ch; // Implement ASSOP
                    if(in.peek() == '*'){
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(SREPEAT, lexeme, linenumber);
                    }
                    return LexItem(MULT, lexeme, linenumber);
                }
                else if(ch == '/'){
                    lexeme += ch;
                    return LexItem(DIV, lexeme, linenumber);
                }
                else if(ch == '^'){
                    lexeme += ch;
                    return LexItem(EXPONENT, lexeme, linenumber);
                }
                else if(ch == '='){
                    lexeme += ch;
                    if(in.peek() == '='){ // Checks for NEQ
                        in.get(ch);
                        lexeme+=ch;
                        return LexItem(NEQ, lexeme, linenumber);
                    }
                    return LexItem(ASSOP, lexeme, linenumber);
                }
                else if(ch == '>'){
                    lexeme += ch;
                    return LexItem(NGTHAN, lexeme, linenumber);
                }
                else if(ch == '<'){
                    lexeme += ch;
                    return LexItem(NLTHAN, lexeme, linenumber);
                    
                }
                else if(ch == '.'){
                    lexeme += ch;
                    return LexItem(CAT, lexeme, linenumber);
                }
                else if(ch == '('){
                    lexeme += ch;
                    return LexItem(LPAREN, lexeme, linenumber);
                }
                else if(ch == ')'){
                    lexeme += ch;
                    return LexItem(RPAREN, lexeme, linenumber);
                }
                else if(ch == '{'){
                    lexeme += ch;
                    return LexItem(LBRACES, lexeme, linenumber);
                }
                else if(ch == '}'){
                    lexeme += ch;
                    return LexItem(RBRACES, lexeme, linenumber);
                }
                else if(ch == ';'){
                    lexeme += ch;
                    return LexItem(SEMICOL, lexeme, linenumber);
                }
                else if(ch == ','){
                    lexeme += ch;
                    return LexItem(COMMA, lexeme, linenumber);
                }
                else if(isdigit(ch)){
                    lexeme += ch;
                    lexstate = ININT;
                }
                else if(ch == '\''){
                    lexeme += ch;
                    lexstate = INSTRING;
                }
                else if(isalpha(ch) || ch == '@' || ch == '$' || ch == '_'){
                    lexeme += ch;
                    lexstate = INID;
                }
                else if(ch == '#'){
                    lexstate = COMMENT;
                }
                else{
                    return LexItem(ERR, lexeme+=ch, linenumber);
                }
                break;
                
            }
            case ININT:{ // Integers
                if( regex_match(lexeme + ch, regex("[0-9]+"))){
                    lexeme+=ch;
                }
                else if( ch == '.'){
                    lexeme += ch;
                    lexstate = INREAL;
                }
                else{
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, linenumber);
                }
                break;
            }
            case INREAL: {// Real Numbers
                if( regex_match(lexeme + ch, regex("([0-9]+)\\.([0-9]*)"))){
                    lexeme += ch;
                }
                else if (ch == '.'){
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenumber);
                }

                else{
                    in.putback(ch);
                    return LexItem(RCONST, lexeme, linenumber);
                }
                break;
            }
                
            case INSTRING:{
                if(ch == '\n'){
                    return LexItem(ERR, lexeme, linenumber);
                }
                else if( ch == '\''){
                    return LexItem(SCONST, lexeme.substr(1, lexeme.size()), linenumber);
                }
                
                else{
                    lexeme+= ch;
                }
                break;
            }
                
            case INID:{ // IDENTIFIERS
                if (std::regex_match(lexeme + ch, regex("[_a-zA-Z]+[_a-zA-Z0-9]+"))){
                    lexeme += ch;
                }
                
                else if (std::regex_match(lexeme + ch, regex("^\\@[_a-zA-Z][_a-zA-Z0-9]*"))){
                    lexeme += ch;
                }
                else if (std::regex_match(lexeme + ch, regex("^\\$[_a-zA-Z][_a-zA-Z0-9]*"))){
                    lexeme+=ch;
                }
                
                else{
                    in.putback(ch);
                    return id_or_kw(lexeme, linenumber);
                }
                break;
            }
            
            case COMMENT:{
                if(ch == '\n'){
                    linenumber++;
                    lexstate = START;
                }
                break;
            }
            default:
                break;
        }
        
    }
    if(in.peek() == EOF){
        return LexItem(DONE, "", linenumber);
    }
    else{
        return LexItem(ERR, lexeme, linenumber);
    }
}






map<string, Token> keyWords =
{
    {"IF",IF},
    {"ELSE",ELSE},
    {"WRITELN",WRITELN},
    {"if", IF},
    {"else", ELSE},
    {"writeln", WRITELN}
};
LexItem id_or_kw(const string& lexeme, int linenum){
    
    
    if(keyWords.find(lexeme)!= keyWords.end()){
        return LexItem(keyWords[lexeme], lexeme, linenum);
    }
    else if(lexeme[0] == '@'){
        return LexItem(SIDENT, lexeme, linenum);
    }
    else if(lexeme[0] == '$'){
        return LexItem(NIDENT, lexeme, linenum);
    }
    else{
        return LexItem(IDENT, lexeme, linenum);
    }
}



ostream& operator<<(ostream& out, const LexItem& tok){
    Token tokType = tok.GetToken();
        switch (tokType)
        {
        case WRITELN:
        {
            out << "WRITELN" << endl;
            break;
        }
        case IF:
        {
            out << "IF" << endl;
            break;
        }
        case ELSE:
        {
            out << "ELSE" << endl;
            break;
        }
        case IDENT:
        {
            out << "IDENT(" << tok.GetLexeme() << ')' << endl;
            break;
        }
        case NIDENT:
        {
            out << "NIDENT("<< tok.GetLexeme() << ')' << endl;
            break;
        }
        case SIDENT:
        {
            out << "SIDENT(" << tok.GetLexeme() << ')' << endl;
            break;
        }
        case ICONST:
        {
            out << "ICONST(" << tok.GetLexeme() << ')' << endl;
            break;
        }
        case RCONST:
        {
            out << "RCONST(" << tok.GetLexeme() << ')' << endl;
            break;
        }
        case SCONST:
        {
            out << "SCONST(" << tok.GetLexeme() << ')' << endl;
            break;
        }
        case PLUS:
        {
            out << "PLUS"  << endl;
            break;
        }
        case MINUS:
        {
            out << "MINUS" << endl;
            break;
        }
        case MULT:
        {
            out << "MULT" << endl;
            break;
        }
        case DIV:
        {
            out << "DIV" << endl;
            break;
        }
        case EXPONENT:
        {
            out << "EXPONENT" << endl;
            break;
        }
        case ASSOP:
        {
            out << "ASSOP" << endl;
            break;
        }
        case NEQ:
        {
            out << "NEQ" << endl;
            break;
        }
        case NGTHAN:
        {
            out << "NGTHAN" << endl;
            break;
        }
        case NLTHAN:
        {
            out << "NLTHAN"<< endl;
            break;
        }
        case CAT:
        {
            out << "CAT" << endl;
            break;
        }
        case SREPEAT:
        {
            out << "SREPEAT" << endl;
            break;
        }
        case SEQ:
        {
            out << "SEQ" << endl;
            break;
        }
        case SLTHAN:
        {
            out << "SLTHAN" << endl;
            break;
        }
        case SGTHAN:
        {
            out << "SGTHAN" << endl;
            break;
        }
        case COMMA:
        {
            out << "COMMA" << endl;
            break;
        }
        case SEMICOL:
        {
            out << "SEMICOL" << endl;
            break;
        }
        case LPAREN:
        {
            out << "LPAREN" << endl;
            break;
        }
        case RPAREN:
        {
            out << "RPAREN" << endl;
            break;
        }
        case LBRACES:
        {
            out << "LBRACES" << endl;
            break;
        }
        case RBRACES:
        {
            out << "RBRACES" << endl;
            break;
        }
        case ERR:
        {
            out << "Error in line " << (tok.GetLinenum() + 1) << " (" << tok.GetLexeme() << ')' << endl;
            break;
        }
        default:
            break;
        }
        return out;
    }
