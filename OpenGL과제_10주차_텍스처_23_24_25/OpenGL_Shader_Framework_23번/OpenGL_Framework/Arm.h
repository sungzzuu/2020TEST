#pragma once
#include "Obj.h"
class CArm : public CObj
{


public:
	// CObj��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;
	virtual void Ready() override;
	virtual void Set_ModelTransform(unsigned int _modelLocation) override;

};

