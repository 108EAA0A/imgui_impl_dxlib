#include <tchar.h>
#include <windows.h>
#include "../imgui_impl_dxlib.hpp"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    // suppress warning
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    // フルスクリーン抑止
    DxLib::ChangeWindowMode(TRUE);

    if (DxLib::DxLib_Init() == -1) return -1;

    DxLib::SetDrawScreen(DX_SCREEN_BACK);

    // ユーザーの入力をImGuiも扱えるようにする
    DxLib::SetHookWinProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT /*CALLBACK*/
    {
        // DxLibとImGuiのウィンドウプロシージャを両立させる
        DxLib::SetUseHookWinProcReturnValue(FALSE);
        return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    });

    // imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplDXlib_Init();

    while (!DxLib::ClearDrawScreen() && !DxLib::ProcessMessage() && !DxLib::CheckHitKey(KEY_INPUT_ESCAPE))
    {
        ImGui_ImplDXlib_NewFrame();
        ImGui::NewFrame();

        static int x = 100;
        x += 10;
        x %= 1000;
        DxLib::DrawBox(x, x, x + 100, x + 100, DxLib::GetColor(255, 255, 255), TRUE);
        DxLib::DrawRoundRect(500, 100, 600, 300, 20, 20, DxLib::GetColor(255, 255, 255), TRUE);

        // gui
        // http://www.sanko-shoko.net/note.php?id=qlv3
        {
            ImGui::Begin("Test Window");

            ImGui::Text("Hello, world %d", 123);

            if (ImGui::Button("OK")) {
                DxLib::printfDx(_T("Button\n"));
            }

            static char buf[256] = "aaa";
            if (ImGui::InputText("string", buf, 256)) {
                DxLib::printfDx(_T("InputText\n"));
            }

            static float f = 0.0f;
            if (ImGui::SliderFloat("float", &f, 0.0f, 1.0f)) {
                DxLib::printfDx(_T("SliderFloat\n"));
            }

            ImGui::End();
        }

        // 先にDxLibの描画を終わらせる
        DxLib::RenderVertex();

        // ImGui関連の描画処理
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDXlib_RenderDrawData();

        // 最後にScreenFlip
        DxLib::ScreenFlip();
    }

    ImGui_ImplDXlib_Shutdown();
    ImGui::DestroyContext();

    DxLib::DxLib_End();

    return 0;
}
