#ifndef MRC_HPLOTTER_CALLBACK
#define MRC_HPLOTTER_CALLBACK

#include <windows.h>

#include "resource.h"

#include "plotter_private.h" // per VER_STRING (la versione del programma)

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

INT_PTR CALLBACK AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HFONT hBoldFont = NULL;
	
	if(msg == WM_INITDIALOG){
		// titolo in grassetto:
		HFONT hFont = (HFONT)SendDlgItemMessage(hwnd, IDC_STATIC_TITLE, WM_GETFONT, 0, 0); // Recupera il font attualmente usato dal controllo
		LOGFONT lf;
        if(hFont)GetObject(hFont, sizeof(LOGFONT), &lf);
		else GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf); // Se non c'è un font specifico, prende quello di sistema
        lf.lfWeight = FW_BOLD; // Imposta lo spessore su Grassetto
        hBoldFont = CreateFontIndirect(&lf); // Crea il nuovo font personalizzato
        SendDlgItemMessage(hwnd, IDC_STATIC_TITLE, WM_SETFONT, (WPARAM)hBoldFont, TRUE); // Assegna il nuovo font all'LTEXT desiderato
        // stringa versione:
		char buffer[256];
		sprintf(buffer, "Versione: %s\nCompilato: %s %s", VER_STRING, __DATE__, __TIME__); // Formatta la stringa usando VER_STRING (da plotter_private.h) e __DATE__ / __TIME__
		SetDlgItemText(hwnd, IDC_STATIC_VERSION, buffer); // Imposta il testo nel controllo creato nel dialogo
		return TRUE;
	}
	if(msg == WM_CLOSE || LOWORD(wParam) == IDC_BUTTON){
		EndDialog(hwnd,0);
		return TRUE;
	}
	if(msg == WM_DESTROY){
		if(hBoldFont){ // cancella il font grassetto
			DeleteObject(hBoldFont);
			hBoldFont = NULL;
		}
        return TRUE;
	}
	return FALSE;
}


struct DisplayOptions {
    int margin;
    bool darkMode;
};

INT_PTR CALLBACK OptionsProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static DisplayOptions* pOpts = NULL;

    switch (msg)
    {
    case WM_INITDIALOG:
        pOpts = (DisplayOptions*)lParam;
        if (pOpts) {
            SetDlgItemInt(hwnd, IDC_EDIT_MARGIN, pOpts->margin, FALSE);
            CheckDlgButton(hwnd, IDC_CHECK_DARKMODE, pOpts->darkMode ? BST_CHECKED : BST_UNCHECKED);
        }
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            if (pOpts) {
                pOpts->margin = GetDlgItemInt(hwnd, IDC_EDIT_MARGIN, NULL, FALSE);
                pOpts->darkMode = (IsDlgButtonChecked(hwnd, IDC_CHECK_DARKMODE) == BST_CHECKED);
            }
            EndDialog(hwnd, IDOK);
            return TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#endif // MRC_HPLOTTER_CALLBACK
