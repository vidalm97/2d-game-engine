#ifndef CROTATION_GIZMO__H
#define CROTATION_GIZMO__H

#include "AGizmo.h"

class CRotationGizmo : public AGizmo
{
public:
	CRotationGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize );
	~CRotationGizmo() = default;

	void UpdateTransform( CComponentTransform* aTransform, const glm::vec2& aInitialPoint, const glm::vec2& aFinalPoint )  override;
};

#endif
