#include <iostream>
#include <chrono>
#include <thread>

#include "manager.h"


#include "porter_algorithm.h"
#include "vector.h"
#include "sort.h"
int main()
{
	std::string files_path = "C:\\Users\\diego\\OneDrive\\Escritorio\\EDA\\archivos_txt\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";
	manager managersin;

	
	managersin.load_files(files_path);
	managersin.configure();
	managersin.menu();
	
	


	/*
	vector<int> mio;

	mio.push_back(12);
	mio.push_back(1);
	mio.push_back(3);
	mio.push_back(4);
	mio.push_back(5);
	mio.push_back(6);
	mio.push_back(9);
	mio.push_back(9);
	mio.push_back(100);
	mio.push_back(9);
	mio.push_back(9);
	mio.push_back(9);
	mio.push_back(9);

	for (auto& i : mio)
		std::cout << i << "\t";
	std::cout << "\n";

	
	mio.sort([](int a, int b) { return a < b; }); // ascendente
	// o: mio.sort([](int a, int b) { return a > b; }); // descendente

	for (auto& i : mio)
		std::cout << i << "\t";
	std::cout << "\n";
	*/
}