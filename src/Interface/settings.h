//
//  settings.h
//  Trusses
//
//  Created by Patrick on 11/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__settings__
#define __Trusses__settings__

enum bars_color_mode_t {STRAIN_C = 0, TEMP_C};
enum bool_settings {LENGTHS, EXTENSIONS, GRID, IDS, PARTICLES, BOUNDING_BOXES, TRIANGULATION, GRAVITY};

class Settings
{
public:
    Settings();
    
    // Special types
    bars_color_mode_t bars_color_mode;
    
    void set(bool_settings bool_var, bool state);
    void toggle(bool_settings bool_var);
    bool get(bool_settings bool_var);
    
private:
    bool& bool_member(bool_settings bool_var);
    
    // Booleans
    bool accelerations;
    bool velocities;
    bool lengths;
    bool extensions;
    bool grid;
    bool ids;
    bool show_particles;
    bool draw_bounding_boxes;
    bool draw_triangulation;
    bool gravity;
};

extern Settings settings;

#endif /* defined(__Trusses__settings__) */
