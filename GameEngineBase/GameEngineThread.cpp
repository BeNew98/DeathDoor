#include "PrecompileHeader.h"
#include "GameEngineThread.h"
#include "GameEngineString.h"

std::atomic_bool GameEngineThread::End = false;

GameEngineThread::GameEngineThread() 
{
}

GameEngineThread::GameEngineThread(std::string _ThreadName, std::function<void()> _CallBack)
{
	Start(_ThreadName, _CallBack);
}

GameEngineThread::~GameEngineThread() 
{
	Join();
}

void GameEngineThread::Start(std::string _ThreadName, std::function<void()> _CallBack)
{
	CallBack = _CallBack;
	Thread = std::thread(ThreadBaseFunction, this, _ThreadName);

}

void GameEngineThread::ThreadBaseFunction(GameEngineThread* Thread, std::string Name) 
{
	// 쓰레드버그가 최악인게
	// 절대 일정하지 않다.
	// 애초에 그게 병렬처리.

	std::wstring WName = GameEngineString::AnsiToUniCode(Name);
	// GetCurrentThreadId() 우리가 쓰레드 요청하면 윈도우는 우리 쓰레드에 id를 부여하는데
	SetThreadDescription(GetCurrentThread(), WName.c_str());
	Thread->CallBack();
}

void GameEngineThread::Join()
{
	// 정말정말 신나게 100개가 돌아가고 있어.
	// CPu가 100%치고 있고 다른 로직들이 끼어들어갈 여지가 없다면
	// 내가 강제로 다른 로직 하나를 Sleep하거나 정지시켜서 급한애를 먼저처리하게 만들어줘야 
	// 컨텍스트 스위칭이 악이 아니야.

	if (nullptr != CallBack)
	{
		// Sleep(0);
		Thread.join();
		CallBack = nullptr;
	}
}