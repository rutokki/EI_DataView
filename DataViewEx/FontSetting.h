#pragma once
class FontSetting
{
public:
	// 프로그램 종료 시 폰트 해제를 위해 static으로 선언
	static CFont FontDungGeunMo; // 둥근모 꼴 (14)
	static CFont FontDungGeunMoCombo;// 둥근모 꼴 (22)
	static CFont FontDungGeunMoStatic;
	static CFont FontDungGeunMoSmall; // 11
	static CFont FontDungGeunMoSmall2;// 12

	static void Initialize();
	static void ReleaseFonts();
};



