#pragma once
class CObj
{
public:
	CObj();
	~CObj();

public:
	virtual void Update()=0;
	virtual void Render()=0;
	virtual void Ready()=0;

private:
	OBJECT object;
	
};

