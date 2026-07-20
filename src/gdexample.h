#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/ray_cast2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <fstream>
#include <godot_cpp/classes/line2d.hpp>
#include <utility>
#include "m_tile.h"


namespace godot {

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

class GDExample : public Node2D {
	GDCLASS(GDExample, Node2D)
	

private:
	

protected:
	static void _bind_methods();

public:
	GDExample();
	~GDExample();
	RayCast2D *raycast = nullptr;
	Line2D *line = nullptr;
	
	void _ready();
	void _process(double delta) override;
	
	bool is_empty();
	//void update_tile_sprite(std::pair<m_tile * const, Tile> &entry);
	void update_tile_sprite(m_tile *tile, const Tile &data);
	std::fstream file;
	
	void _input(const Ref<InputEvent> &event) override;
	void _physics_process(double delta) override;
	


};

}

#endif