#ifndef M_TILE_H
#define M_TILE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/variant/color.hpp>

#include <fstream>
namespace godot {

class m_tile : public ColorRect {
	GDCLASS(m_tile, ColorRect)
	

private:
	// int m_x, m_y;
    // Vector2 pos;

protected:
	static void _bind_methods();

public:
	int id;
	Color color;

    m_tile();
	~m_tile();

	void color_set (const Color&);

	void _draw() override;
	void _ready();
	void _process(double delta) override;



};

}

#endif