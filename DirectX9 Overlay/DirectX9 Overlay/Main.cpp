#include "Header.h"
HWND hwnd;

// 전체화면 해상도
int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
MARGINS margins = { 0, 0, ScreenWidth, ScreenHeight };

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	const MARGINS margin = { 0, 0, Render.RenderWidth, Render.RenderHeight };

	switch (message) {

	case WM_PAINT:
		DwmExtendFrameIntoClientArea(hWnd, &margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	/* 필요없음
	// Define Window Name
	auto window_name = "notepad";

	// Find Window
	auto WindowHWND = FindWindow(NULL, window_name);

	RECT wndRc;

	if (WindowHWND) {
		GetWindowRect(WindowHWND, &wndRc);
	}
	else {
		// Failed to Find Target.
		ExitProcess(0);
	}
	*/

	Render.RenderWidth = ScreenWidth;
	Render.RenderHeight = ScreenHeight;

	auto centerX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (Render.RenderWidth / 2);
	auto centerY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (Render.RenderHeight / 2);

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(0, "WindowClass", "", WS_EX_TOPMOST | WS_POPUP, 0, 0, Render.RenderWidth, Render.RenderHeight, NULL, NULL, hInstance, NULL);

	SetWindowLong(hwnd, GWL_EXSTYLE, (int)GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

	/* 레거시 transparent
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);
	*/
	DwmExtendFrameIntoClientArea(hwnd, &margins);


	ShowWindow(hwnd, nCmdShow);

	Render.initD3D(hwnd);

	MSG msg;

	SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	while (true)
	{
#if 1

		Render.RenderWidth = ScreenWidth;
		Render.RenderHeight = ScreenHeight;
#endif
		MoveWindow(hwnd, 0, 0, Render.RenderWidth, Render.RenderHeight, true);

		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, Render.RenderWidth, Render.RenderHeight, SWP_NOMOVE | SWP_NOSIZE);

		Render.render();


		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			exit(0);
		}
	}

	Render.cleanD3D();

	return msg.wParam;
}