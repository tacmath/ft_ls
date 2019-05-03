#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <stdio.h>

int ft_strlen(char *str)
{
	int n;
	
	while (str[++n])
		;
	return (n);	
}

int main(int ac, char **av)
{
	DIR *open;
	struct dirent *info;
	
	if (ac <= 1)
	{
		if (!(open = opendir(".")))
			return (-1);
	}
	else if (!(open = opendir(av[1])))
		return (-1);
	while ((info = readdir(open)))
	{
		printf("type = %d  name = %s\n", info->d_type, info->d_name);
	}
	closedir(open);
	return (0);
}
