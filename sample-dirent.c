#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int
main (void)
{
  DIR *dp;
  struct dirent *ep;

  dp = opendir ("./");
  if (dp != NULL)
    {
      while (ep = readdir (dp)){
		struct stat st;
		if(stat(ep->d_name,&st) < 0)
			continue;
		printf("name: %s dir ? %d file ? %d\n",ep->d_name,
		S_ISDIR(st.st_mode),S_ISREG(st.st_mode));
	}
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");

  return 0;
}

