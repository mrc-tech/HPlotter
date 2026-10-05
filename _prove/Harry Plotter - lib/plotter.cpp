#include "plotter.h"
#include "resource.h"
#include <windows.h>
#include <winuser.h> //per il GWL_USERDATA
#include <cmath> //fabs()
#include <stdio.h>
#include "dxf_exporter.h"
#include "callback.h"



Plotter::Plotter()
{
	this->minx = -1;
	this->maxx = 1;
	this->miny = -1;
	this->maxy = 1;
	
	//inizializzazione assi cartesiani
	this->xi = 40;
	this->xf = 400;
	this->yi = 40;
	this->yf = 400;
	
	//inizializzazione variabili di controllo
	this->plotCoords = true;
}


Plotter::~Plotter()
{
	//Cancella tutti i dati
}


void Plotter::insertData(std::vector<point> pts)
{
	this->data = pts;
}


void Plotter::freeData()
{
	
}


void Plotter::addItem(point pt)
{
	this->data.push_back(pt);
}


void Plotter::addItem(double x,double y)
{
	point temp;
	temp.x = x;
	temp.y = y;
	
	this->data.push_back(temp);
}


void Plotter::autoSize()
{
	double xmin,xmax,ymin,ymax;
	
	if(this->data.size() == 0){
		return;
	}else{
		// inizia con un valore dello stesso ordine di grandezza
		xmin = xmax = this->data[0].x;
		ymin = ymax = this->data[0].y;
		// controlla i limiti
		for(int i=0;i<this->data.size();i++){
			if(xmin > this->data[i].x) xmin = this->data[i].x;
			if(xmax < this->data[i].x) xmax = this->data[i].x;
			if(ymin > this->data[i].y) ymin = this->data[i].y;
			if(ymax < this->data[i].y) ymax = this->data[i].y;
		}
		// Assegna le variabili di classe
		this->minx = xmin;
		this->miny = ymin;
		this->maxx = xmax;
		this->maxy = ymax;
		updateAxis(xmin,ymin,xmax,ymax);
	}
}


void Plotter::updateAxis()
{
	updateAxis(this->minx,this->miny,this->maxx,this->maxy);
}


void Plotter::setBoundaries(double Xi,double Yi,double Xf,double Yf)
{
	this->xi = Xi;
	this->yi = Yi;
	this->xf = Xf;
	this->yf = Yf;
	updateAxis();
}


void Plotter::setMinMax(double Xmin,double Ymin,double Xmax,double Ymax)
{
	this->minx = Xmin;
	this->maxx = Xmax;
	this->miny = Ymin;
	this->maxy = Ymax;
	updateAxis(Xmin,Ymin,Xmax,Ymax);
}


double Plotter::getRatio()
{
	return this->scalaX / this->scalaY;
}


double Plotter::getMaxx()
{
	return this->maxx;
}


double Plotter::getMaxy()
{
	return this->maxy;
}


double Plotter::getMinx()
{
	return this->minx;
}


double Plotter::getMiny()
{
	return this->miny;
}


void Plotter::plotCoord(bool b)
{
	this->plotCoords = b;
}


void Plotter::updateAxis(double xmin,double ymin,double xmax,double ymax)
{
	this->scalaX = (double)(this->xf-this->xi)/(xmax-xmin);
	this->scalaY = (double)(this->yf-this->yi)/(ymax-ymin);
	this->centro.x = this->xi-xmin*this->scalaX;
	this->centro.y = this->yi+ymax*this->scalaY;
}


LRESULT CALLBACK Plotter::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x;
	
	switch (msg)
	{
		case WM_CREATE:
			return 0;
		
		case WM_PAINT:
			HDC hDC;
			PAINTSTRUCT ps;
			HPEN hOldPen, hNewPen, hRedPen;
			char buf[256];
			
			hDC = BeginPaint(hwnd, &ps);
			hNewPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));   // Grigio
			hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));   // Rosso


			
			
			//disegna il Box che contiene il plot
//			MoveToEx(hDC, this->centro.x+(this->xi*this->scalaX), this->centro.y, NULL);
//			LineTo(hDC, this->centro.x+(this->xf*this->scalaX), this->centro.y);
//			MoveToEx(hDC, this->centro.x, this->centro.y-(this->yi*this->scalaY), NULL);
//			LineTo(hDC, this->centro.x, this->centro.y-(this->yf*this->scalaY));
			MoveToEx(hDC, this->xi, this->yi, NULL);
			LineTo(hDC, this->xf, this->yi);
			LineTo(hDC, this->xf, this->yf);
			LineTo(hDC, this->xi, this->yf);
			LineTo(hDC, this->xi, this->yi);
			//Disegna gli assi
			if(this->maxx*this->minx < 0){
				//calcola il centro
				this->centro.x = this->xi-this->minx*this->scalaX;
				//disegna gli assi
				MoveToEx(hDC, this->centro.x, this->yi, NULL);
				LineTo(hDC, this->centro.x, this->yf);
			}
			if(this->maxy*this->miny < 0){
				//calcola il centro
				this->centro.y = this->yi+this->maxy*this->scalaY;
				//disegna gli assi
				MoveToEx(hDC, this->xi, this->centro.y, NULL);
				LineTo(hDC, this->xf, this->centro.y);
			}
			//Scrive i limiti del rettangolo
			if(fabs(this->maxx)< 0.01 && this->maxx != 0){
				_set_output_format(_TWO_DIGIT_EXPONENT);
				sprintf(buf,"%.02E",this->maxx);
				TextOut(hDC, this->xf + 1, this->yf -16, buf, strlen(buf));
			}else{
				//usare SetTextAlign per allineare il testo ai bordi per maggiore precisione...
				sprintf(buf,"%.02lf",this->maxx);
				TextOut(hDC, this->xf + 1, this->yf -16, buf, strlen(buf));
			}
			if(fabs(this->maxy) < 0.01 && this->maxy != 0){
				_set_output_format(_TWO_DIGIT_EXPONENT);
				sprintf(buf,"%.02E",this->maxy);
				TextOut(hDC, this->xi, this->yi - 16, buf, strlen(buf));
			}else{
				sprintf(buf,"%.02lf",this->maxy);
				TextOut(hDC, this->xi, this->yi - 16, buf, strlen(buf));
			}
			if(fabs(this->minx) < 0.01 && this->minx != 0){
				_set_output_format(_TWO_DIGIT_EXPONENT);
				sprintf(buf,"%.02E",this->minx);
				TextOut(hDC, this->xi - 40, this->yf - 16, buf, strlen(buf));
			}else{
				sprintf(buf,"%.02lf",this->minx);
				//ruotare di 90° questo testo....
				TextOut(hDC, this->xi - 40, this->yf - 16, buf, strlen(buf));
			}
			if(fabs(this->miny) < 0.01 && this->miny != 0){
				_set_output_format(_TWO_DIGIT_EXPONENT);
				sprintf(buf,"%.02E",this->miny);
				TextOut(hDC, this->xi, this->yf + 1, buf, strlen(buf));
			}else{
				sprintf(buf,"%.02lf",this->miny);
				TextOut(hDC, this->xi, this->yf + 1, buf, strlen(buf));
			}
			
			hOldPen = (HPEN)SelectObject(hDC, hNewPen); //usa il grigio
			
			//disegna le righe che compongono la griglia primaria			
			
			hOldPen = (HPEN)SelectObject(hDC, hRedPen); //usa il Rosso
			
			
			//DISEGNA I DATI
			
			//disegna il primo dato
			MoveToEx(hDC,this->centro.x+(this->data[0].x*this->scalaX),this->centro.y-(this->data[0].y*this->scalaY),NULL);
			sprintf(buf,"%0.2f,%0.2f",this->data[0].x,this->data[0].y);
			if(plotCoords) TextOut(hDC, this->centro.x+(this->data[0].x*this->scalaX)+5, this->centro.y-(this->data[0].y*this->scalaY)+5, buf, 10);
			
			//disegna i restanti dati come polilinea
			for(int i=1;i<this->data.size();i++)
			{
				LineTo(hDC,this->centro.x+(this->data[i].x*this->scalaX),this->centro.y-(this->data[i].y*this->scalaY));
				sprintf(buf,"%0.2f,%0.2f",this->data[i].x,this->data[i].y);
				if(plotCoords) TextOut(hDC, this->centro.x+(this->data[i].x*this->scalaX)+5, this->centro.y-(this->data[i].y*this->scalaY)+5, buf, 10);
				
			}
			
			
			EndPaint(hwnd, &ps);
			return 0;
			
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_DXF:
					//salva in DXF
					if(export_dxf(this->data)) MessageBox(hwnd,"Errore Esportazione File","ERRORE",0);
					else MessageBox(hwnd,"File scritto con successo.","",0);
					break;
				
				case IDM_ABOUT:
					DialogBox(0,MAKEINTRESOURCE(IDD_DIALOG),hwnd,(DLGPROC)AboutProc);
					break;
					
				case IDM_EXIT:
					PostQuitMessage(0);
					break;					
			}
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK Plotter::StaticWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	Plotter* pParent;
	
	//Get pointer to window
	if(msg == WM_CREATE)
	{
		pParent = (Plotter*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)pParent);
	}
	else
	{
		pParent = (Plotter*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
		if(!pParent) return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	
	return pParent->WndProc(hwnd,msg,wParam,lParam);
}


int Plotter::showWindow(HINSTANCE hInstance,int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;
	
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = this->StaticWndProc; //funziona perchè la funzione Callback è STATIC
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
	wndclass.lpszClassName = "WinApp";
	
	if (! RegisterClass(&wndclass))
    {
		MessageBox(NULL, "Could not create window.", "Error", 0);
		return 0;
	}
	
	hWnd = CreateWindow("winApp", "Harry Plotter", WS_OVERLAPPEDWINDOW,
    					CW_USEDEFAULT,   CW_USEDEFAULT, 580, 500, NULL, NULL, hInstance, this); //deve mandare THIS!!!
      
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int) msg.wParam;
}


int Plotter::showWindow()
{
	//Questa funzione carica automaticamente il hInstance senza che lo si debba specificare
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;
	
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = this->StaticWndProc; //funziona perchè la funzione Callback è STATIC
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = (HINSTANCE)GetModuleHandle(NULL); //ritorna l'attuale hInstance
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
	wndclass.lpszClassName = "WinApp";
	
	if (! RegisterClass(&wndclass))
    {
		MessageBox(NULL, "Could not create window.", "Error", 0);
		return 0;
	}
	
	hWnd = CreateWindow("winApp", "Harry Plotter", WS_OVERLAPPEDWINDOW,
    					CW_USEDEFAULT,   CW_USEDEFAULT, 580, 500, NULL, NULL, GetModuleHandle(NULL), this); //deve mandare THIS!!!
      
	
	ShowWindow(hWnd, SW_SHOW); //nCmdShow = SW_SHOW
	UpdateWindow(hWnd);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int) msg.wParam;
}
