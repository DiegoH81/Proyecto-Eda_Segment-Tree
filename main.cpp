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
	std::string files_path = "D:\\Archivos_EDA\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";

	std::string time_manual = "-1",
				time_batch = "-1",
				time_complete = "-1",
				time_manual_query = "0.05",
				time_query = "-1";

	manager managersin;

	managersin.load_files(files_path);
	managersin.configure(1024);
	managersin.menu(time_manual, time_batch, time_complete, time_manual_query, time_query);

	return 0;
}