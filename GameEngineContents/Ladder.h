#pragma once
#include "TriggerBase.h"
#define LadderHeight 550.f

//class InteractionData
//{
//	friend class Player;
//	friend class Ladder;
//	friend class Frog_Lever;
//
//	enum class InteractionDataType
//	{
//		None,
//		Ladder,
//		Lever,
//	};
//	InteractionDataType Type = InteractionDataType::None;
//	float4 Pos = float4::ZERONULL;
//	float4 Dir = float4::ZERONULL;
//
//};

// 설명 :
class Ladder : public TriggerBase
{
public:
	// constrcuter destructer
	Ladder();
	~Ladder();

	// delete Function
	Ladder(const Ladder& _Other) = delete;
	Ladder(Ladder&& _Other) noexcept = delete;
	Ladder& operator=(const Ladder& _Other) = delete;
	Ladder& operator=(Ladder&& _Other) noexcept = delete;


	void SetHidden(bool _Value)
	{
		m_bHidden = _Value; 
		if (true == m_bHidden)
		{
			SetNextState(TriggerState::OFF,true);
		}
	}

	void SetHeight(int _Amount)
	{
		//Ladder clone의 크기 확인해서 한개의 크기만큼 줄여버리기
		m_fHeight = 50.f*static_cast<float>(_Amount);
		m_pRenderer->GetTransform()->SetLocalPosition(float4(0, -(LadderHeight - m_fHeight), 0));
	}

	

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void InitComponent() override;
	void InitAnimation();

private:
	void SetFSMFUNC();

	bool m_bHidden = false;

	float m_fHeight = 0.f;
	InteractionData m_sData;
	//LadderData m_sData = {};
};
