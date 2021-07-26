#include "pch.h"	// 자동으로 추가됨
#include "Game.h"
#include "Engine.h"
shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
shared_ptr<Texture> texture = make_shared<Texture>();

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	// 중점(0,0)   X: 왼쪽 -1, 오른쪽 1 / Y: 위 1, 아래 -1 // 세번째는 무시하고 있음 (shader에서 작업지시를 안했기 때문)
	//vector<Vertex> vec(3);
	//vec[0].pos = Vec3(0.f, 0.5f, 0.5f);
	//vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);	// 빨 R
	//vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	//vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);	// 초 G
	//vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	//vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);	// 파 B


	// VertexOnly Ver.
	//vector<Vertex> vec(6);
	//vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	//vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	//vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	//vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	//vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	//vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	//vec[3].pos = Vec3(0.5f, -0.5f, 0.5f);
	//vec[3].color = Vec4(0.f, 0.f, 1.f, 1.f);
	//vec[4].pos = Vec3(-0.5f, -0.5f, 0.5f);
	//vec[4].color = Vec4(0.f, 1.f, 0.f, 1.f);
	//vec[5].pos = Vec3(-0.5f, 0.5f, 0.5f);
	//vec[5].color = Vec4(1.f, 0.f, 0.f, 1.f);


	// Vertex + Index Buffer Ver.
	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vec2(0.0f, 0.0f);
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vec2(1.0f, 0.0f);
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vec2(1.0f, 1.0f);
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vec2(0.0f, 1.0f);

	vector<uint32> indexvec;
	{
		indexvec.push_back(0);
		indexvec.push_back(1);
		indexvec.push_back(2);
	}
	{
		indexvec.push_back(0);
		indexvec.push_back(2);
		indexvec.push_back(3);
	}

	mesh->Init(vec, indexvec);

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");	//셰이더 매핑
	texture->Init(L"..\\Resources\\Texture\\Viktor.jpg");	//텍스처 매핑

	GEngine->GetCmdQueue()->WaitSync(); //동기화 대기

}

void Game::Update()
{
	GEngine->RenderBegin();
	shader->Update();	// 일감 기술서 / RootSignature도 간접적으로 포함됨
	{
		Transform t;
		t.offset = Vec4(0.25f, 0.25f, 0.f, 0.f);	// depth값이 0.2 더해져 더 뒤로 가게 된다
		mesh->SetTransform(t);	// Shader에 넘겨져 offset을 세팅함(오른쪽 이동 & 더 진하게 빨강)

		mesh->SetTexture(texture);

		mesh->Render();
	}


	{
		Transform t;
		t.offset = Vec4(0.f, 0.f, 0.f, 0.f);	
		mesh->SetTransform(t);	// Shader에 넘겨져 offset을 세팅함(오른쪽 이동 & 더 진하게 빨강)

		mesh->SetTexture(texture);
		
		mesh->Render();
	}




	GEngine->RenderEnd();
}
