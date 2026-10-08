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


// Trova un numero "bello" vicino a x [Paul Heckbert (Graphics Gems)]
inline double niceNum(double x, bool round)
{
	int exp = static_cast<int>(std::floor(std::log10(x)));
	double f = x / std::pow(10.0, exp); // frazione tra 1 e 10
	double niceF;
	if(round){
		if (f < 1.5) niceF = 1.0;
		else if (f < 3.0) niceF = 2.0;
		else if (f < 7.0) niceF = 5.0;
		else niceF = 10.0;
	}else{
		if (f <= 1.0) niceF = 1.0;
		else if (f <= 2.0) niceF = 2.0;
		else if (f <= 5.0) niceF = 5.0;
		else niceF = 10.0;
	}
	return niceF * std::pow(10.0, exp);
}

// Genera il vettore di posizioni per i tick [Paul Heckbert (Graphics Gems)]
inline std::vector<double> generateNiceTicks(double minVal, double maxVal, int maxTicks = 6)
{
	std::vector<double> ticks;
	if (minVal >= maxVal) return ticks;
	
	double range = niceNum(maxVal - minVal, false);
	double tickSpacing = niceNum(range / (maxTicks - 1), true);
	double graphMin = std::floor(minVal / tickSpacing) * tickSpacing;
	double graphMax = std::ceil(maxVal / tickSpacing) * tickSpacing;
	
	for (double x = graphMin; x <= graphMax + (tickSpacing * 0.5); x += tickSpacing) {
		if (x >= minVal - 1e-9 && x <= maxVal + 1e-9) ticks.push_back(x);
	}
	return ticks;
}


#endif // MRC_HPLOTTER_UTIL
