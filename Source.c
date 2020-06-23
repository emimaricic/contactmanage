#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "Header.h"


int main(void) {

	char* fp = "clanovi.bin";
	unsigned int ContactNumber = 0;

	kreiranje(fp, &ContactNumber);

	mainMenu(fp, &ContactNumber);

	return 0;
}