#include "PreCompileHeader.h"
#include "FSMObjectBase.h"

FSMObjectBase::FSMObjectBase() 
{
}

FSMObjectBase::~FSMObjectBase() 
{
}

void FSMObjectBase::Update(float _DeltaTime)
{
	if (CurState != NextState)  // state�� ����Ǵ� ���
	{
		if (FSMFunc.end() == FSMFunc.find(NextState))
		{
			MsgAssert("State�� �ش��ϴ� func�� �������� �ʾҽ��ϴ�");
		}

		if (nullptr != FSMFunc[CurState].End) // �� FSM�� End Function
		{
			FSMFunc[CurState].End();
		}

		if (nullptr != ChangeFSMCallBack) // FSM�� ����� �� �ϰ������� �����ϴ� ��
		{
			ChangeFSMCallBack();
		}

		if (nullptr != FSMFunc[NextState].Start) // �ٲ� FSM�� Start Function
		{
			FSMFunc[NextState].Start();
		}
		CurState = NextState;
	}
	if (nullptr != FSMFunc[CurState].Update) // FSM Update
	{
		FSMFunc[CurState].Update(_DeltaTime);
	}
}


