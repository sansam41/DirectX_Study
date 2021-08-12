#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

public:


private:
	// 사용 불가
	virtual void FinalUpdate() sealed {}
};

