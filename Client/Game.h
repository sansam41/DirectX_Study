#pragma once

// 실제 게임을 구현하는 클래스
// 클라이언트 부분에 게임을 구현하면 너무 복잡하기 때문에 따로 빼서 관리
class Game
{
public:

	// cntl + . 을 누르면 cpp파일에 바로 구현부가 만들어짐
	void Init(const WindowInfo& info);
	void Update();
};


