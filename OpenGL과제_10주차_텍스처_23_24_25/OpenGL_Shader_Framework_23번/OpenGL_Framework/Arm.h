#pragma once
#include "Obj.h"
class CArm : public CObj
{


public:
	// CObj을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void Ready() override;
	virtual void Set_ModelTransform(unsigned int _modelLocation) override;

};

