//
//  game.h
//  Trusses
//
//  Created by Patrick on 10/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__game__
#define __Trusses__game__

#define HORIZON 1000

#include "window.h"

class Game
{
public:
    Game();
    
    unsigned long long int simulation_time; // In us
    unsigned long long int t; // In us
    unsigned long long int prev_t; // In us
    double delta_t; // In s
    void microsecond_time (unsigned long long &t);
    void update_time();
    void update_simulation();
    bool simulation_is_paused();
    void pause_simulation();
    void resume_simulation();
private:
    bool simulation_paused = true;
};

extern Game game;

#endif /* defined(__Trusses__game__) */
