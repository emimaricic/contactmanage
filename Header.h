#ifndef MYHEADER
#define MYHEADER

typedef struct contact {
	unsigned int brojac;
	char Name[51];
	char Organisation[51];
	char Mobile[14];
	char Email[31];
	struct contact* nextNode;
}CONTACT;

void kreiranje(char* datoteka, unsigned int* pBrojKorisnika);
void mainMenu(char* datoteka, unsigned int* BrojKorisnika);

#endif // !MYHEADER

