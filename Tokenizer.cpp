//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);

    return name;
}



int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        intValue = intValue * 10 + c - '0';
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

std::string Tokenizer::readString(){
    std::string myString;
    char c;
    inStream.get(c);
    //std::cout << "We read: " << c << std::endl;
    char delimiter = c;
    while( inStream.get(c) && c != delimiter ) {
        //std::cout << "We read: " << c << std::endl;
        myString += c;
    }
    //std::cout << "We read: " << c << std::endl;
    // last character read was either a ' or a ", so we don't put it back
    return myString;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, parsingNewLine{false}, indentOnCurrentLine{0}, inStream{stream}, lastToken{} {
    _indentStack.push_back(0); // no. of spacs at indentation level 0
}




Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    char c1;
    int numberOfSpaces = 0;
    Token token;

    std::cout << "Called getToken()\n";
    
    // This checks for indents and dedents.
    // If an indent or dendent is detected, the token is returned and we do not reach our main
    //      evaulation list to determine what a token is.
    if(parsingNewLine){
        parsingNewLine = 0;
        while(inStream.get(c) && (c == ' ' || c == '\t')){
            if(c == '\t'){
                indentOnCurrentLine += 4 - (indentOnCurrentLine % 4); //to nearest multiple of 4
            }
            else if(c == ' '){
                indentOnCurrentLine++;
            }
        }

        if(c == '#'){
            while( inStream.get(c) && c != '\n' && c != EOF) // go to end of line
                ;
        }

        if(c == '\n'){
            // send no indent/dedent: empty line.
        }
        else if(indentOnCurrentLine > _indentStack.back()){ // send an indent
            parsingNewLine = 1;
            std::cout << "returning character " << c << " to the input stream\n";
            inStream.putback(c); // we are sending an indent, so we need to retain this character
            _indentStack.push_back(indentOnCurrentLine);
            token.setIndent(indentOnCurrentLine);
            std::cout << "Returning this token: ";
            token.print();
            std::cout << std::endl;
            
            _tokens.push_back(token);
            return lastToken = token;
        }
        else if(indentOnCurrentLine < _indentStack.back()){ // send a dedent
            parsingNewLine = 1;
            std::cout << "returning character " << c << " to the input stream\n";
            inStream.putback(c); // we are sending a dedent, so we need to retain this character
            int last_indent = _indentStack.back();
            _indentStack.pop_back();
            token.setDedent(indentOnCurrentLine);
            if(indentOnCurrentLine > _indentStack.back()){ // the indention on the line has too many spaces.
                std::cout << "INDENTATION ERROR\n";
                exit(1);
            }
            std::cout << "Returning this token: ";
            token.print();
            std::cout << std::endl;
            _tokens.push_back(token);
            return lastToken = token;
        }
    }
    else{
        while( inStream.get(c) && (isspace(c) || c == '#') && c != '\n' && !inStream.eof() ){
            if( c == '#'){
                while( inStream.get(c) && c != '\n' && !inStream.eof())
                    ;
            }
        }
    }

    // Main evaulation list //

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    // if this evaulates to true, we leave the function at this spot.
    // it will send an appropiate amount of dedents.
    if( inStream.eof()) {
        if( _indentStack.size() > 1){ // we need to send a dedent(s) before program exits
            int value = _indentStack.back();
            _indentStack.pop_back();
            token.setDedent(value);
            return token;
        }
        token.eof() = true;
    }
    else if( c == '\n' ) {
        parsingNewLine = true;
        token.eol() = true;
        indentOnCurrentLine = 0;
    }
    else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setInteger( readInteger() );

    } 
    else if ( c == ','){
        token.symbol(c);
    }
    else if (c == ':'){
        token.symbol(c);
    }
    else if( c == '=' ){
        inStream.get(c1);
        if(c1 == '='){
            std::string s = "";
            s += c;
            s += c1;
            token.stringSymbol(s);
        }
        else{
            inStream.putback(c1);
            token.symbol(c);
        }
    }
    else if( c == '!' ){
        inStream.get(c1);
        if(c1 == '='){
            std::string s = "";
            s += c;
            s += c1;
            token.stringSymbol(s);
        }
        else{
            inStream.putback(c1);
            std::cout << "Unknown character in input. ->" << c1 << "<-" << std::endl;
            exit(1);
        }
    }
    else if ( c == '<'){
        inStream.get(c1);
        if(c1 == '='){
            std::string s = "";
            s += c;
            s += c1;
            token.stringSymbol(s);
        }
        else if(c1 == '>'){ // we substitute the '<>' operator with a '!='
            token.stringSymbol("!=");
        }
        else{
            inStream.putback(c1);
            std::string s = "";
            s += c;
            token.stringSymbol(s);
        }
    }
    else if ( c == '>'){
        inStream.get(c1);
        if(c1 == '='){
            std::string s = "";
            s += c;
            s += c1;
            token.stringSymbol(s);
        }
        else{
            inStream.putback(c1);
            std::string s = "";
            s += c;
            token.stringSymbol(s);
        }
    }
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%'){
        inStream.get(c1);
        if(c == '/' && c1 == '/'){
            std::string s = "";
            s += c;
            s += c1;
            token.stringSymbol(s);
        }
        else{
            inStream.putback(c1);
            token.symbol(c);
        }
    }
    else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')' || c == '{' || c == '}')
        token.symbol(c);
    else if( c == '\'' || c == '\"' ){
        inStream.putback(c);
        std::string word = readString();
        token.setString( word );
    }
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        std::string word = readName();
    
        // CHANGED
        if(inStream.peek() == '('){
            token.isFuncName() = true;
            std::cout << "READ A FUNCTION NAME: ";
        }
        token.setName( word );
    } 
    else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        //std::cout << "Next char is" << inStream.peek() << std::endl;
        exit(1);
    }
    std::cout << "Returning this token: ";
    token.print();
    std::cout << std::endl;
    _tokens.push_back(token);
    return lastToken = token;
}



void Tokenizer::ungetToken() {
    ungottenToken = true;
}



void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}