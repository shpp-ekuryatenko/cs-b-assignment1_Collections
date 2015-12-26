/********************************************************************************************
* File: Calculator.cpp
* ----------------------
* v.2 2015/12/24 - code reformatted
*
* Program gets formula string from user in console, and calculates
* it's due to Shunting-Yard algorythm.
********************************************************************************************/

#include <iostream>
#include <cmath>
#include "strlib.h"
#include "console.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "MyStack.h"

using namespace std;

/*------------------------------------------------------------------------------------------//
 * Declarations.
 * -----------------------------------------------------------------------------------------*/

void setScannerOptions(TokenScanner & scanner);
double getFormulaResult(string formula);
double scanFormulaString(TokenScanner& scanner,
                         bool &bracketsOpened);
void sortTokenByStacks(string token,
                       MyStack<double>& numbersStack,
                       MyStack<string>& operatorsStack);
double getSimpleCalculationResult(double num1,
                                  string operatorToken,
                                  double num2);
double getFinalStacksResult(MyStack<double> numbersStack,
                            MyStack<string> operatorsStack);
void calculateResultForTopStacksValues(MyStack<double>& numbersStack,
                                       MyStack<string>& operatorsStack);
int getOperatorPriority(string operatorToken);
double powFunction(TokenScanner& scanner);
double sqrtFunction(TokenScanner& scanner);

/* Global variable: failFlag
 * --------------------------
 * Rises if there are some fails are obtained during program.
 * It's used globaly to sipmlifies code. */
bool FAIL_FLAG = false;

/*------------------------------------------------------------------------------------------//
 * Implementation section.
 * -----------------------------------------------------------------------------------------*/

int main() {
    setConsoleWindowTitle("Calculator");
    while (true) {
        /* User's formula input  */
        string formula = getLine("TYPE YOUR FORMULA: ");
        FAIL_FLAG = false;
        /* Main programm process */
        double result = getFormulaResult(formula);
        cout << "= " << result << endl;
    }
    return 0;
}

/* Function: getUserFormulaResult()
 * --------------------------------
 * Creates TokenScanner, and invokes main process function formulaStringScanning()
 * for this scanner. Returns double of calculated result. In case of process
 * faults zero result is returned, and fail message is shown.
 *
 * @param formula User formula to process */
double getFormulaResult(string formula) {
    TokenScanner scanner(formula);
    setScannerOptions(scanner);
    bool bracketsOpenedFlag = false;//Initiate flag for next recursive call

    double result = scanFormulaString(scanner, bracketsOpenedFlag);

    if (FAIL_FLAG) {//Shows if there were some errors due to calculation process */
        cout << "   - CALCULATION FAULT!" << endl;
    }

    return result;
}

/* Function: scanFormulaString()
 * -----------------------------
 * Recursively scanns formula string and sorts each token through stacks due to shunting-yard
 * algorithm. If "()" appear in this recursion  it controls brackets condition. Detects two
 * pow, sqrt - additional library function in user  formula. Breaks process due to global
 * failFlag. Returns double of calculated result. In case of process faults zero result
 * is returned, and fail message is shown.
 *
 * @param scanner               Scanner for main formula string
 * @param bracketsOpenedBefore  Brackets were opened before this recursion invocation */
double scanFormulaString(TokenScanner& scanner, bool& bracketsOpenedBefore) {
    if (FAIL_FLAG) {
        return 0; //Global flag apearance
    }

    MyStack<double> numbersStack; //Stacks for Shunting-Yard process
    MyStack<string> operatorsStack; //in current recursion invocation
    string token = "";
    bool bracketsOpenedHere = false;//Rises if "(" appear in this recursion

    /* Main string scanning. It stops due to conditions:
     * - end of formula string
     * - closed brackets condition
     * - FAIL_FLAG appearance                       */
    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();

        if (token == "pow") {
            /* Lunches library pow function process  */
            double powResult = powFunction(scanner);
            numbersStack.push(powResult);
        } else if (token == "sqrt") {
            /* Lunches library sqrt function process  */
            double sqrtResult = sqrtFunction(scanner);
            numbersStack.push(sqrtResult);
        } else {//Numbers, brackets or garbage symbol occur cases
            if (token == "(") {
                bracketsOpenedHere = true;
                /* Calls new formula recursion for this scanner */
                numbersStack.push(scanFormulaString(scanner, bracketsOpenedHere));
            } else if (token == ")") {
                if (bracketsOpenedBefore) { //Brackets are closed correctly
                    bracketsOpenedBefore = false;//it is end of this recursion
                    break;
                } else {//Token is ")" and no bracketsOpenedBefore flag
                    FAIL_FLAG = true; //Brackets weren't opened
                    cout << "   - NOT OPENED BRACKETS! " << endl;
                    break; //Break to show  error to user
                }
            } else {
                if (FAIL_FLAG) {
                    break;//Break if some faults appear in this recursion
                }
                /* Call Shunting-Yard sorting for this token  */
                sortTokenByStacks(token, numbersStack, operatorsStack);
            }
        }
    }//End of while(scanner.hasMoreTokens())

    /* Shunting-yard final calculation */
    if (bracketsOpenedBefore) {//If brackets haven't been closed in this recursion
        cout << "   - NOT CLOSED BRACKETS! " << endl;
        FAIL_FLAG = true;
        return 0;
    } else {
        if (FAIL_FLAG) {//Main scanning cycle was breaked by some faults
            return 0;
        } else {
            /* Final stacks processing */
            return getFinalStacksResult(numbersStack, operatorsStack);
        }
    }
}

/* Function: sortTokenByStacks()
 * -----------------------------
 * Sort this token through the stacks. If token is number - push it to numbersStack.
 * If token is valid operator token - process it due to Shunting-Yard conditions.
 *
 * @param token             Current token in formula string
 * @param numbersStack      Stack of number values for current recursion
 * @param operatorsStack    Stack of operators for current recursion */
void sortTokenByStacks(string token,
                       MyStack<double>& numbersStack,
                       MyStack<string>& operatorsStack) {
    if (stringIsDouble(token)) {//Token is number
        double number = stringToDouble(token);

        /* Fix 8^(-8 + 3) problem */
        if ((operatorsStack.size() == 1) && (operatorsStack.peek() == "-") && (numbersStack.isEmpty())) {
            number = -1 * number;
            operatorsStack.pop();
        }

        numbersStack.push(number);
    } else {//Token is operator
        if (operatorsStack.isEmpty()) {
            operatorsStack.push(token);
        } else {
            string topOperator = operatorsStack.peek();

            if (getOperatorPriority(topOperator) < getOperatorPriority(token)) {
                /* Due to Shunting-Yard condition - save this token  */
                operatorsStack.push(token);
            } else {
                if (!FAIL_FLAG) {
                    /* Get result for two top numbers with top
                     * operator, and push result into numbersStack */
                    calculateResultForTopStacksValues(numbersStack, operatorsStack);
                    /* Sort current token again  */
                    sortTokenByStacks(token, numbersStack, operatorsStack);
                }
            }
        }
    }
}

/* Function: getFinalStacksResult()
 * ---------------------------------
 * Calculates main result, due to stacks, for current formulaStringScanning() recursion
 * stage.
 * Precondition: it's end of main formula string or brackets closed process.
 *
 * @param numbersStack      Stack of number values for current recursion
 * @param operatorsStack    Stack of operators for current recursion */
double getFinalStacksResult(MyStack<double> numbersStack,
                            MyStack<string> operatorsStack) {
    /* Checks if stacks ready */
    if ((numbersStack.size() - operatorsStack.size()) != 1) {
        cout << "   - CHECK YOUR INPUT, NOT ENOUGH NUMBERS IN FORMULA!" << endl;
        FAIL_FLAG = true;
    }

    /* Lunches calculations for all remain values from stacks */
    while (!operatorsStack.isEmpty()) {
        if (FAIL_FLAG) {
            break;//Some fails occur in cycle
        }
        /* Get result for two top numbers with top
         * operator, and push result into numbersStack */
        calculateResultForTopStacksValues(numbersStack, operatorsStack);
    }

    /* If all operators are processed -
     * end result value remains at top of numbers stack */
    if (FAIL_FLAG) {
        return 0;
    } else {
        return numbersStack.pop();
    }
}

/* Function: powFunction()
 * ------------------------
 * Library pow function execution. Expects sequentally:
 * - brackets, param1, coma, param2, brackets.
 *
 * @param scanner      TokenScanner for user formula */
double powFunction(TokenScanner& scanner) {
    bool powFailFlag = false;
    string token = "";
    string powParam_1 = ""; //pow function param1
    string powParam_2 = ""; //pow function param2
    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();//waiting for brackets at this place
        if (token == "(") {
            powParam_1 = scanner.nextToken();
            token = scanner.nextToken();//waiting for coma at this place
            if (token == ",") {
                powParam_2 = scanner.nextToken();
                token = scanner.nextToken();//waiting for brackets - end of pow params input
                if (token == ")") {
                    break;
                } else {
                    powFailFlag = true;
                }
            } else {
                powFailFlag = true;
            }
        } else {
            powFailFlag = true;
        }

        if (powFailFlag) {
            cout << "   - POW FUNCTION INPUT FAULT!" << endl;
            FAIL_FLAG = true;
            return 0;
        }
    }

    /* Returns pow function result */
    double param1 = stringToDouble(powParam_1);
    double param2 = stringToDouble(powParam_2);
    return pow(param1, param2);
}

/* Function: sqrtFunction()
 * -------------------------
 * Library sqrt function execution. Expects for brackets and involves
 * formulaStringScanning process for expression in brackets. Return sqrt function result
 * for obtained value in brackets.
 *
 * @param scanner   TokenScanner for user formula */
double sqrtFunction(TokenScanner& scanner) {
    bool sqrtFailFlag = false;
    string token = "";
    double sqrtParam = 0;

    if (scanner.hasMoreTokens()) {
        token = scanner.nextToken();//waiting for brackets at this place
        if (token == "(") {
            bool bracketsOpened = true;
            /* Calls internal scanFormulaString process for expression in brackets */
            sqrtParam = scanFormulaString(scanner, bracketsOpened);
        } else {
            sqrtFailFlag = true;
        }
    }

    if (sqrtFailFlag) {
        cout << "   - SQRT FUNCTION FAULT" << endl;
        FAIL_FLAG = true;
        return 0;
    }

    /* Returns sqrt function result */
    return sqrt(sqrtParam);
}

/* Function: calculateResultForTopStacksValues()
 * ---------------------------------------------
 * Makes single calculation for two top numbers in stack, and return result to
 * numbersStack back.
 *
 * @param numbersStack      Stack of number values for current recursion
 * @param operatorsStack    Stack of operators for current recursion */
void calculateResultForTopStacksValues(MyStack<double>& numbersStack,
                                       MyStack<string>& operatorsStack) {
    /* Stacks validation for calculating process */
    if ((numbersStack.size() - operatorsStack.size()) != 1) {
        cout << "   - CHECK YOUR INPUT, NOT ENOUGH NUMBERS IN FORMULA!" << endl;
        FAIL_FLAG = true;
    } else {
        /* Calculating process */
        double number2 = numbersStack.pop();
        double number1 = numbersStack.pop();
        string topOperator = operatorsStack.pop();

        double result = getSimpleCalculationResult(number1, topOperator, number2);

        numbersStack.push(result);
    }
}

/* Function: getOperatorPriority()
 * -------------------------------
 * Is called by sortTokenByStacks() function to get param operator priority */
int getOperatorPriority(string operatorToken) {
    int result = 0;

    if (operatorToken == "-") {
        result = 0;
    } else if (operatorToken == "+") {
        result = 0;
    } else if (operatorToken == "*") {
        result = 1;
    } else if (operatorToken == "/") {
        result = 1;
    } else if (operatorToken == "%") {
        result = 1;
    } else if (operatorToken == "^") {
        result = 2;
    } else {
        FAIL_FLAG = true;
        cout << "   - UNKNOWN OPERATOR!" << endl;
    }

    return result;
}

/* Function: getSimpleCalculationResult()
 * ---------------------------------------
 * Is called by calculateResultForTopStacksValues() function to make single
 * operation with two param numbers due to this param operator */
double getSimpleCalculationResult(double num1, string operatorToken, double num2) {
    double result = 0;

    if (operatorToken == "+") {
        result = num1 + num2;
    } else if (operatorToken == "-") {
        result = num1 - num2;
    } else if (operatorToken == "*") {
        result = num1 * num2;
    } else if (operatorToken == "/") {
        if (num2 == 0) {
            cout << "   - ZERO DIVISION!" << endl;
            FAIL_FLAG = true;
            result = 0;
        }
        result = num1 / num2;
    } else if (operatorToken == "%") {
        result = (int)num1 % (int)num2;
    } else if (operatorToken == "^") {
        result = pow(num1, num2);
    } else {
        cout << "   - OPERATOR FAIL!" << endl;
        result = 0;
    }

    return result;
}

/* Function: setScannerOptions()
 * ------------------------------
 * Sets scanner features. */
void setScannerOptions(TokenScanner & scanner) {
    scanner.ignoreWhitespace();
    /* Return numbers */
    scanner.scanNumbers();
    scanner.addOperator("%");
    scanner.addOperator("/");
    scanner.addOperator("*");
    scanner.addOperator("(");
    scanner.addOperator(")");
}
