#pragma once
#include <vector>
#include <windows.h>


typedef struct _point{
	double x;
	double y;
}point;

class Plotter
{
	public:
		Plotter();
		~Plotter();
		
		void insertData(std::vector<point> pts);
		void freeData();
		void addItem(point pt);
		void addItem(double x,double y);
		
		void autoSize();
		void updateAxis();
		void setBoundaries(double Xi,double Yi,double Xf,double Yf);
		void setMinMax(double Xmin,double Ymin,double Xmax,double Ymax);
		double getRatio();
		double getMaxx();
		double getMaxy();
		double getMinx();
		double getMiny();
		
		void plotCoord(bool b);
		
		int showWindow(HINSTANCE hInstance,int nCmdShow);
		int showWindow();
		void closeWindow();
	private:
		//DATI
		std::vector<point> data;
		//valori massimi dei dati
		double minx,maxx,miny,maxy;		
		//variabili per convertire in coordinate schermo
		double xi,xf,yi,yf; //limiti del rettagolo plottato
		point centro; //centro O degli assi
		int scalaX, scalaY; //scala degli assi X e Y
		//vaiabili di controllo
		bool plotCoords;
		//FUNZIONI
		void updateAxis(double xmin,double ymin,double xmax,double ymax);
	protected:
		//Callback
		static LRESULT CALLBACK StaticWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam); //funzione statica per evitare che aggiunga il parametro THIS
		LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam); //reale funzione della finestra che può accedere ai dati della classe
};
