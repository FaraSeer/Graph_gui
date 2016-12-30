/*
 * graph_logic.cc
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: Sergeus
 */

#include "graph_logic.h"
#include "graph_gui.h"

void set_mask(Status& st, int bit){ st |= bit; }
void clr_mask(Status& st, int bit){ st &= (~bit); }

void    on(Status& st){ st |= ST_ON; st &= (~ST_OFF); }
void   off(Status& st){ st |= ST_OFF; st &= (~ST_ON); }
void   sel(Status& st){ st |= ST_SEL;                 }
void unsel(Status& st){ st &= (~ST_SEL);              }

bool  is_on(Status st){ return (st & ST_ON)  != 0 ? true : false; }
bool is_off(Status st){ return (st & ST_OFF) != 0 ? true : false; }
bool is_sel(Status st){ return (st & ST_SEL) != 0 ? true : false; }


VertexInfo all_vertices[MAX_VERTICES_NUM];
size_t all_vertices_size = 0;

VertexInfo* get_vertex_by_id(int id)
{
	return NULL;
}

VertexInfo* add_vertex(VertexInfo* einfo)
{
	VertexInfo* vertex = NULL;
	if(all_vertices_size < MAX_VERTICES_NUM){
		memcpy(&all_vertices[all_vertices_size], einfo, sizeof(all_vertices[all_vertices_size]));
		vertex = &all_vertices[all_vertices_size];
		vertex->id = all_vertices_size;
		all_vertices_size++;
	}
	return vertex;
}

VertexInfo* get_next_vertex(size_t* i)
{
	VertexInfo* vertex = NULL;
	if(*i != all_vertices_size){
		vertex = &all_vertices[*i];
		(*i)++;
	}
	return vertex;
}

void remove_vertex()
{

}

void remove_all_vertices()
{
	for(size_t i = 0; i < all_vertices_size; i++)
	{
		erase_vertex(&all_vertices[i]);
		memset(&all_vertices[i], 0, sizeof(all_vertices[i]));
	}
	all_vertices_size = 0;
}

void init_vertex(VertexInfo* einfo)
{
	memset(einfo, 0, sizeof(einfo));
}
