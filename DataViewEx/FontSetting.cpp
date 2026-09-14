#include "stdafx.h"
#include "FontSetting.h"

CFont FontSetting::FontDungGeunMo;
CFont FontSetting::FontDungGeunMoStatic;
CFont FontSetting::FontDungGeunMoCombo;
CFont FontSetting::FontDungGeunMoSmall;
CFont FontSetting::FontDungGeunMoSmall2;

void FontSetting::Initialize()
{
	CString forward = _T("둥근모꼴");
	//CString forward = _T("맑은고딕");
	// 이미 폰트가 생성되어 있다면 중복 생성 방지
	if (FontDungGeunMo.GetSafeHandle()) return;
	if (FontDungGeunMoCombo.GetSafeHandle()) return;
	if (FontDungGeunMoStatic.GetSafeHandle()) return;
	if (FontDungGeunMoSmall.GetSafeHandle()) return;
	if (FontDungGeunMoSmall2.GetSafeHandle()) return;
	// 1. 일반 텍스트용 (18)

	FontDungGeunMo.CreateFont(-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		forward);
	// 6. 둥근모꼴 중간 텍스트 (20
	FontDungGeunMoStatic.CreateFont(-18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		forward);
	// 7. 둥근모꼴 굵은 텍스트 (22)
	FontDungGeunMoCombo.CreateFont(-22, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, forward);
	FontDungGeunMoSmall.CreateFont(-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, forward);
	FontDungGeunMoSmall2.CreateFont(-11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, forward);
}

void FontSetting::ReleaseFonts()
{
	if (FontDungGeunMo.GetSafeHandle())  FontDungGeunMo.DeleteObject();
	if (FontDungGeunMoStatic.GetSafeHandle())  FontDungGeunMoStatic.DeleteObject();
	if (FontDungGeunMoCombo.GetSafeHandle()) FontDungGeunMoCombo.DeleteObject();
	if (FontDungGeunMoSmall.GetSafeHandle()) FontDungGeunMoSmall.DeleteObject();
	if (FontDungGeunMoSmall2.GetSafeHandle()) FontDungGeunMoSmall2.DeleteObject();
}
class AppFontDestructor {
public:
	~AppFontDestructor() {
		FontSetting::ReleaseFonts(); // 프로그램 종료 시 자동 실행
	}
};

// 실제 객체를 생성 (이 녀석이 감시자 역할을 합니다)
static AppFontDestructor _destructor;