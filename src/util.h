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

#endif // MRC_HPLOTTER_UTIL
