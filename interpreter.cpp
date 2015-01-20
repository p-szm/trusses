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

bool command_mode = false;
std::string command;

void interpret_command(std::string cmd)
{
    std::cout << "Command: " << cmd << std::endl;
    
    // ids on
    if (cmd == "set ids on")
    {
        ids = true;
    }
    
    // ids off
    else if (cmd == "set ids off")
    {
        ids = false;
    }
    
    // Load
    else if (cmd.substr(0, 4) == "load")
    {
        std::string filepath = cmd.substr(5);
        
        // Check if the path is absolute or relative
        // If it is relative, add the save directory to the beginning
        if (filepath.find("/") == -1)
        {
            filepath = "/Users/patrick/workspace/saves/" + filepath;
        }
        
        // Try to load
        if (load(filepath))
        {
            std::cout << "Could not load " << filepath << std::endl;
        }
        
    }
    
    // Save
    else if (cmd == "save")
    {
        std::string path = "/Users/patrick/Desktop/save-";
        path += date_str() + '-' + time_str();
        save(path);
    }
    
    // Oscillations on
    else if (cmd == "set oscillations on")
    {
        oscillate = true;
    }
    
    // Oscillations off
    else if (cmd == "set oscillations off")
    {
        oscillate = false;
    }
    
    // Gravity on
    else if (cmd == "set gravity on")
    {
        gravity = true;
    }
    
    // Gravity off
    else if (cmd == "set gravity off")
    {
        gravity = false;
    }
    
    // Reset
    else if (cmd == "reset")
    {
        reset();
    }
    
    // Zoom in
    else if (cmd == "zoom in")
    {
        scale *= 1.1;
    }
    
    // Zoom out
    else if (cmd == "zoom out")
    {
        scale /= 1.1;
    }
    
    else
    {
        std::cout << "Command not found" << std::endl;
    }
}