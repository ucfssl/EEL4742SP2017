#include <stdio.h>
#include <inttypes.h>

/* ANSI VT100 Escape Codes */
#define BYELLOW		"\033[1;33m"
#define BGREEN		"\033[1;32m"
#define NORMAL		"\033[0m"

int main(void) {
	printf(BYELLOW "sizeof" NORMAL "(" BGREEN "long long" NORMAL ")\t= %d\n",
			sizeof(long long));
	printf(BYELLOW "sizeof" NORMAL "(" BGREEN "long" NORMAL ")\t\t= %d\n",
			sizeof(long));
	printf(BYELLOW "sizeof" NORMAL "(" BGREEN "int" NORMAL ")\t\t= %d\n",
			sizeof(int));
	printf(BYELLOW "sizeof" NORMAL "(" BGREEN "short" NORMAL ")\t\t= %d\n",
			sizeof(short));
	printf(BYELLOW "sizeof" NORMAL "(" BGREEN "char" NORMAL ")\t\t= %d\n",
			sizeof(char));

	return 0;
}

/* vim:set ts=4 tw=79 syntax=c et: */
