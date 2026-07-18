#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/ray_cast2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <fstream>
#include <godot_cpp/classes/line2d.hpp>

namespace godot {

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
	std::fstream file;

	void _input(const Ref<InputEvent> &event) override;
	void _physics_process(double delta) override;
	


};

}

#endif