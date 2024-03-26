#include <string>
#include <vector>
#include <fstream>

using namespace std;

/**
 * Abstract class for Asynchronous Syntax Tree
 * */
class ASTNode
{
public:
    static int tempIndex;
    virtual string generateCode(ofstream &output) = 0;
 };

/**
 * Stores identifier of variables. Can generate code with temp variables
 * */
class IdentifierNode : public ASTNode
{
public:
    string name;
    IdentifierNode(string _name);
    string generateCode(ofstream &output);
    string getID();
};


/**
 * Stores values for numbers. generateCode() function returns the integer value
 * */
class NumberNode : public ASTNode
{
public:
    string value;
    NumberNode(string _value);
    string generateCode(ofstream &output);
};

/**
 * Node for choose expression, stores the expressions inside parantheses. 
 * Generates code for both expressions and choose function.
 * */
class ChooseNode : public ASTNode
{
public:
    ASTNode *expr1, *expr2, *expr3, *expr4;
    static int chooseIndex;
    ChooseNode(ASTNode *_expr1, ASTNode *_expr2, ASTNode *_expr3, ASTNode *_expr4);
    string generateCode(ofstream &output);
};

/**
 * Node for binary operations. Stores the operation type, right and left handside as expressions
 * and generates code for the calculation
 * */
class BinaryOperationNode : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    char operation;

    BinaryOperationNode(ASTNode *_left, ASTNode *_right, char _operation);
    string generateCode(ofstream &output);
};

/**
 * Node for print statements. Generates code for print statement and expression inside the statement
 * */
class PrintNode : public ASTNode
{
public:
    ASTNode *expr;
    PrintNode(ASTNode *_expr);
    string generateCode(ofstream &output);
};

/**
 * Node to store conditional statements. Stores the condition, conditional type and statements
 * inside the code block and generates code for all of them
 * */
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


/***********
 * IdentifierNode
 *************/

IdentifierNode::IdentifierNode(string _name)
{
    this->name = _name;
}

/**
 * Loads given variable to a temp variable
 * */
string IdentifierNode::generateCode(ofstream &output)
{
    string id = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << id << " = load i32, i32* %" << name << "\n";

    return id;
}

/**
 * Returns the identifier of the variable for assignment statements
 * */
string IdentifierNode::getID()
{
    return this->name;
}

///////////////////////////////////////////////////////////////////////////////

/*******************************
 * NumberNode
 * **************/

NumberNode::NumberNode(string _value)
{
    this->value = _value;
}

/**
 * Returns the value of number for expression code generation
 * */
string NumberNode::generateCode(ofstream &output)
{
    return this->value;
}

/****************
 * ChooseNode
 * **************/

ChooseNode::ChooseNode(ASTNode *_expr1, ASTNode *_expr2, ASTNode *_expr3, ASTNode *_expr4)
{
    this->expr1 = _expr1;
    this->expr2 = _expr2;
    this->expr3 = _expr3;
    this->expr4 = _expr4;
}

/**
 * Generates code for choose function with expressions inside it.
 * It uses a branching approach to calculate the result of choose function
 * */
string ChooseNode::generateCode(ofstream &output)
{
    string labelName = "choose_" + to_string(chooseIndex);
    chooseIndex++;

    string ret_var = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << ret_var << " = alloca i32\n"
           << "\tstore i32 0, i32* " << ret_var << "\n";

    string labelif = labelName + "if";
    string el = labelName + "else";
    string elif = labelName + "elif";
    string end = labelName + "end";

    string id1 = this->expr1->generateCode(output);

    output << "\tbr label %" << labelif << "\n\n";

    //IF COND
    output << labelif << ":\n";
    string tempVar1 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << tempVar1 << " = icmp eq i32 " << id1 << ", 0\n";
    output << "\tbr i1 " << tempVar1 << ", label %" << labelif << "body, label %" << elif << "\n\n";

    //IF BODY
    output << labelif << "body:\n";
    string tempVar2 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    string id2 = this->expr2->generateCode(output);
    output << "\tstore i32 " << id2 << ", i32* " << ret_var << "\n"
           << "\tbr label %" << end << "\n\n";

    //ELSE IF COND
    output << elif << ":\n";
    string tempVar3 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << tempVar3 << " = icmp sgt i32 " << id1 << ", 0\n";
    output << "\tbr i1 " << tempVar3 << ",label %" << elif << "body, label %" << el << "\n\n";

    //ELSE IF BODY
    output << elif << "body:\n";
    string tempVar4 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    string id3 = this->expr3->generateCode(output);
    output << "\tstore i32 " << id3 << ", i32* " << ret_var << "\n"
           << "\tbr label %" << end << "\n\n";

    //ELSE BODY
    output << el << ":\n";
    string tempVar5 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    string id4 = this->expr4->generateCode(output);
    output << "\tstore i32 " << id4 << ", i32* " << ret_var << "\n"
           << "\tbr label %" << end << "\n\n";

    //END
    output << end << ":\n";
    string tempVar6 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << tempVar6 << " = load i32, i32* " << ret_var << "\n";

    return tempVar6;
}

/****************
 * BinaryOperationNode
 * **************/

BinaryOperationNode::BinaryOperationNode(ASTNode *_left, ASTNode *_right, char _operation)
{
    this->left = _left;
    this->right = _right;
    this->operation = _operation;
}


/**
 * Generates code for binary operations by calling left and right handside code generation first
 * */
string BinaryOperationNode::generateCode(ofstream &output)
{
    string tempId, operand1, operand2, opType;
    operand1 = left->generateCode(output);  //Generate left side code
    operand2 = right->generateCode(output); //Generate right side code

    tempId = "%temp_var" + to_string(tempIndex);
    tempIndex++;

    //Checks operation type
    switch (operation)
    {
    case ('+'):
        opType = "add";
        break;
    case ('-'):
        opType = "sub";
        break;
    case ('*'):
        opType = "mul";
        break;
    case ('/'):
        opType = "sdiv";
        break;
    }

    output << "\t" << tempId << " = " << opType << " i32 " << operand1 << " , " << operand2 << "\n";

    return tempId;
}


/****************
 * PrintNode
 * **************/

PrintNode::PrintNode(ASTNode *_expr)
{
    this->expr = _expr;
}

/**
*  Generates print code by firstly calling code generation of expression inside print statement
* */
string PrintNode::generateCode(ofstream &output)
{
    string text = expr->generateCode(output);
    output << "\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 " << text << ")\n";
    return "";
}


/****************
 * ConditionalNode
 * **************/

ConditionalNode::ConditionalNode(int _type, ASTNode *_condition)
{
    this->type = _type;
    this->condition = _condition;
}


/**
 * Generates code for conditional statements, first generates the condition code
 * then generates the code in {} block
 * */
string ConditionalNode::generateCode(ofstream &output)
{

    string conditionName = "cond_" + to_string(conditionalIndex);
    conditionalIndex++;

    output << "\tbr label %" << conditionName << "entry\n\n";
    output << conditionName << "entry:\n";

    string id = condition->generateCode(output); //Generates condition code
    string tempVar = "%temp_var" + to_string(tempIndex);
    tempIndex++;

    output << "\t" << tempVar << " = icmp ne i32 " << id << ", 0\n";
    output << "\tbr i1 " << tempVar << ", label %" << conditionName << "body, label %" << conditionName << "end\n\n";

    output << conditionName << "body:\n";

    //Generates code for the statements inside conditional
    for (auto expression : statements)
    {
        expression->generateCode(output);
    }

    //checks type op conditional and adjusts the code for it
    if (this->type == 0)
    {
        output << "\tbr label %" << conditionName << "end\n\n";
    }
    else
    {
        output << "\tbr label %" << conditionName << "entry\n\n";
    }

    output << conditionName << "end:\n";
    tempIndex++;

    return "";
}


/****************
 * AssignNode
 * **************/

AssignNode::AssignNode(IdentifierNode *id, ASTNode *expr)
{
    this->identifier = id;
    this->expr = expr;
}

/**
 * Code generation for assignment statements
 * */
string AssignNode::generateCode(ofstream &output)
{
    string value = expr->generateCode(output);
    string id = identifier->getID();
    output << "\tstore i32 " << value << ", i32* %" << id << "\n";
    return "";
}
