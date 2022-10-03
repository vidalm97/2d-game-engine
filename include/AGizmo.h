#ifndef AGIZMO__H
#define AGIZMO__H

#include "CComponentTransform.h"
#include "CGameObject.h"

#include "glm.hpp"

struct SGizmoTexture
{
	std::string mTexturePath;
	unsigned int mUniqueColorId;
	glm::vec2 mOffset;
};

class AGizmo
{
public:
	AGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize );
	~AGizmo() = default;

	void SetPosition( const glm::vec2& aPosition );
	void SetSize( const glm::vec2& aSize );
	const glm::vec2& GetSize() const;
	void SetCurrentSelectable( const int aCurrentSelectable );

	const std::vector<std::pair<CGameObject, glm::vec2>>& GetSelectables() const;
	void AddSelectables( const std::vector<SGizmoTexture>& aGizmoTextures );

	virtual void UpdateTransform( CComponentTransform* aTransform, const glm::vec2& aInitialPoint, const glm::vec2& aFinalPoint ) = 0;
protected:
	int mCurrentSelectable;
	std::vector<std::pair<CGameObject, glm::vec2>> mSelectables;
	glm::vec2 mSize;
};

#endif
