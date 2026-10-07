#include "plotter.h"
#include "resource.h"
#include <windows.h>
#include <winuser.h> //per il GWL_USERDATA
#include <commctrl.h> // Per la Status Bar
#include <cmath> //fabs()
#include <stdio.h>
#include <fstream>
#include "dxf_exporter.h"
#include "callback.h"
#include "util.h"

#define MAX_COLOR 10



Plotter::Plotter()
{
	this->minx = -1;
	this->maxx = 1;
	this->miny = -1;
	this->maxy = 1;
	
	//inizializzazione variabili di controllo
	this->plotCoords = true;
	hWndStatus = NULL; // Handle per la barra di stato
	
	// OPZIONI:
	margin = 40;        // Margine di default in pixel
	lineWidth = 2;		// spessore delle linee dei dati (pixel)
	darkMode = false;   // Tema chiaro di default
	
	//inizializzazione assi cartesiani
	this->xi = margin;
	this->xf = 400;
	this->yi = margin;
	this->yf = 400;
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
	
//	if(this->data.size() == 0){
//		return;
//	}else{
//		// inizia con un valore dello stesso ordine di grandezza
//		xmin = xmax = this->data[0].x;
//		ymin = ymax = this->data[0].y;
//		// controlla i limiti
//		for(int i=0;i<this->data.size();i++){
//			if(xmin > this->data[i].x) xmin = this->data[i].x;
//			if(xmax < this->data[i].x) xmax = this->data[i].x;
//			if(ymin > this->data[i].y) ymin = this->data[i].y;
//			if(ymax < this->data[i].y) ymax = this->data[i].y;
//		}
//		// Assegna le variabili di classe
//		this->minx = xmin;
//		this->miny = ymin;
//		this->maxx = xmax;
//		this->maxy = ymax;
//		updateAxis(xmin,ymin,xmax,ymax);
//	}
	
	
	if(mdata.size() > 0){
		// inizia con un valore dello stesso ordine di grandezza
		xmin = xmax = this->mdata[0][0].x;
		ymin = ymax = this->mdata[0][0].y;	
		//cerca in tutti i dati se ci sono dei valori maggiori
		for(int i=0;i<mdata.size();i++){
			for(int j=0;j<mdata[i].size();j++){
				if(xmin > this->mdata[i][j].x) xmin = this->mdata[i][j].x;
				if(xmax < this->mdata[i][j].x) xmax = this->mdata[i][j].x;
				if(ymin > this->mdata[i][j].y) ymin = this->mdata[i][j].y;
				if(ymax < this->mdata[i][j].y) ymax = this->mdata[i][j].y;
			}
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
	return (double) this->scalaX / this->scalaY;
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


int Plotter::Tx(double x)
{
	return (int) this->centro.x + ( x * this->scalaX);
}


int Plotter::Ty(double y)
{
	return (int) this->centro.y - ( y * this->scalaY);
}


void Plotter::addFile(std::string filename)
{
	std::ifstream dati(filename.c_str()); //NON APRE FILE CHE CONTENGONO SPAZI NEL PATH
	
	if(!dati.is_open()) MessageBox(0,"File non trovato!\nC'è un errore con i PATH contenenti spazi...","Errore",0);
	
	std::string str;
	std::vector<std::string> temp;
	std::vector<point> pts;
	point pt;
	
	//calcola la riga iniziale (che indica nome e quantità delle variabili
	getline(dati,str); //Prende tutta la linea dal file
	
	varName = splitTokens(str,' ');
	numvar = varName.size() - 1; // il numero delle variabili è minore perchè var0 non è una variabile ma l'ordinata
	mdata.resize(numvar); //alloca la memoria per le variabili (mdata[3] è la variabile tre var3)
	//prende tutti i dati
	while(!dati.eof()){
		getline(dati,str);
		temp = splitTokens(str,' ');
		for(int j=1;j<temp.size();j++){
			pt.x = atof(temp[0].c_str());
			pt.y = atof(temp[j].c_str());
			pts.push_back(pt);
			mdata[j-1].push_back(pt);
		}
	}
	
//	std::ofstream log("log.txt");
//	for(int i=0;i<mdata[0].size();i++){
//		log << mdata[0][i].x << " " << mdata[0][i].y  << std::endl;
//	}
	
	
	
	
	dati.close();
}


LRESULT CALLBACK Plotter::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x;
	
	switch (msg)
	{
		case WM_CREATE:
			// Inizializza i controlli comuni
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC = ICC_BAR_CLASSES;
			InitCommonControlsEx(&icex);
			
			// Crea la Status Bar in fondo alla finestra
			this->hWndStatus = CreateWindowEx(
				0,
				STATUSCLASSNAME,
				"Posiziona il mouse sul grafico per leggere le coordinate",
				WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
				0, 0, 0, 0,
				hwnd,
				(HMENU)1000,
				GetModuleHandle(NULL),
				NULL
			);
			return 0;
		
		case WM_PAINT:
		{
			HDC hDC;
			PAINTSTRUCT ps;
			HPEN hOldPen, hNewPen, hRedPen;
			char buf[256];
			
			hDC = BeginPaint(hwnd, &ps);
			hNewPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));   // Grigio
			hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));   // Rosso
			//colori dei grafici
			HPEN grafico[MAX_COLOR];
			grafico[0] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,0,0)); //Nero
			grafico[1] = CreatePen(PS_SOLID, this->lineWidth, RGB(255,0,0)); //Rosso
			grafico[2] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,0,255)); //Blu
			grafico[3] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,100,0)); //Verde
			grafico[4] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,255,255));
			grafico[5] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,0,0));
			grafico[6] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,0,0));
			grafico[7] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,0,0));
			grafico[8] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,0,0));
			grafico[9] = CreatePen(PS_SOLID, this->lineWidth, RGB(0,0,0));
			
			
			
			
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
			// Scrive i limiti del rettangolo del grafico
			std::string strMaxX = formatNumber(this->maxx);
			TextOut(hDC, this->xf + 1, this->yf - 16, strMaxX.c_str(), static_cast<int>(strMaxX.length()));
			std::string strMaxY = formatNumber(this->maxy);
			TextOut(hDC, this->xi, this->yi - 16, strMaxY.c_str(), static_cast<int>(strMaxY.length()));
			std::string strMinX = formatNumber(this->minx);
			TextOut(hDC, this->xi - 40, this->yf - 16, strMinX.c_str(), static_cast<int>(strMinX.length()));
			std::string strMinY = formatNumber(this->miny);
			TextOut(hDC, this->xi, this->yf + 1, strMinY.c_str(), static_cast<int>(strMinY.length()));
			
			
			
			hOldPen = (HPEN)SelectObject(hDC, hNewPen); //usa il grigio
			
			//disegna le righe che compongono la griglia primaria			
			
			hOldPen = (HPEN)SelectObject(hDC, hRedPen); //usa il Rosso
			
			
			//-----------------------------------------------------------------
			//-----------------------------------------------------------------
			//                DISEGNA I DATI
			//-----------------------------------------------------------------
			//-----------------------------------------------------------------
			
//			//disegna il primo dato (ci vuole per forza a causa del MoveToEx)
//			if(data.size() > 0){ /* controlla se non ci sono dati */
//				MoveToEx(hDC,Tx(data[0].x),Ty(data[0].y),NULL);
//				if(plotCoords){
//					sprintf(buf,"%0.2f,%0.2f",this->data[0].x,this->data[0].y);
//					TextOut(hDC,Tx(data[0].x),Ty(data[0].y)+5, buf, 10);
//				}
//			}			
//			
//			//disegna i restanti dati come polilinea
//			for(int i=1;i<this->data.size();i++)
//			{
//				LineTo(hDC,Tx(data[i].x),Ty(data[i].y));
//				if(plotCoords){
//					sprintf(buf,"%0.2f,%0.2f",this->data[i].x,this->data[i].y);
//					TextOut(hDC, Tx(data[i].x)+5, Ty(data[i].y)+5, buf, 10);	
//				}
//				
//			}
			
			
			for(int i=0;i<mdata.size();i++){
				SelectObject(hDC,grafico[i % MAX_COLOR]); //usa il colore del grafico
				MoveToEx(hDC,Tx(mdata[i][0].x),Ty(mdata[i][0].y),NULL);
				for(int j=1;j<mdata[i].size();j++){
					LineTo(hDC,Tx(mdata[i][j].x),Ty(mdata[i][j].y));
					if(plotCoords){
						sprintf(buf,"%0.2f,%0.2f",this->mdata[i][j].x,this->mdata[i][j].y);
						TextOut(hDC, Tx(mdata[i][j].x)+5, Ty(mdata[i][j].y)+5, buf, 10);	
					}
				}
			}
			
			
			//-----------------------------------------------------------------
			//-----------------------------------------------------------------
			//-----------------------------------------------------------------
			//-----------------------------------------------------------------
			
			// ##########################################
			// aggiunta da Gemini
			
//			// Esempio di utilizzo dei dati di 'plot' durante la ????????? / WM_PAINT
//			int currentMargin = getMargin();
//			bool isDark = isDarkMode();
//			
//			// Colore di sfondo e delle linee in base al tema
//			COLORREF bgColor   = isDark ? RGB(30, 30, 30)   : RGB(255, 255, 255);
//			COLORREF lineColor = isDark ? RGB(0, 255, 128)  : RGB(0, 0, 255);
			
			// ##########################################
			
			
			
			EndPaint(hwnd, &ps);
			return 0;
		}
			
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_DXF:
					//salva in DXF
					if(export_dxf(this->data)) MessageBox(hwnd,"Errore Esportazione File","ERRORE",0);
					else MessageBox(hwnd,"File scritto con successo.","",0);
					break;
				
				case IDM_OPTIONS:
				{ // parentesi graffe { ... } attorno al case IDM_OPTIONS: per evitare che la dichiarazione della variabile DisplayOptions opts crei conflitti di scope all'interno dello switch.
					DisplayOptions opts;
					opts.margin = this->getMargin(); // Recupera valore corrente
					opts.lineWidth = this->getLineWidth(); // legge lo spessore delle linee
					opts.darkMode = this->isDarkMode(); // Recupera stato corrente
					
					if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPTIONS_DIALOG), hwnd, OptionsProc, (LPARAM)&opts) == IDOK){
			            this->setMargin(opts.margin);
						this->setLineWidth(opts.lineWidth); // imposta il nuovo spessore
						this->setDarkMode(opts.darkMode);
						// Recupera l'area client della finestra:
						RECT rc;
						GetClientRect(hwnd, &rc);
						// Calcola l'altezza della Status Bar (se presente):
						int statusHeight = 0;
						if (this->hWndStatus) {
							RECT rcStatus = {0};
							GetWindowRect(this->hWndStatus, &rcStatus);
							statusHeight = rcStatus.bottom - rcStatus.top;
						}
						// 3. Calcola la dimensione utile sottraendo la barra di stato
						int clientWidth = rc.right;
						int clientHeight = rc.bottom - statusHeight;
						setBoundaries(this->getMargin(), this->getMargin(), clientWidth - this->getMargin(), clientHeight - this->getMargin()); // setta i nuovi margini
						InvalidateRect(hwnd, NULL, TRUE); // Forza il ridisegno della finestra con i nuovi parametri
					}
					break;
				}
				
				case IDM_ABOUT:
					DialogBox(0,MAKEINTRESOURCE(IDD_DIALOG),hwnd,(DLGPROC)AboutProc);
					break;
					
				case IDM_EXIT:
					PostQuitMessage(0);
					break;					
			}
			return 0;
		
		case WM_MOUSEMOVE:
		{
			int px = LOWORD(lParam);
			int py = HIWORD(lParam);
			
			// Verifica se il cursore è all'interno del rettangolo (xi, yi, xf, yf)
			if (px >= this->xi && px <= this->xf && py >= this->yi && py <= this->yf){
				double x = InvTx(px);
				double y = InvTy(py);
				
				char buf[128];
//				sprintf(buf, "X: %.4g   Y: %.4g", x, y);
				sprintf(buf, "X: %s   Y: %s", 
	                formatNumber(x, 0.01, 10000.0).c_str(), 
	                formatNumber(y, 0.01, 10000.0).c_str());
				SendMessage(this->hWndStatus, SB_SETTEXT, 0, (LPARAM)buf);
			}else{
				SendMessage(this->hWndStatus, SB_SETTEXT, 0, (LPARAM)"Fuori dall'area del grafico");
			}
			return 0;
		}
		
		case WM_SIZE:
		{
			if(this->hWndStatus) SendMessage(this->hWndStatus, WM_SIZE, 0, 0); // Ridimensiona la barra di stato
			
			// Recupera l'altezza della Status Bar per sottrarla dall'area utile del plot
			RECT rcStatus = {0};
			if(this->hWndStatus) GetWindowRect(this->hWndStatus, &rcStatus);
			int statusHeight = rcStatus.bottom - rcStatus.top;
			
			int clientWidth = LOWORD(lParam);
			int clientHeight = HIWORD(lParam) - statusHeight;
			
			// Aggiorna i confini del rettangolo di disegno tenendo conto dello spazio occupato dalla barra
			setBoundaries(this->getMargin(), this->getMargin(), clientWidth - this->getMargin(), clientHeight - this->getMargin());
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
			
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
	WNDCLASSEX wndclass;
	
	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);
	
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = this->StaticWndProc; //funziona perchè la funzione Callback è STATIC
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst; //ritorna l'attuale hInstance
    wndclass.hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR); // Icona grande (Barra delle applicazioni / Alt+Tab)
    wndclass.hIconSm = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR); // Icona piccola (Barra del Titolo in alto a sinistra)
//	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // questo non carica l'icona (come prima)
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
	wndclass.lpszClassName = "WinApp";
	
	if (! RegisterClassEx(&wndclass))
    {
		MessageBox(NULL, "Could not create window.", "Error", 0);
		return 0;
	}
	
	hWnd = CreateWindow("WinApp", "Harry Plotter", WS_OVERLAPPEDWINDOW,
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
