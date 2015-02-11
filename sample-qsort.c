#include <stdio.h>
#include <stdlib.h>

struct sel{
	size_t idx;
	union {
		size_t sz;
		void *d;
	}dt;
};

int cmp_sz(const void *a,const void *b)
{
	struct sel *s1 = (struct sel *)a;
	struct sel *s2 = (struct sel *)b;
	return s1->dt.sz - s2->dt.sz;
}

int cmp_szd(const void *a,const void *b)
{
	struct sel *s1 = (struct sel *)a;
	struct sel *s2 = (struct sel *)b;
	if(s1->dt.sz - s2->dt.sz < 0)
		return -1;
	else if(s1->dt.sz - s2->dt.sz > 0)
		return 1;
	return 0;
}

int main()
{
	size_t i,SZ = 10;
	struct sel *sels = (struct sel *)calloc(SZ,sizeof(struct sel));
	for(i = 0; i < SZ;i++){
		sels[i].idx = i;
		sels[i].dt.sz = SZ - i - 1;
		printf("%zu idx: %zu sz: %zu\n",i,sels[i].idx,sels[i].dt.sz);
	}
	qsort(sels,SZ,sizeof(struct sel),cmp_sz);
	printf("%s\n","after calling qsort() with cmp_sz()");
	for(i = 0; i < SZ;i++)
		printf("%zu idx: %zu sz: %zu\n",i,sels[i].idx,sels[i].dt.sz);
	qsort(sels,SZ,sizeof(struct sel),cmp_szd);
	printf("%s\n","after calling qsort() with cmp_szd()");
	for(i = 0; i < SZ;i++)
		printf("%zu idx: %zu sz: %zu\n",i,sels[i].idx,sels[i].dt.sz);

	free(sels);
	return 0;
}
