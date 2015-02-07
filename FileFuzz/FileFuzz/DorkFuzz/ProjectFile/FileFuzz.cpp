// FileFuzz.cpp : a console application
//

#include "stdafx.h"
#include "DorkFuzz/DorkFuzz.h"

int main(int argc, char* argv[])
{
	if (Init())
	{
		switch (argc)
		{
		case 2:
			FuzzBegin(atoi(argv[1]));
			break;
		default:
			printf("param error!\n");
			printf("\n1: create fuzz file\n2: create fuzz process\n3: both 1 and 2\n");
			printf("eg: %s 1 (1 or 2 or 3)\n", argv[0]);
			break;
		}
	}

	return 0;
}

