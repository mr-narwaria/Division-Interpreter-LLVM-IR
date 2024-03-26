#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>
using namespace std;


enum TokenType
{
    token_identifier,
    token_number,
    token_operator,
    token_print,
    token_choose,
    token_conditional,
    token_eol,
    token_eof
};

struct Token
{
    string value;
    TokenType type;
    int line;
};

class Tokenizer
{
public:
    ifstream *inputFile;
    char lastChar;
    int line;

    Tokenizer(ifstream *inputFile);
    ~Tokenizer();

    // void tokenizeInput(ifstream &input);
    Token getNextToken();
};

// Abstract class for Asynchronous Syntax Tree
class ASTNode
{
public:
    static int tempIndex;
    virtual string generateCode(ofstream &output) = 0;
 };


// Stores identifier of variables. Can generate code with temp variables
class IdentifierNode : public ASTNode
{
public:
    string name;
    IdentifierNode(string _name);
    string generateCode(ofstream &output);
    string getID();
};



// Stores values for numbers. generateCode() function returns the integer value

class NumberNode : public ASTNode
{
public:
    string value;
    NumberNode(string _value);
    string generateCode(ofstream &output);
};


//  Node for choose expression, stores the expressions inside parantheses. 
//  Generates code for both expressions and choose function.
 
class ChooseNode : public ASTNode
{
public:
    ASTNode *expr1, *expr2, *expr3, *expr4;
    static int chooseIndex;
    ChooseNode(ASTNode *_expr1, ASTNode *_expr2, ASTNode *_expr3, ASTNode *_expr4);
    string generateCode(ofstream &output);
};

// Node for binary operations. Stores the operation type, right and left handside as expressions
// and generates code for the calculation
 
class BinaryOperationNode : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    char operation;

    BinaryOperationNode(ASTNode *_left, ASTNode *_right, char _operation);
    string generateCode(ofstream &output);
};

// Node for print statements. Generates code for print statement and expression inside the statement

class PrintNode : public ASTNode
{
public:
    ASTNode *expr;
    PrintNode(ASTNode *_expr);
    string generateCode(ofstream &output);
};

// Node to store conditional statements. Stores the condition, conditional type and statements
// inside the code block and generates code for all of them

class ConditionalNode : public ASTNode
{
public:
    static int conditionalIndex;
    int type;
    ASTNode *condition;
    vector<ASTNode *> statements;

    ConditionalNode(int _type, ASTNode *_condition);
    string generateCode(ofstream &output);
};

/**
 * Node for assignment statement. Stores identifier and expression 
 * Generates code for assignment statement
 * */
class AssignNode : public ASTNode
{
public:
    IdentifierNode *identifier;
    ASTNode *expr;
    AssignNode(IdentifierNode *id, ASTNode *expr);
    string generateCode(ofstream &output);
};

class Parser
{
public:
    Tokenizer *tokenizer;
    int line, errLine;
    bool error;
    unordered_set<string> variables;
    Token currentToken, lastToken;

    Parser(Tokenizer *_tokenizer);

    ASTNode *parseParanExpr();
    ASTNode *parseFactor();
    ASTNode *parseMorefactor();
    ASTNode *parseTerm();
    ASTNode *parsePrint();
    ASTNode *parseChoose();
    ASTNode *parseMoreterms();
    ASTNode *parseExpr();
    ASTNode *parseStatement();
    ASTNode *parse();

    void syntaxError(int line);
    Token getToken();
};

/**
 * Creates the code for syntax error in given line
 * */
void syntaxError(int line, ofstream &output)
{
    output << "; ModuleID = \'division-interpreter\'\n"
           << "declare i32 @printf(i8*, ...)\n"
           << "@print.str = constant [23 x i8] c\"Line %d: syntax error\\0A\\00\"\n\n"
           << "define i32 @main() {\n"
           << "\tcall i32 (i8*, ...)* @printf(i8* getelementptr ([23 x i8]* @print.str, i32 0, i32 0), i32 " << line << ")\n"
           << "\tret i32 0\n"
           << "}";
}

/**
 * Generates IR code by adding headers to file, allocating space for all variables 
 * and running generateCode() function from AST nodes which creates IR code for its type.
 * Takes parameters ofstream output to print to file, vector<ASTNode> program is the AST
 * and varmap stores all the declared variables which is used to allocate them.
 * */
void generateIR(ofstream &output, vector<ASTNode *> &program, unordered_set<string> &varmap)
{

    //Adding header to .ll file
    output << "; ModuleID = \'division-interpreter\'\n"
           << "declare i32 @printf(i8*, ...)\n"
           << "@print.str = constant [4 x i8] c\"%d\\0A\\00\"\n\n"
           << "define i32 @main() {\n";

    //Allocates declared variables
    for (auto test : varmap)
    {
        output << "\t%" << test << " = alloca i32\n";
    }

    output << "\n";

    //Gives all allocated vars value of 0
    for (auto identifier : varmap)
    {
        output << "\tstore i32 0, i32* %" << identifier << "\n";
    }

    output << "\n";

    //Creates the code from given program
    for (auto expression : program)
    {
        expression->generateCode(output);
    }

    //Finishes the code creation
    output << "\tret i32 0\n"
           << "}";
}

int main(int argc, char *argv[])
{

    vector<ASTNode *> program; //vector to hold Nodes for code generation

    string inputFile = argv[1]; 
    string outputFile = inputFile.substr(0, inputFile.size() - 3) + ".ll";

    ifstream inFile(inputFile);

    Tokenizer *tokenizer = new Tokenizer(&inFile);
    Parser *parser = new Parser(tokenizer);

    //Parse till there is an error or it is end of file
    while (!parser->error && parser->currentToken.type != token_eof)
    {

        ASTNode *node = parser->parse(); //parses file

        if (node != NULL)
            program.push_back(node);
        else
            break;
    }

    inFile.close();

    ofstream outFile(outputFile);

    //if there is an error generate error code
    if (parser->error)
    {
        syntaxError(parser->errLine, outFile);
        return 0;
    }

    //generate code
    generateIR(outFile, program, parser->variables);

    outFile.close();

    return 0;
}