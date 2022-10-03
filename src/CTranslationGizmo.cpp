#include "CTranslationGizmo.h"

#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CTranslationGizmo::CTranslationGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize ) :
	AGizmo( aGizmoTextures, aSize )
{
}

void CTranslationGizmo::UpdateTransform( CComponentTransform* aTransform, const glm::vec2& aInitialPoint, const glm::vec2& aFinalPoint )
{
	auto deltaPos = aFinalPoint-aInitialPoint;
	switch( mCurrentSelectable )
	{
		// X Axis
		case 0:
			aTransform->SetPosition( aTransform->GetPosition() + ( glm::vec2(deltaPos.x,0) ) );
			break;
		// Y Axis
		case 1:
			aTransform->SetPosition( aTransform->GetPosition() + ( glm::vec2(0,deltaPos.y) ) );
			break;
	}
}
