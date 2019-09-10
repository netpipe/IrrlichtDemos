/**
 * Duo Tao
 * CSGError.c
 */
#include "CSGError.h"
#include <stdlib.h>

void heapAllocationFailure() {
	printf("Heap Allocation Failed\n");
	fflush(stdout);
	exit(1);
}

void externalError() {
	fprintf(stderr, "External Error\n");
	fflush(stdout);
	exit(2);
}

void triangleSplitError() {
	fprintf(stderr, "Triangle Split Error\n");
	fflush(stdout);
	exit(3);
}

void debug(char msg[]) {
	printf("### DEBUG:%s\n", msg);
}
