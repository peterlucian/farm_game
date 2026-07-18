#ifndef M_TILE_H
#define M_TILE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/ray_cast2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/variant/color.hpp>

#include <fstream>
namespace godot {

class m_tile : public Area2D {
	GDCLASS(m_tile, Area2D)
	

private:

protected:
	static void _bind_methods();

public:
	int id;
	Color color;
	CollisionShape2D *collision = nullptr;
	Sprite2D *sprite = nullptr;
	//RayCast2D *raycast = nullptr;

    m_tile();
	~m_tile();

	void _input(const Ref<InputEvent> &event) override;
	void _ready();
	void _process(double delta) override;



};

}

#endif