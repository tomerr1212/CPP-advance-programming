#include "scanner.h"
#include <regex>
using namespace std;
// putback() puts its argument back into the Token_stream's buffer
void Token_stream::putback(Token t)
{
    if (full)
        error("putback into a full buffer");
    buffer = t;  // copy t to buffer
    full = true; // buffer is now full
}

Token Token_stream::get()
{
    if (full)
    {                 // do we already have a Token ready?
        full = false; // remove token from buffer
        return buffer;
    }
    char ch;
    cin.get(ch); // cin.get() does NOT skip whitespace
    while (isspace(ch))
    {
        if (ch == '\n')
            return Token(print);
        cin.get(ch);
    }
    switch (ch)
    {
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case 'q':
        return Token(ch); // let each character represent itself
    default:
        if (isdigit(ch))
        {
            string s;
            s += ch;
            while (cin.get(ch) && (isdigit(ch) || ch == '.'))
                s += ch;
            cin.unget();
            std::regex Base1("[+-]?[0-9]+|[+-]?[0-9]+[.][0-9][0-9]");
            if (regex_match(s, Base1))
                return Token(number, stod(s));
            cout<<"error: Illegal number"<<endl;
            exit(1);
        }
        if (isalpha(ch) || ch == '_')
        {
            string s;
            s += ch;
            while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))
                s += ch;
            cin.unget();
            if (s == "var")
                return Token(variable);
            if (s == "const")
                return Token(constant);
            std::regex str1("[a-z]+[A-Z]*[a-z]*[0-9]*|[A-Z]+[A-Z]*[a-z]*[0-9]*|[_][_][a-z]+[_][_]");
            if (regex_match(s, str1) )
                return Token(name, s);
            cout<<"error: Illegal name"<<endl;    
            exit(1);
        }
        error("Bad token");
    }
    return 0;
}
