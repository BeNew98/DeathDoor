#pragma once

enum class OrderNum
{
	TEST
};


enum class PhysXFilterGroup
{
	PlayerDynamic = (1 << 0),		// �÷��̾� �浹ü(HitCallback�� ȣ���� �뵵)
	Ground = (1 << 1),				// �ٴ� �浹ü(HitCallback�� ȣ���� �뵵)
	Obstacle = (1 << 2),			// ��ֹ� �浹ü(HitCallback�� ȣ���� �뵵)
	GroundTrigger = (1 << 3),		// �� Ʈ���� (����)
};