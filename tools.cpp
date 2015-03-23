//
//  tools.cpp
//  Trusses
//
//  Created by Patrick on 22/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "tools.h"
#include "wall.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif
#include "graphics.h"
#include "interface.h"
#include "particle.h"
#include "bar.h"
#include "physics.h"
#include "temporary_label.h"
#include "mouse.h"

#define SELECTION_RES 10

void Tool::set(Tool* new_tool)
{
    delete current_tool;
    current_tool = new_tool;
}

Tool::~Tool() {}

// * * * * * * * * * * //
// WallTool implementation

void WallTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    Vector2d mouse_pos = mouse.snap();
    
    if (points.size() != 2)
        points.push_back(Vector2d(mouse_pos.x, mouse_pos.y));
    if (points.size() == 2)
    {
        Wall::create(points[0], points[1]);
        Tool::set(new BarsTool);
    }
}

void WallTool::passive()
{
    return;
}

void WallTool::drag()
{
    return;
}

void WallTool::display()
{
    Vector2d tool_pos = mouse.pos_world;
    
    // Snap the position vector
    if (mouse.particle_in_range())
        tool_pos = particles[mouse.closest_particle].position_;
    else if (mouse.grid_in_range())
        tool_pos = mouse.closest_grid;
    
    // Draw the current mouse position
    glLineWidth(1);
    glColor3f(WHITE);
    draw_cross(tool_pos, 10);
    
    // Draw the wall points
    for (int i = 0; i < points.size(); i++)
        draw_cross(points[i], 10);
}

void WallTool::key_down(unsigned char key)
{
    return;
}

WallTool::WallTool()
{
    TempLabel::create("Create a wall by defining two corner points", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
    std::cout << "WallTool " << this << " created" << std::endl;
}

WallTool::~WallTool()
{
    std::cout << "WallTool " << this << " destroyed" << std::endl;
}

// * * * * * * * * * * //
// BarsTool implementation

void BarsTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    int clicked_particle_id = -1;
    if (mouse.particle_in_range())
        clicked_particle_id = mouse.closest_particle;
    
    // Shift-click destroys the clicked particle
    if (state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT && clicked_particle_id != -1)
        Particle::destroy(clicked_particle_id);
    
    // If a particle was clicked
    else if (clicked_particle_id != -1)
        selected_particles_ids.push_back(clicked_particle_id);
    else
    {
        Vector2d mouse_pos = mouse.grid_in_range() ? mouse.closest_grid : mouse.pos_world;
        
        // Create a new particle and select it
        int new_p_id;
        if (button == GLUT_RIGHT_BUTTON)
            new_p_id = Particle::create(mouse_pos.x, mouse_pos.y, true); // Fixed
        else
            new_p_id = Particle::create(mouse_pos.x, mouse_pos.y, false); // Free
        
        selected_particles_ids.push_back(new_p_id);
    }
    
    // If precisely two particles are selected, create a bar between them and clear the vector of selected particles
    if (selected_particles_ids.size() == 2)
    {
        Bar::create(selected_particles_ids[0], selected_particles_ids[1], 0.0, ROOM_TEMPERATURE);
        selected_particles_ids.clear();
    }
}

void BarsTool::passive()
{
    return;
}

void BarsTool::drag()
{
    return;
}

void BarsTool::display()
{
    Vector2d tool_pos = mouse.pos_world;
    
    // Snap the position vector
    bool snapped = true;
    if (mouse.particle_in_range())
        tool_pos = particles[mouse.closest_particle].position_;
    else if (mouse.grid_in_range())
        tool_pos = mouse.closest_grid;
    else
        snapped = false;
    
    // Draw dashed lines between the selected particles and the mouse
    if (selected_particles_ids.size() != 0)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
        glLineWidth(1.0);
        glColor3f(GOLD);
        glBegin(GL_LINES);
        for (int i = 0; i < selected_particles_ids.size(); i++)
        {
            if (particles.exists(selected_particles_ids[i])) // Make sure that this particle exists
            {
                Vector2d selected_pos = particles[selected_particles_ids[i]].position_;
                glVertex2f(selected_pos.x, selected_pos.y);
                glVertex2f(tool_pos.x, tool_pos.y);
            }
        }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }
    
    if (snapped)
    {
        glColor3f(GOLD);
        glPointSize(10);
        draw_point(tool_pos);
    }
}

void BarsTool::key_down(unsigned char key)
{
    return;
}

BarsTool::BarsTool()
{
    std::cout << "BarsTool " << this << " created" << std::endl;
}

BarsTool::~BarsTool()
{
    std::cout << "BarsTool " << this << " destroyed" << std::endl;
}

// * * * * * * * * * * //
// SelectionTool implementation

void SelectionTool::Polygon::add_point(const Vector2d &p)
{
    points.push_back(p);
}

size_t SelectionTool::Polygon::no_sides() const
{
    if (points.size() < 3)
        return 0;
    return points.size();
}

bool SelectionTool::Polygon::point_inside(const Vector2d& p) const
{
    bool inside = false;
    for (int i = 0; i < no_sides(); i++)
    {
        int j = (i + 1) % no_sides();
        Vector2d pi = points[i];
        Vector2d pj = points[j];
        // If the point lies between the y scope of the edge
        if ((p.y < pi.y) != (p.y < pj.y))
        {
            // Components of the vector representing the edge
            double x = pj.x - pi.x;
            double y = pj.y - pi.y;
            
            // Make sure it points up
            if (y < 0)
            {
                x = -x;
                y = -y;
            }
            
            // If the point lies to the left (use cross product)
            double cross = x * (p.y - pi.y) - y * (p.x - pi.x);
            if (cross > 0)
                inside = !inside;
            // TODO: what if cross is 0 ?
        }
    }
    return inside;
}

void SelectionTool::mouse_click(int button, int state)
{
    return;
}

void SelectionTool::passive()
{
    return;
}

void SelectionTool::drag()
{
    // Add new points to the selection polygon
    Vector2d pos = mouse.pos_world;
    double res = px_to_m(SELECTION_RES);
    if (poly.no_sides() == 0 || abs_d(pos.x - poly.points.back().x) > res || abs_d(pos.y - poly.points.back().y) > res)
        poly.add_point(Vector2d(pos.x, pos.y));
    
    // For each particle decide if it lies inside or outside the polygon.
    // Use a map to record results (so it's easy to avoid adding the same particle multiple times)
    SlotMap<Particle>::iterator p_it;
    for (p_it = particles.begin(); p_it != particles.end(); p_it++)
    {
        if (poly.point_inside(p_it->position_))
            selection_map[p_it->id_] = true;
        else
            selection_map[p_it->id_] = false;
    }
}

void SelectionTool::display()
{
    // Draw the polygon
    glColor3f(GOLD);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < poly.no_sides(); i++)
        glVertex2f(poly.points[i].x, poly.points[i].y);
    glEnd();
    
    // Highlight the selected points
    glPointSize(10);
    glBegin(GL_POINTS);
    for (int i = 0; i < selection_map.size(); i++)
    {
        if (selection_map[i])
        {
            Vector2d pos = particles[i].position_;
            glVertex2d(pos.x, pos.y);
        }
    }
    glEnd();
}

void SelectionTool::key_down(unsigned char key)
{
    // Backspace
    if (key == 127)
    {
        // Destroy each selected particle
        for (int i = 0; i < selection_map.size(); i++)
        {
            if (selection_map[i])
                Particle::destroy(i);
        }
        // Update the selection map
        selection_map.clear();
        
        // Switch the tool
        Tool::set(new BarsTool);
    }
}

SelectionTool::SelectionTool()
{
    TempLabel::create("Create a selection by dragging", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
    std::cout << "BarsTool " << this << " created" << std::endl;
}

SelectionTool::~SelectionTool()
{
    std::cout << "BarsTool " << this << " destroyed" << std::endl;
}


// * * * * * * * * * * //
// DragTool implementation

void DragTool::mouse_click(int button, int state)
{
    // Release a particle
    if (state == GLUT_UP && particles.exists(dragged_particle))
    {
        // Zero its acceleration
        particles[dragged_particle].external_acceleration_ = Vector2d(0.0, 0.0);
        dragged_particle = -1;
        return;
    }
    else if (state == GLUT_UP)
        return;
    
    // A particle was clicked and can be dragged around by mouse
    if (mouse.particle_in_range())
        dragged_particle = mouse.closest_particle;
}

void DragTool::passive()
{
    return;
}

void DragTool::drag()
{
    if (!particles.exists(dragged_particle))
        return;
    
    Particle* p = &particles[dragged_particle];

    // Particle is fixed
    if (p->fixed_)
        p->position_ = mouse.pos_world;
    // Particle is not fixed
    else
        p->external_acceleration_ = dragging_force * (mouse.pos_world - p->position_) / world.scale;
}

void DragTool::display()
{
    bool highlighted = false;
    
    // Highlight a particle if it's close to the mouse
    if (mouse.particle_in_range())
    {
        Vector2d closest_pos = particles[mouse.closest_particle].position_;
        glColor3f(GOLD);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(closest_pos.x, closest_pos.y);
        glEnd();
        highlighted = true;
    }
    
    // Draw the active particle
    if (!highlighted && particles.exists(dragged_particle))
    {
        Particle* active_p = &particles[dragged_particle];
        Vector2d particle_pos_gl = active_p->position_ ;
        
        glColor3f(GOLD);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
        glEnd();
        
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
        glVertex2f(mouse.pos_world.x, mouse.pos_world.y);
        glEnd();
    }
}

void DragTool::key_down(unsigned char key)
{
    return;
}

DragTool::DragTool()
{
    TempLabel::create("Drag the joints around", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
    std::cout << "DragTool " << this << " created" << std::endl;
}

DragTool::~DragTool()
{
    std::cout << "DragTool " << this << " destroyed" << std::endl;
}
