#include "PreCompileHeader.h"
#include "Player.h"
#include "PhysXCapsuleComponent.h"

//IDLE,
//WALK,
//BASE_ATT,		// ��Ŭ��
//ROLL,			// �����̽���
//SLIDE_ATT,		// �����̽���+��Ŭ��
//CHARGE_ATT,		// ��Ŭ��
//SKILL,			// ��Ŭ��
//




void Player::SetFSMFunc()
{

	FSMFunc[PlayerState::IDLE].Start = [this]
		{
			Renderer->ChangeAnimation("Player_Idle");

		};

	FSMFunc[PlayerState::IDLE].Update = [this](float Delta)
		{
			m_pCapsuleComp->GetDynamic()->setLinearVelocity({ 0,0,0 });
			m_pCapsuleComp->SetMoveSpeed(float4::ZERO);
			CheckInput(Delta);

		};

	FSMFunc[PlayerState::IDLE].End = [this]
		{

		};



	FSMFunc[PlayerState::WALK].Start = [this]
		{
			Renderer->ChangeAnimation("Player_Walk_S");

			//StateDuration = 0.5f;

			//float rot = MoveDir.x * 90 - (1 - MoveDir.z) * 90;
			//GetTransform()->SetLocalRotation(float4{ 0,rot,0 });
		};

	FSMFunc[PlayerState::WALK].Update = [this](float Delta)
		{
			CheckInput(Delta);
			//StateDuration -= Delta;
			//if (StateDuration < 0.0f)
			//{
			//	NextState = PlayerState::IDLE;
			//}

		};

	FSMFunc[PlayerState::WALK].End = [this]
		{

		};



	FSMFunc[PlayerState::BASE_ATT].Start = [this]
		{
			if (true == isRightAttack)
			{
				Renderer->ChangeAnimation("Player_Att_R");
			}
			else
			{
				Renderer->ChangeAnimation("Player_Att_Left");
			}
			isRightAttack = !isRightAttack;
		};

	FSMFunc[PlayerState::BASE_ATT].Update = [this](float Delta)
		{
			if (true == Renderer->IsAnimationEnd())
			{
				NextState = PlayerState::IDLE;
			}

		};

	FSMFunc[PlayerState::BASE_ATT].End = [this]
		{

		};



	FSMFunc[PlayerState::ROLL].Start = [this]
		{
			Renderer->ChangeAnimation("Player_Roll");
			//StateDuration = 2.0f;
			mButton = false;
		};

	FSMFunc[PlayerState::ROLL].Update = [this](float Delta)
		{
			if (true == GameEngineInput::IsDown("PlayerMBUTTON"))
			{
				mButton = true;
			}
			if (true == Renderer->IsAnimationEnd())
			{
				if (true == mButton)
				{
					NextState = PlayerState::SLIDE_ATT;
				}
				else
				{
					CheckInput(Delta);
				}
			}
			//StateDuration -= Delta;
			//if (StateDuration < 0.0f)
			//{
			//	NextState = PlayerState::IDLE;
			//}

			//if (true == GameEngineInput::IsPress("PlayerMBUTTON"))
			//{
			//	NextState = PlayerState::SLIDE_ATT;
			//	return;
			//}
		};

	FSMFunc[PlayerState::ROLL].End = [this]
		{

		};



	FSMFunc[PlayerState::SLIDE_ATT].Start = [this]
		{
			Renderer->ChangeAnimation("Player_SlideAtt");

		};

	FSMFunc[PlayerState::SLIDE_ATT].Update = [this](float Delta)
		{
			if (true == Renderer->IsAnimationEnd())
			{
				CheckInput(Delta);
			}
		};

	FSMFunc[PlayerState::SLIDE_ATT].End = [this]
		{

		};



	FSMFunc[PlayerState::CHARGE_ATT].Start = [this]
		{
			if (true == isRightAttack)
			{
				Renderer->ChangeAnimation("Player_Att1");
			}
			else
			{
				Renderer->ChangeAnimation("Player_Att2");
			}
			isRightAttack = !isRightAttack;
		};

	FSMFunc[PlayerState::CHARGE_ATT].Update = [this](float Delta)
		{
			
			if (true == Renderer->IsAnimationEnd())
			{
				CheckInput(Delta);
			}
		};

	FSMFunc[PlayerState::CHARGE_ATT].End = [this]
		{

		};



	FSMFunc[PlayerState::SKILL].Start = [this]
		{
			switch (CurSkill)
			{
			case Player::PlayerSkill::ARROW:
				Renderer->ChangeAnimation("Player_Arrow");
				break;
			case Player::PlayerSkill::FIRE:
				Renderer->ChangeAnimation("Player_Magic");
				break;
			case Player::PlayerSkill::BOMB:
				break;
			case Player::PlayerSkill::HOOK:
				break;
			case Player::PlayerSkill::MAX:
				break;
			default:
				break;
			}

		};

	FSMFunc[PlayerState::SKILL].Update = [this](float Delta)
		{
			if (true == Renderer->IsAnimationEnd())
			{
				CheckInput(Delta);
			}
		};

	FSMFunc[PlayerState::SKILL].End = [this]
		{

		};
	


	FSMFunc[PlayerState::ATTED].Start = [this]
		{
			isDown = false;
			Renderer->ChangeAnimation("Player_HitBack");
		};

	FSMFunc[PlayerState::ATTED].Update = [this](float Delta)
		{
			if (false == isDown && true == Renderer->IsAnimationEnd())
			{
				Renderer->ChangeAnimation("Player_HitIdle");
				isDown = true;
			}
			else if (true == isDown && true == Renderer->IsAnimationEnd())
			{
				NextState = PlayerState::IDLE;
			}
		};

	FSMFunc[PlayerState::ATTED].End = [this]
		{

		};



}