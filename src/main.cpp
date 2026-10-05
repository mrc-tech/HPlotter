// non so perchè ma il computerino mi da problemi con la libreria windows
// aggiungere -mwindows al linker!!!

#include <iostream>
#include <fstream>
#include <vector>

#include "plotter.h"

using namespace std;


INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
{	
	Plotter plot;
	
//	string filename;
//	for(int i=0;i<strlen(lpCmdLine);i++){
//		if(lpCmdLine[i] == '\\')  filename.push_back('/'); //mi sa che è inutile
//		else if(lpCmdLine[i] == ' ') {
//			filename.push_back('%');
//			filename.push_back('2');
//			filename.push_back('0');
//		}else filename.push_back(lpCmdLine[i]);
//	}
	
	
	if(strlen(lpCmdLine) > 0){
		plot.addFile(lpCmdLine);
	}
	
//	plot.addFile("output.txt");
	
	plot.autoSize();
	
//	plot.setMinMax(plot.getMinx(),plot.getMiny(),plot.getMaxx() + 10,plot.getMaxy());
//	plot.setBoundaries(40,40,640,440);
	
	plot.plotCoord(false);
	
	//va chiamata dopo l'inserimento dei dati (soluzione penosa) [andrebbe fatta non-modale] {pare che solo un dialog può essere non-modale} 
	plot.showWindow();
	
	cout << "Visualizzazione terminata. Controllo tornato al programma" << std::endl;
	
	return 0;
}
