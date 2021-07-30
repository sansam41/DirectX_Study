#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>	// �ڱ� �ڽ��� shared_pointer�� �ǳ��ְ��� �� �� ���
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
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;	// ������ ��ȣ, Ÿ�Դ� 1���� ����
	vector<shared_ptr<MonoBehaviour>> _scripts;	// scripts�� ���� ����
};

