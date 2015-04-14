//
//  bar.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "bar.h"
#include "particle.h"
#include "temporary_label.h"
#include "renderer.h"
#include "game.h"

SlotMap<Bar> bars;

int Bar::create(int id1, int id2, double e, double temp)
{
    if (!particles.exists(id1) || !particles.exists(id2))
    {
        issue_label("One or more required particles don't exist", WARNING_LABEL_TIME);
        return -1;
    }
    
    if (id1 == id2)
    {
        issue_label("Cannot create a bar connecting the same particle", WARNING_LABEL_TIME);
        return -1;
    }
    
    // TODO
    // Write a method that searches contents of a vector (or use an existing one)
    int no_bars = (int)particles[id1].bars_connected.size();
    for (int i = 0; i < no_bars; i++)
    {
        int bar_id = particles[id1].bars_connected[i];
        if ( bars[bar_id].p1_id == id2 || bars[bar_id].p2_id == id2)
        {
            issue_label("Bar between these particles already exists", WARNING_LABEL_TIME);
            return -1;
        }
    }
    
    Bar new_bar(id1, id2);
    
    // This sets temperature, r0 and stiffness
    new_bar.temperature = temp; // Load every bar at room temperature (for now)
    new_bar.set_strain(e);
    new_bar.stiffness = -(1.0 - STIFFNESS_AT_TM) * new_bar.temperature / MELTING_POINT + 1.0; // Stiffness is 1 at 0K
    
    int new_id = bars.add(new_bar);
    
    // Particles have to know which bars are connected to them
    particles[id1].bars_connected.push_back(new_id);
    particles[id2].bars_connected.push_back(new_id);

    return new_id;
}

int Bar::create(int id1, int id2)
{
    return Bar::create(id1, id2, 0.0, ROOM_TEMP);
}

void Bar::set_temperature(double t)
{
    temperature = t;
    r0 = r_0K * (1 + THERMAL_COEFF * temperature); // Assume linear expansion with temperature
    stiffness = -(1.0 - STIFFNESS_AT_TM) * temperature / MELTING_POINT + 1.0; // Stiffness is 1 at 0K
}

double Bar::get_temperature() const
{
    return temperature;
}

void Bar::set_strain(double e)
{
    r0 = length() / (e + 1.0);
    r_0K = r0 / (1 + THERMAL_COEFF * temperature);
}

double Bar::get_strain() const
{
    return extension() / r0;
}

int Bar::destroy(int obj_id)
{
    if (!bars.exists(obj_id))
    {
        issue_label("This bar does not exist", WARNING_LABEL_TIME);
        return 1;
    }
    
    Particle& p1 = particles[bars[obj_id].p1_id];
    Particle& p2 = particles[bars[obj_id].p2_id];
    
    for (int i = 0; i < p1.bars_connected.size(); i++)
    {
        if (p1.bars_connected[i] == obj_id)
        {
            p1.bars_connected[i] = p1.bars_connected.back();
            p1.bars_connected.pop_back();
        }
    }
    for (int i = 0; i < p2.bars_connected.size(); i++)
    {
        if (p2.bars_connected[i] == obj_id)
        {
            p2.bars_connected[i] = p2.bars_connected.back();
            p2.bars_connected.pop_back();
        }
    }
    
    bars.remove(obj_id);
    
    return 0;
}

double Bar::length() const
{
    Vector2d pos1 = particles[p1_id].position_;
    Vector2d pos2 = particles[p2_id].position_;
    double ans = (pos1 - pos2).abs();
    return ans;
}

Vector2d Bar::unit12() const
{
    return (particles[p1_id].position_ - particles[p2_id].position_).norm();
}

Vector2d Bar::unit21() const
{
    return -unit12();
}

double Bar::extension() const
{
    return length() - r0;
}

void Bar::impose_constraint()
{
    Particle& p1 = particles[p1_id];
    Particle& p2 = particles[p2_id];
    
    double ext = extension();
    
    double im1 = 1/p1.mass_;
    double im2 = 1/p1.mass_;
    float mult1 = (im1 / (im1 + im2)) * stiffness;
    float mult2 = stiffness - mult1;
    
    // If one particle is fixed, the other should move two times farther in the direction of the fixed particle.
    if (!p1.fixed_ && !p2.fixed_)
    {
        p1.position_ += mult1 * ext * unit21();
        p2.position_ += mult2 * ext * unit12();
    }
    else if (!p1.fixed_) // and p2 is fixed
        p1.position_ += 2 * mult1 * ext * unit21();
    else if (!p2.fixed_) // and p1 is fixed
        p2.position_ += 2 * mult2 * ext * unit12();
    // else both are fixed
}

// Returns 1 if bar will be destroyed, 0 otherwise
int Bar::update()
{
    // Temperature expansion
    if (abs_d(temperature - game.environment_temp) > SMALL_NUM)
        set_temperature( temperature + game.dt_s()/10.0 * (game.environment_temp - temperature) );
    
    // Destroy bars which are extended by too much
    double ext = extension() / r0;
    if (abs_d(ext) > MAX_STRAIN || (temperature >= MELTING_POINT && random(1.0) > (1 - (temperature - MELTING_POINT) / 10000.0)))
        return 1;
    return 0;
}

void Bar::split(int bar_id, unsigned int n_parts)
{
    if (!bars.exists(bar_id))
    {
        issue_label("This bar does not exist", WARNING_LABEL_TIME);
    }
    
    if (n_parts < 2)
    {
        issue_label("Cannot divide a bar into less than two parts", WARNING_LABEL_TIME);
        return;
    }
    
    Bar& this_bar = bars[bar_id];
    
    int id_start = this_bar.p1_id;
    int id_end = this_bar.p2_id;
    Vector2d pos_start = particles[id_start].position_;
    Vector2d pos_end = particles[id_end].position_;
    
    double temp = this_bar.temperature;
    double new_r0 = this_bar.r0 / n_parts;
    double new_r_0K = this_bar.r_0K / n_parts;
    Vector2d Dr = (pos_end - pos_start) / n_parts;
    
    // Create new particles
    std::vector<int> new_ids;
    for (int i = 1; i < n_parts; i++)
    {
        int new_id = particles.add(Particle(pos_start.x + i * Dr.x,
                                            pos_start.y + i * Dr.y, false));
        new_ids.push_back(new_id);
    }
    
    // Remove the first bar
    Bar::destroy(bar_id);
    
    // Connect particles with bars
    // Don't delete the first one, but modify it instead
    for (int i = 0; i < new_ids.size() + 1; i++)
    {
        int new_bar_id;
        
        if (i == 0)
            new_bar_id = Bar::create(id_start, new_ids[0]);
        else if (i == new_ids.size())
            new_bar_id = Bar::create(new_ids.back(), id_end);
        else
            new_bar_id = Bar::create(new_ids[i-1], new_ids[i]);
        
        bars[new_bar_id].r0 = new_r0;
        bars[new_bar_id].r_0K = new_r_0K;
        bars[new_bar_id].temperature = temp;
    }
    
}

void print_bars()
{
    for (int i = 0; i < bars.size(); i++)
    {
        std::cout << "Bar " << bars.at(i).id_ << std::endl;
    }
}

void Bar::draw(const Renderer& rend) const
{
    rend.render(*this);
}