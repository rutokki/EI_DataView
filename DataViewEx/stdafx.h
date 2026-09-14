// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

//#include <vld.h>

#include <SDKDDKVer.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>


#include <afxdisp.h>        // MFC Automation classes

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib") // GDI+ lib Link

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <span>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include<fstream>	

#include "BitUtility.h"
#include "GridColumnDefine.h"
#include "FontSetting.h"
#include "Logic_Typedef.h"
#include "EI_IP_DBStruct_Typedef.h"
#include "EI_IP_IOCard_Typedef.h"
//#include "TSC_DBStruct_Typedef.h"
//#include "TSC_IOCard_Typedef.h"
#include <EI_define.h>
//#include <Logic_define.h>
//#include <Logic_Typedef.h>
//

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <BCGCBProInc.h>			// BCGControlBar Pro


#define WM_CONTAINER_CMD (WM_USER + 8888)







#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
