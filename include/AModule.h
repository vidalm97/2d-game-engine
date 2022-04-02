#ifndef AMODULE__H
#define AMODULE__H

// Class to handle an specific functionality of the Engine
class AModule
{
public:
	AModule() = default;
	virtual ~AModule() = default;

	virtual bool Init(){ return true; }

	virtual bool PreUpdate(){ return true; }
	virtual bool Update() = 0;
	virtual bool PostUpdate(){ return true; }

	virtual bool Clear(){ return true; }
};

#endif
