
/*  The majority of the work is done by the class 'convert'.
    This class builds an expression tree using the input infix
    expression.  A post-order traversal of the expression tree 'dumps'
    it into an array in postfix form.  The iterator copies the token
    from this array to user's arrays.

*/

#include <vector>
#include <iostream>
//#include "Token.hpp"
#include "Parser.hpp"
//#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements* Parser::file_input(){
    Statements *statements = new Statements();
    Token tok = tokenizer.getToken();

    //THE MAIN LOOP THAT BUILDS THE EXPRESSION TREE
    while( !tok.eof() ){
        if(tok.isName()){
            tokenizer.ungetToken();
            Statement* newStatement = stmt();
            statements->addStatement(newStatement);
        }
        else if( !tok.eol() ){
            die("Parser::file_input()", "Expected NEWLINE, instead got", tok);
        }
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return statements;
}


Statement *Parser::stmt() {

    //Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    Statement* newStatement;

    if(tok.isCompoundStatement()){
        std::cout << "<<COMPOUND STATEMENT>>\n";
        tokenizer.ungetToken();
        newStatement = compound_statement();
    }
    else if(tok.isSimpleStatement()){
        std::cout << "<<SIMPLE STATEMENT>>\n";
        tokenizer.ungetToken();
        newStatement = simple_statement();
    }
    // else if : token is an 'if' statement
    else{   //we read a non-keyword name
        die("Parser::stmt()","Expected a compound or simple statement, instead got", tok);
    }
    return newStatement;
}

// creates an infix node if we get multiple statements
Statement* Parser::simple_statement(){
    Token tok = tokenizer.getToken();
    Statement* newStatement;
    if(tok.isPrintKeyword() ){
        std::cout << "Parser::simple_statement() READ PRINT KEYWORD\n";
        tokenizer.ungetToken();
        newStatement = print_statement();
    }
    else if(tok.isFuncName() ){
        std::cout << "Parser::simple_statement() READ FUNC NAME\n";
        tokenizer.ungetToken();
        newStatement = call_stmt();
    }
    else if(tok.isReturnStatement() ){
        std::cout << "Parser::simple_statement() READ RETURN KEYWORD\n";
        tokenizer.ungetToken();
        newStatement = return_stmt();
    }
    else if(tok.isName()){   //we read a non-keyword name, assignment statement
        std::cout << "Parser::simple_statement() READ A NAME\n";
        tokenizer.ungetToken();
        newStatement = assign_statement();
    }

    tok = tokenizer.getToken();
    if(!tok.eol()){
        die("Parser::simple_statement()", "Expected a NEWLINE character, instead got", tok);
    }
    return newStatement;
}



PrintStatement *Parser::print_statement(){

    Token tok = tokenizer.getToken();
    if(!tok.isPrintKeyword()){
        die("Parser::print_statement", "Expected a print keyword, instead got", tok);
    }

    std::vector<ExprNode*> rightHandSideExpressions = testList();

    return new PrintStatement(rightHandSideExpressions);

}

// and assignemnt statement never shows up on the right side
AssignmentStatement *Parser::assign_statement() {
    
    Token nameTok = tokenizer.getToken();
    if (!nameTok.isName())
        die("Parser::assign_statement", "Expected a name token, instead got", nameTok);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assign_statement", "Expected an equal sign, instead got", assignOp);


    ExprNode *rightHandSideExpr = test();

    return new AssignmentStatement(nameTok.getName(), rightHandSideExpr);
}


Statement *Parser::compound_statement() {

    //Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    Statement* newStatement;

    if(tok.isForLoopKeyword()){
        tokenizer.ungetToken();
        //ForStatement *forStmt = for_statement();
        newStatement = py_for_statement();
    }
    else if(tok.isIfStatementKeyword()){
        tokenizer.ungetToken();
        newStatement = if_statement();
    }
    else if(tok.isFuncDefKeyword()){
        tokenizer.ungetToken();
        newStatement = func_def_statement();
    }
    else{
        die("Parser::compound_statement()", "Expected a compound statement keyword, instead got", tok);
    }
    return newStatement;
}



// this actully needs to call call()?
// this is basically a light-weight wrapper around call()
FunctionCallStatement *Parser::call_stmt(){
    
    CallFunctionNode* new_call_func_node = call();

    return new FunctionCallStatement(new_call_func_node);
    
}





// ’return’ [test]
FunctionReturnStatement* Parser::return_stmt(){ // should this be a special return statement node? It would exit the function.
    Token tok = tokenizer.getToken();
    if(!tok.isReturnStatement()){
        die("Parser::return_stmt", "expected a return keyword, instead got", tok);
    }
    ExprNode* return_expression = test();
    return new FunctionReturnStatement(return_expression);
}


std::vector<ExprNode*> Parser::testList(){

    std::vector<ExprNode*> testlistVector;
    testlistVector.push_back( test() );
    Token tok = tokenizer.getToken();
    while (tok.isComma()){
        testlistVector.push_back( test() );
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return testlistVector;
}



/*
ForStatement *Parser::for_statement(){
    Token tok = tokenizer.getToken();
    if(!tok.isForLoopKeyword()){
        die("Parser::for_statement", "Expected for keyword, instead got", tok);
    }
    tok = tokenizer.getToken();
        if(!tok.isOpenParen()){
            die("Parser::for_statement", "Expected an '(', instead got", tok);
        }

        AssignmentStatement *assignmentStatement1 = assign_statement(); // 1st assignment statement

        tok = tokenizer.getToken();
        if(!tok.isSemiColon()){
            die("Parser::for_statement", "Expected an ';', instead got", tok);
        }

        //ExprNode *cndtlExpr = relExpr();
        ExprNode *cndtlExpr = test();

        tok = tokenizer.getToken();
        if(!tok.isSemiColon()){
            die("Parser::for_statement", "Expected an ';', instead got", tok);
        }

        tok = tokenizer.getToken();
        if (!tok.eol()){
            die("Parser::for_statement", "Expected a NEWLINE, instead got", tok);
        }
        

        AssignmentStatement *assignmentStatement2 = assign_statement(); // 2nd assignment statement

        tok = tokenizer.getToken();
        if(!tok.isCloseParen()){
            die("Parser::for_statement", "Expected an ')', instead got", tok);
        }

        tok = tokenizer.getToken();
        if (!tok.eol()){
            die("Parser::for_statement", "Expected a NEWLINE, instead got", tok);
        }

        tok = tokenizer.getToken();
        if(!tok.isIndent()){
            die("Parser::for_statement", "Expected an INDENT token, instead got ", tok);
        }

        Statements *blockStatements = statements();

        tok = tokenizer.getToken();
        if(!tok.isDedent()){
            die("Parser::for_statement", "Expected a DEDENT token, instead got", tok);
        }

        std::cout << "Parser is returning a new ForStatement...)\n";
        
        return new ForStatement(assignmentStatement1, cndtlExpr, assignmentStatement2, blockStatements);
}
*/






// NEWLINE INDENT stmt+ DEDENT (my newlines aren't )
Statements* Parser::suite(){
    Token tok = tokenizer.getToken();
    if(!tok.eol()){
        die("Parser::suite()", "Expected NEWLINE (eol) token, instead got", tok);
    }
    tok = tokenizer.getToken();
    if(!tok.isIndent()){
        die("Parser::suite()", "Expected an INDENT token, instead got", tok);
    }
    
    Statements *statements = new Statements();
    Statement* newStatement = stmt();
    statements->addStatement(newStatement);

    tok = tokenizer.getToken();

    while(!tok.isDedent()){
        if(tok.isName()){
            tokenizer.ungetToken();
            newStatement = stmt();
            statements->addStatement(newStatement);
        }
        else if( !tok.eol()  ){
            die("Parser::suite()", "Expected NEWLINE or EOF, instead got", tok);
        }
        tok = tokenizer.getToken();
    }

    if(!tok.isDedent()){
        die("Parser::suite()", "Expected a DEDENT token, instead got", tok);
    }

    std::cout << "Leaving Parser::suite()\n";
    return statements;

}




// NEWLINE INDENT {stmt | return_stmt}+ DEDENT 
Statements* Parser::func_suite(){

    Token tok = tokenizer.getToken();
    if(!tok.eol()){
        die("Parser::suite()", "Expected NEWLINE (eol) token, instead got", tok);
    }
    tok = tokenizer.getToken();
    if(!tok.isIndent()){
        die("Parser::suite()", "Expected an INDENT token, instead got", tok);
    }
    
    Statements *statements = new Statements();
    Statement* newStatement = stmt();
    statements->addStatement(newStatement);

    tok = tokenizer.getToken();

    while(!tok.isDedent()){
        if(tok.isReturnStatement()){
            tokenizer.ungetToken();
            FunctionReturnStatement* rtrn_stmt_ptr = return_stmt();
            statements->addStatement(rtrn_stmt_ptr);
        }
        else if(tok.isName()){
            tokenizer.ungetToken();
            newStatement = stmt();
            statements->addStatement(newStatement);
        }
        else if( !tok.eol()  ){
            die("Parser::suite()", "Expected NEWLINE or EOF, instead got", tok);
        }
        tok = tokenizer.getToken();
    }

    if(!tok.isDedent()){
        die("Parser::suite()", "Expected a DEDENT token, instead got", tok);
    }

    std::cout << "Leaving Parser::suite()\n";
    return statements;

}


// 'if' test ':' suite {'elif' test ':' suite}* ['else' ':' suite]
IfStatement* Parser::if_statement(){
    Token tok = tokenizer.getToken();
    if(!tok.isIfStatementKeyword()){
        die("Parser::if_statement()", "Expected a 'if' token, instead got ", tok);
    }

    // these will always have the same number of elements
    std::vector<ExprNode*> if_tests_vector;
    std::vector<Statements*> block_statements_vector;
    
    Statements* else_statements = nullptr; // might not be used

    if_tests_vector.push_back( test() );

    tok = tokenizer.getToken();
    if(!tok.isColon()){
        die("Parser::if_statement()", "Expected a ':' token, instead got ", tok);
    }

    Statements* block_statements = suite();
    block_statements_vector.push_back( block_statements );
    

    tok = tokenizer.getToken();
    while( tok.getName() == "elif" ){
        if_tests_vector.push_back( test() );
        tok = tokenizer.getToken();
        if(!tok.isColon()){
            die("Parser::if_statement()", "Expected a ':' token, instead got ", tok);
        }
        block_statements = suite();
        block_statements_vector.push_back( block_statements );
        tok = tokenizer.getToken();
    }

    std::cout << "our token is: ";
    tok.print();
    std::cout << "\n";
    
    tokenizer.ungetToken();

    tok = tokenizer.getToken();
    if(tok.getName() == "else"){
        tok = tokenizer.getToken();
        if(!tok.isColon()){
            die("Parser::if_statement()", "Expected a ':' token, instead got ", tok);
        }
        else_statements = suite();
    }
    else{
        tokenizer.ungetToken();
    }


    return new IfStatement(if_tests_vector, block_statements_vector, else_statements);

}




//does not support negative iteration (need to support negative integer tokens)
PyForStatement *Parser::py_for_statement(){

    // for i in range(...): statements
    Token tok = tokenizer.getToken();
    if(!tok.isForLoopKeyword()){
        die("Parser::Pyfor_statement()", "Expected a 'for' token, instead got ", tok);
    }

    Token iterationVariable = tokenizer.getToken();

    if(!iterationVariable.isName()){
        die("Parser::Pyfor_statement()", "Expected an identifier token, instead got ", tok);
    }

    tok = tokenizer.getToken();
    if(tok.getName() != "in"){
        die("Parser::Pyfor_statement()", "Expected an 'in' token, instead got ", tok);
    }

    tok = tokenizer.getToken();
    if(tok.getName() != "range"){
        die("Parser::Pyfor_statement()", "Expected a 'range' token, instead got ", tok);
    }

    tok = tokenizer.getToken();
    if(!tok.isOpenParen()){
        die("Parser::Pyfor_statement()", "Expected a open parans token, instead got ", tok);
    }

    std::vector<ExprNode*> rangeStatements = testList();

    std::cout << "we are in parser::PyForLoop() and we have just exited testList.\n";
    std::cout << "We generated " << rangeStatements.size() << " statements in our range vector\n";
    for(int i = 0; i<rangeStatements.size();i++){
        std::cout << "Statement " << i+1 << " is: ";
        rangeStatements[i]->print();
        std::cout << std::endl;
    }

    // get our range expressions
    AssignmentStatement* init_index_ptr;
    AssignmentStatement* step_index_ptr; // rhs should be an infix expression node
    
    ExprNode* range_start; 
    ExprNode* range_end;
    ExprNode* range_step;
    if(rangeStatements.size() == 0 || rangeStatements.size() > 3){
        std::cout << "Incorrect number of range statements detected (1 or 3 allowed)\n";
        exit(1);
    }
    // do the parsing here because the # of values determines behavior
    // This is kind of wonky to make tokens and make expressions. 
    //      A helper function is probably better, like one which takes a token and two exprnodes to make an infix node.
    else{
        if(rangeStatements.size() == 1){ // end
            range_end = rangeStatements[0];
            Token startToken;
            startToken.setInteger(0);
            range_start = new NumberNode(startToken);
            Token stepToken;
            stepToken.setInteger(1);
            range_step = new NumberNode(stepToken);
            init_index_ptr = new AssignmentStatement(iterationVariable.getName(), range_start);
            Token operation;
            operation.symbol('+');
            InfixExprNode * InfixNode = new InfixExprNode(operation);
            InfixNode->left() = new Variable(iterationVariable);
            InfixNode->right() = range_step;
            step_index_ptr = new AssignmentStatement(iterationVariable.getName(), InfixNode);
        }
        else if(rangeStatements.size() == 2){ // start, end
            range_start = rangeStatements[0];
            range_end = rangeStatements[1];
            Token stepToken;
            stepToken.setInteger(1);
            range_step = new NumberNode(stepToken);
            init_index_ptr = new AssignmentStatement(iterationVariable.getName(), range_start);
            Token operation;
            operation.symbol('+');
            InfixExprNode * InfixNode = new InfixExprNode(operation);
            InfixNode->left() = new Variable(iterationVariable);
            InfixNode->right() = range_step;
            step_index_ptr = new AssignmentStatement(iterationVariable.getName(), InfixNode);
        }
        else{ // start, end, step
            range_start = rangeStatements[0];
            range_end = rangeStatements[1];
            range_step = rangeStatements[2];
            init_index_ptr = new AssignmentStatement(iterationVariable.getName(), range_start);
            Token operation;
            operation.symbol('+');
            InfixExprNode * InfixNode = new InfixExprNode(operation);
            InfixNode->left() = new Variable(iterationVariable);
            InfixNode->right() = range_step;
            step_index_ptr = new AssignmentStatement(iterationVariable.getName(), InfixNode);
        }
    }

    if(!tokenizer.getToken().isCloseParen()){
        die("Parser::Pyfor_statement()", "Expected a closed parans token, instead got ", tok);
    }
    tok = tokenizer.getToken();
    if(!tok.isColon()){
        die("Parser::py_for_statement()", "Expected a colon token, instead got", tok);
    }

    Statements *block_statements = suite();

    std::cout << "Parser is returning a new PyForStatement...)\n";
    
    return new PyForStatement(init_index_ptr, block_statements, range_start, range_end, range_step, step_index_ptr);
}




// 'def' ID '(' [ parameter_list ] ')' ':' func_suite
FunctionDefinitionStatement *Parser::func_def_statement(){
    Token tok = tokenizer.getToken();
    if(!tok.isFuncDefKeyword()){
        die("Parser::func_def_statement()", "Expected a def keyword, instead got", tok);
    }

    Token nameTok = tokenizer.getToken();
    if(!tok.isName()){
        die("Parser::func_def_statement()", "Expected a function name, instead got", tok);
    }

    tok = tokenizer.getToken();
    if(!tok.isOpenParen()){
        die("Parser::func_def_statement()", "Expected an open parens, instead got", tok);
    }

    std::vector<std::string> functionParams;
    tok = tokenizer.getToken();
    if(tok.isCloseParen()){ } // no parameters, continue parsing
    else{
        tokenizer.ungetToken();
        functionParams = parameter_list(); // return parameters
        tok = tokenizer.getToken();
        if(!tok.isCloseParen()){
            die("Parser::func_def_statement()", "Expected a close parens, instead got", tok);
        }
    }

    tok = tokenizer.getToken();
    if(!tok.isColon()){
        die("Parser::func_def_statement()", "Expected a colon, instead got", tok);
    }

    Statements* func_block_statements = func_suite();
    std::cout << "Returning new FunctionDefinitionStatement***\n";
    return new FunctionDefinitionStatement(nameTok.getName(), functionParams, func_block_statements);

}


// ID '(' { testList } ')'
CallFunctionNode* Parser::call(){
    std::cout << "ENTERED Parser::call()\n";
    Token nameTok = tokenizer.getToken();
    if(!nameTok.isName()){
        die("Parser::call()", "Expected an name token, instead got", nameTok);
    }

    Token tok = tokenizer.getToken();
    if(!tok.isOpenParen()){
        die("Parser::call()", "Expected an open paren token, instead got", nameTok);
    }

    std::vector<ExprNode*> functionArgs;
    tok = tokenizer.getToken();
    if(tok.isCloseParen()){ // no arguements
    /*
        tok = tokenizer.getToken();
        if(!tok.isCloseParen()){
            std::cout << "IN THE FIRST CLAUSE\n";
            die("Parser::call()", "Expected a close parens, instead got", tok);
        }
        */
    }
    else{ // we have arguments
        tokenizer.ungetToken();
        functionArgs = testList(); // return arguements
        tok = tokenizer.getToken();
        if(!tok.isCloseParen()){
            std::cout << "IN THE SECOND CLAUSE\n";
            die("Parser::call()", "Expected a close parens, instead got", tok);
        }
    }

    // the call function node's evaulate will handle the call object responsibilites
    CallFunctionNode* func_node = new CallFunctionNode(nameTok);
    func_node->setFunctionArgs(functionArgs);
    return func_node;
}




// ID {',' ID}*
std::vector<std::string> Parser::parameter_list(){ // not recursive
    std::vector<std::string> string_vector;
    Token tok = tokenizer.getToken();
    if (!tok.isName()){
        die("Parser::parameter_list", "expected a name token, instead got ", tok);
    }
    
    string_vector.push_back( tok.getName() ); // adds the necessary 1st parameter

    tok = tokenizer.getToken();
    while(tok.isComma()){
        tok = tokenizer.getToken();
        if (!tok.isName()){
            die("Parser::parameter_list", "expected a name token, instead got ", tok);
        }
        else{
            string_vector.push_back( tok.getName() );
        }
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();

    return string_vector;
}



ExprNode* Parser::test(){
    return or_test();
}



ExprNode* Parser::or_test(){
    ExprNode *left = and_test();
    Token tok = tokenizer.getToken();
    while(tok.isBooleanOr()){
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


// not_test {'and' not_test}*
ExprNode* Parser::and_test(){
    ExprNode *left = not_test();
    Token tok = tokenizer.getToken();
    while(tok.isBooleanAnd()){
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = not_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;

}


// 'not' not_test | compairson
// unary infix node, so 
ExprNode* Parser::not_test(){
    ExprNode* left = nullptr;
    Token tok = tokenizer.getToken();
    if(tok.isBooleanNot()){
        UnaryExprNode *p = new UnaryExprNode(tok);
        //p->right() = nullptr;
        p->left() = not_test();
        left = p;
    }
    else{
        tokenizer.ungetToken();
        left = compairson();
    }
    return left;
}

// arith_expr {comp_op arith_expr}*
ExprNode* Parser::compairson(){
    ExprNode *left = arith_expr();
    Token tok = tokenizer.getToken();
    if(tok.isCompairsonOperator()){
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = arith_expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

// CURRENTLY NOT USED, 
// returns one of the following operators in an OperatorNode:
// '<' | '>' | '==' | '>=' | '<=' | '!=' 
ExprNode* Parser::comp_op(){
    Token tok = tokenizer.getToken();
    if(tok.isCompairsonOperator()){
        OperatorNode* op_ptr = new OperatorNode(tok);
        return op_ptr;
    }
    else{
        die("Parser::comp_op()", "Expected a compairson operator, got", tok);
    }
}




ExprNode *Parser::arith_expr() {
    // This function parses the grammar rules:

    // <arith_expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:
    // However, the implementation makes the <mult-op> left associative.
    ExprNode *left = atom();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isIntegerDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = atom();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::atom() {
    // This function parses the grammar rules:

    // <atom> -> [0-9]+
    // <atom> -> [_a-zA-Z]+
    // <atom> -> (<arith_expr>)
    // negative symbols would be recognized here ?

    Token tok = tokenizer.getToken();

    if (tok.isNumeric())
        return new NumberNode(tok);
    else if( tok.isString())
        return new StringNode(tok);
    else if( tok.isFuncName()){
        tokenizer.ungetToken();
        return call();
    }
    else if( tok.isName() )
        return new Variable(tok);
    else if (tok.isOpenParen()) {
        //ExprNode *p = relExpr();
        ExprNode *p = test();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::atom", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::atom", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
