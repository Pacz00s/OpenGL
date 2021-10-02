#pragma once
#include "mesh.h"
#include "transform3d.h"
class SceneObj
{
public:
	SceneObj();
	bool load(std::string filepath);
	void draw();
	void ustaw(int x, int y, int z);
	Transform3D zwroc();

private:

	Mesh* model;
	Transform3D polozenie;
};

