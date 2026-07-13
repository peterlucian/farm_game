#include "gdexample.h"
#include "m_tile.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node2d.hpp>
//#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/classes/time.hpp>
#include <fstream>
#include <godot_cpp/variant/dictionary.hpp>
#include <iostream>
#include <vector>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/memory.hpp>
#include <map>



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
    Color color;    
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

    
    
    //file.close();
}


void GDExample::_ready()
{
   
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

   
    const int CELL_SIZE = 50;
    const int COLS = 25;
    const int ROWS = 14;
    const int THICKNESS = 4;
    
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
                tile->set_size(Vector2(50, 50));
                tile->set_color( Color(0, 0, 1, 1));
                tile->set_position(Vector2(col * CELL_SIZE, row * CELL_SIZE));
                tile->id = next_id++;
                UtilityFunctions::print("adding a child", tile->id); 
                
                add_child(tile);

                Tile til;
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
                if(!is_empty()){
                if (!file.write(reinterpret_cast<char*>(&til), sizeof(Tile))){
                    UtilityFunctions::print("adding a child to file", tile->id); 

                }}
            }
        }
    }



    auto map_tile = std::next(map_tiles.begin(), 16);
    map_tile->second.color = Color(0, 1, 1, 1);
    map_tile->first->color_set(map_tile->second.color); 

    file.flush();      // make sure data is written
    file.clear();      // clear eof/fail flags
    file.seekg(0);     // move read position to beginning


    Tile p;
    auto it = map_tiles.begin();
    while (file.read((char*)&p, sizeof(Tile)) && it != map_tiles.end()) {
        it->second = p;      // Replace the Tile data
        ++it;
    }

    file.close();
}



void GDExample::_process(double delta) {

}


