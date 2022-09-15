#ifndef CCAMERA__H
#define CCAMERA__H

#include "glm.hpp"

class CCamera
{
public:
	CCamera( const float aMovementSpeed = 1.0f, const float aFocusSpeed = 1.0f, const float aDistance = 1.0f,
			const glm::vec3& aPosition = glm::vec3( 0.0f, 0.0f, 1.0f ), const glm::vec3& aFront = glm::vec3( 0.0f, 0.0f, -1.0f ),
			const glm::vec3& aUp = glm::vec3( 0.0f, 1.0f, 0.0f ) );
	~CCamera() = default;

	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;

	void MoveCamera( const glm::vec3& aDirection );
	void Focus( const float aDistance );
	void UpdateProjectionMatrix();

private:
	float mMovementSpeed;
	float mFocusSpeed;
	float mDistance;

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;

	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
};

#endif
