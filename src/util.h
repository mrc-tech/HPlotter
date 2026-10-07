#ifndef MRC_HPLOTTER_UTIL
#define MRC_HPLOTTER_UTIL

#include <vector>
#include <string>


std::vector<std::string> splitTokens(std::string str,char c)
{
	std::vector<std::string> res;
	std::string temp;
	
	for(int i=0;i<str.length();i++)
	{
		if(str[i] == c){
			res.push_back(temp);
			temp.clear();
		} else {
			temp = temp + str[i];
		}
	}
	//aggiunge l'ultimo
	if(temp != "") res.push_back(temp); 
	
	return res;
}


#include <cstdio>
#include <cmath>
#include <string>

// per formattare i numeri (scientifici se troppo grandi o troppo piccoli)
// in realta' posso fare piu' semplicemente con "%g", anche se ho meno controllo
inline std::string formatNumber(double val, double smallThresh = 0.01, double largeThresh = 10000.0)
{
	char buf[64];
	double absVal = std::fabs(val);
	
	// Se diverso da zero e fuori dal range desiderato -> Notazione scientifica
	if (absVal != 0.0 && (absVal < smallThresh || absVal >= largeThresh)) {
		_set_output_format(_TWO_DIGIT_EXPONENT);
		std::sprintf(buf, "%.2E", val); // Es: 1.23e-04 oppure 1.50e+05
	} else {
		std::sprintf(buf, "%.2f", val); // Notazione decimale standard
	}
	
	return std::string(buf);
}

#endif // MRC_HPLOTTER_UTIL
