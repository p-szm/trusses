//
//  settings.h
//  Trusses
//
//  Created by Patrick on 11/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__settings__
#define __Trusses__settings__

#include <iostream>

enum bool_settings {LENGTHS, EXTENSIONS, GRID, IDS, PARTICLES, BOUNDING_BOXES, TRIANGULATION, GRAVITY};
enum string_settings {SAVE_PATH};

class Settings
{
public:
    Settings();
    void set(bool_settings bool_var, bool state);
    void set(string_settings string_var, std::string str);
    
    bool get(bool_settings bool_var);
    std::string& get(string_settings string_var);
    
    void toggle(bool_settings bool_var);
    
    // Reset all the settings to default
    void reset();
    
private:
    bool& bool_member(bool_settings bool_var);
    std::string& string_member(string_settings string_var);
    
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
    
    // Strings
    std::string save_path;
};

extern Settings settings;

#endif /* defined(__Trusses__settings__) */
