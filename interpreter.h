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

void interpret_command(std::string cmd);

extern bool command_mode;
extern std::vector<std::string> commands;
extern unsigned int current_cmd;

#endif /* defined(__Trusses__interpreter__) */
