#ifndef CTRANSLATION_GIZMO__H
#define CTRANSLATION_GIZMO__H

#include "AGizmo.h"

class CTranslationGizmo : public AGizmo
{
public:
	CTranslationGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize );
	~CTranslationGizmo() = default;

	void UpdateTransform( CComponentTransform* aTransform, const glm::vec2& aInitialPoint, const glm::vec2& aFinalPoint )  override;
};

#endif
