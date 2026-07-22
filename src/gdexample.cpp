#include "gdexample.h"
#include "m_tile.h"
#include <vector>
#include <random>
#include <algorithm>
#include <filesystem>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/variant/color.hpp>
#include <fstream>
#include <iostream>
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
#include <utility>
#include <iterator>


using namespace godot;


void GDExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_preview_timeout"),
                         &GDExample::_on_preview_timeout);
}




struct CompareTile{
    bool operator()(const m_tile* a, const m_tile* b) const {
        return a->id < b->id;
    }
};

std::map<m_tile*, TileData, CompareTile> map_tiles;

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

    

void GDExample::update_tile_sprite(m_tile *tile, const TileData &data)
{
    const String texture_paths[] = {
        "res://assets/potato.png",
        "res://assets/bee.png",
        "res://assets/weed.png",
        "res://assets/carrot.png",
        "res://assets/corn.png",
        "res://assets/apple.png",
        "res://assets/chicken.png",
        "res://assets/cow.png",
        "res://assets/sheep.png",
        "res://assets/tree.png"
    };
    
    int index = static_cast<int>(data.type);
    constexpr int TEXTURE_COUNT = 10;
    if (data.state == FACE_DOWN)
        {
            Ref<Texture2D> tex = ResourceLoader::get_singleton()->load("res://assets/back.png");
        } else {
        
        if (index < 0 || index >= TEXTURE_COUNT) {
            UtilityFunctions::printerr("Invalid TileType: ", index);
            return;
        }

        Ref<Texture2D> tex = ResourceLoader::get_singleton()->load(texture_paths[index]);

        Vector2 tex_size = tex->get_size();
        tile->sprite->set_scale(Vector2(
            75.0f / tex_size.x,
            75.0f / tex_size.y
        ));

        tile->sprite->set_texture(tex);
        tile->sprite->set_centered(false);
        tile->sprite->set_position(Vector2(0, 0));
    }
}

void GDExample::_on_preview_timeout()
{
    UtilityFunctions::print("5 seconds have passed!");

    for (auto &entry : map_tiles)
    {
        // Turn every card face down
        entry.first->sprite->set_texture(
            ResourceLoader::get_singleton()->load("res://assets/back.png")
        );
    }
}

void GDExample::_ready()
{
    
    preview_timer = memnew(Timer);
    preview_timer->set_wait_time(5.0);
    preview_timer->set_one_shot(true);

    add_child(preview_timer);

    preview_timer->connect(
        "timeout",
        Callable(this, "_on_preview_timeout")
    );

    preview_timer->start();

    raycast = get_node<RayCast2D>("../RayCast2D");
    line = get_node<Line2D>("../RayCast2D/Line2D");

   
    //file.open("save.txt", std::ios::in | std::ios::out | std::ios::trunc);
	 // If file doesn't exist, create it
   

    if (!std::filesystem::exists("save.txt")) {
        std::ofstream create("save.txt", std::ios::binary);
        create.close();
    }

    file.open("save.txt", std::ios::binary | std::ios::in | std::ios::out);

    if (!file.is_open()) {
        UtilityFunctions::printerr("Failed to open file");
    }

    const int CELL_SIZE = 75;
    const int COLS = 5;
    const int ROWS = 4;
    const int THICKNESS = 3;
    
    int next_id = 0;

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            // bool draw =
            //     (col < THICKNESS) ||                      // Left side
            //     (col >= COLS - THICKNESS) ||             // Right side
            //     (row >= ROWS - THICKNESS);               // Bottom
            
            if (true)
            {
                m_tile *tile = memnew(m_tile);
                tile->set_position(Vector2(col * CELL_SIZE, row * CELL_SIZE ));
                tile->id = next_id++;
        
                add_child(tile);       
                
                TileData icon = {TileType::APPLE, CardState::FACE_UP};
                map_tiles[tile] = icon;
                
                
            }
        }
    }

    std::vector<TileType> types;
    for (int i = 0; i < 10; i++) {
        types.push_back(static_cast<TileType>(i));
        types.push_back(static_cast<TileType>(i));
    }
    

    UtilityFunctions::print("mnaptiles", map_tiles.size());
    UtilityFunctions::print("tiles", types.size());
   

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(types.begin(), types.end(), gen);

    size_t tile_count = map_tiles.size();
    size_t expected_size = tile_count * sizeof(TileType);
    bool save_exists =
    std::filesystem::exists("save.txt") &&
    std::filesystem::file_size("save.txt") == expected_size;

    file.clear();
    
    if (save_exists) {
        
        file.seekg(0, std::ios::beg);
        TileType p;
        auto it = map_tiles.begin();

        while (it != map_tiles.end() &&
            file.read(reinterpret_cast<char*>(&p), sizeof(TileType))) {

            it->second.type = p;
            update_tile_sprite(it->first, it->second);
            ++it;
        }
        UtilityFunctions::print("im inside true save existss");

    } else {
        
        file.seekp(0, std::ios::beg);
        int index = 0;

        for (auto &entry : map_tiles) {

            entry.second.type = types[index++];

            file.write(reinterpret_cast<const char*>(&entry.second), sizeof(TileType));

            update_tile_sprite(entry.first, entry.second);
        }
        UtilityFunctions::print("im inside false save existss");
        
        file.flush();
    }



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
                //map_tile->second.type = TileType::BEE;      

                map_tile->second.state = FACE_UP;
                update_tile_sprite(map_tile->first, map_tile->second);

                file.seekp(tile->id * sizeof(TileType), std::ios::beg);
                file.write(reinterpret_cast<char*>(&map_tile->second), sizeof(TileType));
                
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


