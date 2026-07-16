#include "m_tile.h"
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;

void m_tile::_bind_methods() {
}


m_tile::m_tile(){

}

m_tile::~m_tile() {
}


void m_tile::_ready()
{
    
    sprite = memnew(Sprite2D);
    // Load texture
    Ref<Texture2D> tex = ResourceLoader::get_singleton()->load("res://assets/grass.png");
    Vector2 tex_size = tex->get_size();
    
    sprite->set_scale(Vector2(
        50.0f / tex_size.x,
        50.0f / tex_size.y
    ));
    sprite->set_texture(tex);
    sprite->set_centered(false);
    sprite->set_position(Vector2(0, 0));
    add_child(sprite);

    collision = memnew(CollisionShape2D);
    Ref<RectangleShape2D> rect;
    rect.instantiate();
    rect->set_size(Vector2(50, 50));

    collision->set_shape(rect);
    // UtilityFunctions::print("Texture size: ", tex->get_size());
    // UtilityFunctions::print("Sprite scale: ", sprite->get_scale());

    add_child(collision);
}

void m_tile::_process(double delta) {

}
