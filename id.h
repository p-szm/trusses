//
//  id.h
//  Trusses
//
//  Created by Patrick Szmucer on 20/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__id__
#define __Trusses__id__

#include <stdio.h>

struct ID
{
    ID(int n, int v): number(n), version(v) {};
    ID() {}
    int number;
    int version;
};

#endif /* defined(__Trusses__id__) */
