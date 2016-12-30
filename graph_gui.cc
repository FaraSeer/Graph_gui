
#include "graph_map.h"

#include <cstdlib>
#include <stdio.h>

typedef int (*photon_cb)(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info);

static VertexInfo* vertices[MAX_VERTICES_NUM];
static size_t vertices_size = 0;

static PtWidget_t* win = NULL;

static GraphCmd   cur_cmd;

int cb_save_to_file(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info);
int cb_load_from_file(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info);
int cb_cmd_handler(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info);
int cb_create_element_menu(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info);
int draw_edge(PtWidget_t* vertex1, PtWidget_t* vertex2);

void cmd_cancel()
{
	for(size_t i = 0; i < vertices_size; i++)
	{
		unsel(vertices[i]->status);
	}
	vertices_size = 0;
	switch(cur_cmd.type)
	{
	case CT_CREATE_LINK:
	{
		size_t i = 0;
		VertexInfo* cur;
		while((cur = get_next_vertex(&i)) != NULL)
		{
			PtSetResource(cur->vertex, Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		}
		vertices_size = 0;
		break;
	}
	case CT_IS_BINDED:
		break;
	case CT_GET_INTER_VERTICES:
		break;
	default:
		break;
	}
}

int cb_cmd_handler(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	switch(cur_cmd.type)
	{
	case CT_CREATE_LINK:
		VertexInfo* einfo;
		PtGetResource(wgt, Pt_ARG_POINTER, &einfo, 0);
		vertices[vertices_size] = einfo;
		sel(vertices[vertices_size]->status);
		vertices_size++;

		if(vertices_size == 2){
			draw_edge(vertices[0]->vertex, vertices[1]->vertex);
			cmd_cancel();
		}
		break;
	case CT_IS_BINDED:
		break;
	case CT_GET_INTER_VERTICES:
		break;
	default:
		break;
	}

	return Pt_CONTINUE;
}

void show_activate_zone(PtWidget_t* wgt) // for debug
{
	PhArea_t* area;
	PtGetResource(wgt, Pt_ARG_AREA, &area, 0);

	const PtArg_t args[] = {
		Pt_ARG(Pt_ARG_AREA, area, 0)
	};
	PtWidget_t* rect = PtCreateWidget(PtRect, win, sizeof(args)/sizeof(args[0]), args);
	PtRealizeWidget(rect);
}

int draw_edge(PtWidget_t* vertex1, PtWidget_t* vertex2)
{
	PhArea_t* vertex1_area;
	PtGetResource(vertex1, Pt_ARG_AREA, &vertex1_area, 0);
	PhArea_t* vertex2_area;
	PtGetResource(vertex2, Pt_ARG_AREA, &vertex2_area, 0);

	PhPoint_t vertex1_pos = { vertex1_area->pos.x + vertex1_area->size.w / 2, vertex1_area->pos.y + vertex1_area->size.h / 2 };
	PhPoint_t vertex2_pos = { vertex2_area->pos.x + vertex2_area->size.w / 2, vertex2_area->pos.y + vertex2_area->size.h / 2 };

	PhPoint_t points[] = { vertex1_pos, vertex2_pos };

	const PtArg_t args[] = {
		Pt_ARG(Pt_ARG_POINTS, points, sizeof(points)/sizeof(points[0])),
		Pt_ARG(Pt_ARG_GRAPHIC_FLAGS, Pt_TRUE, Pt_FLOAT_POS)
	};
	PtWidget_t* edge = PtCreateWidget(PtPolygon, win, sizeof(args)/sizeof(args[0]), args);

	PtRealizeWidget(edge);
	PtWidgetToBack(edge);

#ifdef _SHOW_ACTIVATE_ZONES
	show_activate_zone(edge);
#endif

	return Pt_CONTINUE;
}

void erase_edge(VertexInfo* einfo)
{

}

void draw_vertex(VertexInfo* einfo)
{
	PhArea_t area;
	area.size.h = 40;
	area.size.w = 40;

	area.pos = einfo->pos;

	const PtArg_t args[] = {
		Pt_ARG(Pt_ARG_AREA, &area, 0),
		Pt_ARG(Pt_ARG_FLAGS, Pt_TRUE, Pt_MENUABLE),
		Pt_ARG(Pt_ARG_INSIDE_COLOR, Pg_WHITE, 0),
		Pt_ARG(Pt_ARG_POINTER, einfo, 0)
	};
	einfo->vertex = PtCreateWidget(PtEllipse, win, sizeof(args)/sizeof(args[0]), args);

	PtAddCallback(einfo->vertex, Pt_CB_ACTIVATE, cb_cmd_handler, NULL);
	PtAddCallback(einfo->vertex, Pt_CB_MENU, cb_create_element_menu, NULL);

	PtRealizeWidget(einfo->vertex);
	PtWidgetToFront(einfo->vertex);

	const PtArg_t args1[] = {
		Pt_ARG(Pt_ARG_AREA, &area, 0),
		Pt_ARG(Pt_ARG_HORIZONTAL_ALIGNMENT, Pt_CENTER, 0),
		Pt_ARG(Pt_ARG_TEXT_FONT, "arial12b", 0),
		Pt_ARG(Pt_ARG_TEXT_STRING, einfo->name, 0)
	};
	einfo->label_type = PtCreateWidget(PtLabel, win, sizeof(args1)/sizeof(args[0]), args1);

	PtRealizeWidget(einfo->label_type);
	PtWidgetToFront(einfo->label_type);

#ifdef _SHOW_ACTIVATE_ZONES
	show_activate_zone(vertex);
	show_activate_zone(label);
#endif
}

void erase_vertex(VertexInfo* einfo)
{
	if(einfo->vertex     != NULL) PtDestroyWidget(einfo->vertex);
	if(einfo->label_type != NULL) PtDestroyWidget(einfo->label_type);
	if(einfo->label_name != NULL) PtDestroyWidget(einfo->label_name);
}

int cb_timer_tick(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	static bool flg = false;

	size_t i = 0;
	VertexInfo* cur;
	while((cur = get_next_vertex(&i)) != NULL)
	{
		if(is_sel(cur->status)){
			if(!flg){
				PtSetResource(cur->vertex, Pt_ARG_INSIDE_COLOR, Pg_GREEN, 0);
				flg = true;
			}
			else{
				PtSetResource(cur->vertex, Pt_ARG_INSIDE_COLOR, Pg_WHITE, 0);
				flg = false;
			}
		}
		else{
			if(is_on(cur->status)){
				PtSetResource(cur->vertex, Pt_ARG_INSIDE_COLOR, Pg_GREEN, 0);
			}
			if(is_off(cur->status)){
				PtSetResource(cur->vertex, Pt_ARG_INSIDE_COLOR, Pg_WHITE, 0);
			}
		}
	}

	return Pt_CONTINUE;
}

void destroy_menu(PtWidget_t* win)
{
	printf("Destroyed menu window!\n");
	fflush(stdout);

	PtDestroyWidget(win);
}

int cb_menu_lost_focus(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	PtWidget_t* win = (PtWidget_t*)data;
	destroy_menu(win);
	return Pt_CONTINUE;
}

void create_menu_button(PtWidget_t* parent, int offset, int height, const char* text, photon_cb cb, void* data)
{
	PhArea_t* parent_area;
	PtGetResource(parent, Pt_ARG_AREA, &parent_area, 0);

	PhArea_t area;
	area.pos.x = 0;
	area.pos.y = offset;
	area.size.h = height;
	area.size.w = parent_area->size.w;

	const PtArg_t args[] = {
		Pt_ARG(Pt_ARG_AREA, &area, 0),
		Pt_ARG(Pt_ARG_TEXT_STRING, text, 0),
		Pt_ARG(Pt_ARG_FLAGS, Pt_FALSE, Pt_GETS_FOCUS)
	};
	PtWidget_t* btn = PtCreateWidget(PtButton, parent, sizeof(args)/sizeof(args[0]), args);

	if(cb != NULL) PtAddCallback(btn, Pt_CB_ACTIVATE, cb, data);

	PtRealizeWidget(btn);
}

void create_vertex(VertexInfo* einfo)
{
	switch(einfo->type)
	{
	case GT_SOURCE:
		einfo->name = "S";
		break;
	case GT_KEY:
		einfo->name = "K";
		break;
	case GT_WIRE:
		einfo->name = "W";
		break;
	default:
		break;
	}

	draw_vertex(einfo);
	off(einfo->status);
}

void delete_vertex(VertexInfo* einfo)
{
	erase_vertex(einfo);
}
/*
int cb_delete_vertex(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Deleting a vertex!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;


	return Pt_CONTINUE;
}
*/
int cb_create_source(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Creating a source!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;
	einfo = add_vertex(einfo);
	einfo->type = GT_SOURCE;
	create_vertex(einfo);

	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

int cb_create_key(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Creating a key!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;
	einfo = add_vertex(einfo);
	einfo->type = GT_KEY;
	create_vertex(einfo);

	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

int cb_create_wire(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Creating a wire!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;
	einfo = add_vertex(einfo);
	einfo->type = GT_WIRE;
	create_vertex(einfo);

	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

int cb_create_edge(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Creating a link!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;

	memcpy(&(cur_cmd.info), einfo, sizeof(cur_cmd.info));
	cur_cmd.type = CT_CREATE_LINK;

	size_t i = 0;
	VertexInfo* cur;
	while((cur = get_next_vertex(&i)) != NULL)
	{
		PtSetResource(cur->vertex, Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
	}

	destroy_menu(einfo->menu_win);
	if(cur_cmd.type != CT_CREATE_LINK) cmd_cancel();

	return Pt_CONTINUE;
}

int cb_cancel(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Cancel!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;

	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

int cb_switch_on(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("ON!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;
	on(einfo->status);

	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

int cb_switch_off(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("OFF!\n");
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;
	off(einfo->status);

	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

PtWidget_t* create_win(PhArea_t area)
{
	const PtArg_t args[] = {
		Pt_ARG(Pt_ARG_AREA, &area, 0),
		Pt_ARG(Pt_ARG_WINDOW_RENDER_FLAGS, Pt_FALSE, ~(Ph_WM_RENDER_ASDIALOG | Ph_WM_RENDER_BORDER | Ph_WM_RENDER_INLINE)),
		Pt_ARG(Pt_ARG_WINDOW_MANAGED_FLAGS, Pt_FALSE, ~(Ph_WM_FOCUS | Ph_WM_TOFRONT | Ph_WM_TOBACK | Ph_WM_MOVE | Ph_WM_RESIZE)),
		Pt_ARG(Pt_ARG_WINDOW_NOTIFY_FLAGS, Pt_FALSE, -1),
		Pt_ARG(Pt_ARG_CONTAINER_FLAGS, Pt_FALSE, -1),
		Pt_ARG(Pt_ARG_FLAGS, Pt_FALSE, -1),
		Pt_ARG(Pt_ARG_RESIZE_FLAGS, Pt_FALSE, -1),
		Pt_ARG(Pt_ARG_EFLAGS, Pt_FALSE, -1)
	};
	PtWidget_t* menu_win = PtCreateWidget(PtWindow, win, sizeof(args)/sizeof(args[0]), args);
	PtAddCallback(menu_win, Pt_CB_LOST_FOCUS, cb_menu_lost_focus, menu_win);

	PtRealizeWidget(menu_win);

	return menu_win;
}

int cb_create_element_menu(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Creating element menu window!\n");
	fflush(stdout);

	PhArea_t area;
	area.size.h = 150;
	area.size.w = 100;

	PhPointerEvent_t* event = (PhPointerEvent_t*)PhGetData(info->event);
	PhPoint_t p1 = event->pos;
	PhPoint_t p2 = info->event->translation;
	PhPoint_t pos;
	pos.x = p1.x;
	pos.y = p1.y;

	area.pos = pos;

	VertexInfo* einfo;
	PtGetResource(wgt, Pt_ARG_POINTER, &einfo, 0);

	einfo->menu_win = create_win(area);

	const unsigned short btn_height = 50;
	unsigned short cur_offset = 0;

	create_menu_button(einfo->menu_win, cur_offset, btn_height, "Включить" , cb_switch_on , einfo);
	cur_offset += btn_height;
	create_menu_button(einfo->menu_win, cur_offset, btn_height, "Отключить", cb_switch_off, einfo);
	cur_offset += btn_height;
	create_menu_button(einfo->menu_win, cur_offset, btn_height, "Отмена"   , cb_cancel    , einfo);

	return Pt_CONTINUE;
}

int cb_create_adding_menu(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Creating adding menu window!\n");
	fflush(stdout);

	PhArea_t area;
	area.size.h = 350;
	area.size.w = 100;

	PhPointerEvent_t* event = (PhPointerEvent_t*)PhGetData(info->event);
	PhPoint_t p1 = event->pos;
	PhPoint_t p2 = info->event->translation;
	PhPoint_t pos;
	pos.x = p1.x;
	pos.y = p1.y;

	area.pos = pos;

	static VertexInfo einfo;
	init_vertex(&einfo);

	einfo.pos.x = pos.x + p2.x;
	einfo.pos.y = pos.y + p2.y;
	einfo.menu_win = create_win(area);

	const unsigned short btn_height = 50;
	unsigned short cur_offset = 0;

	create_menu_button(einfo.menu_win, cur_offset, btn_height, "Создать\nисточник", cb_create_source , &einfo);
	cur_offset += btn_height;
	create_menu_button(einfo.menu_win, cur_offset, btn_height, "Создать\nключ"    , cb_create_key    , &einfo);
	cur_offset += btn_height;
	create_menu_button(einfo.menu_win, cur_offset, btn_height, "Создать\nпровод"  , cb_create_wire   , &einfo);
	cur_offset += btn_height;
	create_menu_button(einfo.menu_win, cur_offset, btn_height, "Связать"          , cb_create_edge   , &einfo);
	cur_offset += btn_height;
	create_menu_button(einfo.menu_win, cur_offset, btn_height, "Сохранить"        , cb_save_to_file  , &einfo);
	cur_offset += btn_height;
	create_menu_button(einfo.menu_win, cur_offset, btn_height, "Загрузить"        , cb_load_from_file, &einfo);
	cur_offset += btn_height;
	create_menu_button(einfo.menu_win, cur_offset, btn_height, "Отмена"           , cb_cancel        , &einfo);

	return Pt_CONTINUE;
}

void create_main_window()
{
// Creating Main Window
	                 //  X    Y        W    H
	PhArea_t area = { { 100, 100 }, { 500, 500 } };

	const PtArg_t args[] = {
		Pt_ARG(Pt_ARG_AREA, &area, 0)
	};
	win = PtCreateWidget(PtWindow, Pt_NO_PARENT, sizeof(args)/sizeof(args[0]), args);

	PtRealizeWidget(win);

// Creating Timer
	PtCallback_t cb;
	cb.event_f = cb_timer_tick;
	cb.data = NULL;

	const PtArg_t args1[] = {
			Pt_ARG(Pt_ARG_TIMER_INITIAL, 500, 0),
			Pt_ARG(Pt_ARG_TIMER_REPEAT, 500, 0),
			Pt_ARG(Pt_CB_TIMER_ACTIVATE, &cb, 0)
		};
	PtWidget_t* timer = PtCreateWidget(PtTimer, win, sizeof(args1)/sizeof(args1[0]), args1);
	PtRealizeWidget(timer);

// Creating Activating Area
	PhDim_t* dim2;
	PtGetResource(win, Pt_ARG_DIM, &dim2, 0);

	const PtArg_t args2[] = {
		Pt_ARG(Pt_ARG_DIM, dim2, 0),
		Pt_ARG(Pt_ARG_FLAGS, Pt_TRUE, Pt_MENUABLE)
	};
	PtWidget_t* rect = PtCreateWidget(PtRect, win, sizeof(args2)/sizeof(args2[0]), args2);
	PtRealizeWidget(rect);

	PtAddCallback(rect, Pt_CB_MENU, cb_create_adding_menu, NULL);
}

extern char* map_file;

int cb_save_to_file(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Saving graph to file %s!\n", map_file);
	fflush(stdout);

	VertexInfo* einfo = (VertexInfo*)data;
	save_map(map_file);

	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

int cb_load_from_file(PtWidget_t* wgt, void* data, PtCallbackInfo_t* info)
{
	printf("Loading graph from file %s!\n", map_file);
	fflush(stdout);

	remove_all_vertices();

	load_map(map_file);

	size_t i = 0;
	VertexInfo* cur;
	while((cur = get_next_vertex(&i)) != NULL)
	{
		create_vertex(cur);
	}

	VertexInfo* einfo = (VertexInfo*)data;
	destroy_menu(einfo->menu_win);
	cmd_cancel();

	return Pt_CONTINUE;
}

