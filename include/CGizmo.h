#ifndef CGIZMO__H
#define CGIZMO__H

#include "CGameObject.h"

#include "glm.hpp"

class CGizmo
{
public:
	CGizmo( const bool aIsActive );
	~CGizmo() = default;

	const bool IsActive() const;

	void SetActive( const bool aIsActive );
	void SetPosition( const glm::vec2& aPosition );
	void SetSize( const glm::vec2& aSize );

	const glm::vec2& GetXOffset() const;
	const glm::vec2& GetYOffset() const;

	void AttachAxisTextures( const std::string& aXAxisTextPath, const std::string& aYAxisTextPath );

	CGameObject mXAxis;
	CGameObject mYAxis;

private:
	bool mIsActive;

	glm::vec2 mXOffset;
	glm::vec2 mYOffset;
};

#endif
