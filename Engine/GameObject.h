#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>	// 자기 자신의 shared_pointer을 건네주고자 할 때 사용
{
public:
	GameObject();
	virtual ~GameObject();

	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	shared_ptr<Transform> GetTransform();

	void AddComponent(shared_ptr<Component> component);

private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;	// 고정된 번호, 타입당 1개만 존재
	vector<shared_ptr<MonoBehaviour>> _scripts;	// scripts는 따로 관리
};

