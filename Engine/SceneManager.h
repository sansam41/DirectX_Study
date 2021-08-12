#pragma once
class Scene;

enum {
	MAX_LAYER = 32,
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager);
public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;

	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;
};

