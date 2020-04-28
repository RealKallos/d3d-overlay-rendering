#include "Header.h"

cRender Render;

void cRender::Text(char *text, int x, int y, int orientation, DWORD color, ID3DXFont* pFont)
{
	RECT rect = { x, y, x + 120, y + 16 };

	switch (orientation)
	{
	case lefted:
		pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
		break;
	case centered:
		pFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
		break;
	case righted:
		pFont->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
		break;
	}
}


void cRender::initFonts() {

	IDirect3DDevice9* pDevice;
	pDevice = d3ddev;

	D3DXCreateFont(pDevice, 20, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Tahoma", &Tahoma);
}

void cRender::initD3D(HWND hwnd)
{
	// create the Direct3D interface
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	// create a struct to hold various device information
	D3DPRESENT_PARAMETERS d3dpp;    
	// clear out the struct for use
	ZeroMemory(&d3dpp, sizeof(d3dpp));   
	// program windowed, not fullscreen
	d3dpp.Windowed = TRUE;    
	// discard old frames
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// set the window to be used by Direct3D
	d3dpp.hDeviceWindow = hwnd;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	// Initialise all fonts to be used.
	initFonts();
}


void cRender::draw_outlined_rect(float x, float y, float w, float h, unsigned long color)
{
	draw_rect(x - 1, y - 1, w + 2, h + 2, D3DCOLOR_RGBA(1, 1, 1, 255));
	draw_rect(x + 1, y + 1, w - 2, h - 2, D3DCOLOR_RGBA(1, 1, 1, 255));
	draw_rect(x, y, w, h, color);
}

void cRender::draw_line(float x0, float y0, float x1, float y1, unsigned long color)
{
	/*D3DTLVERTEX qV[ 2 ] = {
		{ float(x0), float(y0), 0.f, 1.f, color },
		{ float(x1), float(y1), 0.f, 1.f, color },
	};

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_LINELIST, 2, qV, sizeof(D3DTLVERTEX));*/

	D3DXVECTOR2 lines[2] = {
		D3DXVECTOR2(x0, y0),
		D3DXVECTOR2(x1, y1)
	};

	d3Line->Begin();
	d3Line->Draw(lines, 2, color);
	d3Line->End();
}

void cRender::draw_rect(float x, float y, float w, float h, unsigned long color)
{
	draw_line(x, y, x + w, y, color);
	draw_line(x, y, x, y + h, color);
	draw_line(x + w, y, x + w, y + h, color);
	draw_line(x, y + h, x + w + 1, y + h, color);
}

void cRender::draw_filled_rect(float x, float y, float w, float h, unsigned long color)
{
	D3DTLVERTEX qV[4] = {
		{ x, y + h, 0.f, 1.f, color },
		{ x, y, 0.f, 1.f, color },
		{ x + w, y + h, 0.f, 1.f, color },
		{ x + w, y , 0.f, 1.f, color }
	};

	d3dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	d3dev->SetTexture(0, nullptr);
	d3dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV, sizeof(D3DTLVERTEX));
}

void cRender::DrawHealthbars(float PosX, float PosY, float height, float Value1, unsigned long color)
{
	int HealthR = 0, HealthG = 0, HealthB = 0; // Lets make some integers so we can use them for the healthbar. What we will be using this for is to change the color of the healthbar according to the damage done to the enemy.

	float Value2 = static_cast<float>(Value1) / 100.f * height;
	int Value = static_cast<int>(Value2);

	if (Value1 > 0 && Value1 < 101) // if Value is greater then 75 and not greater then 90 draw the health has a darker shade of green.
	{
		draw_rect(PosX - 1, PosY - 1, 5, height, D3DCOLOR_RGBA(0, 0, 0, 255));
		draw_filled_rect(PosX, PosY, 3, Value - 1, color);
	}
}

void cRender::DrawCircle(int X, int Y, int radius, int numSides, DWORD color) {
	D3DXVECTOR2 Line[128];
	float Step = M_PI * 2.0 / numSides;
	int Count = 0;
	for (float a = 0; a < M_PI * 2.0; a += Step)
	{
		float X1 = radius * cos(a) + X;
		float Y1 = radius * sin(a) + Y;
		float X2 = radius * cos(a + Step) + X;
		float Y2 = radius * sin(a + Step) + Y;
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count + 1].x = X2;
		Line[Count + 1].y = Y2;
		Count += 2;
	}
	d3Line->Draw(Line, Count, color);
}

void cRender::FillArea(float x, float y, float width, float height, D3DCOLOR color) {
	D3DXVECTOR2 vectorLine[2];
	d3Line->SetWidth(width);

	vectorLine[0].x = x + width / 2;
	vectorLine[0].y = y;
	vectorLine[1].x = x + width / 2;
	vectorLine[1].y = y + height;

	d3Line->Draw(vectorLine, 2, color);
}

void cRender::FillRectangle(double x, double y, double w, double h, D3DCOLOR color)
{
	struct Vertex
	{
		float x, y, z, h;
		D3DCOLOR color;
	};

	Vertex vertices[4] =
	{
		x,  y, 0.0f, 1.0f, color,
		x + w, y, 0.0f, 1.0f, color,

		x + w, y + h, 0.0f, 1.0f, color,
		x,  y + h, 0.0f, 1.0f, color,
	};

	d3dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	d3dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(Vertex));
}

void cRender::DrawBox(float x, float y, float width, float height, D3DCOLOR color) {
	D3DXVECTOR2 points[5];
	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + width, y);
	points[2] = D3DXVECTOR2(x + width, y + height);
	points[3] = D3DXVECTOR2(x, y + height);
	points[4] = D3DXVECTOR2(x, y);
	d3Line->SetWidth(1);
	d3Line->Draw(points, 5, color);
}

void cRender::draw_text(std::string_view text, float x, float y, unsigned long color, bool center, bool outline, LPD3DXFONT font) {
	if (center) {
		RECT dimensions = get_text_dimensions(text, font);
		x -= (dimensions.right - dimensions.left) / 2;
	}

	auto _text = [&](std::string_view _text, int _x, int _y, unsigned long _color) {
		RECT r{ _x, _y, _x, _y };
		font->DrawTextA(NULL, _text.data(), -1, &r, DT_NOCLIP, _color);
	};

	if (outline) {
		_text(text, x - 1, y, D3DCOLOR_RGBA(1, 1, 1, 255));
		_text(text, x + 1, y, D3DCOLOR_RGBA(1, 1, 1, 255));
		_text(text, x, y - 1, D3DCOLOR_RGBA(1, 1, 1, 255));
		_text(text, x, y + 1, D3DCOLOR_RGBA(1, 1, 1, 255));
	}

	_text(text, x, y, color);
}

RECT  cRender::get_text_dimensions(std::string_view text, LPD3DXFONT font) {
	RECT r;
	font->DrawTextA(NULL, text.data(), -1, &r, DT_CALCRECT, 0xFFFFFFFF);
	return r;
}


void cRender::render() {
	d3ddev->BeginScene();    // begins the 3D scene
	// clear the window alpha
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	IDirect3DDevice9* pDevice;
	pDevice = d3ddev;

	D3DVIEWPORT9 screen;

	// Get the screen
	pDevice->GetViewport(&screen);

	if (Tahoma) {
		Tahoma->OnLostDevice();
	}

	// Call Rendering here.
	Text((char*)"kitto d3d overlay", 10, + 50, lefted, D3DCOLOR_RGBA(255, 0, 0, 255), Tahoma);


	d3ddev->EndScene();    // ends the 3D scene
	d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}

void cRender::cleanD3D()
{
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
}
