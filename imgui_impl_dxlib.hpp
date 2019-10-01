#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <DxLib.h>
#include <exception>

// Win32 message handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int GetUseDxLibDirect3DVersion() {
    switch (DxLib::GetUseDirect3DVersion()) {
        case DX_DIRECT3D_9:
        case DX_DIRECT3D_9EX:
            return 9;
        case DX_DIRECT3D_11:
            return 11;
        case DX_DIRECT3D_NONE:
        default:
            //return -1;
            throw std::exception("Can't get DxLib Direct3D Version");
    }
}

bool ImGui_ImplDXlib_Init() {
    ImGui_ImplWin32_Init(DxLib::GetMainWindowHandle());

    switch (GetUseDxLibDirect3DVersion()) {
        case 9: {
            auto d = static_cast<IDirect3DDevice9*>(const_cast<void*>(GetUseDirect3DDevice9()));
            return ImGui_ImplDX9_Init(d);
        }
        case 11: {
            auto d = static_cast<ID3D11Device*>(const_cast<void*>(GetUseDirect3D11Device()));
            auto dc = static_cast<ID3D11DeviceContext*>(const_cast<void*>(GetUseDirect3D11DeviceContext()));
            return ImGui_ImplDX11_Init(d, dc);
        }
    }
    return false;
}

void ImGui_ImplDXlib_NewFrame() {
    switch (GetUseDxLibDirect3DVersion()) {
        case 9:  ImGui_ImplDX9_NewFrame();  break;
        case 11: ImGui_ImplDX11_NewFrame(); break;
    }
    ImGui_ImplWin32_NewFrame();
}

void ImGui_ImplDXlib_RenderDrawData() {
    switch (GetUseDxLibDirect3DVersion()) {
        case 9:  ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());  return;
        case 11: ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); return;
    }
}

void ImGui_ImplDXlib_Shutdown() {
    switch (GetUseDxLibDirect3DVersion()) {
        case 9:  ImGui_ImplDX9_Shutdown();  break;
        case 11: ImGui_ImplDX11_Shutdown(); break;
    }
    ImGui_ImplWin32_Shutdown();
}