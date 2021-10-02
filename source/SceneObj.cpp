#include "../header/SceneObj.h"
SceneObj::SceneObj()
{
	model = nullptr;
}
bool SceneObj::load(std::string filepath) {
	std::cout << "Laduje " << filepath << std::endl;
	model = new Mesh(filepath);
	return 0;
}
void SceneObj::draw() {
	model->Draw();
}
void SceneObj::ustaw(int x, int y, int z) {
	polozenie.SetPosition(glm::vec3(x, y, z));
}
Transform3D SceneObj::zwroc() {
	return polozenie;
}
