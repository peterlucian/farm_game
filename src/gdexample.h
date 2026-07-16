#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/ray_cast2d.hpp>
#include <fstream>
namespace godot {

class GDExample : public Node2D {
	GDCLASS(GDExample, Node2D)
	

private:
	

protected:
	static void _bind_methods();

public:
	GDExample();
	~GDExample();

	void _ready();
	void _process(double delta) override;

	bool is_empty();
	RayCast2D* hand = nullptr;
	std::fstream file;


};

}

#endif