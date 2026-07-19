#include "gdexample.h"
#include "m_tile.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/classes/time.hpp>
#include <fstream>
#include <godot_cpp/variant/dictionary.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <map>
#include <godot_cpp/classes/ray_cast2d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/object.hpp>


using namespace godot;

void GDExample::_bind_methods() {
}

enum typesOfSoil{
    weed,
    patatoes,
    bees
};

struct Tile {

    int type = typesOfSoil::weed;   
    int growth = 1;
    bool occupied = true;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

};

struct CompareTile{
    bool operator()(const m_tile* a, const m_tile* b) const {
        return a->id < b->id;
    }
};

std::map<m_tile*, Tile, CompareTile> map_tiles;

GDExample::GDExample() 
    /* : file("save.txt", std::ios::in | std::ios::out | std::ios::trunc)*/ {
	// Initialize any variables here
  
}

bool GDExample::is_empty()
{
    file.clear();
    auto pos = file.tellg();

    file.seekg(0, std::ios::end);
    bool empty = (file.tellg() == 0);

    file.seekg(pos); // restore position
    file.clear();

    return empty;
}

GDExample::~GDExample() {
	// Add your cleanup here.
    file.close();
}


void GDExample::_ready()
{
    if (!is_inside_tree())
        return;
    
    raycast = get_node<RayCast2D>("../RayCast2D");
    line = get_node<Line2D>("../RayCast2D/Line2D");

    line->clear_points();
    line->add_point(Vector2(0, 0));
    line->add_point(raycast->get_target_position());
   
    //file.open("save.txt", std::ios::in | std::ios::out | std::ios::trunc);
	 // If file doesn't exist, create it
    if (!file.is_open())
    {
        file.open("save.txt", std::ios::out);
        file.close();

        file.open("save.txt", std::ios::in | std::ios::out | std::ios::binary);
    }
	if (!file.is_open())
    {
        UtilityFunctions::print("Failed to open file");
        std::cout << "Failed to open file\n";
    }

    //UtilityFunctions::print("I am: ", get_path());

    // UtilityFunctions::print("Children: ", get_child_count());

    // for (int i = 0; i < get_child_count(); i++) {
    //     Node *child = get_child(i);
    //     UtilityFunctions::print("Child ", i, ": ", child->get_name(), " path=", child->get_path());
    // }
    
    //Tile tile;
    //  // Get current datetime dictionary
    // Dictionary datetime =
    //     Time::get_singleton()->get_datetime_dict_from_system();

    // tile.year   = datetime["year"];
    // tile.month  = datetime["month"];
    // tile.day    = datetime["day"];
    // tile.hour   = datetime["hour"];
    // tile.minute = datetime["minute"];
    // tile.second = datetime["second"];

    // int count = 1;
    // while (count <= 30)
    // {
    //     if (!file.write(reinterpret_cast<char*>(&tile), sizeof(Tile)))
    //     {
    //         UtilityFunctions::print("WRITE FAILED");
    //         break;
    //     }

    //     count++;
    // }}

    // file.flush();      // make sure data is written
    // file.clear();      // clear eof/fail flags
    // file.seekg(0);     // move read position to beginning

    // Tile p;
    // std::vector<Tile> tiles;
    // while (file.read((char*)&p, sizeof(Tile))){   
    //     tiles.push_back(p);
    //     UtilityFunctions::print("inside vector"); 
    // }

   
    // const int CELL_SIZE = 50;
    // const int COLS = 25;
    // const int ROWS = 14;
    // const int THICKNESS = 4;
    // const int OFFSET = 25;

    const int CELL_SIZE = 75;
    const int COLS = 17;
    const int ROWS = 9;
    const int THICKNESS = 3;
    
    int next_id = 0;
     

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            bool draw =
                (col < THICKNESS) ||                      // Left side
                (col >= COLS - THICKNESS) ||             // Right side
                (row >= ROWS - THICKNESS);               // Bottom
            
            if (draw)
            {
                m_tile *tile = memnew(m_tile);
                tile->set_position(Vector2(col * CELL_SIZE, row * CELL_SIZE ));
                tile->id = next_id++;
                //UtilityFunctions::print("adding a child", tile->id); 
                //UtilityFunctions::print(Vector2(col * CELL_SIZE, row * CELL_SIZE)); 
                
                add_child(tile);

                Tile til;

                til.type = typesOfSoil::weed;
                til.growth = 25;

                // Get current datetime dictionary
                Dictionary datetime =
                    Time::get_singleton()->get_datetime_dict_from_system();

                til.year   = datetime["year"];
                til.month  = datetime["month"];
                til.day    = datetime["day"];
                til.hour   = datetime["hour"];
                til.minute = datetime["minute"];
                til.second = datetime["second"];

                map_tiles[tile] = til;
                if (!file.write(reinterpret_cast<char*>(&til), sizeof(Tile))){
                    UtilityFunctions::print("adding a child to file", tile->id); 
                }
                
            }
        }
    }



    //auto map_tile = std::next(map_tiles.begin(), 16);
   

    file.flush();      // make sure data is written
    file.clear();      // clear eof/fail flags
    file.seekg(0);     // move read position to beginning


    Tile p;

    auto it = map_tiles.begin();

    while (file.read((char*)&p, sizeof(Tile)) && it != map_tiles.end()) {
        it->second = p;

        String path;

        switch (it->second.type)
        {
        case patatoes:
            path = "res://assets/icon.svg";
            break;
        case weed:
            path = "res://assets/grass.png";
            break;
        default:
            path = "res://assets/stuff.png";
            break;
        } 
        
        
        Ref<Texture2D> tex = ResourceLoader::get_singleton()->load(path);
        
        Vector2 tex_size = tex->get_size();
        it->first->sprite->set_scale(Vector2(
            75.0f / tex_size.x,
            75.0f / tex_size.y
        ));
        
        it->first->sprite->set_texture(tex);
        it->first->sprite->set_centered(false);
        it->first->sprite->set_position(Vector2(0, 0));
        
        it++;
    }

    // Tile bloco;
    // bloco.texture = ResourceLoader::get_singleton()->load("res://icon.svg");
}

void GDExample::_physics_process(double delta)
{
    if (!is_inside_tree())
        return;

    // line->clear_points();
    // line->set_default_color(Color(0, 0, 0)); // Black
    // line->add_point(Vector2(0, 0));
    // line->add_point(raycast->get_target_position());

    // float ray_length = 10.0f;
    // Vector2 mouse = get_global_mouse_position();
    // // Direction from the player to the mouse
    // Vector2 dir = mouse - get_global_position();
    // // Point the ray toward the mouse
    
    //float ray_length = 300.0f;

    // Vector2 mouse = get_global_mouse_position();
    // Vector2 direction = (mouse - raycast->get_global_position()).normalized();

    // raycast->set_target_position(direction * ray_length);
    
    // Vector2 mouse = get_global_mouse_position();
    // raycast->set_global_position(mouse);
    
    // // Example: point to the right
    // raycast->set_target_position(Vector2(0, 10));
    // raycast->force_raycast_update();

    Vector2 mouse = get_global_mouse_position();

    raycast->set_global_position(mouse + Vector2(0, -10));
    raycast->set_target_position(Vector2(0, 5));

    raycast->force_raycast_update();

    if (Input::get_singleton()->is_action_just_pressed("grab"))
    {
        if (raycast->is_colliding())
        {

            //UtilityFunctions::print("Clicked tile ");
            Object *obj = raycast->get_collider();

            if (m_tile *tile = Object::cast_to<m_tile>(obj))
            {
                UtilityFunctions::print("Clicked tile: ", tile->id);

                file.flush();      // make sure data is written
                file.clear();      // clear eof/fail flags
                file.seekg(0);

                auto map_tile = std::next(map_tiles.begin(), tile->id);
                map_tile->second.type = typesOfSoil::patatoes;

                
                String path;

                switch (map_tile->second.type)
                {
                case patatoes:
                    path = "res://assets/icon.svg";
                    break;
                case weed:
                    path = "res://assets/grass.png";
                    break;
                default:
                    path = "res://assets/stuff.png";
                    break;
                } 

                
                Ref<Texture2D> tex = ResourceLoader::get_singleton()->load(path);
                
                Vector2 tex_size = tex->get_size();
                
                map_tile->first->sprite->set_scale(Vector2(
                    75.0f / tex_size.x,
                    75.0f / tex_size.y
                ));
                
                map_tile->first->sprite->set_texture(tex);
                map_tile->first->sprite->set_centered(false);
                map_tile->first->sprite->set_position(Vector2(0, 0));

                file.seekp(tile->id * sizeof(Tile), std::ios::beg);
                file.write(reinterpret_cast<const char*>(&(map_tile->second)), sizeof(Tile));

            }
        }
    } 
    
    file.flush();      // make sure data is written
    file.clear();      // clear eof/fail flags
    file.seekg(0);     // move read position to beginning

    
}

void GDExample::_input(const Ref<InputEvent> &event)
{

}



void GDExample::_process(double delta) {

}


