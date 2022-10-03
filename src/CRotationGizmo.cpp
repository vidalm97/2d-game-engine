#include "CRotationGizmo.h"

#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CRotationGizmo::CRotationGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize ) :
	AGizmo( aGizmoTextures, aSize )
{
}

void CRotationGizmo::UpdateTransform( CComponentTransform* aTransform, const glm::vec2& aInitialPoint, const glm::vec2& aFinalPoint )
{
	float c = glm::distance( aTransform->GetPosition(), aFinalPoint );
	float b = glm::distance( aTransform->GetPosition(), aInitialPoint );
	float a = glm::distance( aFinalPoint, aInitialPoint );

	auto angle = glm::degrees(glm::acos( ( b*b + c*c - a*a )/(2.0f*b*c) ));
	if( aInitialPoint.x > aFinalPoint.x && aTransform->GetRotation() + angle < 180.0f )
		aTransform->SetRotation( aTransform->GetRotation() + angle );
	else if( aInitialPoint.x < aFinalPoint.x && aTransform->GetRotation() - angle > -180.0f )
		aTransform->SetRotation( aTransform->GetRotation() - angle );
}
