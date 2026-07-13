#include "m_tile.h"
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/vector2.hpp>
using namespace godot;

void m_tile::_bind_methods() {
}

void m_tile::color_set(const Color &color){
    set_color(color);
}

m_tile::m_tile(){

}

m_tile::~m_tile() {
}



void m_tile::_draw()
{
    draw_rect(Rect2(Vector2(0, 0), Vector2(50, 50)), Color(1, 1, 1), false, 2.0);
    
}
void m_tile::_ready()
{
    // pos = Vector2(m_x, m_y);
    // this->set_position(pos);
}

void m_tile::_process(double delta) {

}
