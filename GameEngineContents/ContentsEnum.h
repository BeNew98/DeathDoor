#pragma once

enum class OrderNum
{
	TEST
};


enum class PhysXFilterGroup //PhysX �浹 �� �׷�
{
	None = 0,
	PlayerDynamic = (1 << 0),		// �÷��̾� �浹ü
	Ground = (1 << 1),				// �ٴ� �浹ü
	Obstacle = (1 << 2),			// ��ֹ� �浹ü
	GroundTrigger = (1 << 3),		// Ʈ����

	LeverTrigger = (1 << 4 ),	    // Frog_Lever
};