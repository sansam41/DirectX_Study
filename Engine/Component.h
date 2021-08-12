#pragma once
#include "Object.h"
enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	// ...
	MONO_BEHAVIOUR,	// 마지막 순서로 등장해야함, 여러개를 넣어줄 수 있기 때문에 따로 관리하여야하기 때문
	END,
};

enum
{
	// MONO_BEHAVIOUR을 제외한 Component개수를 저장
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component:public Object
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();	// 부모 클래스로 사용되기 때문에 virtual 필수

public:
	// 4총사
	virtual void Awake() { }	// 가장 먼저
	virtual void Start() { }	// Awake 다음에
	virtual void Update() { }	// 매 틱마다 실행
	virtual void LateUpdate() { }	// Update 이후 실행
	virtual void FinalUpdate() {}

public:
	COMPONENT_TYPE GetType() { return _type; }
	bool IsValid() { return _gameObject.expired() == false; }	// 소멸 체크

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject;	// GameObject에게만 접근 권한을 설정
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	weak_ptr<GameObject> _gameObject;	// gameObject와 Component는 서로 누구와 연결되어 있는지
										// 알고 있어야 하기 때문에 Shared_ptr을 사용하면 순환구조 발생
										// 따라서 Weak_ptr로 선언
};

