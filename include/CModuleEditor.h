#ifndef CMODULEEDITOR__H
#define CMODULEEDITOR__H

#include "AModule.h"

class CModuleEditor : public AModule
{
public:
	CModuleEditor() = default;
	~CModuleEditor() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;
};

#endif
