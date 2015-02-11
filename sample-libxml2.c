#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <libxml/HTMLparser.h>
#include <libxml/parser.h>

const char *INPUT_PATH = "/home/seiyak/Documents/Document_Object_Model.htm";
const char *TARGET = "div";

void walkTree(xmlNode * a_node,const char *target,size_t *cnt)
{
	xmlNode *cur_node = NULL;
	xmlAttr *cur_attr = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		// do something with that node information, like… printing the tag’s name and attributes
		if(cur_node->name != NULL && strcmp(target,cur_node->name) == 0){
			(*cnt)++;
			printf("\ttag: %s target: %s line: %hu\n",cur_node->name,target,cur_node->line);
			for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {
				printf("\t\tattribute. key: %s value: %s\n",
					cur_attr->name,xmlGetProp(cur_node,cur_attr->name));
			}
		}
		walkTree(cur_node->children,target,cnt);
	}
}

int main()
{
	xmlDocPtr pDoc = NULL;
	xmlNodePtr pRoot = NULL;
	clock_t t1 = clock();
	pDoc = htmlParseFile(INPUT_PATH,
		(const char *)"utf-8");
	clock_t t2 = clock();
	printf("%f sec took to parse file at %s\n",
		(double)(t2 - t1) / CLOCKS_PER_SEC,INPUT_PATH);

	size_t SZ = 10;
	char *targets[10];
	targets[0] = "div";
	targets[1] = "a";
	targets[2] = "link";
	targets[3] = "sup";
	targets[4] = "table";
	targets[5] = "tbody";
	targets[6] = "li";
	targets[7] = "ul";
	targets[8] = "tr";
	targets[9] = "td";
	if(pDoc != NULL){
		pRoot = xmlDocGetRootElement(pDoc);
		size_t cnt = 0;
//		t1 = clock();
		size_t i;
		for(i = 0; i < SZ;i++){
			walkTree(pRoot,targets[i],&cnt);
			printf("found %zu <%s> tags\n",cnt,targets[i]); 
			cnt = 0;
		}
//		t2 = clock();
//		printf("%f sec took to find %s %zu tags\n",
//		(double)(t2 - t1) / CLOCKS_PER_SEC,TARGET,cnt);
	}
	xmlFreeDoc(pDoc);
	return 0;
}
