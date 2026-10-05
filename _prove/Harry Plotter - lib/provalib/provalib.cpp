// non so perchè ma il computerino mi da problemi con la libreria windows
// linker: -mwindows -lplotter

#include <iostream>
#include <vector>
#include "plotter.h"

int main()
{	
	Plotter plot;
	
	plot.addItem(-0.5,-0.5);
	plot.addItem(0.5,-0.5);
	plot.addItem(0,0.5);
	
	plot.addItem(-10,-10);
	plot.addItem(-9,-10);
	plot.addItem(-9.5,-9);
	plot.autoSize();
	
//	plot.setMinMax(plot.getMinx(),plot.getMiny(),plot.getMaxx() + 10,plot.getMaxy());
//	plot.setBoundaries(40,40,640,440);
	
	plot.plotCoord(false);	
	
	//va chiamata dopo l'inserimento dei dati (soluzione penosa) [andrebbe fatta non-modale] {pare che solo un dialog può essere non-modale} 
	plot.showWindow();
	
	std::cout << "Visualizzazione terminata. Controllo tornato al programma" << std::endl;
	
	return 0;
}
