// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define _SILENCE_CXX20_CISO646_REMOVED_WARNING  //어디서 나는 오류인지 도저히 못찾겠어서 여기다 넣었다 physX로 추정...

// 여기에 미리 컴파일하려는 헤더 추가
#include <windows.h>
#include "framework.h"

// UTIL
#include <cassert>

// STL
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <type_traits>
#include <typeindex>
#include <memory>
#include <any>
#include <filesystem>
#include <fstream>

// Header
#include "TypeIndex.h"
#include "StringUtils.h"
#include "FileUtils.h"


// MathModule
#include "MathHeaders.h"
#include "GraphicsTransfer.h"

// Reflection 
#include "ReflectionHeader.h"

// Json 헤더 파일 전방 선언 (fwd = foward)
// 실제 Json 헤더파일을 담고있는 json.hpp 는 
// json을 사용하는 cpp에서 include 해주자.
#include "json_fwd.hpp"
#include "json.hpp"

#endif //PCH_H
