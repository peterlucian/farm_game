#include "m_tile.h"
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

using namespace godot;

void m_tile::_bind_methods() {
}


m_tile::m_tile(){

}

m_tile::~m_tile() {
}


void m_tile::_ready()
{
    //raycast = get_node<RayCast2D>("../RayCast2D");

    
    sprite = memnew(Sprite2D);
    // Load texture
    Ref<Texture2D> tex = ResourceLoader::get_singleton()->load("res://assets/grass.png");
    Vector2 tex_size = tex->get_size();
    
    sprite->set_scale(Vector2(
        75.0f / tex_size.x,
        75.0f / tex_size.y
    ));
    sprite->set_texture(tex);
    sprite->set_centered(false);
    sprite->set_position(Vector2(0, 0));
    add_child(sprite);

    collision = memnew(CollisionShape2D);

    Ref<RectangleShape2D> rect;
    rect.instantiate();
    rect->set_size(Vector2(75, 75));

    collision->set_shape(rect);
    collision->set_position(Vector2(37.5f, 37.5f));


    add_child(collision);
}


void m_tile::_input(const Ref<InputEvent> &event)
{
//     if (!is_inside_tree())
//         return;

//     Ref<InputEventMouseButton> mb = event;

//     if (mb.is_valid() &&
//         mb->is_pressed() &&
//         mb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT)
//     {
//         raycast->force_raycast_update();

//         if (raycast->is_colliding())
//         {
//             if (m_tile *tile = Object::cast_to<m_tile>(raycast->get_collider()))
//             {
//                 UtilityFunctions::print("Clicked tile ", tile->id);
//             }
//         }
//     }

}

void m_tile::_process(double delta) {

}
