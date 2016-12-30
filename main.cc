/*
 * main.cc
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: Sergeus
 */

#include "graph_gui.h"
#include "graph_logic.h"
#include "graph_map.h"

char* map_file = NULL;

int main(int argc, char *argv[])
{
	if(PtInit(NULL) == -1) return EXIT_FAILURE;

	create_main_window();

	for(int i = 0; i < argc; i++)
	{
		if(!strcmp(argv[i], "-map")){
			map_file = argv[i + 1];
		}
		if(!strcmp(argv[i], "-plugin")){

		}
	}

	if(map_file != NULL){
		//load_map(map_file);
		//save_map(map_file);
	}

	PtMainLoop();

	return EXIT_SUCCESS;
}


