#include <stdio.h>
#include <stdlib.h>

struct aa{
	int val;
};

int main()
{
	struct aa *a = (struct aa *)NULL;
	printf("a at: %p\n",a);
	void *av = (void *)NULL;
	a = (struct aa *)av;
	printf("a again at: %p\n",a);
	return 0;
}
