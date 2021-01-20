#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <algorithm>
using namespace std;

int evaluate(string infix, string& postfix, bool& result);
// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of the expression, and return zero.  If
//   infix is not a syntactically valid expression, return 1; in
//   that case, postfix may or may not be changed, but result must
//   be unchanged.


int evaluate(string infix, string & postfix, bool & result)
{
	if (infix == "") {
		return 1;
	}
	int leftcount = 0;
	int rightcount = 0;
	int excount = 0;
	//cerr << "infix before:" << infix << endl;
	//cerr << "infix size before:" << infix.size() << endl;
	infix.erase(std::remove_if(infix.begin(), infix.end(), ::isspace), infix.end());
	//cerr << "infix after:" << infix << endl;
	//cerr << "infix size after:" << infix.size() << endl;
	int size = infix.size();
	postfix = "";
	stack<char> OP;


	for (int i = 0; i < size; i++) {
		char c = infix[i];

		//case operand
		if (c == 'T' || c == 'F') { // || c=='t'  ||c=='f' ?? 
			if ((i - 1) >= 0 && (infix[i - 1] != '&' && infix[i - 1] != '!' && infix[i - 1] != '|' && infix[i - 1] != ' ' && infix[i - 1] != '(')) {
				return 1;
			}
			if ((i + 1) < size && (infix[i + 1] != '&' && infix[i + 1] != '|' && infix[i + 1] != ' ' && infix[i + 1] != ')')) {
				return 1;
			}
			postfix += c;
			continue;
		}

		//case (
		if (c == '(') {
			if ((i - 1) >= 0 && (infix[i - 1] != '&' && infix[i - 1] != '!' && infix[i - 1] != '|' && infix[i - 1] != ' ' && infix[i - 1] != '(')) {
				return 1;
			}
			if ((i + 1) < size && (infix[i + 1] != 'T' && infix[i + 1] != '!' && infix[i + 1] != 'F' && infix[i + 1] != ' ' && infix[i + 1] != '(')) {
				return 1;
			}
			OP.push(c);
			leftcount++;
			continue;
		}

		//case )
		if (c == ')') {
			if ((i - 1) >= 0 && (infix[i - 1] != 'T' && infix[i - 1] != 'F' && infix[i - 1] != ' ' && infix[i - 1] != ')')) {
				return 1;
			}
			if ((i + 1) < size && (infix[i + 1] != '&' && infix[i + 1] != '|' && infix[i + 1] != ' ' && infix[i + 1] != ')')) {
				return 1;
			}
			while (OP.top() != '(') {
				postfix += OP.top();
				OP.pop();
			}
			OP.pop();
			rightcount++;
			continue;
		}


		//case operator
		if (c == '!' || c == '&' || c == '|') { //order of precedence
			int precedence = 0;
			int stackprec = 0;
			//cerr << "postfix:"<<postfix << endl;
			if (c == '!') {
				if ((i + 1) < size && infix[i + 1] == '!') {
					OP.push(infix[i]);
					while ((i + 1) < size && infix[i + 1] == '!') {
						OP.push(infix[i]);
						i++;
					}
					continue;
				}
				
				//if (exCount % 2 == 0) {
				//	continue;
				//}
				
				if ((i - 1) >= 0 && (infix[i - 1] != '&' && infix[i - 1] != '!' && infix[i - 1] != '|' && infix[i - 1] != ' ' && infix[i - 1] != '(')) {
					return 1;
				}
				if ((i + 1) < size && (infix[i + 1] != 'T' && infix[i + 1] != '!' && infix[i + 1] != 'F' && infix[i + 1] != ' ' && infix[i + 1] != '(')) {
					return 1;
				}
				precedence = 3;
			}
			if (c == '&') {
				if ((i - 1) >= 0 && (infix[i - 1] != 'T' && infix[i - 1] != 'F' && infix[i - 1] != ' ' && infix[i - 1] != ')')) {
					return 1;
				}
				if ((i + 1) < size && (infix[i + 1] != 'T' && infix[i + 1] != '!' && infix[i + 1] != 'F' && infix[i + 1] != ' ' && infix[i + 1] != '(')) {
					return 1;
				}
				precedence = 2;
			}
			if (c == '|') {
				if ((i - 1) >= 0 && (infix[i - 1] != 'T' && infix[i - 1] != 'F' && infix[i - 1] != ' ' && infix[i - 1] != ')')) {
					return 1;
				}
				if ((i + 1) < size && (infix[i + 1] != 'T' && infix[i + 1] != '!' && infix[i + 1] != 'F' && infix[i + 1] != ' ' && infix[i + 1] != '(')) {
					return 1;
				}
				precedence = 1;
			}
			if (!OP.empty() && OP.top() == '!') {
				stackprec = 3;
			}
			if (!OP.empty() && OP.top() == '&') {
				stackprec = 2;
			}
			if (!OP.empty() && OP.top() == '|') {
				stackprec = 1;
			}
			if (!OP.empty() && OP.top() == 'T') {
				stackprec = 0;
			}
			if (!OP.empty() && OP.top() == 'F') {
				stackprec = 0;
			}
			if (OP.empty()) {
				stackprec = 0;
			}
			while (!OP.empty() && OP.top() != '(' && precedence <= stackprec) {
				postfix += OP.top();
				OP.pop();
				if (!OP.empty() && OP.top() == '!') {
					stackprec = 3;
				}
				if (!OP.empty() && OP.top() == '&') {
					stackprec = 2;
				}
				if (!OP.empty() && OP.top() == '|') {
					stackprec = 1;
				}
				if (!OP.empty() && OP.top() == 'T') {
					stackprec = 0;
				}
				if (!OP.empty() && OP.top() == 'F') {
					stackprec = 0;
				}
				if (OP.empty()) {
					stackprec = 0;
				}
			}
			OP.push(c);
			continue;
		}

	}

	//cerr << "postfix2: " << postfix << endl;
	if (leftcount != rightcount) {
		return 1;
	}
	if (infix[0] == '|' || infix[0] == '&' || infix[0]==')' || infix[size - 1] == '|' || infix[size - 1] == '&' || infix[size-1]=='(') {
		return 1;
	}
	while (!OP.empty()) {
		postfix += OP.top();
		OP.pop();
	}

	cerr << "eval postfix: " << postfix << endl;
	//evaluating postfix
	stack<bool> eval;
	for (int x = 0; x < postfix.size(); x++) {
		bool operand2;
		bool operand1;
		bool fin;
		char y = postfix[x];
		if (y == 'T') {
			eval.push(true);
			continue;
		}
		if (y == 'F') {
			eval.push(false);
			continue;
		}
		if (y == '&') {
			operand2 = eval.top();
			eval.pop();
			operand1 = eval.top();
			eval.pop();
			fin = operand2 && operand1;
			eval.push(fin);
			continue;
		}
		if (y == '|') {
			operand2 = eval.top();
			eval.pop();
			operand1 = eval.top();
			eval.pop();
			fin = operand2 || operand1;
			eval.push(fin);
			continue;
		}
		if (y == '!') {
			operand1 = eval.top();
			eval.pop();
			fin = !operand1;
			eval.push(fin);
			continue;
		}
	}
	result=eval.top();

	return 0;
}

int main()
{
	string pf;
	bool answer;
	assert(evaluate("T| F", pf, answer) == 0 && pf == "TF|"  &&  answer);
	assert(evaluate("T|", pf, answer) == 1);
	assert(evaluate("F F", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("()", pf, answer) == 1);
	assert(evaluate("T(F|T)", pf, answer) == 1);
	assert(evaluate("T(&T)", pf, answer) == 1);
	assert(evaluate("(T&(F|F)", pf, answer) == 1);
	assert(evaluate("", pf, answer) == 1);
	assert(evaluate("F  |  !F & (T&F) ", pf, answer) == 0
		&& pf == "FF!TF&&|" && !answer);
	assert(evaluate(" T&(F) ", pf, answer) == 0 && pf == "TF&" && !answer);
	assert(evaluate("T&!(F|T&T|F)|!!!(F&T&F)", pf, answer) == 0  &&  answer);
	cout << "Passed all tests" << endl;

}

/*T                          evaluates to true
        (F)                        evaluates to false
        T&(F)                      evaluates to false
        T & !F                     evaluates to true
        !(F|T)                     evaluates to false
        !F|T                       evaluates to true
        T|F&F                      evaluates to true
		T&!(F|T&T|F)|!!!(F&T&F)    evaluates to true*/


