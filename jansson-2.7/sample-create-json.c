#include <stdlib.h>
#include <stdio.h>
#include <jansson.h>

int main()
{
	//// {"firstName" : "first","lastName" : "last"}
	json_t *root = json_object();
	json_object_set_new(root,"firstName",json_string("first"));
	json_object_set_new(root,"lastName",json_string("last"));

	char *res = json_dumps(root,0);
	printf("res, flag == 0,simple:\n%s\n\n",res);
	free(res);

	res = json_dumps(root,JSON_INDENT(4));
	printf("res flag == JSON_INDENT(4),simple:\n%s\n\n",res);
	free(res);

	res = json_dumps(root,JSON_COMPACT);
	printf("res flag == JSON_COMPACT,simple:\n%s\n\n",res);
	free(res);
	json_decref(root);

	//// {"data":[{"firstName": "first1","lastName": "last1"},{"firstName": "first2","lastName": "last2"}]}
	root = json_object();
	json_t *ar = json_array();

	json_t *el1 = json_object();
	json_object_set_new(el1,"firstName",json_string("first1"));
	json_object_set_new(el1,"lastName",json_string("last1"));

	json_t *el2 = json_object();
	json_object_set_new(el2,"firstName",json_string("first2"));
	json_object_set_new(el2,"lastName",json_string("last2"));

	json_array_append_new(ar,el1);
	json_array_append_new(ar,el2);

	json_object_set_new(root,"data",ar);

	res = json_dumps(root,JSON_INDENT(4));
	printf("res flag == JSON_INDENT(4),array:\n%s\n\n",res);
	free(res);
	json_decref(root);
	return 0;
}
