
#include "graph_map.h"
#include <libxml/parser.h>

void save_map(char* filename)
{
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");

	xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "graph");

	xmlDocSetRootElement(doc, root);

	char attr_val[100];
	size_t i = 0;
	VertexInfo* cur_vertex;
	while((cur_vertex = get_next_vertex(&i)) != NULL)
	{
		xmlNodePtr cur_node = xmlNewNode(NULL, BAD_CAST "vertex");
		itoa(cur_vertex->pos.x, attr_val, 10);
		xmlNewNsProp(cur_node, NULL, BAD_CAST "x", BAD_CAST attr_val);
		itoa(cur_vertex->pos.y, attr_val, 10);
		xmlNewNsProp(cur_node, NULL, BAD_CAST "y", BAD_CAST attr_val);
		itoa(cur_vertex->type, attr_val, 10);
		xmlNewNsProp(cur_node, NULL, BAD_CAST "type", BAD_CAST attr_val);

		xmlAddChild(root, cur_node);
	}

	xmlSaveFormatFile(filename, doc, 1);
}

void load_map(char* filename)
{
	xmlDocPtr doc = xmlReadFile(filename, NULL, XML_PARSE_NOBLANKS);

	xmlNodePtr root = xmlDocGetRootElement(doc);

	xmlNodePtr cur = root->children;

	char attr_val[100];

	bool bad_attr = false;

	VertexInfo vertex;
	init_vertex(&vertex);

	while(cur != NULL)
	{
		if(xmlHasProp(cur, BAD_CAST "x")){
			char* val_str = (char*)xmlGetProp(cur, BAD_CAST "x");
			strncpy(attr_val, val_str, 100);
			int val = atoi(attr_val);
			printf("x = %d\n", val);
			vertex.pos.x = val;
		}
		else{
			bad_attr = true;
		}
		if(xmlHasProp(cur, BAD_CAST "y")){
			char* val_str = (char*)xmlGetProp(cur, BAD_CAST "y");
			strncpy(attr_val, val_str, 100);
			int val = atoi(attr_val);
			printf("y = %d\n", val);
			vertex.pos.y = val;
		}
		else{
			bad_attr = true;
		}
		if(xmlHasProp(cur, BAD_CAST "type")){
			char* val_str = (char*)xmlGetProp(cur, BAD_CAST "type");
			strncpy(attr_val, val_str, 100);
			int val = atoi(attr_val);
			printf("type = %d\n", val);
			vertex.type = (VertexType)val;
		}
		else{
			bad_attr = true;
		}
		if(!bad_attr){
			add_vertex(&vertex);
		}
		cur = cur->next;
	}
}

/*
void start_element_callback(void* user_data, const xmlChar* name, const xmlChar** attrs)
{
	printf("Beg of element %s\n", name);

	while((attrs != NULL) && (attrs[0] != NULL))
	{
		printf("Attr %s = %s\n", attrs[0], attrs[1]);
		attrs = &attrs[2];
	}
}
*/

/*
void load_map(char* filename)
{
	int res = 0;

	xmlSAXHandler SH = { 0 };

	SH.startElement = start_element_callback;

	xmlParserCtxtPtr ctxt = xmlNewParserCtxt();

	if(ctxt == NULL){
		printf("Context were not created\n");
		return;
	}

	res = xmlInitParserCtxt(ctxt);
	printf("init res = %p\n", res);

	ctxt->sax = &SH;

	res = xmlSAXUserParseFile(&SH, NULL, filename);
	printf("parse res = %p\n", res);

	if(!ctxt->wellFormed){
		printf("Bad xml document\n");
		return;
	}

}
*/
