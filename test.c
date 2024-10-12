#include "include/sha256.h"
#include <string.h>

int main(int ac, char **av){
	char key[256];

	if (ac < 2)
		return (1);
	sha256(av[1], strlen(av[1]), key);
}
