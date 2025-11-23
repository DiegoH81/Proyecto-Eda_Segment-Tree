#include <iostream>
#include <chrono>

#include "manager.h"

int main()
{
	std::string files_path = "D:\\Archivos_EDA\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";

	std::string time_manual = "-1",
				time_batch = "2",
				time_complete = "-1",
				time_manual_query = "0.75",
				time_query = "-1";

	manager managersin;

	managersin.load_files(files_path);
	managersin.show_cover();
	managersin.configure(1024, 4);
	managersin.menu(time_manual, time_batch, time_complete, time_manual_query, time_query);

	return 0;
}