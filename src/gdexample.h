#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/ray_cast2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <fstream>
#include <godot_cpp/classes/line2d.hpp>
#include <utility>
#include <godot_cpp/classes/timer.hpp>
#include "m_tile.h"


namespace godot {
enum CardState
{
    FACE_DOWN,
    FACE_UP,
    MATCHED
};

enum TileType {
    POTATO,
    BEE,
    WEED,
    CARROT,
    CORN,
    APPLE,
    CHICKEN,
    COW,
    SHEEP,
    TREE
};

struct TileData
{
    TileType type;
    CardState state = FACE_UP;
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
	Timer *preview_timer = nullptr;

	void _ready();
	void _process(double delta) override;
	
	bool is_empty();
	//void update_tile_sprite(std::pair<m_tile * const, Tile> &entry);
	void update_tile_sprite(m_tile *tile, const TileData &data);
	void _on_preview_timeout();

	std::fstream file;
	
	void _input(const Ref<InputEvent> &event) override;
	void _physics_process(double delta) override;
	


};

}

#endif