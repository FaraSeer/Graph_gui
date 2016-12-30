/*
 * grph_logic.h
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: Sergeus
 */

#ifndef GRAPH_LOGIC_H_
#define GRAPH_LOGIC_H_

#include <Pt.h>

#define MAX_VERTICES_NUM 1000

enum VertexType { GT_SOURCE, GT_KEY, GT_WIRE };

typedef char Status;
#define ST_ON  (1<<0)
#define ST_OFF (1<<1)
#define ST_SEL (1<<2)

struct VertexInfo
{
	int         id;
	PhPoint_t   pos;
	PtWidget_t* vertex;
	PtWidget_t* label_type;
	PtWidget_t* label_name;
	VertexType  type;
	const char* name;
	Status      status;
	PtWidget_t* menu_win;
};

enum CmdType { CT_CREATE_LINK, CT_IS_BINDED, CT_GET_INTER_VERTICES };

struct GraphCmd
{
	CmdType    type;
	VertexInfo info;
};

//void set_mask(Status& st, int bit);
//void clr_mask(Status& st, int bit);

void    on(Status& st);
void   off(Status& st);
void   sel(Status& st);
void unsel(Status& st);

bool  is_on(Status st);
bool is_off(Status st);
bool is_sel(Status st);

VertexInfo* add_vertex(VertexInfo* einfo);
VertexInfo* get_next_vertex(size_t* i);
void remove_all_vertices();
void init_vertex(VertexInfo* einfo);

#endif /* GRAPH_LOGIC_H_ */
