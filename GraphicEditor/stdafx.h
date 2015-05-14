
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

//2012.05.08 SHC 추가
#define _USE_MATH_DEFINES //math.h의 M_PI 를 사용하기 위함
#include <math.h> //삼각함수를 사용하기 위함

//undo 기능 사용을 위해 추가
#include "undo.h"


//GDI+ 사용을 위해 추가 120528 by 신희철
#include <GdiPlus.h>
#include <afxdlgs.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

// 유저 메시지 추가 by 신희철
#define UM_CHANGEALPHA			WM_USER+10
#define UM_CHANGELINEJOIN		WM_USER+11
#define UM_CHANGELINEWEIGHT		WM_USER+12
#define UM_CHANGELINECAPSTART	WM_USER+13
#define UM_CHANGELINECAPEND		WM_USER+14
#define UM_CHANGEFONT			WM_USER+15
#define UM_CHANGELINECOLOR		WM_USER+16
#define UM_CHANGEFILLCOLOR		WM_USER+17
#define UM_CHANGEGRDTCOLOR		WM_USER+18

// MS에서 인정한 XP 테마 버그의 수정을 위해 추가
#pragma comment(lib, "UxTheme.lib")