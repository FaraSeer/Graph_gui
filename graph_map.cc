
#include "graph_map.h"
#include <libxml/parser.h>

int xmlGetPropInt(xmlNodePtr node, const xmlChar* name)
{
	return atoi((char*)xmlGetProp(node, name));
}

void xmlSetPropInt(xmlNodePtr node, const xmlChar* name, int val)
{
	static char attr_val[100];
	xmlNewNsProp(node, NULL, name, BAD_CAST itoa(val, attr_val, 10));
}

void save_map(char* filename)
{
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");

	xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "graph");

	xmlDocSetRootElement(doc, root);

	size_t i = 0;
	VertexInfo* cur_vertex;
	while((cur_vertex = get_next_vertex(&i)) != NULL)
	{
		xmlNodePtr cur_node = xmlNewNode(NULL, BAD_CAST "vertex");

		xmlSetPropInt(cur_node, BAD_CAST "id"  , cur_vertex->id   );
		xmlSetPropInt(cur_node, BAD_CAST "x"   , cur_vertex->pos.x);
		xmlSetPropInt(cur_node, BAD_CAST "y"   , cur_vertex->pos.y);
		xmlSetPropInt(cur_node, BAD_CAST "type", cur_vertex->type );

		xmlAddChild(root, cur_node);
	}

	xmlSaveFormatFile(filename, doc, 1);

	xmlFreeDoc(doc);
}

void load_map(char* filename)
{
	xmlDocPtr doc = xmlReadFile(filename, NULL, XML_PARSE_NOBLANKS);
	if(doc == NULL){
		printf("Bad map file!\n");
		xmlFreeDoc(doc);
		return;
	}

	xmlNodePtr root = xmlDocGetRootElement(doc);
	if(!xmlStrEqual(root->name, BAD_CAST "graph")){
		printf("Bad map file format!\n");
		xmlFreeDoc(doc);
		return;
	}

	xmlNodePtr cur = root->children;

	bool bad_attr = false;

	VertexInfo vertex;
	init_vertex(&vertex);

	while(cur != NULL)
	{
		if(!xmlStrEqual(cur->name, BAD_CAST "vertex")){
			cur = cur->next;
			continue;
		}

		if(xmlHasProp(cur, BAD_CAST "id")){
			int val = xmlGetPropInt(cur, BAD_CAST "id");
			printf("id = %d\n", val);
			vertex.id = val;
		}
		else{
			bad_attr = true;
		}
		if(xmlHasProp(cur, BAD_CAST "x")){
			int val = xmlGetPropInt(cur, BAD_CAST "x");
			printf("x = %d\n", val);
			vertex.pos.x = val;
		}
		else{
			bad_attr = true;
		}
		if(xmlHasProp(cur, BAD_CAST "y")){
			int val = xmlGetPropInt(cur, BAD_CAST "y");
			printf("y = %d\n", val);
			vertex.pos.y = val;
		}
		else{
			bad_attr = true;
		}
		if(xmlHasProp(cur, BAD_CAST "type")){
			int val = xmlGetPropInt(cur, BAD_CAST "type");
			printf("type = %d\n", val);
			vertex.type = (VertexType)val;
		}
		else{
			bad_attr = true;
		}

		if(!bad_attr){
			add_vertex(&vertex);
		}
		else{
			printf("Bad vertex description!\n");
		}
		cur = cur->next;
	}
	xmlFreeDoc(doc);
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
