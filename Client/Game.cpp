#include "pch.h"	// �ڵ����� �߰���
#include "Game.h"
#include "Engine.h"
shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
shared_ptr<Texture> texture = make_shared<Texture>();

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	// ����(0,0)   X: ���� -1, ������ 1 / Y: �� 1, �Ʒ� -1 // ����°�� �����ϰ� ���� (shader���� �۾����ø� ���߱� ����)
	//vector<Vertex> vec(3);
	//vec[0].pos = Vec3(0.f, 0.5f, 0.5f);
	//vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);	// �� R
	//vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	//vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);	// �� G
	//vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	//vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);	// �� B


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

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");	//���̴� ����
	texture->Init(L"..\\Resources\\Texture\\Viktor.jpg");	//�ؽ�ó ����

	GEngine->GetCmdQueue()->WaitSync(); //����ȭ ���

}

void Game::Update()
{
	GEngine->RenderBegin();
	shader->Update();	// �ϰ� ����� / RootSignature�� ���������� ���Ե�
	{
		Transform t;
		t.offset = Vec4(0.25f, 0.25f, 0.f, 0.f);	// depth���� 0.2 ������ �� �ڷ� ���� �ȴ�
		mesh->SetTransform(t);	// Shader�� �Ѱ��� offset�� ������(������ �̵� & �� ���ϰ� ����)

		mesh->SetTexture(texture);

		mesh->Render();
	}


	{
		Transform t;
		t.offset = Vec4(0.f, 0.f, 0.f, 0.f);	
		mesh->SetTransform(t);	// Shader�� �Ѱ��� offset�� ������(������ �̵� & �� ���ϰ� ����)

		mesh->SetTexture(texture);
		
		mesh->Render();
	}




	GEngine->RenderEnd();
}
