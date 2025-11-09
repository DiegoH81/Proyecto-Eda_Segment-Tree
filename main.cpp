#include <iostream>
#include <chrono>
#include <thread>

#include "manager.h"


#include "porter_algorithm.h"
#include "funciones.h"
#include "vector.h"
#include "unordered_map.h"

int main()
{
	std::string files_path = "C:\\Users\\diego\\OneDrive\\Escritorio\\EDA\\archivos_txt\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";
	manager managersin;
	
	managersin.load_files(files_path);
	managersin.configure();
	managersin.menu();
	

    return 0;
}