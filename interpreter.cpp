//
//  interpreter.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 20/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "interpreter.h"

#include "graphics.h"
#include "save.h"
#include "physics.h"
#include "wall.h"

#include <sstream>
#include <vector>

std::vector<std::string> commands;
unsigned int current_cmd = 0; // From the back

template<typename T>
void extract_words(std::string str, std::vector<T> & target_v)
{
    // Convert the string to the stringstream
    std::istringstream s(str);
    
    // Read every "word" of a stringstream
    while (s)
    {
        // If s is of type T
        T n;
        if (s >> n)
            target_v.push_back(n);
    }
}

bool begins_with(std::string str, std::string substring)
{
    size_t sub_length = substring.size();
    if (str.substr(0, sub_length) == substring)
        return true;
    else
        return false;
}

bool is_number(std::string input)
{
    std::stringstream s(input);
    double n;
    if (s >> n)
        return true;
    else
        return false;
}

// Takes a string and returns a number
// It is up to a user to check if the string represents a valid number
template <typename T>
T get_number(std::string input)
{
    std::stringstream s(input);
    T n;
    if (s >> n)
        return n;
    else
        return 0;
}

// Cuts substring from the beginning of str and returns a new string
std::string cut_out(std::string str, std::string substring)
{
    if (begins_with(str, substring))
        return str.substr(substring.size());
    else
        return str;
}

void interpret_command(std::string cmd)
{
    // Put all the words of this command into the "words" vector of strings
    std::vector<std::string> words;
    extract_words(cmd, words);
    
    size_t words_number = words.size();
    
    // If no command given, return
    if (words_number == 0)
        return;
    
    std::string first_word = words[0];
    
    if (first_word == "ids")
    {
        if (words_number == 1)
            std::cout << "ids=" << ids << std::endl;
        else if (words_number == 2 && words[1] == "on")
            ids = true;
        else if (words_number == 2 && words[1] == "off")
            ids = false;
        else
            std::cout << "Usage: ids <on/off>" << std::endl;
    }
    
    else if (first_word == "gravity")
    {
        if (words_number == 1)
            std::cout << "gravity=" << ids << std::endl;
        else if (words_number == 2 && words[1] == "on")
            gravity = true;
        else if (words_number == 2 && words[1] == "off")
            gravity = false;
        else
            std::cout << "Usage: gravity <on/off>" << std::endl;
    }
    
    else if (first_word == "oscillation")
    {
        if (words_number == 3)
        {
            if (words[1] == "on")
            {
                if (is_number(words[2]))
                {
                    // Interpret negative numbers as 0
                    int n = get_number<int>(words[2]);
                    if (n < 0)
                        n = 0;
                    particles[n].oscillate = true;
                }
            }
            else if (words[1] == "off")
            {
                if (is_number(words[2]))
                {
                    // Interpret negative numbers as 0
                    int n = get_number<int>(words[2]);
                    if (n < 0)
                        n = 0;
                    particles[n].oscillate = false;
                }
            }
        }
    }
    
    else if (first_word == "snap")
    {
        if (words_number == 1)
            std::cout << "snap=" << snap << std::endl;
        else if (words_number == 2 && words[1] == "on")
            snap = true;
        else if (words_number == 2 && words[1] == "off")
            snap = false;
        else
            std::cout << "Usage: snap <on/off>" << std::endl;
    }
    
    else if (first_word == "coords")
    {
        if (words_number == 1)
            std::cout << "coords=" << coords << std::endl;
        else if (words_number == 2 && words[1] == "on")
            coords = true;
        else if (words_number == 2 && words[1] == "off")
            coords = false;
        else
            std::cout << "Usage: coords <on/off>" << std::endl;
    }
    
    else if (first_word == "load")
    {
        if (words_number == 2)
        {
            std::string filepath = words[1];
            
            // Check if the path is absolute or relative.
            // If it is relative, assume that it means the save directory.
            if (filepath.find("/") == -1)
                filepath = SAVE_PATH + filepath;
            
            // Try to load the file
            if (load(filepath))
                std::cout << "Could not load " << filepath << std::endl;
        }
        else
            std::cout << "Usage: load <file>" << std::endl;
    }
    
    else if (first_word == "save")
    {
        if (words_number == 1)
        {
            std::string path = SAVE_PATH;
            path += "save-" + date_str() + '-' + time_str();
            save(path);
        }
        else if (words_number == 2)
        {
            std::string path = SAVE_PATH;
            path += words[1]; // TODO: SECURITY: Check if the filaname is valid
            save(path);
        }
        else
        {
            std::cout << "Usage: save (filename)" << std::endl;
        }
    }
    
    // Reset
    else if (first_word == "reset")
    {
        if (words_number == 1)
            reset();
        else
            std::cout << "Usage: reset" << std::endl;
    }
    
    else if (first_word == "zoom")
    {
        if (words_number == 2 && words[1] == "in")
            scale *= 1.2;
        else if (words_number == 2 && words[1] == "out")
            scale /= 1.2;
        else
            std::cout << "Usage: zoom <in/out>" << std::endl;
    }
    
    // TODO: Maybe scale should be just an int?
    else if (first_word == "scale")
    {
        if (words_number == 1)
            std::cout << "scale=" << scale << std::endl;
        else if (words_number == 2 && is_number(words[1]))
        {
            double new_scale = get_number<double>(words[1]);
            if (new_scale <= 0.0)
                std::cout << "Scale has to be positive!" << std::endl;
            else
                scale = new_scale;
        }
        else
            std::cout << "Usage: scale <double>" << std::endl;
    }
    
    else if (first_word == "wall")
    {
        if (words_number == 1)
            print_walls();
        else if (words_number == 5 && is_number(words[1]) && is_number(words[2])
                 && is_number(words[3]) && is_number(words[4]))
            Wall::create(Vector2d(get_number<double>(words[1]), get_number<double>(words[2])),
                         Vector2d(get_number<double>(words[3]), get_number<double>(words[4])));
        else
            std::cout << "Usage: wall <double> <double> <double> <double>" << std::endl;
    }
    
    else if (first_word == "particle")
    {
        if (words_number == 1)
            print_particles();
        else if (words_number == 3 && is_number(words[1]) && is_number(words[2]))
            Particle::create(get_number<double>(words[1]), get_number<double>(words[2]), false);
        else
            std::cout << "Usage: particle <double> <double>" << std::endl;
    }
    
    // TODO: accept only positive integers
    else if (first_word == "bar")
    {
        if (words_number == 1)
            print_bars();
        else if (words_number == 3 && is_number(words[1]) && is_number(words[2])) // TODO: Prevent it from taking doubles
        {
            // Accept only positive integers
            // Interpret negative numbers as 0
            int n1 = get_number<int>(words[1]);
            int n2 = get_number<int>(words[2]);
            if (n1 < 0)
                n1 = 0;
            if (n2 < 0)
                n2 = 0;
            Bar::create(n1, n2);
        }
        else if (words_number == 4 && words[2] == "stiffness" && is_number(words[3]))
        {
            double new_stiffness = get_number<double>(words[3]);
            if (new_stiffness < 0.0)
                new_stiffness = 0.0;
            else if (new_stiffness > 1.0)
                new_stiffness = 1.0;
            
            if (is_number(words[1]))
            {
                int bar_id = get_number<int>(words[1]);
                if (bar_id < 0)
                    bar_id = 0;
                if (bars.exists(bar_id))
                    bars[bar_id].stiffness = new_stiffness;
            }
            else if (words[1] == "*")
            {
                SlotMap<Bar>::iterator bars_it;
                for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
                {
                    bars_it->stiffness = new_stiffness;
                }
            }
        }
        else
            std::cout << "Usage: bar <int> <int>" << std::endl;
    }
    
    else if (first_word == "fix")
    {
        if (words_number == 2)
        {
            if (is_number(words[1]))
            {
                // Interpret negative numbers to 0
                int n = get_number<int>(words[1]);
                if (n < 0)
                    n = 0;
                if (particles.exists(n))
                    particles[n].fixed_ = true;
            }
        }
    }
    
    else if (first_word == "trace")
    {
        if (words_number == 2)
        {
            if (is_number(words[1]))
            {
                // Interpret negative numbers to 0
                int n = get_number<int>(words[1]);
                if (n < 0)
                    n = 0;
                if (particles.exists(n))
                    particles[n].trace_on = true;
            }
        }
    }
    
    else if (first_word == "untrace")
    {
        if (words_number == 2)
        {
            if (is_number(words[1]))
            {
                // Interpret negative numbers to 0
                int n = get_number<int>(words[1]);
                if (n < 0)
                    n = 0;
                if (particles.exists(n) && particles[n].trace_on)
                {
                    particles[n].trace_on = false;
                    particles[n].trace.clear();
                }
            }
        }
    }
    
    else if (first_word == "remove")
    {
        if (words_number == 3)
        {
            if (words[1] == "bar")
            {
                if (is_number(words[2]))
                {
                    // Interpret negative numbers as 0
                    int n = get_number<int>(words[2]);
                    if (n < 0)
                        n = 0;
                    Bar::destroy(n);
                }
            }
            
            if (words[1] == "particle")
            {
                if (is_number(words[2]))
                {
                    // Interpret negative numbers tas 0
                    int n = get_number<int>(words[2]);
                    if (n < 0)
                        n = 0;
                    Particle::destroy(n);
                }
            }
        }
    }
    
    else if (first_word == "temperature")
    {
        if (words_number == 2)
        {
            if (is_number(words[1]))
            {
                // Interpret negative numbers to 0
                int n = get_number<double>(words[1]);
                if (n < 0.0)
                    n = 0.0;
                environment_temp = n;
            }
        }
    }
    
    // TODO: accept only positive integers
    else if (first_word == "split")
    {
        if (words_number == 3 && is_number(words[1]) && is_number(words[2])) // TODO: Prevent it from taking doubles
        {
            // Accept only positive integers
            // Interpret negative numbers as 0
            int n1 = get_number<int>(words[1]);
            int n2 = get_number<int>(words[2]);
            if (n1 < 0)
                n1 = 0;
            if (n2 < 0)
                n2 = 0;
            if (bars.exists(n1))
                bars[n1].split(n2);
        }
    }
    
    // The command was not recognised
    else
        std::cout << "Command not found" << std::endl;
}