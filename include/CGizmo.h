#ifndef CGIZMO__H
#define CGIZMO__H

#include "CGameObject.h"

#include "glm.hpp"

class CGizmo
{
public:
	CGizmo( const bool aIsActive, const unsigned int aUniqueColorId, const unsigned int aYUniqueColorId );
	~CGizmo() = default;

	const bool IsActive() const;

	void SetActive( const bool aIsActive );
	void SetPosition( const glm::vec2& aPosition );
	void SetSize( const glm::vec2& aSize );

	const CGameObject& GetXAxis() const;
	const CGameObject& GetYAxis() const;
	const glm::vec2& GetXOffset() const;
	const glm::vec2& GetYOffset() const;

	void AttachAxisTextures( const std::string& aXAxisTextPath, const std::string& aYAxisTextPath );

private:
	bool mIsActive;

	CGameObject mXAxis;
	CGameObject mYAxis;
	glm::vec2 mXOffset;
	glm::vec2 mYOffset;
};

#endif
