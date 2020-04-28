#pragma once

class cRender {
public:

	void draw_outlined_rect(float x, float y, float w, float h, unsigned long color);

	void draw_line(float x0, float y0, float x1, float y1, unsigned long color);

	void draw_rect(float x, float y, float w, float h, unsigned long color);

	void draw_filled_rect(float x, float y, float w, float h, unsigned long color);

	void DrawCircle(int X, int Y, int radius, int numSides, DWORD color);

	void FillArea(float x, float y, float width, float height, D3DCOLOR color);

	void FillRectangle(double x, double y, double w, double h, D3DCOLOR color);

	void DrawBox(float x, float y, float width, float height, D3DCOLOR color);

	void draw_text(std::string_view text, float x, float y, unsigned long color, bool center, bool outline, LPD3DXFONT font);

	RECT get_text_dimensions(std::string_view text, LPD3DXFONT font);

	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);

	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	LPDIRECT3D9 d3;
	LPDIRECT3DDEVICE9 d3dev;

	LPD3DXFONT d3Font;

	HWND hWindow;
	HWND tWindow;

	MARGINS margins = { 0, 0, ScreenWidth, ScreenHeight };

	ID3DXLine* d3Line;

#pragma region Window Properties
	uintptr_t RenderWidth;
	uintptr_t RenderHeight;
#pragma endregion

#pragma region DXD


	LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

	void initD3D(HWND hwnd);
	void render();
	void cleanD3D();



#pragma region fonts
	LPD3DXFONT Tahoma;
	void initFonts();
#pragma endregion

#pragma region drawing

	enum text_alignment
	{
		lefted,
		centered,
		righted
	};

	void Text(char *text, int x, int y, int orientation, DWORD color, ID3DXFont* pFont);


#pragma endregion 

#pragma endregion
};

extern cRender Render;