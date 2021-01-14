#include "Query.h"
#include "TextQuery.h"
#include <memory>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <regex>
#include <sstream>
#include <typeinfo>
#include <string>
using namespace std;
/////////////////////////////////////////////////////////
shared_ptr<QueryBase> QueryBase::factory(const string &s)
{
    /*
    split string and inser to vector
    */
    std::size_t current, previous = 0;
    vector<string> str_vec;
    current = s.find(" ");
    if (current == std::string::npos && s.length() != 0)
        str_vec.push_back(s);
    else
    {
        string insert_value;
        while (current != std::string::npos)
        {
            insert_value = s.substr(previous, current - previous);
            if (insert_value.length() != 0)
            {
                str_vec.push_back(insert_value);
            }
            previous = current + 1;
            current = s.find(" ", previous);
        }
        insert_value = s.substr(previous, current - previous + 1);
        if (insert_value.length() != 0)
        {
            str_vec.push_back(insert_value);
        }
    }//end split
    string s0, s1, s2;

    if (str_vec.size() == 3)
    {
        s0 = str_vec[0];
        s1 = str_vec[1];
        s2 = str_vec[2];

        if (str_vec[0] == "AND")
        {
            // . . .
            // if AND
            return std::shared_ptr<QueryBase>(new AndQuery(s1, s2));
        }
        else if (str_vec[0] == "OR")
        {
            return std::shared_ptr<QueryBase>(new OrQuery(s1, s2));
        }
        else if (str_vec[0] == "AD")
        {
            return std::shared_ptr<QueryBase>(new AdjacentQuery(s1, s2));
        }
    }
    // check if vector has one element
    if (str_vec.size() == 1)
    {
        s0 = str_vec[0];
        return std::shared_ptr<QueryBase>(new WordQuery(str_vec[0]));
    }

    else
    {
        throw invalid_argument("Unrecognized search \n");
    }
}
// . . .
/////////////////////////////////////////////////////////
QueryResult AndQuery::eval(const TextQuery &text) const
{
    QueryResult left_result = text.query(left_query);
    QueryResult right_result = text.query(right_query);
    auto ret_lines = make_shared<set<line_no>>();
    set_intersection(left_result.begin(), left_result.end(),
                     right_result.begin(), right_result.end(),
                     inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left_result.get_file());
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
    QueryResult left_result = text.query(left_query);
    QueryResult right_result = text.query(right_query);
    auto ret_lines = make_shared<set<line_no>>(left_result.begin(), left_result.end());
    ret_lines->insert(right_result.begin(), right_result.end());
    return QueryResult(rep(), ret_lines, left_result.get_file());
}
/////////////////////////////////////////////////////////
QueryResult AdjacentQuery::eval(const TextQuery &text) const
{
    QueryResult left_result = text.query(left_query);
    QueryResult right_result = text.query(right_query);
    vector<int> vec_1;
    vector<int> vec_2;
    vector<int> vec_3;
    auto ret_lines = std::make_shared<std::set<line_no>>();
    for (auto i = left_result.begin(); i != left_result.end(); ++i)
    {
        vec_1.push_back(*i);
        vec_3.push_back(*i);
    }

    for (auto i = right_result.begin(); i != right_result.end(); ++i)
    {
        vec_2.push_back(*i);
        vec_3.push_back(*i);
    }
    for (int i = 0; i < vec_3.size(); i++)
    {
        /* v contains x */
        if ((find(vec_1.begin(), vec_1.end(), vec_3[i]) != vec_1.end()) && find(vec_2.begin(), vec_2.end(), vec_3[i] + 1) != vec_2.end())

        {
            ret_lines->insert(vec_3[i]);
            ret_lines->insert(vec_3[i] + 1);
        }
        else if ((find(vec_1.begin(), vec_1.end(), vec_3[i]) != vec_1.end()) && (find(vec_2.begin(), vec_2.end(), vec_3[i] - 1) != vec_2.end()))
        {
            ret_lines->insert(vec_3[i]);
            ret_lines->insert(vec_3[i] - 1);
        }
        // v does not contain x * /
    }

    return QueryResult(rep(), ret_lines, left_result.get_file());
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
std::ostream &print(std::ostream &os, const QueryResult &qr)
{
     /*
    split string and inser to vector
    */
    string s = qr.sought;
 
    std::size_t current, previous = 0;
    vector<string> str_vec;
    current = s.find(" ");
    if (current == std::string::npos && s.length() != 0)
        str_vec.push_back(s);
    else
    {
        string insert_value;
        while (current != std::string::npos)
        {
            insert_value = s.substr(previous, current - previous);
            if (insert_value.length() != 0)
            {
                str_vec.push_back(insert_value);
            }
            previous = current + 1;
            current = s.find(" ", previous);
        }
        insert_value = s.substr(previous, current - previous + 1);
        if (insert_value.length() != 0)
        {
            str_vec.push_back(insert_value);
        }
    }//end split
    
    //check if command is AD
    if (str_vec[0] == "AD" && str_vec.size() == 3)
    {

        int q_lines = 0;
        bool _odd = false, _even = false;
        // if lines is even
        if (qr.lines->size() % 2 == 0)
        {
            if (qr.lines->size() == 2)
                _even = true;
            q_lines = qr.lines->size() / 2;
        }
        else
        {
            q_lines = qr.lines->size() / 2 + 1; // number of adj lines = odd which means there is a line that is adj of 2 different lines.
            _odd = true;
        }
        os << "\"" << qr.sought << "\""
           << " occurs " << q_lines << " times:" << std::endl;
        int pair = 0;
        int count = 0;
        for (auto i : *qr.lines)
        {
            pair++;
            count++;
            os << "\t(line " << i + 1 << ") "
               << *(qr.file->begin() + i) << std::endl;
            if (pair % 2 == 0 && _even == 0 && count != 6)
            {
                os << endl;
                pair = 0;
            }
            if (count % 2 == 0 && _odd)
            {
                os << "\t(line " << i + 1 << ") "
                   << *(qr.file->begin() + i) << std::endl;
            }
        }
    }
    else
    {
        os << "\"" << qr.sought << "\""
           << " occurs " << qr.lines->size() << " times:" << std::endl;
        for (auto num : *qr.lines)
        {
            os << "\t(line " << num + 1 << ") "
               << *(qr.file->begin() + num) << std::endl;
        }
    }
    return os;
}
/////////////////////////////////////////////////////////