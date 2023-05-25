#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "DxLib.h"
#include "TextFont.h"
#include <string>

#define GAME_WIDTH      10000     // 画面の幅
#define GAME_HEIGHT     200     // 画面の高さ
#define GAME_COLOR      16      // 画面の色
#define GAME_VSYNC      TRUE    // V-Syncを使うからTRUE
#define GAME_ICON       555     // 画面アイコン
#if defined(_DEBUG) || defined(_NDEBUG)
#define GAME_TITLE      u8"UTF‐8ファイルの読み - Read UTF-8 file (DEBUG)"	// 画面タイトル
#else
#define GAME_TITLE      u8"UTF‐8ファイルの読み - Read UTF-8 file"			// 画面タイトル
#endif

struct Enemy {
    char Name[128];
    int Level;
    int MaxHP;
    struct {
        int A;
        float B;
        int BHit;
        float C;
        int CHit;
    } p;
    int Def;
    int Air;
};

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeFont(MS_PGOTHIC_FONT, DX_CHARSET_UTF8);
    // ウインドウモードで起動するか確認する
    if (MessageBoxW(NULL, L"ウインドウモードで起動しますか？\n\nWould you like to launch in window mode?", L"画面モード確認 - Screen mode confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        // ウインドウモードで起動
        ChangeWindowMode(TRUE);
    }

    SetOutApplicationLogValidFlag(FALSE);

    // 使用する文字コードを UTF-8 に変更
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

    // ウインドウ名のセット
    SetMainWindowText(GAME_TITLE);

    SetWaitVSyncFlag(GAME_VSYNC);

    if (DxLib_Init() < 0)		// ＤＸライブラリ初期化処理
    {
        printfDx(u8"Failed to initialize DXライブラリ!\n");
        return -1;			// エラーが起きたら直ちに終了
    }

    // 画面モードを指定
    SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);

#ifdef SMALL_SCREEN
    SetWindowSizeExtendRate(0.5f);
#endif

    //初期設定いろいろ
    SetAlwaysRunFlag(FALSE);
    SetDrawScreen(DX_SCREEN_BACK);

    // Open the file
    FILE* fi = fopen("EnemyList.txt", "r");
    if (!fi) {
        printfDx(u8"Failed to open file!\n");
        printfDx(u8"ファイルのオープンに失敗しました！\n");
        DxLib_End(); // Cleanup DXライブラリ before returning
        return 1;
    }

    char buf[256];
    Enemy Enemy[62];

    // skip the first line of the text file
    for (int j = 0; j < 10; j++) fscanf(fi, u8"%s", buf);

    for (int i = 0; i < 62; i++) {
        fscanf(fi, u8"%s", Enemy[i].Name);
        buf[19] = '\0'; // make sure the last byte is set to null

        fscanf(fi, u8"%s", buf);
        Enemy[i].Level = atoi(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].MaxHP = atoi(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].p.A = atoi(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].p.B = atof(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].p.BHit = atoi(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].p.C = atof(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].p.CHit = atoi(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].Def = atoi(buf);

        fscanf(fi, u8"%s", buf);
        Enemy[i].Air = atoi(buf);
    }

    fclose(fi);


    /**********************/
    /** メインゲームループ **/
    /**********************/
    while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
        // Draw enemy data on the screen
        for (int i = 0; i < 62; i++) {
            int x = 10 + i * 160; // Adjust horizontal position for each enemy
            int y = 1; // Adjust vertical position for each enemy

            DrawString(x, y, Enemy[i].Name, GetColor(255, 45, 0));
            DrawFormatString(x, y + 20, GetColor(247, 255, 0), u8"Level: %d", Enemy[i].Level);
            DrawFormatString(x, y + 40, GetColor(255, 255, 255), u8"MaxHP: %d", Enemy[i].MaxHP);
            DrawFormatString(x, y + 60, GetColor(255, 255, 255), u8"p.A: %d", Enemy[i].p.A);
            DrawFormatString(x, y + 80, GetColor(255, 255, 255), u8"p.B: %.2f", Enemy[i].p.B);
            DrawFormatString(x, y + 100, GetColor(255, 255, 255), u8"p.BHit: %d", Enemy[i].p.BHit);
            DrawFormatString(x, y + 120, GetColor(255, 255, 255), u8"p.C: %.2f", Enemy[i].p.C);
            DrawFormatString(x, y + 140, GetColor(255, 255, 255), u8"p.CHit: %d", Enemy[i].p.CHit);
            DrawFormatString(x, y + 160, GetColor(255, 255, 255), u8"Def: %d", Enemy[i].Def);
            DrawFormatString(x, y + 180, GetColor(255, 255, 255), u8"Air: %d", Enemy[i].Air);
        }

        ScreenFlip(); // Update the screen
    }

	WaitKey();				// キー入力待ち

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}