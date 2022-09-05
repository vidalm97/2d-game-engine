#include "CComponentBoxCollider.h"

CComponentBoxCollider::CComponentBoxCollider( const glm::vec2& aCenter, const glm::vec2& aSize, const bool aIsTrigger ) :
	mCenter( aCenter ),
	mSize( aSize ),
	mIsTrigger( aIsTrigger )
{
}

const glm::vec2& CComponentBoxCollider::GetCenter() const
{
	return mCenter;
}

const glm::vec2& CComponentBoxCollider::GetSize() const
{
	return mSize;
}

const bool CComponentBoxCollider::IsTrigger() const
{
	return mIsTrigger;
}

void CComponentBoxCollider::SetCenter( const glm::vec2& aCenter )
{
	mCenter = aCenter;
}

void CComponentBoxCollider::SetSize( const glm::vec2& aSize )
{
	mSize = aSize;
}

void CComponentBoxCollider::SetTrigger( const bool aIsTrigger )
{
	mIsTrigger = aIsTrigger;
}
