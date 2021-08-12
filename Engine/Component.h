#pragma once
#include "Object.h"
enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	// ...
	MONO_BEHAVIOUR,	// ������ ������ �����ؾ���, �������� �־��� �� �ֱ� ������ ���� �����Ͽ����ϱ� ����
	END,
};

enum
{
	// MONO_BEHAVIOUR�� ������ Component������ ����
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component:public Object
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();	// �θ� Ŭ������ ���Ǳ� ������ virtual �ʼ�

public:
	// 4�ѻ�
	virtual void Awake() { }	// ���� ����
	virtual void Start() { }	// Awake ������
	virtual void Update() { }	// �� ƽ���� ����
	virtual void LateUpdate() { }	// Update ���� ����
	virtual void FinalUpdate() {}

public:
	COMPONENT_TYPE GetType() { return _type; }
	bool IsValid() { return _gameObject.expired() == false; }	// �Ҹ� üũ

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject;	// GameObject���Ը� ���� ������ ����
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	weak_ptr<GameObject> _gameObject;	// gameObject�� Component�� ���� ������ ����Ǿ� �ִ���
										// �˰� �־�� �ϱ� ������ Shared_ptr�� ����ϸ� ��ȯ���� �߻�
										// ���� Weak_ptr�� ����
};

