#include "CComponentTransform.h"

#include "gtc/matrix_transform.hpp"

CComponentTransform::CComponentTransform( const bool aActive ) :
	AComponent( TRANSFORM, aActive ),
	mModelMatrix( glm::mat4(1.0f) ),
	mPosition( glm::vec2(0.0f,0.0f) ),
	mScale( glm::vec2(1.0f,1.0f) ),
	mRotation( 0.0f )
{
}

std::unique_ptr<AComponent> CComponentTransform::Clone() const
{
	return std::unique_ptr<AComponent>(new CComponentTransform(*this));
}

const glm::vec2& CComponentTransform::GetPosition() const
{
	return mPosition;
}

const glm::vec2& CComponentTransform::GetScale() const
{
	return mScale;
}

const float& CComponentTransform::GetRotation() const
{
	return mRotation;
}

void CComponentTransform::SetPosition( const glm::vec2& aPosition )
{
	mPosition = aPosition;
	UpdateModelMatrix();
}

void CComponentTransform::SetScale( const glm::vec2& aScale )
{
	mScale = aScale;
	UpdateModelMatrix();
}

void CComponentTransform::SetRotation( const float aRotation )
{
	mRotation = aRotation;
	UpdateModelMatrix();
}

const glm::mat4& CComponentTransform::GetModelMatrix() const
{
	return mModelMatrix;
}

void CComponentTransform::UpdateModelMatrix()
{
	const auto cos = glm::cos( glm::radians( mRotation ) );
	const auto sin = glm::sin( glm::radians( mRotation ) );

	mModelMatrix[3][0] = mPosition.x;
	mModelMatrix[3][1] = mPosition.y;
	mModelMatrix[0][0] = mScale.x*cos;
	mModelMatrix[1][1] = mScale.y*cos;
	mModelMatrix[0][1] = mScale.x*sin;
	mModelMatrix[1][0] = mScale.y*(-sin);
}

void CComponentTransform::Serialize( CSerializator::json& aJson ) const
{
	aJson["Position"] = {{"x", mPosition.x}, {"y", mPosition.y}};
	aJson["Scale"] = {{"x", mScale.x}, {"y", mScale.y}};
	aJson["Rotation"] = mRotation;
}

void CComponentTransform::Deserialize( const CSerializator::json& aJson )
{
	mPosition.x = aJson.at("Position")["x"];
	mPosition.y = aJson.at("Position")["y"];

	mScale.x = aJson.at("Scale")["x"];
	mScale.y = aJson.at("Scale")["y"];

	mRotation = aJson.at("Rotation");

	UpdateModelMatrix();
}
