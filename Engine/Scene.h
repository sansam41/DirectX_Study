#pragma once

class GameObject;

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);


private:
	vector<shared_ptr<GameObject>> _gameObjects;
	// vector로 관리할 경우 특정 Object를 찾을 때 불편함
	// 따라서 상용엔진의 경우 Vector를 번호를 매겨서(Layer) 벡터의 배열로 나누어 저장함
	// Layer
	// [0] wall
	// [1] Monster
};

