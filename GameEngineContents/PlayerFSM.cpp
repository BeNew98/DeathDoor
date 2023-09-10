#include "PreCompileHeader.h"
#include "Player.h"
#include "PhysXCapsuleComponent.h"
#include "PlayerDefinition.h"





void Player::SetFSMFunc()
{
	InitFSM(PlayerState::MAX);



	//	IDLE,// Idle_0, Idle_1
	SetChangeFSMCallBack([this]
		{
			StateDuration = 0.0f;
			StateChecker = false;
		});

	SetFSM(PlayerState::IDLE,
		[this]
		{
			Renderer->ChangeAnimation("Idle_0");

		},
		[this](float Delta)
		{
			if (Renderer->IsAnimationEnd())
			{
				if (false == StateChecker)
				{
					Renderer->ChangeAnimation("Idle_1");

				}
				else
				{
					Renderer->ChangeAnimation("Idle_0");

				}
				StateChecker = !StateChecker;
			}

			CheckInput(Delta);
		},
		[this]
		{

		}
	);


	//WALK	// Walk, Run
	SetFSM(PlayerState::WALK,
		[this]
		{
			Renderer->ChangeAnimation("Run");


		},
		[this](float Delta)
		{
			//StateDuration += Delta;
			if (true == StateChecker)
			{
				Renderer->ChangeAnimation("Walk");
				if (true == Renderer->IsAnimationEnd())
				{
					SetNextState(PlayerState::IDLE);
				}

			}
			else
			{
				CheckInput(Delta);
			}

		},
		[this]
		{

		}
	);


	//SKILL	// ��Ŭ�� Arrow, Arrow_bomb, Arrow_magic, Hookshot, Hookshot_fly
	SetFSM(PlayerState::SKILL,
		[this]
		{
			switch (CurSkill)
			{
			case Player::PlayerSkill::ARROW:
				Renderer->ChangeAnimation("Arrow");
				break;
			case Player::PlayerSkill::MAGIC:
				Renderer->ChangeAnimation("Arrow_magic");
				break;
			case Player::PlayerSkill::BOMB:
				Renderer->ChangeAnimation("Arrow_bomb");
				break;
			case Player::PlayerSkill::HOOK:
				Renderer->ChangeAnimation("Hookshot");
				break;
			case Player::PlayerSkill::MAX:
				break;
			default:
				break;
			}
		},
		[this](float Delta)
		{
			StateDuration += Delta;
			if (true == GameEngineInput::IsPress("PlayerRBUTTON"))
			{
				// ���콺 ������ �ٶ󺸵��� ��
				NextForwardDir = GetMousDirection();
				DirectionUpdate(Delta);
			}

			if (true == Renderer->IsAnimationEnd() && false == GameEngineInput::IsPress("PlayerRBUTTON"))
			{
				//switch (CurSkill)
				//{
				//case Player::PlayerSkill::ARROW:
				//	break;
				//case Player::PlayerSkill::MAGIC:
				//	break;
				//case Player::PlayerSkill::BOMB:
				//	break;
				//case Player::PlayerSkill::HOOK:
				//	break;
				//case Player::PlayerSkill::MAX:
				//	break;
				//default:
				//	break;
				//}
			}
			CheckInput(Delta);
		},
		[this]
		{

		}
	); 


	//HOOK_FLY	// Hookshot_fly
	SetFSM(PlayerState::HOOK_FLY,
		[this]
		{
			Renderer->ChangeAnimation("Hookshot_fly"); 
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);


	//BASE_ATT	// ��Ŭ�� Slash_Light_L_new, Slash_Light_R_new
	SetFSM(PlayerState::BASE_ATT,
		[this]
		{
			if (true == isRightAttack)
			{
				Renderer->ChangeAnimation("Slash_Light_R_new");
			}
			else
			{
				Renderer->ChangeAnimation("Slash_Light_L_new");
			}
			isRightAttack = !isRightAttack;

			// ���콺 ������ �ٶ󺸵��� ��
			NextForwardDir = GetMousDirection(); },
		[this](float Delta)
		{
			DirectionUpdate(Delta);
			MoveDir = NextForwardDir;
			MoveUpdate(Delta);

			if (true == Renderer->IsAnimationEnd())
			{
				StateInputDelayTime = 0.3f;
				SetNextState(PlayerState::IDLE);
			}},
		[this]
		{
		}
	); 


	//ROLL	// �����̽��� Roll, Roll_slash
	// Roll �ϴ� ���� ��������
	SetFSM(PlayerState::ROLL,
		[this]
		{
			Renderer->ChangeAnimation("Roll");
			//StateDuration = 2.0f;
			mButton = false; },
		[this](float Delta)
		{
			m_pCapsuleComp->GetDynamic()->setLinearVelocity({ 0,0,0 });
			m_pCapsuleComp->SetMoveSpeed(MoveDir* MoveSpeed* RollSpeedRatio);
			if (true == GameEngineInput::IsDown("PlayerMBUTTON"))
			{
				mButton = true;
			}
			if (true == Renderer->IsAnimationEnd())
			{
				if (true == mButton)
				{
					SetNextState(PlayerState::ROLL_ATT);
				}
				else
				{
					StateInputDelayTime = 0.1f;
					SetNextState(PlayerState::IDLE);
				}
			}},
		[this]
		{
		}
	); 


	//ROLL_ATT	// �����̽���+��Ŭ�� Charge_slam_overhead, Roll_slash_end
	SetFSM(PlayerState::ROLL_ATT,
		[this]
		{
			Renderer->ChangeAnimation("Charge_slam_overhead");
		},
		[this](float Delta)
		{
			if (true == Renderer->IsAnimationEnd())
			{
				StateInputDelayTime = 0.5f;
				SetNextState(PlayerState::IDLE);
			}
		},
		[this]
		{
		}
	); 


	// CHARGE_ATT// ��Ŭ�� Charge_slash_L, Charge_slash_R
	SetFSM(PlayerState::CHARGE_ATT,
		[this]
		{
			if (true == isRightAttack)
			{
				Renderer->ChangeAnimation("Charge_slash_R");
			}
			else
			{
				Renderer->ChangeAnimation("Charge_slash_L");
			}
		},
		[this](float Delta)
		{
				if (false == GameEngineInput::IsPress("PlayerMBUTTON"))
				{
					SetNextState(PlayerState::BASE_ATT);
				}
		},
		[this]
		{
		}
	); 


	//HIT  	// ���ݴ��� Hit_back, Hit_idle,Hit_Recover
	SetFSM(PlayerState::HIT,
		[this]
		{
			Renderer->ChangeAnimation("Hit_back");
		},
		[this](float Delta)
		{
			static float IdleEndTime = 0;
			StateDuration += Delta;
			if (false == StateChecker && true == Renderer->IsAnimationEnd())
			{
				Renderer->ChangeAnimation("Player_HitIdle");
				StateChecker = true;
				IdleEndTime = StateDuration + PlayerHitIdleTime;
			}
			if (true == StateChecker && StateDuration > IdleEndTime)
			{
				Renderer->ChangeAnimation("Hit_Recover");
				if (true == Renderer->IsAnimationEnd())
				{
					SetNextState(PlayerState::IDLE);
				}
			}
		},
		[this]
		{
		}
	); 


	//CLIMB	// ��ٸ� Climbing_ladder, Climbing_ladder_down, Climbing_off_ladder_top
	SetFSM(PlayerState::CLIMB,
		[this]
		{
			Renderer->ChangeAnimation("Climbing_ladder");
			Renderer->PauseOn();

			m_pCapsuleComp->TurnOffGravity(); 
		},
		[this](float Delta)
		{
			CheckClimbInput(Delta);
			// ���� ��ٸ� ���� �����ؿ����� üũ�ϴ� �Լ�
			//if ()
			//{
			//
			//}
		},
		[this]
		{
				m_pCapsuleComp->TurnOnGravity();
				Renderer->PauseOff(); 
		}
	); 

	
	//LEVER // ������ ���� Push_Lever
	SetFSM(PlayerState::HOOK_FLY,
		[this]
		{
			Renderer->ChangeAnimation("Push_Lever");
		},
		[this](float Delta)
		{
			if (true == Renderer->IsAnimationEnd())
			{
				SetNextState(PlayerState::IDLE);
			}},
		[this]
		{
		}
	); 


	//ITEM
	// �������� ���� GetItem
	SetFSM(PlayerState::HOOK_FLY,
		[this]
		{
			Renderer->ChangeAnimation("GetItem");
		},
		[this](float Delta)
		{
			if (true == Renderer->IsAnimationEnd())
			{
				SetNextState(PlayerState::IDLE);
			}},
		[this]
		{
		}
	); 



	//DEAD
	// �ǰ����� ���� ��� Dead
	SetFSM(PlayerState::HOOK_FLY,
		[this]
		{
			Renderer->ChangeAnimation("Dead");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	); 


	//DROWN
	// �ͻ� Drown
	SetFSM(PlayerState::HOOK_FLY,
		[this]
		{
			Renderer->ChangeAnimation("Drown");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	); 



	//FALLING
	// ���� Falling
	SetFSM(PlayerState::HOOK_FLY,
		[this]
		{
			Renderer->ChangeAnimation("Falling");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	); 


	//FLY
	// ���̰� ���� ���� �� Fly, Land
	SetFSM(PlayerState::HOOK_FLY,
		[this]
		{
			Renderer->ChangeAnimation("Fly");
		},
		[this](float Delta)
		{
				// ���� �����Ͽ����� üũ
				//if ()
				//{
				//	Renderer->ChangeAnimation("Fly");
				//	StateChecker = true;
				//}
				//if (true == StateChecker && true == Renderer->IsAnimationEnd())
				//{
				//	CheckInput(Delta);
				//}
		},
		[this]
		{
		}
	); 

}


void Player::CheckClimbInput(float _DeltaTime)
{
	// ��ٸ� Climbing_ladder, Climbing_ladder_down, Climbing_off_ladder_top
	if (true == GameEngineInput::IsPress("PlayerUp"))
	{
		Renderer->ChangeAnimation("Climbing_ladder");
		Renderer->PauseOff();
		m_pCapsuleComp->SetMoveSpeed(float4::UP * MoveSpeed * ClimbSpeedRatio);
	}
	else if (true == GameEngineInput::IsPress("PlayerDown"))
	{
		Renderer->ChangeAnimation("Climbing_ladder_down");
		Renderer->PauseOff();
		m_pCapsuleComp->SetMoveSpeed(float4::DOWN * MoveSpeed * ClimbSpeedRatio);

	}
	else
	{
		Renderer->PauseOn();
	}
}
