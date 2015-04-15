//
//  particle.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "particle.h"
#include "bar.h"
#include "temporary_label.h"
#include "renderer.h"
#include "game.h"
#include "settings.h"

SlotMap<Particle> particles;
const double g = 9.81;

int Particle::create(double a, double b, bool fixed)
{
    return particles.add(Particle(a, b, fixed));
}

Particle::Particle(double a, double b, bool fixed): trace_points(500)
{
    position_ = Vector2d(a, b);
    velocity_ = Vector2d(0.0, 0.0);
    double dt = game.dt_s();
    prev_position_ = position_ - dt * velocity_;
    prev_position_verlet_ = position_ - dt * velocity_;
    acceleration_ = Vector2d(0.0, 0.0);
    external_acceleration_ = Vector2d(0.0, 0.0);
    mass_ = 1.0;
    fixed_ = (fixed) ? true : false;
    trace_enabled = false;
}

void Particle::trace()
{
    trace_enabled = true;
}

void Particle::untrace()
{
    trace_enabled = false;
    trace_points.clear();
}

bool Particle::traced() const
{
    return trace_enabled;
}

void Particle::update()
{
    if (!fixed_)
    {
        // External acceleration is the acceleration added by dragging with a mouse
        acceleration_ = external_acceleration_;
        
        // Remember the presious position
        prev_position_ = position_;
        
        // Trace the previous position
        if (trace_enabled)
            trace_points.add(position_);
        
        if (settings.get(GRAVITY))
            acceleration_ += Vector2d(0.0, -g);
        
        // Verlet integration
        double dt = game.dt_s();
        Vector2d next_position = 2 * position_ - prev_position_verlet_ + pow(dt, 2) * acceleration_;
        velocity_ = (0.5 / dt) * (next_position - prev_position_);
        prev_position_verlet_ = position_;
        position_ = next_position;
    }
}

void print_particles()
{
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles.at(i);
        std::cout << "Particle " << p.id_ << ": ";
        for (int j = 0; j < p.bars_connected.size(); j++)
        {
            std::cout << p.bars_connected[j] << " ";
        }
        std::cout << std::endl;
    }
}

int Particle::destroy(int obj_id)
{
    if (!particles.exists(obj_id))
    {
        issue_label("This particle does not exist", WARNING_LABEL_TIME);
        return 1;
    }
    
    // Remove all the bars that this particle was connected to
    Particle& this_p = particles[obj_id];
    size_t no_bars_connected = this_p.bars_connected.size();
    for (int i = 0; i < no_bars_connected; i++)
        Bar::destroy(this_p.bars_connected.back());
    
    int result = particles.remove(obj_id);
    return result;
}

void Particle::draw(const Renderer& rend) const
{
    rend.render(*this);
}