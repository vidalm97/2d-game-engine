#include "CScaleGizmo.h"

#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CScaleGizmo::CScaleGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize ) :
	AGizmo( aGizmoTextures, aSize )
{
}

void CScaleGizmo::UpdateTransform( CComponentTransform* aTransform, const glm::vec2& aInitialPoint, const glm::vec2& aFinalPoint )
{
	auto deltaPos = aFinalPoint-aInitialPoint;
	switch( mCurrentSelectable )
	{
		// X Axis
		case 0:
			aTransform->SetScale( aTransform->GetScale() + ( glm::vec2(deltaPos.x,0) ) );
			break;
		// Y Axis
		case 1:
			aTransform->SetScale( aTransform->GetScale() + ( glm::vec2(0,deltaPos.y) ) );
			break;
	}
}
