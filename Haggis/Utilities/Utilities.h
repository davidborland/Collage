///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Utilities.h
//
// Author:      David Borland
//
// Description: Contains various utility functions
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef UTILITIES_H
#define UTILITIES_H


#include <string>
#include <vector>


class Utilities {
public:
    static std::vector<std::string> Tokenize(const std::string& s, const std::string& delimiters);
};


#endif