//
//  renderer.cpp
//  Trusses
//
//  Created by Patrick on 27/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "renderer.h"
#include "physics.h"
#include "mouse.h"
#include "interface.h"
#include "graphics.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif
#include <sstream>

#include "particle.h"
#include "bar.h"
#include "obstacle.h"
#include "temporary_label.h"
#include "button.h"
#include "bars_tool.h"
#include "drag_tool.h"
#include "obstacle_tool.h"
#include "selection_tool.h"
#include "grid.h"
#include "window.h"
#include "settings.h"

void Renderer::render(const Particle& obj) const
{
    // Draw the trace if it is enabled
    if (obj.trace_on)
    {
        glColor3f(GOLD);
        glLineWidth(1);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < obj.trace.size(); i++)
        {
            Vector2d t = obj.trace.get(i);
            glVertex2f(t.x, t.y);
        }
        glEnd();
    }
    
    // Particle's position
    Vector2d pos = obj.position_;
    
    // If fixed
    if (obj.fixed_)
    {
        glColor3f(RED);
        glPointSize(8);
        glBegin(GL_POINTS);
        glVertex2f(pos.x, pos.y);
        glEnd();
    }
    // If show particles
    else if (settings.get(PARTICLES))
    {
        glColor3f(WHITE);
        glPointSize(6);
        glBegin(GL_POINTS);
        glBegin(GL_POINTS);
        glVertex2f(pos.x, pos.y);
        glEnd();
    }
    
    // If show ids
    if (settings.get(IDS))
    {
        std::stringstream s;
        s << obj.id_;
        
        // Add 5 pixels in eah direction
        glColor3f(GOLD);
        glut_print(pos.x + px_to_m(5.0), pos.y + px_to_m(5.0), s.str());
    }
}

void Renderer::render(const Bar& obj) const
{
    int mult = 5;
    
    // Color bars according to their strain
    if (settings.bars_color_mode == STRAIN_C)
    {
        // Relative extension
        double strain = obj.get_strain();
        if (strain > 1.0)
            strain = 1.0;
        else if (strain < -1.0)
            strain = -1.0;
        
        if (strain > 0.0)
            glColor3f(1.0, 1.0 - mult * strain / MAX_STRAIN, 1.0 - mult * strain / MAX_STRAIN);
        else
            glColor3f(1.0 + mult * strain / MAX_STRAIN, 1.0, 1.0);
    }
    // Color bars according to their temperature
    // TODO: Color it appropriately: black-red-yellow-white
    else if (settings.bars_color_mode == TEMP_C)
    {
        double temp_fraction = (obj.get_temperature() - ROOM_TEMPERATURE) / (MELTING_POINT - ROOM_TEMPERATURE);
        if (temp_fraction > 0.0)
            glColor3f(1.0, 1.0 - temp_fraction, 1.0 - temp_fraction); // red
        else
            glColor3f(1.0 + temp_fraction, 1.0, 1.0); // cyan
        
        // TODO
        vec3 color;
        double temp_chunk = MELTING_POINT / 3.0;
        double temp = obj.get_temperature();
        if (temp <= temp_chunk)
            color = vec3(temp / temp_chunk, 0, 0);
        else if (temp <= 2 * temp_chunk)
            color = vec3(1.0, (temp - temp_chunk) / temp_chunk, 0.0);
        else if (temp <= 3 * temp_chunk)
            color = vec3(1.0, 1.0, (temp - 2 * temp_chunk) / temp_chunk);
        else
            color = vec3(1.0, 1.0, 1.0);
        glColor3f(color.x, color.y, color.z);
    }
    
    Vector2d start = particles[obj.p1_id].position_;
    Vector2d end = particles[obj.p2_id].position_;
    Vector2d m_mid = 0.5 * (start + end);
    
    // Draw the lines
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    glEnd();
    
    std::stringstream s;
    s.precision(3);
    if (settings.get(IDS))
    {
        glColor3f(FUCHSIA);
        s << obj.id_;
        glut_print(m_mid.x, m_mid.y, s.str());
    }
    if (settings.get(LENGTHS))
    {
        glColor3f(WHITE);
        s << obj.length();
        glut_print(m_mid.x, m_mid.y, s.str());
    }
    if (settings.get(EXTENSIONS))
    {
        glColor3f(WHITE);
        s.str("");
        s << obj.get_strain();
        glut_print(m_mid.x, m_mid.y - px_to_m(12.0), s.str());
    }
}

void Renderer::render(const Obstacle& obj) const
{
    // Draw the triangulated polygon
    if (settings.get(TRIANGULATION))
    {
        glColor3f(RED);
        glLineWidth(1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glColor3f(0, 0.15, 0.3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < obj.triangulation.size(); i++)
    {
        Vector2d pos = obj.points[obj.triangulation[i]];
        glVertex2f(pos.x, pos.y);
    }
    glEnd();
    
    // Draw the boundary
    glColor3f(WHITE);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < obj.no_sides(); i++)
        glVertex2f(obj.points[i].x, obj.points[i].y);
    glEnd();
    
    // Draw the bounding boexs
    if (settings.get(BOUNDING_BOXES))
    {
        glColor3f(RED);
        glLineWidth(1.0);
        draw_rectangle(obj.box_min, obj.box_max, false);
    }
}

void Renderer::render(const TempLabel& obj) const
{
    glColor4f(1.0, 1.0, 1.0, obj.alpha());
    if (obj.centre)
        glut_print(obj.position.x + px_to_ui_x(obj.offset.x - obj.text.size() * 2.8),
                   obj.position.y + px_to_ui_y(obj.offset.y - 6),
                   obj.text);
    else
        glut_print(obj.position.x + px_to_ui_x(obj.offset.x),
                   obj.position.y + px_to_ui_y(obj.offset.y),
                   obj.text);
}

void Renderer::render(const Button& obj) const
{
    Vector2d centre = obj.position + px_to_ui(obj.offset);
    Vector2d size = px_to_ui(Vector2d(obj.width_/2.0, obj.height_/2.0));
    glColor3f(DARK_GREY);
    draw_rectangle(centre - size, centre + size, true);
    
    if (obj.active_)
    {
        glColor3f(YELLOW);
        glLineWidth(2.0);
    }
    else if (obj.highlighted_)
    {
        glColor3f(GOLD);
        glLineWidth(2.0);
    }
    else
    {
        glColor3f(WHITE);
        glLineWidth(2.0);
    }
    draw_rectangle(centre - size, centre + size, false);
    
    glut_print(obj.position.x + px_to_ui_x(obj.offset.x - obj.width_/2.0 + 6),
               obj.position.y + px_to_ui_y(obj.offset.y - 5), obj.text_);
}

void Renderer::render(const BarsTool &obj) const
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
    if (obj.selected_particles_ids.size() != 0)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
        glLineWidth(1.0);
        glColor3f(GOLD);
        glBegin(GL_LINES);
        for (int i = 0; i < obj.selected_particles_ids.size(); i++)
        {
            // Make sure that this particle exists
            int p_id = obj.selected_particles_ids[i];
            if (particles.exists(p_id))
            {
                Vector2d selected_pos = particles[p_id].position_;
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

void Renderer::render(const DragTool &obj) const
{
    // Highlight a particle if it's close to the mouse
    std::vector<int> close_particles;
    mouse.particles_within(px_to_m(mouse.min_click_dist), close_particles);
    for (int i = 0; i < close_particles.size(); i++)
    {
        Vector2d closest_pos = particles[close_particles[i]].position_;
        glColor3f(GOLD);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(closest_pos.x, closest_pos.y);
        glEnd();
    }
    
    // Draw the dragged particles
    for (int i = 0; i < obj.dragged_particles.size(); i++)
    {
        int p_id = obj.dragged_particles[i];
        if (particles.exists(i))
        {
            Particle& active_p = particles[p_id];
            Vector2d particle_pos_gl = active_p.position_ ;
            
            glColor3f(GOLD);
            glPointSize(10);
            glBegin(GL_POINTS);
            glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
            glEnd();
            
            if (!active_p.fixed_)
            {
                glLineWidth(1.0);
                glColor4f(GOLD, 0.6);
                glBegin(GL_LINES);
                glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
                glVertex2f(mouse.pos_world.x, mouse.pos_world.y);
                glEnd();
            }
        }
    }
    
    // Draw the dragging tool
    if (obj.dragged_particles.size() == 0)
    {
        glColor4f(GOLD, 0.15);
        draw_circle(mouse.pos_world, px_to_m(mouse.min_click_dist), 20, true);
    }
}

void Renderer::render(const ObstacleTool &obj) const
{
    // Draw the polygon
    glColor3f(WHITE);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < obj.poly.no_sides(); i++)
        glVertex2f(obj.poly.points[i].x, obj.poly.points[i].y);
    glEnd();
}

void Renderer::render(const SelectionTool &obj) const
{
    // Draw the polygon
    glColor3f(GOLD);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < obj.poly.no_sides(); i++)
        glVertex2f(obj.poly.points[i].x, obj.poly.points[i].y);
    glEnd();
    
    // Highlight the selected points
    glPointSize(10);
    glBegin(GL_POINTS);
    for (int i = 0; i < obj.selection_map.size(); i++)
    {
        if (obj.selection_map.find(i)->second)
        {
            Vector2d pos = particles[i].position_;
            glVertex2d(pos.x, pos.y);
        }
    }
    glEnd();
}

void Renderer::render(const Grid &obj) const
{
    if (!settings.get(GRID))
        return;
    
    double left = window.left();
    double right = window.right();
    double bottom = window.bottom();
    double top = window.top();
    
    // Draw the centre lines
    glColor3f(DARK_GREY);
    glLineWidth(2.0);
    
    glBegin(GL_LINES);
    glVertex2f(0, bottom);
    glVertex2f(0, top);
    glVertex2f(left, 0);
    glVertex2f(right, 0);
    glEnd();
    
    glColor3f(DARK_GREY);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    
    // Distance between lines in metres
    double m_dist = px_to_m(obj.spacing);
    
    // TODO: Clean this up
    // For +ve y
    for (int i = 0; i * m_dist < top; i++)
    {
        double y_pos = i * m_dist;
        glVertex2f(left, y_pos);
        glVertex2f(right, y_pos);
    }
    // For -ve y
    for (int i = -1; i * m_dist > bottom; i--)
    {
        double y_pos = i * m_dist;
        glVertex2f(left, y_pos);
        glVertex2f(right, y_pos);
    }
    // For +ve x
    for (int i = 0; i * m_dist < right; i++)
    {
        double x_pos = i * m_dist;
        glVertex2f(x_pos, bottom);
        glVertex2f(x_pos, top);
    }
    // For -ve x
    for (int i = -1; i * m_dist > left; i--)
    {
        double x_pos = i * m_dist;
        glVertex2f(x_pos, bottom);
        glVertex2f(x_pos, top);
    }
    
    glEnd();
    
    // Choose the right units to display
    double si_dist = 0.0;
    std::string unit;
    
    if (m_dist < 1e-3)
    {
        si_dist = m_dist * 1e6;
        unit = "um";
    }
    else if (m_dist < 1e-2)
    {
        si_dist = m_dist * 1e3;
        unit = "mm";
    }
    else if (m_dist < 1.0)
    {
        si_dist = m_dist * 1e2;
        unit = "cm";
    }
    else
    {
        si_dist = m_dist;
        unit = "m";
    }
    
    // Draw the scale (as a number)
    std::ostringstream s;
    s.precision(2);
    s << si_dist;
    glColor3f(GREY);
    glut_print(m_dist, 0.0, s.str() + unit);
}