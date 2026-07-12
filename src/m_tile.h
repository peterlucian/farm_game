#ifndef M_TILE_H
#define M_TILE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/color_rect.hpp>

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
    m_tile();
	//m_tile(int x, int y): m_x(x), m_y(y){};
	~m_tile();

	void _draw() override;
	void _ready();
	void _process(double delta) override;



};

}

#endif