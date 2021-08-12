#pragma once
#include"Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE {
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영

};
class Camera:public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;


	void SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	PROJECTION_TYPE GetProjectionType() { return _type; }


	void SortGameObject();
	void Render_Deferred();
	void Render_Forward();

	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			_cullingMask |= (1 << layer);	// 해당 플래그를 무조건 On
		else
			_cullingMask &= ~(1 << layer);	// 해당 플래그를 무조건 Off
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;	// 45˚
	float _scale = 1.f;

	Matrix _matView = {};
	Matrix _matProjection = {};


	Frustum _frustum;

	// 비트의 켜짐 꺼짐 여부에 따라 유효한 Layer을 판단
	uint32 _cullingMask = 0;


private:
	vector<shared_ptr<GameObject>>	_vecDeferred;
	vector<shared_ptr<GameObject>>	_vecForward;


public:
	// TEMP
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

