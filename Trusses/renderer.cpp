//
//  renderer.cpp
//  Trusses
//
//  Created by Patrick on 27/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "renderer.h"
#include "physics.h" // TODO: This doesn't belong here
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
#include "wall.h"
#include "obstacle.h"
#include "temporary_label.h"
#include "button.h"

bool draw_bounding_boxes = false;
const int wall_lines_spacing = 12; // px

void Renderer::render(const Particle& obj) const
{
    // Draw the trace if it is enabled
    glColor3f(GOLD);
    glLineWidth(1);
    
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < obj.trace.size(); i++)
    {
        Vector2d t = obj.trace.get(i);
        glVertex2f(t.x, t.y);
    }
    glEnd();
    
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
    else if (show_particles)
    {
        glColor3f(WHITE);
        glPointSize(6);
        glBegin(GL_POINTS);
        glVertex2f(pos.x, pos.y);
        glEnd();
    }
    
    if (ids)
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
    if (bars_color_mode == STRAIN_C)
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
    else if (bars_color_mode == TEMP_C)
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
    
    if (!fancy_bars)
    {
        glLineWidth(2.0);
        
        glBegin(GL_LINES);
        glVertex2f(start.x, start.y);
        glVertex2f(end.x, end.y);
        glEnd();
    }
    
    else
    {
        glLineWidth(1.0);
        
        Vector2d p1_pos = particles[obj.p1_id].position_;
        Vector2d p2_pos = particles[obj.p2_id].position_;
        
        // Draw circles at the ends
        double b_radius = 0.02;
        draw_circle(p1_pos, b_radius, 20); // 0.2m
        draw_circle(p2_pos, b_radius, 20);
        
        Vector2d unit = (p2_pos - p1_pos).norm();
        Vector2d normal = Vector2d(-unit.y, unit.x);
        
        Vector2d p1 = p1_pos + b_radius * normal;
        Vector2d p2 = p2_pos + b_radius * normal;
        Vector2d p3 = p1_pos - b_radius * normal;
        Vector2d p4 = p2_pos - b_radius * normal;
        
        glBegin(GL_LINES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glVertex2f(p3.x, p3.y);
        glVertex2f(p4.x, p4.y);
        glEnd();
    }
    
    std::stringstream s;
    s.precision(3);
    
    if (ids)
    {
        glColor3f(FUCHSIA);
        s << obj.id_;
        glut_print(m_mid.x, m_mid.y, s.str());
    }
    if (lengths)
    {
        s << obj.length();
        glut_print(m_mid.x, m_mid.y, s.str());
    }
    if (extensions)
    {
        s.str("");
        s << obj.get_strain();
        glut_print(m_mid.x, m_mid.y - px_to_m(12.0), s.str());
    }
}

void Renderer::render(const Wall& obj) const
{
    glColor3f(WHITE);
    glLineWidth(2.0);
    
    draw_rectangle(obj.p1_, obj.p2_, false);
    
    double xmin = obj.x_min();
    double xmax = obj.x_max();
    double ymin = obj.y_min();
    double ymax = obj.y_max();
    
    double width = xmax - xmin;
    double height = ymax - ymin;
    
    // Spacing between the lines, metres
    double d = px_to_m(wall_lines_spacing);
    
    glBegin(GL_LINES);
    for (int i = 0; i * d <= width + height; i++)
    {
        double x1 = xmin + i * d - height;
        double y1 = ymin;
        double x2 = xmin + i * d;
        double y2 = ymax;
        
        if (x1 < xmin)
        {
            x1 = xmin;
            y1 = ymax - i * d;
        }
        if (x2 > xmax)
        {
            x2 = xmax;
            y2 = ymin + width + height - i * d;
        }
        
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        
    }
    glEnd();
    
    if (ids)
    {
        std::ostringstream s;
        s << obj.id_;
        glColor3f(AQUA);
        glut_print(obj.p2_.x, obj.p2_.y, s.str());
    }
}

void Renderer::render(const Obstacle& obj) const
{
    glColor3f(WHITE);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < obj.no_sides(); i++)
        glVertex2f(obj.points[i].x, obj.points[i].y);
    glEnd();
    
    if (draw_bounding_boxes)
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