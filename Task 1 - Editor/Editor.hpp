#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Document.hpp"
using namespace std;

class Editor{

    public:
    Document d;
    
    Editor();
    void loop(); //waiting for user's input
    //and transfer the input to the right function in Document class
};

