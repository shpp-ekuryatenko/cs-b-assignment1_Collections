/********************************************************************************************
 * File: Calculator.cpp
 * ----------------------
 * v.1 2015/11/22
 * Program gets formula string from user in console, and clculates
 * it's due to Shunting-Yard algorythm.
 ********************************************************************************************/

#include <iostream>
#include <cmath>
#include "console.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "myStack.h"

using namespace std;


/* Declarations
 * -----------------------------------------------------------------------------------------*/
void scanCPlusPlusTokens(TokenScanner & scanner);

double formulaProcess(string formula, double xValue);
double formulaStringScanning(TokenScanner& scanner,                            
                             bool &bracketsOpened,
                             double xValue);
void sortTokenByStacks(string token,
                       MyStack<double>& stackNumbers,
                       MyStack<string>& stackOperators);
double singleCalculation(double num1, string operatorToken, double num2);
double getFinalStacksResult(MyStack<double> stackNumbers,
                       MyStack<string> stackOperators);
void twoNumsProcess(MyStack<double>& stackNumbers,
                    MyStack<string>& stackOperators);
int getOperPrecedence(string operatorToken);

double powFunction(TokenScanner& scanner);
double sqrtFunction(TokenScanner& scanner, double xValue);

/* Global variable: failFlag
 * --------------------------
 * Rises if there are some fails are obtained during program.
 * It's used globaly to sipmlifies code. */
bool failFlag = false;

/*------------------------------------------------------------------------------------------//
 * Implementation section.
 * -----------------------------------------------------------------------------------------*/

int main() {
    cout << "TYPE YOUR FORMULA OR PRESS 'q' TO QUIT"  << endl;
    cout << "-----------------------------------------------" << endl;
    while(true){
        /* User's formula input  */
        string formula = getLine();
        /* End of programm condition */
        if(toLowerCase(formula) == "q"){
            break;
        }
        /* Value for x-variable for current formula
         * It may be useful for equation graphics */
        double xValue = 0;
        failFlag = false;
        /* Main programm process */
        double result = formulaProcess(formula, xValue);
        cout << "= " << result << endl;
    }

    cout << "-----------------------------------------------" << endl;
    cout << "PROGRAM IS FINISHED" << endl;
    return 0;
}

/* Function: formulaProcess()
 * -----------------------------------------------------------------------------------------//
 * Creates TokenScanner, and invokes main process function formulaStringScanning()
 * for this scanner. Returns double of calculated result. In case of process
 * faults zero result is returned, and fail message is shown.
 *
 * @param formula User formula to process
 * @param xValue  Value for x variable in user equation */
double formulaProcess(string formula, double xValue){
    TokenScanner scanner(formula);
    scanCPlusPlusTokens(scanner);
    bool bracketsOpenedFlag = false;//Initiate flag for next recursive call
    double result = formulaStringScanning(scanner,
                                          bracketsOpenedFlag,
                                          xValue);
    if(failFlag){
        /* Shows if there were some errors due to calculation process */
        cout << "   - CALCULATION FAULT!" << endl;
    }

    return result;
}

/* Function: formulaStringScanning()
 * -----------------------------------------------------------------------------------------//
 * Recursively scanns formula string and sorts each token through stacks due to shunting-yard
 * algorithm. If "()" appear in this recursion  it controls brackets condition. Detects two
 * pow, sqrt - additional library function in user  formula. Breaks process due to global
 * failFlag. Returns double of calculated result. In case of process faults zero result
 * is returned, and fail message is shown.
 *
 * @param scanner               Scanner for main formula string
 * @param bracketsOpenedBefore  Brackets were opened before this recursion invocation
 * @param xValue                Value for x variable in user equation */
double formulaStringScanning(TokenScanner& scanner,
                             bool &bracketsOpenedBefore,
                             double xValue){
    if(failFlag){return 0;}//Global flag apearance
    else{
        MyStack<double> stackNumbers;//Stacks for current recursion invocation
        MyStack<string> stackOperators;
        string token = "";
        bool bracketsOpenedHere = false;//Rises if "(" appear in this recursion
        while(scanner.hasMoreTokens()){
            token = scanner.nextToken();
            if(token == "x"){
                /* Sabstitute x-token by user xValue param */
                stackNumbers.push(xValue);
            }
            else if(token == "pow"){
                /* Lunches library pow function process  */
                stackNumbers.push(powFunction(scanner));
            }else if(token == "sqrt"){
                /* Lunches library sqrt function process  */
                stackNumbers.push(sqrtFunction(scanner, xValue));
            }else{//Brackets case
                    if(token == "("){
                        bracketsOpenedHere = true;
                        /* Calls new formula recursion for this scanner */
                        stackNumbers.push(formulaStringScanning(scanner,
                                                                bracketsOpenedHere,
                                                                xValue));
                    }else if(token == ")"){
                            if(bracketsOpenedBefore){        //Brackets are closed correctly
                                bracketsOpenedBefore = false;//it is end of this recursion
                                break;
                            }
                            else{       //Token is ")" and no bracketsOpenedBefore flag
                                failFlag = true;        //Brackets were opened
                                cout <<  "   - NOT OPENED BRACKETS! " << endl;
                                break;                  //Break to show  error to user
                            }
                    }else{
                        if(failFlag)break;
                        /* If no fails, and token is valid - lunches
                         * Shunting-Yard sorting  */
                        sortTokenByStacks(token, stackNumbers, stackOperators);
                    }
            }//End of Brackets case else statement
        }//End of while(scanner.hasMoreTokens())

        /* Shunting-yard final calculation */
        if(bracketsOpenedBefore){//If brackets haven't been closed in this recursion
             cout <<  "   - NOT CLOSED BRACKETS! " << endl;
             failFlag = true;
             return 0;
        }else{
            if(failFlag){//There were some other fails
                return 0;
            }else{
                /* Calculate main result, due to stacks, for current calculation stage. */
                return getFinalStacksResult(stackNumbers, stackOperators);
            }
        }
    }//End of else statement (no failFlag at recursion start)
}

/* Function: sortTokenByStacks()
 * Usage: is called by formulaStringScanning() for single token;
 * -----------------------------------------------------------------------------------------//
 * Sort this token through the stacks. If token is number - push it to stackNumbers.
 * If token is valid operator token - process it due to Shunting-Yard conditions.
 *
 * @param token             Current token in formula string
 * @param stackNumbers      Stack of number values for current recursion
 * @param stackOperators    Stack of operators for current recursion */
void sortTokenByStacks(string token,
                MyStack<double> &stackNumbers,
                MyStack<string> &stackOperators){
    if(stringIsDouble(token)){ //Token is number
        double num = stringToDouble(token);
        stackNumbers.push(num);//Just save token to stack
    }else{// Token is operator
        /* Main operators process */
        if(stackOperators.isEmpty()){//Empty - push there without conditions
            stackOperators.push(token);
        }else{//If there are some operators in stack
            string topOper = stackOperators.peek();//Get top operator
            if(getOperPrecedence(topOper) < getOperPrecedence(token)){
                /* Top operator precednce is
                 * weaker then this token operator - just save this token */
                stackOperators.push(token);
            }else{
                /* Top operator precednce is higher - evaluate two top numbers
                 * with top operator, and sort current token again */
                if(!failFlag){//If there some fails - break this function
                 /* Main calculation for top numbers and top operator  */
                 twoNumsProcess(stackNumbers, stackOperators);
                 /* Call sorting again to process current token operator  */
                 sortTokenByStacks(token, stackNumbers, stackOperators);
                }
            }
        }
    }
}

/* Function: getFinalStacksResult()
 * Usage: is called by formulaStringScanning() at the end of current recursion.
 * -----------------------------------------------------------------------------------------//
 * Calculates main result, due to stacks, for current formulaStringScanning() recursion
 * stage.
 * Precondition: it's end of main formula string or brackets closed process.
 *
 * @param stackNumbers      Stack of number values for current recursion
 * @param stackOperators    Stack of operators for current recursion */
double getFinalStacksResult(MyStack<double> stackNumbers,
                       MyStack<string> stackOperators){
    /* Stacks elements validation checking for calculating process */
    if((stackNumbers.size() - stackOperators.size()) != 1){
        cout << "   - CHECK YOUR INPUT, NOT ENOUGH NUMBERS IN FORMULA!" << endl;
        failFlag = true;
    }
    /* Lunches calculations for all remain values from stacks */
    while(!stackOperators.isEmpty()){
        if(failFlag) break;//Some fails appear during calculations
        /* Calculation for two top stack numbers and single top operator  */
        twoNumsProcess(stackNumbers, stackOperators);
    } 
    /* If all operators are processed - end result value remains at top of numbers stack */
    if(!failFlag){
        return stackNumbers.pop();
    }else{return 0;}
}

/* Function: powFunction()
 * Usage: is called by formulaStringScanning() function if it detect pow token in formula
 * -----------------------------------------------------------------------------------------//
 * Library pow function execution. Believe that it's normalized function -
 * without any variables like "x" or "y" - just pow(<value>, <power for value>):
 * pow(2, 4) = 16
 * Expects brackets, and return pow function result for values in brackets.
 *
 * @param scanner      TokenScanner for user formula */
double powFunction(TokenScanner& scanner){
    bool powFail = false;
    string token = "";
    string num1 = "";
    string num2 = "";
    while(scanner.hasMoreTokens()){
        token = scanner.nextToken();//waiting for brackets at this place
        if(token == "("){
            num1 = scanner.nextToken();//pow function param1
            token = scanner.nextToken();//waiting coma at this place
            if(token == ","){
                num2 = scanner.nextToken();//pow function param2
                token = scanner.nextToken();//waiting for brackets at this place
                if(token == ")"){
                    break;
                }else{powFail = true;}
            }else{powFail = true;}
        }else{powFail = true;}

        if(powFail){
             cout <<  "   - POW FUNCTION INPUT FAULT!" << endl;
             failFlag = true;
             return 0;
        }
    }
    /* Returns pow function result */
    return pow(stringToDouble(num1), stringToDouble(num2));
}

/* Function: sqrtFunction()
 * Usage: is called by formulaStringScanning() function if it detect sqrt token in formula
 * -----------------------------------------------------------------------------------------//
 * Library sqrt function execution. Expects for brackets and involves
 * formulaStringScanning process for expression in brackets. Return sqrt function result
 * for obtained value in brackets.
 *
 * @param scanner   TokenScanner for user formula
 * @param xValue    Value for x variable in user equation */
double sqrtFunction(TokenScanner& scanner, double xValue){
    bool sqrtFail = false;
    string token = "";
    double num1 = 0;
    if(scanner.hasMoreTokens()){
        token = scanner.nextToken();
        if(token == "("){//waiting for brackets at this place
            bool bracketsOpened = true;
            /* Calls internal formulaStringScanning process for expression in brackets */
            num1 = formulaStringScanning(scanner,
                                         bracketsOpened,
                                         xValue);
        }else{sqrtFail = true;}
    }

    if(sqrtFail){
         cout << "   - SQRT FUNCTION FAULT" << endl;
         failFlag = true;
         return 0;
    }

    /* Returns sqrt function result */
    return sqrt(num1);
}

/* Function: twoNumsProcess()
 * Usage: is called by sortTokenByStacks() or getFinalStacksResult() functions
 * -----------------------------------------------------------------------------------------//
 * Makes single calculation for two top numbers in stack, and return result to
 * stackNumbers back.
 *
 *
 * @param stackNumbers      Stack of number values for current recursion
 * @param stackOperators    Stack of operators for current recursion */
void twoNumsProcess(MyStack<double> &stackNumbers, MyStack<string> &stackOperators){
    /* Stacks elements validation checking for calculating process */
    if((stackNumbers.size() - stackOperators.size()) != 1){
        cout << "   - CHECK YOUR INPUT, NOT ENOUGH NUMBERS IN FORMULA!" << endl;
        failFlag = true;
    }else{
        /* Calculating process */
        double num2 = stackNumbers.pop();
        double num1 = stackNumbers.pop();
        string thisOper = stackOperators.pop();
        double result = singleCalculation(num1, thisOper, num2);
        stackNumbers.push(result);
    }
}

/* Function: getOperPrecedence()
 * Usage: is called by sortTokenByStacks() function to get param operator priority */
int getOperPrecedence (string operatorToken){
    int result = 0;
    if(operatorToken == "-")result = 0;
    else if(operatorToken == "+")result = 0;
    else if(operatorToken == "*")result = 1;
    else if(operatorToken == "/")result = 1;
    else if(operatorToken == "%")result = 1;
    else if(operatorToken == "^")result = 2;
    else if(operatorToken == "pow")result = 2;
    else if(operatorToken == "sqrt")result = 2;
    else{
        failFlag = true;
        cout << "   - UNKNOWN OPERATOR!" << endl;
    }
    return result;
}

/* Function: singleCalculation()
 * Usage: is called by twoNumsProcess() function to make single
 * operation with two param numbers due to this param operator */
double singleCalculation(double num1, string operatorToken, double num2){
    double result = 0;
    if(operatorToken == "+"){
        result = num1 + num2;
    }else if(operatorToken == "-"){
        result = num1 - num2;
    }else if(operatorToken == "*"){
        result = num1 * num2;
    }else if(operatorToken == "/"){
        if(num2 == 0){
            cout << "   - ZERO DEVISION!" << endl;
            failFlag = true;
            result = 0;
        }
        result = num1 / num2;
    }else if(operatorToken == "%"){
        result = (int)num1 % (int)num2;
    }else if(operatorToken == "^"){
        result = pow(num1, num2);
    }else if(operatorToken == "pow"){
        result = pow(num1, num2);
    }else{
        cout << "   - OPERATOR FAIL!" << endl;
        result = 0;
    }
    return result;
}

/* Function: scanCPlusPlusTokens()
 * Sets scanner features. */
void scanCPlusPlusTokens(TokenScanner & scanner) {
    scanner.ignoreWhitespace();
    /* Return numbers */
    scanner.scanNumbers();
    scanner.addOperator("%");
    scanner.addOperator("/");
    scanner.addOperator("*");
    scanner.addOperator("(");
    scanner.addOperator(")");
}

