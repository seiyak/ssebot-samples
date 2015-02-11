#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SZ 4095

const char *INPUT_PATH = "/home/seiyak/Documents/Document_Object_Model.htm";
//const char *INPUT_PATH = "/home/seiyak/Downloads/sse-related/HTML_Document_Short.html";

int main()
{
	clock_t t1 = clock();
	int fd = open(INPUT_PATH,O_RDONLY);
	if(fd < 0){
		perror("open");
		return -1;
	}
	clock_t t2 = clock();
	printf("%f sec took to read file at %s\n",
		(double)(t2 - t1) / CLOCKS_PER_SEC,INPUT_PATH);
	int n,e = -1,cm = -1,hm = -1;
	char buf[BUF_SZ];
	size_t sz = 0,i,j,o = 0,c = 0,cs = 0,hs = 0,he = 0,tmpsz = 0,tmpidx = 0;
	ssize_t end = 0;
	char *str = NULL,*tmp = NULL;
	char *TARGET = "<a";
	size_t linkcnt = 0;
	t1 = clock();
	while((n = read(fd,buf,BUF_SZ)) > 0){
		for(i = 0; i < n;i++){
			if(buf[i] == '\n' || buf[i] == ' '){
				//// skip '\n' and whitespace.
				continue;
			}
//			printf("i: %zu %c\n",i,buf[i]);
			if(buf[i] == '/'){
				if(cm == -1 && (i - o) == 1){ //// close tag '/'
					e = 0;
					end = i;
				}else if(hm == -1 && cm == 0 && (i - end) == 1){
					//// closing comment, '*/'.
					cm = -1;
					end = 0;
					cs = 0;
					e = -1;
				}else{
					if(hs < 2){
						/// starting comment, '/*'.
						cs = i;
						end = i;
						e = -1;
					}else{
						cs = 0;
						end = 0;
						e = -1;
					}
				}
			}else if(buf[i] == '*'){
				if(hm == -1 && cm == -1 && (i > end)
					&& (i - end) == 1){
					//// starting comment, '/*'.
					e = -1;
					end = i;
					cm = 0;
					cs = (i + 1) < n ? (i + 1) : i;
				}else if(hm == -1 && cm == 0) //// closing comment, '*/'.
					end = i;
				else{
					cm = -1;
					cs = 0;
				}
			}else if(hm == -1 && buf[i] == '!' && (i - o) == 1){
				//// candidate of html starting comment, '<!.
				hm = 0;
				hs = i;
			}else if(hm >= 0 && buf[i] == '-'){
				if((i - hs) == 1){
					hm = 1;
				}else if(hm == 1 && (i - hs) == 2){
					//// found html starting comment.
					hm = 2;
					hs = i + 1;
				}else if(hm == 2){
					//// candidate of html closing comment, '-->'.
					if(he == 0)
						he = i;
					if((i - he) == 1){
						hm = 3;
					}else
						he = i;
				}
			}else if(buf[i] == '>'){
				//// closing tag.
				c = i;
				if(hm == 3 && (i - he) == 2){
					//// found html comment.
					hm = -1;
					hs = 0;
					he = 0;
				}
				if(hm < 2){
					hm = -1;
					hs = 0;
					he = 0;
				}
				if(cm == -1 && hm < 2){
//					printf("o: %zu c: %zu o < c ? %d e: %d\n",o,c,(o < c),e);
					if(o > c){ //// leftovers exit.
						//// concatenate leftovers and buf[o to c].
						str = (char *)malloc(sizeof(char) * ((tmpsz + c) + 1));
						memcpy(str,tmp,tmpsz);
						memcpy(str + tmpsz,buf,c);
						str[tmpsz + c] = '\0';
						free(tmp);
						tmp = NULL;
						//for(j = 0;j < tmpsz + c;j++){
						//	//if(str[j] == ' ')
						//	//	break;
						//	printf("%c",str[j]);
						//}
						if(strncmp(TARGET,str,strlen(TARGET)) == 0){
//							printf("%s\n",str);
							linkcnt++;
						}
						free(str);
						str = NULL;
					}else if(o < c){//// within buf.
						if(strncmp(TARGET,buf + o,strlen(TARGET)) == 0){
//							for(j = o;j < c;j++){
//								printf("%c",buf[j]);
//							}
//							printf("%s","\n");
							linkcnt++;
						}
					}
				}
			}else if(buf[i] == '<'){ //// open tag.
				if(cm == 0 || hm >= 2){
//					printf("about to continue,cm == %d, hm >= %d, at i: %zu\n",cm,hm,i);
					continue;
				}
				o = i;
			}
		}
		//// leftovers.
		if(o > c && o < n){
//			printf("\n\nbelow are the leftovers, from: %zu to: %zu\n",o,n);
			tmp = (char *)malloc(sizeof(char) * ((n - o) + 1));
			memcpy(tmp,buf + o,n - o);
			tmp[n - o] = '\0';
			tmpsz = n - o;
		}
		sz += n;
	}
	t2 = clock();
	printf("%f sec took to search tag, %s\n",(double)(t2 - t1) / CLOCKS_PER_SEC,TARGET);

	if(tmp != NULL)
		free(tmp);
	close(fd);
	printf("sz: %zu\n",sz);
	printf("linkcnt: %zu\n",linkcnt);
	return 0;
}
