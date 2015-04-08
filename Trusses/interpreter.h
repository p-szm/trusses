//
//  interpreter.h
//  Trusses
//
//  Created by Patrick Szmucer on 20/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__interpreter__
#define __Trusses__interpreter__

#include <iostream>
#include <vector>

class Interpreter
{
public:
    void interpret(const std::string& cmd) const;
private:
    // Takes a string str and breaks it up into words or numbers of type T
    // which are put into target_v
    template <typename T>
    void extract(const std::string& str, std::vector<T>& target_v) const;
    
    // Converts a vector of words to a string of types, for example:
    // {test, a, b, c, 123, 123, -4.566} -> "wwwwnnn"
    std::string types_of_words(const std::vector<std::string>& words) const;
    
    // True if the input is a number (int, double, etc)
    bool is_number(const std::string& input) const;
    
    // Converts the string to a number of type T.
    template <typename T>
    T get_number(const std::string& input) const;
    
    // Extracts all the numbers from the string
    template <typename T>
    bool get_numbers(const std::string& input, std::vector<T> number) const;
};

// * * * * * * * * * * //
extern std::vector<std::string> commands;
extern unsigned int current_cmd;

#endif /* defined(__Trusses__interpreter__) */
