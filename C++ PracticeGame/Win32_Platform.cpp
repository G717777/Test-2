#include "Utilities.cpp"
#include <windows.h>
#include <stdio.h>

globalVar bool running = true;

struct RenderState 
{
	void* bufferMemory;
	int bufferWidth, bufferHeight;
	BITMAPINFO bitmapinfo;


};

globalVar RenderState renderState;
#include "PlatformCommon.cpp"
#include "Renderer.cpp"
#include "Game.cpp"


//char str[150];
//void* bufferMemory;
//int bufferWidth, bufferHeight;
//BITMAPINFO bitmapinfo;
void UpdateGraph(HWND hwnd, HDC dc)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	DrawText(dc, L"Hello!", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	
}
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	LRESULT result = 0;
	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		}
		break;
		case WM_SIZE: //Changes in window size
		{
		//get the new size
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderState.bufferWidth = rect.right - rect.left;
			renderState.bufferHeight = rect.bottom - rect.top;

			//buffer contains w * h pixels
			// pixel is a 32 bit integer - r(8), g(8), b(8) and 8 bits of padding.
			int bufferSize = renderState.bufferWidth * renderState.bufferHeight * sizeof(unsigned int);

			if (renderState.bufferMemory) //if memory is already occupied
			{
				VirtualFree(renderState.bufferMemory, 0, MEM_RELEASE); //clear it
			}
			renderState.bufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT| MEM_RESERVE, PAGE_READWRITE);

			renderState.bitmapinfo.bmiHeader.biSize = sizeof(renderState.bitmapinfo.bmiHeader);
			renderState.bitmapinfo.bmiHeader.biWidth = renderState.bufferWidth;
			renderState.bitmapinfo.bmiHeader.biHeight = renderState.bufferHeight;
			renderState.bitmapinfo.bmiHeader.biPlanes = 1;
			renderState.bitmapinfo.bmiHeader.biBitCount = 32;
			renderState.bitmapinfo.bmiHeader.biCompression = BI_RGB;
			
		}
		break;
		//case WM_PAINT:
		//{
		//	RECT rToPaint;
		//	if (!GetUpdateRect(hwnd, &rToPaint, FALSE))
		//		break;    //No regions to update, leave procedure
		//	PAINTSTRUCT ps;
		//	HDC handle = BeginPaint(hwnd, &ps);
		//	
		//	LPCWSTR text = L"Defenestration can be hazardous";
		//	TextOut(handle, rToPaint.left, rToPaint.top, text, 32);

		//	// Bracket end a path 
		//	//EndPath(hdc);
		//	//Do graphical paint
		//	//UpdateGraph(hwnd, ps.hdc);
		//	//sprintf(str, L"Hello");
		//	//LPCWSTR h = L"Thegw";
		//	//TextOutW(handle, 10, 10, );
		//	EndPaint(hwnd, &ps);
		//}
		//	break;        //End of procedure

		default:
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	//window class
	WNDCLASS WindowClass = {};
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpszClassName = L"Game Window Class";
	WindowClass.lpfnWndProc = window_callback;

	// Register class
	RegisterClass(&WindowClass);

	//Create window
	HWND window = CreateWindow(WindowClass.lpszClassName, L"Game Yay", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 540, 360, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window); // handle to divice context - reference that windows use in order to draw to it.
	//HBRUSH hbr = ::CreateSolidBrush(RGB(240, 230, 240)); WindowClass.hbrBackground = hbr;

	Input input = {};

	float deltaTime = 0.016666f; // 60 frames per sec (1/60)
	LARGE_INTEGER frameBeginTime;
	QueryPerformanceCounter(&frameBeginTime);

	float Frequency; 
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		Frequency = (float)perf.QuadPart;
	}

	while (running) 
	{
		//input
		MSG message;

		for (int i = 0; i < Button_Count; i++) // reset change at the beginning of every frame - change only lasts one frame
		{
			input.buttons[i].changed = false;
		}
		
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{

			case WM_KEYUP:
			case WM_KEYDOWN: 
			{
				u32 vkCode = (u32)message.wParam; // what key has been pressed
				bool isDown = ((message.lParam & (1 << 31)) == 0); // key up or key down event

//if the current is down is diff from the recorded is down
#define processButton(b, vk)\
 case vk: {\
 input.buttons[b].changed = isDown != input.buttons[b].isDown;\
 input.buttons[b].isDown = isDown;\
} break;


				switch (vkCode)
				{
					processButton(Button_Up, VK_UP);
					processButton(Button_Down, VK_DOWN);
					processButton(Button_W, 'W');
					processButton(Button_S, 'S');
				

				default:
					break;

				}
				break;
			}

			default:
				TranslateMessage(&message);
				DispatchMessage(&message);
				break;
			}
			
		}
		//simulate

		SimulateGame(&input, deltaTime, 500.f);
		
		//render
		StretchDIBits(hdc, 0,0, renderState.bufferWidth, renderState.bufferHeight, 0, 0, renderState.bufferWidth, renderState.bufferHeight, renderState.bufferMemory, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	
		//bitmapinfo - holds information - what does a pixel look like, compression look like - so window know how to copy info to video card

		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		// difference between beginning of frame and end;
		// divide difference by frequency to get delta time in secs
		deltaTime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / Frequency; //quartPart is in cpu time
		
		frameBeginTime = frameEndTime; // start the new frame
	}
}
