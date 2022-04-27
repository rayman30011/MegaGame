// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows
#include <windows.h>
#include <windowsx.h>

// Файлы заголовков среды выполнения C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <list>
#include <string>
#include <map>

#define DXUT_AUTOLIB

#include <DXUT.h>
#include <SDKmisc.h>
#include <tinyxml2.h>

#include "Utilities/Strings.h"

using std::shared_ptr;
using std::weak_ptr;
using std::string;

namespace xml = tinyxml2;

#ifndef _NEW
#ifdef _DEBUG
#if defined(_MSC_VER) || defined (WIN32)
#include <crtdbg.h>
#endif
#define _NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#define _NEW new
#endif
#endif


#ifndef _ERROR
#ifdef _DEBUG
#define _ERROR(message) MessageBox(NULL, message, TEXT("Error"), MB_OK)
#else
#define _ERROR(message) MessageBox(NULL, message, TEXT("Error"), MB_OK)
#endif
#endif

#ifndef E_ASSERT
#define E_ASSERT(x) assert(x)
#endif


#define KILOBYTE 1024
#define MEGABYTE KILOBYTE * KILOBYTE

#include "../Core/EngineApp.h"
#include "../Utilities/Math.h"

typedef DirectX::Math::Vector3 Vector3;
typedef DirectX::Math::Vector2 Vector2;
typedef DirectX::Math::Color Color;

extern INT WINAPI MegaEngineMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int cmdShow);

