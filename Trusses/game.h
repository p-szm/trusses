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
    
    // Updates time and simulation. Should be called
    // each frame.
    void update();
    
    // Enters the editor mode
    void pause_simulation();
    
    // Enters the simulations mode
    void resume_simulation();
    
    // True if the editor mode, false otherwise
    bool simulation_is_paused() const;
    
    // Resets everything
    void reset();
    
    // Returns the time step in seconds
    double dt_s() const;
    
    // Returns the time step in microseconds
    double dt_us() const;
    
    // Returns the total simulated time in seconds
    double simulation_time_s() const;
    
private:
    bool simulation_paused;
    
    void update_time();
    void update_simulation();
    
    // In microseconds
    unsigned long long int t;
    unsigned long long int prev_t;
    unsigned long long int simulation_time;
    
    // In seconds
    double delta_t;
};

extern Game game;

#endif /* defined(__Trusses__game__) */
