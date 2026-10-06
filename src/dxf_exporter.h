#ifndef MRC_HPLOTTER_DXF_EXPORTER
#define MRC_HPLOTTER_DXF_EXPORTER

#include <vector>
#include "plotter.h" //per la definizione delle variabili point
#include <stdio.h> //per le operazioni di scrittura sul file (DEPRECATED)

int export_dxf(std::vector<point> data)
{
	FILE *file;
	
	file = fopen("output.dxf","w+");
	
	if(!file){
		return -1; //ERRORE
	}
	
	// HEADER SECTION
	fprintf(file,"999\nMarchiTechnology\n0\nSECTION\n2\nHEADER\n9\n$ACADVER\n1\nAC1006\n9\n$INSBASE\n10\n0.0\n20\n0.0\n30\n0.0\n9\n$EXTMIN\n10\n0.0\n20\n0.0\n9\n$EXTMAX\n10\n1000.0\n20\n1000.0\n0\nENDSEC\n");
	
	// TABLES SECTION
	fprintf(file,"0\nSECTION\n2\nTABLES\n0\nTABLE\n2\nLTYPE\n70\n1\n0\nLTYPE\n2\nCONTINUOUS\n70\n64\n3\nSolid line\n72\n65\n73\n0\n40\n0.000000\n0\nENDTAB\n0\nTABLE\n2\nLAYER\n70\n6\n0\nLAYER\n2\n0\n70\n64\n62\n7\n6\nCONTINUOUS\n0\nENDTAB\n0\nTABLE\n2\nSTYLE\n70\n0\n0\nENDTAB\n0\nENDSEC\n");
	
	// BLOCKS SECTION
	fprintf(file,"0\nSECTION\n2\nBLOCKS\n0\nENDSEC\n");					
	
	// ENTITIES SECTION
	fprintf(file,"0\nSECTION\n2\nENTITIES\n");
	
	// intestazione polilinea ??
	fprintf(file,"0\nPOLYLINE\n8\n0\n62\n7\n6\nCONTINUOUS\n70\n0\n66\n1\n10\n0\n20\n0\n30\n0\n"); //colore 7
	
	for(int i=0;i<data.size();i++)
	{
		fprintf(file,"0\nVERTEX\n8\n0\n10\n%f\n20\n%f\n", data[i].x, data[i].y);
	}
	//fine polilinea
	fprintf(file,"0\nSEQEND\n");
	
	fprintf(file,"0\nENDSEC\n");
	
	//fine del file
	fprintf(file,"0\nEOF\n");
	
	fclose(file);
	
	return 0; //nessun errore
}

#endif // MRC_HPLOTTER_DXF_EXPORTER
