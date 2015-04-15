//
//  graphics.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

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
#include "button.h"
#include "interface.h"
#include "window.h"
#include "interpreter.h"
#include "temporary_label.h"
#include "renderer.h"
#include "tool.h"
#include "game.h"
#include "grid.h"
#include "settings.h"

// * * * * * * * * * * //
void glut_print (float x, float y, std::string s);
void display_fps(double dt);
void display_time();
void display_temperature(double temp);
void draw_vector(Vector2d v, Vector2d start, float r, float g, float b);
void draw_command_line();
void draw_rectangle(Vector2d p1, Vector2d p2, bool filled);
void draw_circle(Vector2d centre, double r, unsigned int n_points);
void draw_cross(Vector2d pos, int size_px);
void draw_point(Vector2d pos);
void draw_horizon();

// * * * * * * * * * * //
vec3 hsv_to_rgb(vec3 hsv);

// * * * * * * * * * * //
Renderer renderer;

// * * * * * * * * * * //
void glut_print(float x, float y, std::string s)
{
    glRasterPos2f(x, y);
    for (unsigned short i = 0; i < s.length(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[i]);
}

void display_fps(double dt)
{
    glColor3f(WHITE);
    
    std::ostringstream s;
    s << "fps: " << int(1/dt);
    glut_print(-1 + px_to_ui_x(30), -1 + px_to_ui_y(20), s.str());
}

void display_time()
{
    glColor3f(WHITE);
    
    std::ostringstream s;
    s.precision(1);
    s << "Time: " << std::fixed << game.simulation_time_s() << " s";
    glut_print(0, -1 + px_to_ui_y(BOTTOM_MARGIN), s.str());
}

void display_temperature(double temp)
{    
    if (temp > MELTING_POINT)
        glColor3f(RED);
    else
        glColor3f(WHITE);
    
    std::ostringstream s;
    s.precision(5);
    s << "T = " << int(temp) << " K";
    glut_print(1 - px_to_ui_x(80), -1 + px_to_ui_y(BOTTOM_MARGIN), s.str());
}

void draw_vector(Vector2d v, Vector2d start, float r, float g, float b)
{
    Vector2d end = start + v;
    
    glColor3f(r, g, b);
    glLineWidth(1.0);
    
    glBegin(GL_LINES);
    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    glEnd();
    
    glPointSize(5);
    
    glBegin(GL_POINTS);
    glVertex2f(end.x, end.y);
    glEnd();
}

void draw_command_line()
{
    double cmd_size = px_to_ui_y(COMMAND_LINE_SIZE);
    
    glColor3f(0.3, 0.3, 0.4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, -1 + cmd_size);
    glVertex2f(-1, -1 + cmd_size);
    glEnd();
    
    glColor3f(GOLD);
    glBegin(GL_LINES);
    glVertex2f(1, -1 + cmd_size);
    glVertex2f(-1, -1 + cmd_size);
    glEnd();
    
    glColor3f(WHITE);
    glut_print(-1 + px_to_ui_x(20), -1 + px_to_ui_y(10), interpreter.command);
}

void draw_rectangle(Vector2d p1, Vector2d p2, bool filled)
{
    if (!filled)
        glBegin(GL_LINE_LOOP);
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
    }
    glVertex2f(p1.x, p1.y);
    glVertex2f(p1.x, p2.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p2.x, p1.y);
    glEnd();
}

// All in metres
void draw_circle(Vector2d centre, double r, unsigned int n_points, bool filled)
{
    if (filled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_FAN);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }
    for (int i = 0; i < n_points; i++)
        glVertex2f(centre.x + r * cos(i * 2 * M_PI / n_points), centre.y + r * sin(i * 2 * M_PI / n_points));
    glEnd();
}

// Position in world coords, size in px
void draw_cross(Vector2d pos, int size_px)
{
    glBegin(GL_LINES);
    glVertex2f(pos.x - px_to_m(size_px), pos.y);
    glVertex2f(pos.x + px_to_m(size_px), pos.y);
    glVertex2f(pos.x, pos.y + px_to_m(size_px));
    glVertex2f(pos.x , pos.y - px_to_m(size_px));
    glEnd();
}

// * * * * * * * * * * //
// This is not currently used
vec3 hsv_to_rgb(vec3 hsv) // H is in the range [0,360] degs
{
    // TODO: check if given hsv is valid
    
    double C = hsv.z * hsv.y;
    double B = fmod((hsv.x/60.0), 2.0) - 1;
    double X = C * (1 - abs_d(B));
    double m = hsv.z - C;
    int n = (int)hsv.x / 60;
    
    vec3 rgb;
    if (n == 0)
        rgb = vec3(C, X, 0);
    else if (n == 1)
        rgb = vec3(X, C, 0);
    else if (n == 2)
        rgb = vec3(0, C, X);
    else if (n == 3)
        rgb = vec3(0, X, C);
    else if (n == 4)
        rgb = vec3(X, 0, C);
    else if (n == 5)
        rgb = vec3(C, 0, X);
    
    rgb.x += m;
    rgb.y += m;
    rgb.z += m;
    return rgb;
}

void draw_point(Vector2d pos)
{
    glBegin(GL_POINTS);
    glVertex2f(pos.x, pos.y);
    glEnd();
}

// For debugging
void draw_horizon()
{
    glColor3f(WHITE);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2d(-HORIZON, -HORIZON);
    glVertex2d(HORIZON, -HORIZON);
    glVertex2d(HORIZON, HORIZON);
    glVertex2d(-HORIZON, HORIZON);
    glEnd();
}

// * * * * * * * * * * //
void display()
{
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(WORLD_VIEW);
    
    if (!game.simulation_running())
        renderer.render(grid);
    
    // Draw the obstacles
    for (int i = 0; i < obstacles.size(); i++)
        obstacles.at(i).draw(renderer);
    
    // Draw the bars
    for (int i = 0; i < bars.size(); i++)
        bars.at(i).draw(renderer);
    
    // Draw the particles
    for (int i = 0; i < particles.size(); i++)
        particles.at(i).draw(renderer);
    
    // Draw the tool-specific things
    current_tool->display(renderer);
    
    // Switch to the UI view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(UI_VIEW);
    
    // Draw temporary labels
    for (int i = 0; i < temp_labels.size(); i++)
        temp_labels.at(i).draw(renderer);
    
    // Draw buttons
    for (int i = 0; i < buttons.size(); i++)
        buttons[i].draw(renderer);
    
    display_temperature(game.environment_temp);
    display_time();
    
    // Draw the command line
    if (command_mode)
        draw_command_line();
    
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    // Define the viewport transformation
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(WORLD_VIEW);
    
    window.width = width;
    window.height = height;
}

// * * * * * * * * * * //
void setup_graphics(int argc, char * argv[])
{
    // Initiallize GLUT
    glutInit(&argc, argv);
    
    // Setup for the new window
    glutInitWindowPosition(160, 80);
    glutInitWindowSize(window.width, window.height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    // Create a window
    glutCreateWindow("Trusses simulation");
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glPointSize(5.0);
    glLineWidth(1.0);
    glClearColor(BLACK, 1.0);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
