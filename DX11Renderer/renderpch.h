#pragma once
#ifndef RENDERERPCH_H
#define RENDERERPCH_H

//Windows API ȣ�� �� ������ ���� �˻簡 Ȱ��ȭ�ǰ�, 
//�ڵ忡�� ���� ���� ������ �̸� ������ �� �ְ� �˴ϴ�.
//#define STRICT  

// Windows ��� ���Ͽ��� ���ǵǾ� �ִ� min �� max ��ũ�θ� ��Ȱ��ȭ.
#define NOMINMAX 

// �پ��� ������ �������� Ȱ��� �� �ֵ��� ��
//#define _WIN32_WINNT 0x0601
//#define WINVER 0x0605
///----------------------------------------------------------------------------
///                         winapi & 3D3, 2D2 ��� ����
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

#include <wrl/client.h> //COM ����Ʈ������ ���
using namespace Microsoft::WRL;

#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

#include <dwrite.h> //�ؽ�Ʈ ���
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
#include <memory>  //����Ʈ������
#include <utility> // std::pair

using std::wstring;
using std::vector;
using std::shared_ptr;
using std::make_shared;

///----------------------------------------------------------------------------
///                                 ���������
///----------------------------------------------------------------------------

#include "MathHeaders.h"
#include "GraphicsTransfer.h"

#endif //PCH_H


///----------------------------------------------------------------------------
// Windows API���� ������� ���� ��ɵ��� ��Ȱ��ȭ
// ���α׷� ���� �ÿ� ���ʿ��� ���ҽ� ����� ���̰� ���� �ð��� ���� ����.
///----------------------------------------------------------------------------
#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN 
//GDI�� Campmasks���� ���
#define NOGDICAPMASKS 
// �ý����� ���÷��� �� ��Ʈ ������ ���õ� ������ �����ϴ°�
#define NOSYSMETRICS
// ������ �޴��� ���õ� ���
#define NOMENUS
// �������� �ٷ�� �Ϻ� ���
#define NOICONS
// �ý��۸�ɰ� ���õ� ��� (â�� �ൿ ����)
#define NOSYSCOMMANDS
// ������ �۾�(Raster Operations) ���� (��Ʈ���� �ٷ� ��)
#define NORASTEROPS
// Ư�� �ϵ���� �Ǵ� ������ü�� ���õ� ���ҽ� 
#define OEMRESOURCE
// ���ڿ��� ������ ������ ������ �ڿ�
#define NOATOM
// ������ ���� �� �ٿ��ֱ� Ŭ������
#define NOCLIPBOARD
// ���� ���� ���
#define NOCOLOR
//��Ʈ�� ������(GUI ��Ʈ�� ~ ��ư, �ؽ�Ʈ ����)
#define NOCTLMGR
// �ؽ��� �׸���
#define NODRAWTEXT
// Ŀ�θ�� �Լ��� ���õ� ���(�ý��� ���ҽ��� ����)
#define NOKERNEL
// �޸� ������
#define NOMEMMGR
// ��Ÿ����(�׷��� �׸��� ���õ� Ư�� ���� ����)
#define NOMETAFILE
// ���� ���� ���� ���)
#define NOOPENFILE
// ��ũ�� �� ����
#define NOSCROLL
// ���� (��׶��� �۾�)
#define NOSERVICE
// ���� ���
#define NOSOUND
// �ؽ�Ʈ ��� ���� ����
#define NOTEXTMETRIC
// ������ �ڵ�
#define NOWH
// �ø��� ��� 
#define NOCOMM
// �Ϻ��� �Է� �� ����
#define NOKANJI
// ���� �ý���
#define NOHELP
// �������Ϸ�(�ڵ� ���� �м�)
#define NOPROFILER
// ������ ������ ����
#define NODEFERWINDOWPOS
// Window Media Center
#define NOMCX
// ���� ���ν��� ȣ�� (�л� �ý��� �� ���� ���� ȣ��)
#define NORPC
// ���Ͻ� ����(COM�� ���õ� ���)
#define NOPROXYSTUB
// �̹��� ���� ���(�̹��� ó�� �� ���÷���)
#define NOIMAGE
// ������ ����̺� ���� ��� 
#define NOTAPE
#endif



