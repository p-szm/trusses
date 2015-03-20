//
//  interface.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__interface__
#define __Trusses__interface__

#include "math.h"
#include <vector>
#include <limits>

// * * * * * * * * * * //
void key_down_function(unsigned char key, int x, int y);
//void key_down_function(unsigned char key, int x, int y);
void special_key_up(int key, int x, int y);
void special_key_down(int key, int x, int y);
void editor_mouse(int button, int state, int x, int y);
void simulation_mouse(int button, int state, int x, int y);
void editor_mouse_passive(int x, int y);
void simulation_mouse_passive(int x, int y);
void editor_mouse_drag(int x, int y);
void simulation_mouse_drag(int x, int y);
void start_drawing_wall();
void stop_drawing_wall();
void pause_simulation();
void resume_simulation();
bool simulation_is_paused();
void register_callbacks();

// * * * * * * * * * * //
// Some helper functions to convert between the coordinates
Vector2d px_to_m(const Vector2d& v);
double px_to_m(double d);
Vector2d px_to_ui(const Vector2d& v);
double px_to_ui_x(double d);
double px_to_ui_y(double d);

// * * * * * * * * * * //

// Stores the position of the mouse in different coordinates
class Mouse
{
public:
    // In metres
    Vector2d pos_world = Vector2d(std::numeric_limits<float>::max(),
                                  std::numeric_limits<float>::max());
    
    // In pixels, centred at the centre
    Vector2d pos_screen = Vector2d(std::numeric_limits<float>::max(),
                                   std::numeric_limits<float>::max());
    
    // In UI coords (-1, 1)
    Vector2d pos_ui = Vector2d(std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max());
    
    // Updates pos_world, pos_gl and pos_screen variables from
    // the position of mouse in pixels (starting at top left corner).
    void update_from_px(int x, int y); // In pixels
    
    // Returns the id of the closes particle
    int closest_particle();
    
    // Returns the position of the closest grid point
    Vector2d closest_grid();
    
    // Minimum distance between the mouse and the point (in pixels)
    // for the point to be considered as "clicked".
    int min_click_dist = 10;
    
    // Checks the distance between the mouse and the point against min_click_dist
    bool in_range(Vector2d point);
};

class World
{
public:
    // In px/metre
    double scale = 50.0;
    
    // The position in world coordinates of the centre of the screen
    Vector2d centre = Vector2d(0.0, 0.0);
};

// * * * * * * * * * * //
extern Mouse mouse;
extern World world;

extern double grid_dist_px;
extern std::vector<int> selected_particles_ids;
extern int active_particle_id;
extern bool drawing_wall;
extern std::vector<Vector2d> wall_points;
extern bool command_mode;
extern bool full_screen;

#endif /* defined(__Trusses__interface__) */
