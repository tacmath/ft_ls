#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>

void write_mode(int mode)
{
	static char (right[])[3] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};

	write(1, right[(mode >> 6) & 7], 3);
	write(1, right[(mode >> 3) & 7], 3);
	write(1, right[mode & 7], 3);
}


int main(int ac, char **av)
{
	struct stat buf;

	stat(av[1], &buf);
	printf("name = %s\nmodes = %d\nnb_link = %d\ntaille = %d\n", av[1], buf.st_mode, buf.st_nlink, buf.st_size);
	printf("\n");
	write_mode(buf.st_mode);
	printf("\n");
	return (0);
}
