#ifndef CSCALE_GIZMO__H
#define CSCALE_GIZMO__H

#include "AGizmo.h"

class CScaleGizmo : public AGizmo
{
public:
	CScaleGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize );
	~CScaleGizmo() = default;

	void UpdateTransform( CComponentTransform* aTransform, const glm::vec2& aInitialPoint, const glm::vec2& aFinalPoint )  override;
};

#endif
