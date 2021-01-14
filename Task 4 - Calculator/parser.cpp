#include "scanner.h"
#include "symtab.h"
// globals
Token_stream ts;
Symbol_table st; 

double statement()
{
	Token t = ts.get();
	switch (t.kind) {
	case variable:
		return declaration(false);
	case constant:
		return declaration(true);
	default:
		ts.putback(t);
		return expression();
	}
    return 0 ;
}

double declaration(bool is_const)
{
	Token t = ts.get();
	if (t.kind != name) error("name expected in declaration");
	string name = t.name;
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration");
	double d = expression();
    //add name
    st.declare(name, d, is_const);  
	return d;
}

// deal with + and -
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}
// deal with *, /, and %
double term()
{
    double left = primary();
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);     // put t back into the token stream
            return left;
        }
    }
}
// deal with numbers and parentheses
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(':    // handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
            return d;
    }
    case '-':
	return - primary();
    case '+':
	return primary();
    case number:
        return t.value;                 // return the number's value
    case name:
	{
	string n = t.name;
	Token next = ts.get();
	if (next.kind == '=') {  	// handle name = expression
            double d = expression();
            // set name 
            st.set(n,d);  
            return d;
	}
	else {
            ts.putback(next);		// not an assignment
            // return  value
            double ans = st.get(n).value;
            return ans;   
	}
	}
    default:
        error("primary expected");
    }
    return 0;
}

