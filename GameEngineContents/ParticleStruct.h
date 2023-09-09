#pragma once
#include "PreCompileHeader.h"

struct StaticParticleSetter
{
	/*��ƼŬ ������ ����(�ϴ��� z�� ���� x, y��)

	Example :

		Standard�� ���ذ�
		X : StandardDir.x - RangeDir.x ~ StandardDir.x + RangeDir.x
		Y : StandardDir.y - RangeDir.y ~ StandardDir.y + RangeDir.y
		Z : StandardDir.z - RangeDir.z ~ StandardDir.z + RangeDir.z

		---------------------------------------------------------------

		MaxParticle : ��ƼŬ�� �ִ밳��
		EmitInterTime : ��ƼŬ �����ֱ�
		MaxLiveTime : ��ƼŬ�� ����ִ� �ð� (���� 0.5f���, 0.5�� �Ŀ� �����)

	*/

	float4 StandardDir = { 0.0f, 0.0f, 0.0f };
	float4 RangeDir = { 0.0f, 0.0f, 0.0f };

	float4 StandardScale = { 0.0f, 0.0f, 0.0f };
	float4 RangeScale = { 0.0f, 0.0f, 0.0f };

	float4 StandardPosition = { 0.0f, 0.0f, 0.0f };
	float4 RangePosition = { 0.0f, 0.0f, 0.0f };

	float StandardSpeed = 0.0f;
	float range_Speed = 0.0f;

	//�� ���� ���⿡ ��� �����Ұ���.
	int EmitNum = 1;

	//��ƼŬ�� ���ÿ� ����� ������ �� �ִ°� or Disposable�̶�� ����� �����Ұǰ�.
	int MaxParticle = 0;

	//���� ����
	float EmitInterTime = 0.0f;

	//���� ����, �� �ð��� ���� ���İ� ���� �ð��� ���� (1.0f / MaxLiveTime * Deltatime)
	float MaxLiveTime = 0.0f;
};

struct DynamicParticleSetter
{
	//�÷��̾��� ��ġ�� ����, ���͵� ���ϱ� ������ �� �κе� �������� ���ϰ� �ؾ� �� �� ���� �ѵ� �ϴ��� ����
	float4 StandardDir = { 0.0f, 0.0f, 0.0f };
	float4 RangeDir = { 0.0f, 0.0f, 0.0f };

	float4 StandardScale = { 0.0f, 0.0f, 0.0f };
	float4 RangeScale = { 0.0f, 0.0f, 0.0f };

	//�÷��̾��� ��ġ ��, ���������� ������ �޾ƾ� �ϱ� ������ �����ͷ� ����
	const TransformData* StandardPosition;
	float4 RangePosition = {0.0f, 0.0f, 0.0f };

	float StandardSpeed = 0.0f;
	float range_Speed = 0.0f;

	//�� ���� ���⿡ ��� �����Ұ���.
	int EmitNum = 1;

	int MaxParticle = 0;
	float EmitInterTime = 0.0f;
	float MaxLiveTime = 0.0f;
};