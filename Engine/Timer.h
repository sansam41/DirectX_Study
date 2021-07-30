#pragma once


class Timer
{
	DECLARE_SINGLE(Timer);
public:
	void Init();
	void Update();

	uint32 GetFps() { return _fps; }	// Frame per second 최신 평균적인 Frame
	float GetDeltaTime() { return _deltaTime; }

private:
	uint64	_frequency = 0;
	uint64	_prevCount = 0;
	float	_deltaTime = 0.f;	// 이전 프레임에서 현재 프레임까지 경과된 시간

private:
	// 프레임을 계산하기 위한 용도
	uint32	_frameCount = 0;		// Update문에 몇번 실행되었는지 계산.
	float	_frameTime = 0.f;		// 누적시간
	uint32	_fps = 0;
};

