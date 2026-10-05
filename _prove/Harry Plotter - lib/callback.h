#include <windows.h>

//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int x;
//	
//	switch (message)
//	{
//		case WM_CREATE:
//			return 0;
//		
//		case WM_PAINT:
//			HDC hDC;
//			PAINTSTRUCT ps;
//			HPEN hOldPen, hNewPen;
//			
//			hDC = BeginPaint(hWnd, &ps);
//			hNewPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));   // Grigio 
//			hOldPen = (HPEN)SelectObject(hDC, hNewPen);
//			
//			//*** Students: try changing the section between this comment line and the one marked below. ***
//			
//			// Print a message:
//			TextOut(hDC, 50, 50, "Graph", 5);
//			
//			// Draw the axes:
//			MoveToEx(hDC, 100, 200, NULL);
//			LineTo(hDC, 500, 200);
//			MoveToEx(hDC, 300, 100, NULL);
//			LineTo(hDC, 300, 300);
//			
//			MoveToEx(hDC,100,200,NULL);
//			// Draw the graph of the function g:
//			for (x = -200; x <= 200; x++)
//			//SetPixel(hDC, 300 + x, 200 + g(x), RGB(0, 0, 255));   // blue color 
//			LineTo(hDC,300+x,200+(x));
//			
//			// Draw the graph of the function f, the one that modulates the amplitude of g:
//			for (x = -200; x <= 200; x++)
//			SetPixel(hDC, 300 + x, 200 + (x*x), RGB(255, 0, 0));   // red color
//			
//			//*** Students: this is the end of the section that you might try to change. ***
//
//			EndPaint(hWnd, &ps);
//			return 0;
//			
//		case WM_DESTROY:
//			PostQuitMessage(0);
//			return 0;
//	}
//	
//	return DefWindowProc(hWnd, message, wParam, lParam);
//}

LRESULT CALLBACK AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_CLOSE || LOWORD(wParam) == IDC_BUTTON){
		EndDialog(hwnd,0);
		return TRUE;
	}
	return FALSE;
}
