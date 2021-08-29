#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;

bool convert(string infix, const bool values[], string& postfix);


int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	string postfix1 = postfix;
	if (convert(infix, values, postfix1) == false)
		return 1;
	stack<bool> operandStack;
	bool lastExclamation = false;
	for (int i = 0; i < postfix1.size(); i++)
	{
		char ch = postfix1[i];
		if (isdigit(ch))
		{
			if (lastExclamation)
			{
				bool operand1 = operandStack.top();
				operandStack.push(!operand1);
				lastExclamation = false;
			}
			else
				operandStack.push(values[ch - '0']);

		}
		else if (ch == '!')
		{
			lastExclamation = true;
		}
		else //ch is a binary operator or unary operator
		{
			if (operandStack.empty())
				return 1;
			bool operand2 = operandStack.top();
			operandStack.pop();
			if (operandStack.empty())
				return 1;
			bool operand1 = operandStack.top();
			operandStack.pop();
			if (ch == '|')
				operandStack.push(operand1 || operand2);
			else if (ch == '&')
				operandStack.push(operand1 && operand2);
		}
	}
	result = operandStack.top();
	operandStack.pop();
	if (!operandStack.empty())
		return 1;
	else 
	{
		postfix = postfix1;
		return 0;
	}
}
// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of the expression (where in that expression,
//   each digit k represents element k of the values array), and
//   return zero.  If infix is not a syntactically valid expression,
//   return 1.  (In that case, postfix may or may not be changed, but
//   result must be unchanged.)

bool convert(string infix, const bool values[], string& postfix1)
{
	string postfixTemp = "";
	stack<char> operandStack;
	int digitCount = 0;
	int operatorCount = 0;
	int openParenCount = 0;
	int closeParenCount = 0;
	int previousType = 0;//0 is operator, 1 is digit, 2 is (, 3 is ) 
	for (int i = 0; i < infix.size(); i++)
	{
		char ch = infix[i];
		switch (ch)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (previousType == 1 || previousType == 3)
				return false;
			postfixTemp += ch;
			digitCount++;
			previousType = 1;
			break;
		case '(':
			operandStack.push(ch);
			openParenCount++;
			previousType = 2;
			break;
		case '!': //spec implies to treat ! the same as (
			if (previousType == 1 || previousType == 3)
				return false;
			operandStack.push(ch);
			operatorCount++;
			previousType = 0;
			break;
		case ')':
			closeParenCount++;
			if (closeParenCount > openParenCount)
				return false;
			if (previousType == 0 || previousType == 2)
				return false;
			//pop stack until matching '('
			while (operandStack.top() != '(')
			{
				postfixTemp += operandStack.top();
				operandStack.pop();
			}
			operandStack.pop(); //remove the ')'
			previousType = 3;
			break;
		case '|':
		case '&':
			if (previousType == 0 || previousType == 2)
				return false;
			while (!operandStack.empty() && operandStack.top() != '(')
			{
				if (ch == '&' && operandStack.top() == '|')
					break; //this is the only case that the precendence of ch > precedence of stack top
				postfixTemp += operandStack.top();
				operandStack.pop();
			}
			operandStack.push(ch);
			operatorCount++;
			previousType = 0;
			break;
		case ' ':
		case '  ':
		case '\n':
			break; //do nothing if its whitespace, so it is never added to the postfix and thus postfix has no whitespace
		default:
			return false;
			break;
		}
	}
	while (!operandStack.empty())
	{
		postfixTemp += operandStack.top();
		operandStack.pop();
	}
	if (digitCount == 0)
		return false;
	if (digitCount >= 2 && operatorCount == 0)
		return false;
	if (operatorCount == 1 && digitCount == 1)
		return false;
	postfix1 = postfixTemp;
	return true;
}

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		  true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate(" &6", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("()4", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("x+5", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("2|3|4", ba, pf, answer) == 0
		&& pf == "23|4|" && answer);
	assert(evaluate("2|(3|4)", ba, pf, answer) == 0
		&& pf == "234||" && answer);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9" && answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	cout << "Passed all tests" << endl;
}