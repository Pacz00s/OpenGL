#include "glm/gtc/matrix_transform.hpp"

class Transform2D {

private:
	float m_scale;
	float m_rotation;
	glm::vec2 m_position;

	// m_matrix is only calculated if matrixDirty is true.
	bool m_matrixDirty;
	glm::mat3 m_matrix;

public:
	Transform2D();
	
	// returns the scale
	float Scale();
	// returns the rotation in radians
	float Rotation();
	// returns the position as a vec2
	glm::vec2 Position();

	// sets the scale
	void SetScale(float s);
	// sets the rotation (radians)
	void SetRotation(float r);
	// sets the position vector
	void SetPosition(glm::vec2 v);

	// increments the rotation (radians)
	void Rotate(float r);
	// increments the position vector
	void Translate(glm::vec2 v);

	glm::mat3 GetMatrix();




};