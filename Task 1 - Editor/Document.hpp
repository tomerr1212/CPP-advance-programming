#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <bits/stdc++.h> 

using namespace std;

class Document{

    private:
    vector<string> vec;
    int index=0;
    string old_search;
    
    public:

    Document();//constractor

    void p ();
    void n ();
    void percentP ();// %p
    void num (int num);
    void a ();
    void i ();
    void c ();
    void d ();
    void t_search (string s);// /text
    void r_search (); // /
    void b_search (string s); // ?text
    void s_old_new (string s); // s/old/new
};

