//
//  tool.cpp
//  Trusses
//
//  Created by Patrick on 22/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "tool.h"

void Tool::set(Tool*& tool, Tool* new_tool)
{
    delete tool;
    tool = new_tool;
}

Tool::~Tool() {}