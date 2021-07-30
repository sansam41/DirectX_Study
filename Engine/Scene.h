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
	// vector�� ������ ��� Ư�� Object�� ã�� �� ������
	// ���� ��뿣���� ��� Vector�� ��ȣ�� �Űܼ�(Layer) ������ �迭�� ������ ������
	// Layer
	// [0] wall
	// [1] Monster
};

