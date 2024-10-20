#pragma once
#ifndef RENDERERPCH_H
#define RENDERERPCH_H

//Windows API 호출 시 엄격한 유형 검사가 활성화되고, 
//코드에서 유형 관련 문제를 미리 감지할 수 있게 됩니다.
//#define STRICT  

// Windows 헤더 파일에서 정의되어 있는 min 및 max 매크로를 비활성화.
#define NOMINMAX 

// 다양한 윈도우 버전에서 활용될 수 있도록 함
//#define _WIN32_WINNT 0x0601
//#define WINVER 0x0605
///----------------------------------------------------------------------------
///                         winapi & 3D3, 2D2 헤더 파일
///----------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <windowsx.h>
#include <sdkddkver.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3dcommon.h>
//#include <D3DX11async.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "D3DCompiler.lib")
#pragma comment (lib, "User32.lib")

#include <wrl/client.h> //COM 스마트포인터 사용
using namespace Microsoft::WRL;

#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

#include <dwrite.h> //텍스트 출력
#pragma comment(lib, "dwrite")

#pragma comment(lib, "User32.lib")

///DirectXTK
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "WICTextureLoader.h"
#include "DirectXTex.h"

///----------------------------------------------------------------------------
///                                 S T L
///----------------------------------------------------------------------------
#include <iterator>
#include <exception>
#include <stdexcept> 
#include <locale>
#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include <vector>	
#include <memory>  //스마트포인터
#include <utility> // std::pair

using std::wstring;
using std::vector;
using std::shared_ptr;
using std::make_shared;

///----------------------------------------------------------------------------
///                                 직접만든거
///----------------------------------------------------------------------------

#include "MathHeaders.h"
#include "GraphicsTransfer.h"

#endif //PCH_H


///----------------------------------------------------------------------------
// Windows API에서 사용하지 않을 기능들을 비활성화
// 프로그램 빌드 시에 불필요한 리소스 사용을 줄이고 빌드 시간을 단축 가능.
///----------------------------------------------------------------------------
#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN 
//GDI의 Campmasks관련 기능
#define NOGDICAPMASKS 
// 시스템의 디스플레이 및 폰트 설정과 관련된 정보를 제공하는것
#define NOSYSMETRICS
// 윈도우 메뉴와 관련된 기능
#define NOMENUS
// 아이콘을 다루는 일부 기능
#define NOICONS
// 시스템명령과 관련된 기능 (창의 행동 제어)
#define NOSYSCOMMANDS
// 래스터 작업(Raster Operations) 관련 (비트맵을 다룰 때)
#define NORASTEROPS
// 특정 하드웨어 또는 제조업체와 관련된 리소스 
#define OEMRESOURCE
// 문자열과 숫자의 매핑의 윈도우 자원
#define NOATOM
// 데이터 복사 및 붙여넣기 클립보드
#define NOCLIPBOARD
// 색상 관련 기능
#define NOCOLOR
//컨트롤 관리자(GUI 컨트롤 ~ 버튼, 텍스트 상자)
#define NOCTLMGR
// 텍스쳐 그리기
#define NODRAWTEXT
// 커널모드 함수와 관련된 기능(시스템 리소스에 접근)
#define NOKERNEL
// 메모리 관리자
#define NOMEMMGR
// 메타파일(그래픽 그림과 관련된 특수 파일 형식)
#define NOMETAFILE
// 파일 열기 관련 기능)
#define NOOPENFILE
// 스크롤 바 관련
#define NOSCROLL
// 서비스 (백그라운드 작업)
#define NOSERVICE
// 사운드 기능
#define NOSOUND
// 텍스트 출력 관련 정보
#define NOTEXTMETRIC
// 윈도우 핸들
#define NOWH
// 시리얼 통신 
#define NOCOMM
// 일본어 입력 및 한자
#define NOKANJI
// 도움말 시스템
#define NOHELP
// 프로파일러(코드 실행 분석)
#define NOPROFILER
// 윈도우 포지션 지연
#define NODEFERWINDOWPOS
// Window Media Center
#define NOMCX
// 원격 프로시저 호출 (분산 시스템 및 원격 서버 호출)
#define NORPC
// 프록시 스텁(COM과 관련된 기술)
#define NOPROXYSTUB
// 이미지 관련 기능(이미지 처리 및 디스플레이)
#define NOIMAGE
// 테이프 드라이브 관련 기능 
#define NOTAPE
#endif



