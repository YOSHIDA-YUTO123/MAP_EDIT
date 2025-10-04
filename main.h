//===================================================
//
// メイン [main.h]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// 二重インクルード防止
//***************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// メインクラスの定義
//***************************************************
class CMain
{
public:
	CMain();
	~CMain();
	void ToggleFullscreen(HWND hWnd); // フルスクリーン

	/// <summary>
	/// ウィンドウにXFileをドロップしたときの処理
	/// </summary>
	/// <param name="hWnd"></param>
	/// <param name="wParam"></param>
	void DropXFile(HWND hWnd, WPARAM wParam);

	/// <summary>
	/// ドロップされたファイルのパスの\\を/に変換する処理
	/// </summary>
	/// <param name="path"></param>
	void NormalizePathSlash(TCHAR* path);
private:
	RECT m_windowRect;				  // ウィンドウのサイズ
	bool m_bFullScreen;				  // フルスクリーンかどうか
};
#endif
