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
#include <filesystem>
#include <utility>

using namespace godot;

void GDExample::_bind_methods() {
}



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

void GDExample::update_tile_sprite(m_tile *tile, const Tile &data)
{
    String path;

    switch (data.type) {
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

    tile->sprite->set_scale(Vector2(
        75.0f / tex_size.x,
        75.0f / tex_size.y
    ));

    tile->sprite->set_texture(tex);
    tile->sprite->set_centered(false);
    tile->sprite->set_position(Vector2(0, 0));
}

void GDExample::_ready()
{
    if (!is_inside_tree())
        return;
    
    raycast = get_node<RayCast2D>("../RayCast2D");
    line = get_node<Line2D>("../RayCast2D/Line2D");

   
    //file.open("save.txt", std::ios::in | std::ios::out | std::ios::trunc);
	 // If file doesn't exist, create it
   #include <filesystem>

    if (!std::filesystem::exists("save.txt")) {
        std::ofstream create("save.txt", std::ios::binary);
        create.close();
    }

    file.open("save.txt", std::ios::binary | std::ios::in | std::ios::out);

    if (!file.is_open()) {
        UtilityFunctions::printerr("Failed to open file");
    }

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
                
                
            }
        }
    }

    size_t tile_count = map_tiles.size();
    size_t expected_size = tile_count * sizeof(Tile);
    bool save_exists =
    std::filesystem::exists("save.txt") &&
    std::filesystem::file_size("save.txt") == expected_size;

    
  auto it = map_tiles.begin();

    if (save_exists) {

        Tile p;

        while (it != map_tiles.end() &&
            file.read(reinterpret_cast<char*>(&p), sizeof(Tile))) {

            it->second = p;
            update_tile_sprite(it->first, it->second);
            ++it;
        }
        UtilityFunctions::print("im inside true save existss");

    } else {

        for (auto &entry : map_tiles) {

            Tile &tile = entry.second;

            tile.type = typesOfSoil::weed;
            tile.growth = 25;

            Dictionary datetime =
                Time::get_singleton()->get_datetime_dict_from_system();

            tile.year   = datetime["year"];
            tile.month  = datetime["month"];
            tile.day    = datetime["day"];
            tile.hour   = datetime["hour"];
            tile.minute = datetime["minute"];
            tile.second = datetime["second"];

            file.write(reinterpret_cast<const char*>(&tile), sizeof(Tile));

            update_tile_sprite(entry.first, entry.second);
        }
        UtilityFunctions::print("im inside false save existss");
        file.flush();
    }


    // Tile p;

    // UtilityFunctions::print("expected size", expected_size);
    
    // auto it = map_tiles.begin();
    // if (save_exists){
    //     while (file.read((char*)&p, sizeof(Tile)) && it != map_tiles.end()) {
    //         UtilityFunctions::print("im inside true save existss");
    //         it->second = p;

    //         String path;

    //         switch (it->second.type)
    //         {
    //         case patatoes:
    //             path = "res://assets/icon.svg";
    //             break;
    //         case weed:
    //             path = "res://assets/grass.png";
    //             break;
    //         default:
    //             path = "res://assets/stuff.png";
    //             break;
    //         } 
            
            
    //         Ref<Texture2D> tex = ResourceLoader::get_singleton()->load(path);
            
    //         Vector2 tex_size = tex->get_size();
    //         it->first->sprite->set_scale(Vector2(
    //             75.0f / tex_size.x,
    //             75.0f / tex_size.y
    //         ));
            
    //         it->first->sprite->set_texture(tex);
    //         it->first->sprite->set_centered(false);
    //         it->first->sprite->set_position(Vector2(0, 0));
    //         it++;

    //         }
    // } else {
    //     UtilityFunctions::print("im inside false save existss");

    //     Tile till;
    //     till.type = typesOfSoil::weed;
    //     till.growth = 25;

    //     // Get current datetime dictionary
    //     Dictionary datetime =
    //         Time::get_singleton()->get_datetime_dict_from_system();

    //     till.year   = datetime["year"];
    //     till.month  = datetime["month"];
    //     till.day    = datetime["day"];
    //     till.hour   = datetime["hour"];
    //     till.minute = datetime["minute"];
    //     till.second = datetime["second"];

    //     file.write(reinterpret_cast<char*>(&till), sizeof(Tile));
    // }


    //auto map_tile = std::next(map_tiles.begin(), 16);
   

    file.flush();      // make sure data is written
    file.clear();      // clear eof/fail flags
    file.seekg(0);     // move read position to beginning

    
}

void GDExample::_physics_process(double delta)
{
    if (!is_inside_tree())
        return;

    // line->clear_points();
    // line->set_default_color(Color(0, 0, 0)); // Black
    // line->add_point(Vector2(0, 0));
    // line->add_point(raycast->get_target_position());


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
                file.write(reinterpret_cast<char*>(&map_tile->second), sizeof(Tile));
                
                if (file.fail()) {
                    UtilityFunctions::printerr("Write failed");
                }

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


