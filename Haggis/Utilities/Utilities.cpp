///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Utilities.h
//
// Author:      David Borland
//
// Description: Contains various utility functions
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Utilities.h"


std::vector<std::string> Utilities::Tokenize(const std::string& s, const std::string& delimiters) {
    // The tokens that will be returned
    std::vector<std::string> tokens;

    // Skip delimiters at the beginning
    std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);

    // Look for first token
    std::string::size_type pos = s.find_first_of(delimiters, lastPos);

    // Find the rest
    while (pos != std::string::npos || lastPos != std::string::npos) {
        // Found a token, add it to the vector
        tokens.push_back(s.substr(lastPos, pos - lastPos));

        // Skip delimiters
        lastPos = s.find_first_not_of(delimiters, pos);

        // Look for next token
        pos = s.find_first_of(delimiters, lastPos);
    }

    return tokens;
}