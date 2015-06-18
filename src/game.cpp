//
//  game.cpp
//  Trusses
//
//  Created by Patrick on 10/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "game.h"
#include <sys/time.h>
#include "particle.h"
#include "bar.h"
#include "obstacle.h"
#include "interface.h"
#include "bars_tool.h"
#include "drag_tool.h"
#include "temporary_label.h"
#include "button.h"
#include "mouse.h"
#include "settings.h"

#define RELAX_ITER 30

Game game;

// Returns system time in microseconds
void microsecond_time (unsigned long long &t)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    t = (unsigned long long)tv.tv_usec + 1000000 * (unsigned long long)tv.tv_sec;
}

Game::Game()
{
    // Initialises time
    microsecond_time(t);
    prev_t = t;
    delta_t = 20000;
    simulation_time = 0;
}

void Game::update()
{
    update_time();
    if (simulation_running())
        update_simulation();
    update_labels();
}

void Game::update_time()
{
    prev_t = t;
    microsecond_time(t);
    delta_t = t - prev_t;
}

void Game::update_simulation()
{
    simulation_time += delta_t;
    
    // Update each particle's position by Verlet integration
    for (int i = 0; i < particles.size(); i++)
        particles.at(i).update();
    
    // Use relaxation to satisfy the constraints imposed by bars
    // Large RELAX_ITER means accurate simulation, and hence stiff bars
    for (int j = 0; j < RELAX_ITER; j++)
        for (int i = 0; i < bars.size(); i++)
            bars.at(i).impose_constraint();
    
    // Collisions of particles with obstacles
    for (int i = 0; i < obstacles.size(); i++)
        obstacles.at(i).collide();
    
    // Update each bar and add see if it will be destroyed
    std::vector<int> bars_to_destroy;
    for (int i = 0; i < bars.size(); i++)
    {
        Bar& b = bars.at(i);
        if (b.is_fractured())
            bars_to_destroy.push_back(b.id_);
    }
    
    // Destroy each bar that was previously added to the list
    for (int i = 0; i < bars_to_destroy.size(); i++)
        Bar::destroy(bars_to_destroy[i]);
    
    // Destroy particles which are very far away
    std::vector<int> particles_to_destroy;
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles.at(i);
        if (abs_d(p.position_.x) > HORIZON || abs_d(p.position_.y) > HORIZON)
            particles_to_destroy.push_back(p.id_);
    }
    for (int i = 0; i < particles_to_destroy.size(); i++)
        Particle::destroy(particles_to_destroy[i]);
}

bool Game::simulation_running() const
{
    return simulation_is_running;
}

void Game::enter_editor()
{
    Tool::set(current_tool, new BarsTool);
    simulation_is_running = false;
    
    temp_labels.clear();
    TempLabel::create("Editor mode - you can draw the structure.", 0, 1.0, 0, -TOP_MARGIN, MODE_LABEL_TIME);
    buttons.clear();
    create_buttons_editor();
    
    mouse.min_click_dist = 10;
}

void Game::enter_simulation()
{
    Tool::set(current_tool, new DragTool);
    microsecond_time(t);
    simulation_is_running = true;
    
    temp_labels.clear();
    TempLabel::create("Simulation mode - you can drag the joints.", 0, 1.0, 0, -TOP_MARGIN, MODE_LABEL_TIME);
    buttons.clear();
    create_buttons_simulation();
    
    mouse.min_click_dist = 20;
}

void Game::reset()
{
    // Reset the entities
    bars.clear();
    particles.clear();
    obstacles.clear();
    
    enter_editor();
    simulation_time = 0;
    
    Tool::set(current_tool, new BarsTool);
    
    window.reset();
    settings.reset();
}

double Game::dt_s() const
{
    return delta_t/1000000.0;
}

double Game::dt_us() const
{
    return delta_t;
}

double Game::simulation_time_s() const
{
    return simulation_time/1000000.0;
}
