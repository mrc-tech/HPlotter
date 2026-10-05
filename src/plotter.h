#pragma once
#include <iostream>
#include <vector>
#include <string>
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
		void addFile(std::string filename); //prende i dati da file formattati
		
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
		double scalaX, scalaY; //scala degli assi X e Y
		//vaiabili di controllo
		bool plotCoords;
		//FUNZIONI
		void updateAxis(double xmin,double ymin,double xmax,double ymax);
		//trasformazione delle coordinate
		int Tx(double x);
		int Ty(double y);
		//numero di variabili di cui fare il grafico
		int numvar; //usato per i grafici multipli da file
		std::vector<std::string> varName; //nome delle variabili (usate nei grafici multipli da file)
		std::vector<std::vector<point> > mdata; //dati multipli per più variabili ogni grafico
	protected:
		//Callback
		static LRESULT CALLBACK StaticWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam); //funzione statica per evitare che aggiunga il parametro THIS
		LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam); //reale funzione della finestra che può accedere ai dati della classe
};
