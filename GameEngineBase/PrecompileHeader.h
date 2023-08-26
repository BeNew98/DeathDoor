#pragma once

// Platform

#include <WS2tcpip.h> // inet_pton �Լ��� ����ϱ� ���� ���
#include <WinSock2.h> // window.h���� ������ ���ʿ� �־�� �Ѵ�.
#include <Windows.h>

#include <memory>
#include <iostream>
#include <math.h>
#include <cmath>

// std
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <string_view>
#include <chrono>
#include <filesystem>
#include <functional>

#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>


#pragma comment (lib, "ws2_32") // <= ������ ���� ����� ���� ���̺귯��


//PhysX 4.1
#pragma warning(disable : 4996)
#include <GameEngineCore/ThirdParty/PhysX_4_1/inc/PhysX/PxConfig.h>
#include <GameEngineCore/ThirdParty/PhysX_4_1/inc/PhysX/PxPhysicsAPI.h>
#include <GameEngineCore/ThirdParty/PhysX_4_1/inc/PhysX/PxPhysXConfig.h>

#pragma comment (lib, "PhysX_static_64.lib")
#pragma comment (lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment (lib, "PhysXCommon_static_64.lib")
#pragma comment (lib, "PhysXExtensions_static_64.lib")
#pragma comment (lib, "PhysXFoundation_static_64.lib")
#pragma comment (lib, "PhysXPvdSDK_static_64.lib")
#pragma comment (lib, "PhysXCooking_static_64.lib")
