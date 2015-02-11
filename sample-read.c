static char *read_sample_deflated_data(const char *name,size_t *tl)
{
	struct stat st;
	int res = stat(name,&st);
	if(res == -1)
		return NULL;
	printf("st.st_size: %zu\n",st.st_size);
	*tl = st.st_size;
	char *buf = (char *)malloc(sizeof(char) * st.st_size);
	if(buf == NULL)
		return NULL;
	FILE *fp;
	fp = fopen(name,"r");
	size_t count = 0;
	while(count < st.st_size){
		buf[count++] = fgetc(fp);
	}
	fclose(fp);
	return buf;
}


static char *test_read_cmp_log()
{
	size_t tl = 0,sz = 0;
	char *buf =
		read_sample_deflated_data("/tmp/sse-crwlr-logs/mbdy/rb/-1/0/aef28334-6085-4aa9-bc5e-28c13a905e1c.localhost.zz",&tl);
	printf("buf:%s\n",buf);
	//// inflate with deflate
	int res = read_cmp(&buf,tl,"gzip",&sz);
	mu_assert("error, res != 0",res == 0);
	mu_assert("error.inflate.gzip, out == NULL",buf != NULL);
	printf("inflated sz: %zu\n",sz);
	printf("buf:%s\n",buf);
	if(buf != NULL){
		free(buf);
		buf = NULL;
	}
	return 0;
}

