#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Header.h"
#include <string.h>


void kreiranje(char* datoteka, unsigned int* BrojKorisnika) {

	FILE* fp = fopen(datoteka, "rb");

	if (fp == NULL) {

		perror("File");

		fp = fopen(datoteka, "wb");

		if (fp == NULL) {

			perror("File");
			exit(EXIT_FAILURE);
		}
		else {

			fwrite(BrojKorisnika, sizeof(unsigned int), 1, fp);
			fclose(fp);
			printf("File created on first run!\n");
		}
	}
	else {

		fread(BrojKorisnika, sizeof(unsigned int), 1, fp);
		fclose(fp);
		printf("File exist\n");
	}
}

void startMenu() {
	printf("\n\n\\t==========Welcome to Contacts==========\n\n");
	printf("\n\t----------Choose an option below-----------\n");
	printf("\n\t< 1 >  Add contact:\n");
	printf("\n\t< 2 >  List all contacts:\n");
	printf("\n\t< 3 >  Search contact:\n");
	printf("\n\t< 4 >  Edit contact:\n");
	printf("\n\t< 5 >  Delete all contacts:\n");
	printf("\n\t< 6 >  Show sorted:\n");
	/*printf("\n\t< 7 >  Delete a contact:\n");*/
	printf("\n\t< 0 >  Quit:\n\n");
}

void dodajKontakt(char* datoteka, unsigned int* ContactNumber) {

	FILE* fp = NULL;
	
	fp = fopen(datoteka, "rb+");

	if (fp == NULL) {
		perror("Menu 1 - Add contacts");
		exit(EXIT_FAILURE);
	}
	//datoteka otvorena
	else {
		CONTACT privremeniKontakt = { 0 };
		printf("Name: \n");
		scanf(" %50[^\n]", privremeniKontakt.Name);
		printf("Organisation: \n");
		scanf(" %50[^\n]", privremeniKontakt.Organisation);
		printf("Email\n");
		scanf(" %30[^\n]", privremeniKontakt.Email);
		printf("Phone number without +3850\n");
		char privremeniBroj[10] = { '\0' };
		scanf("%9s", privremeniBroj);
		strcpy(privremeniKontakt.Mobile, "+385");
		strcat(privremeniKontakt.Mobile, privremeniBroj);
		privremeniKontakt.brojac = (*ContactNumber)++;

		fseek(fp, sizeof(unsigned int) + ((*ContactNumber - 1) * sizeof(CONTACT)), SEEK_SET);
		fwrite(&privremeniKontakt, sizeof(CONTACT), 1, fp);
		rewind(fp);
		fwrite(ContactNumber, sizeof(unsigned int), 1, fp);
		fclose(fp);
	}
	}


void fProcitajKontakte(char* datoteka, unsigned int* ContactNumber) {

	FILE* fpRead = NULL;
	fpRead = fopen(datoteka, "rb");

	if (fpRead == NULL) {

		perror("Menu 2 - Read file");
		exit(EXIT_FAILURE);
	}
	//datoteka otvorena
	else {

		CONTACT* sviKontakti = NULL;

		fread(ContactNumber, sizeof(unsigned int), 1, fpRead);

		if (*ContactNumber == 0) {

			printf("No entered contacts!\n");
			fclose(fpRead);
			return;
		}
		//ima kontakta
		else {
			sviKontakti = (CONTACT*)calloc(*ContactNumber, sizeof(CONTACT));

			if (sviKontakti == NULL) {

				perror("Reading all contacts :(");
				exit(EXIT_FAILURE);
			}
			//uspjesno zauzimanje memorije
			else {
				fread(sviKontakti, sizeof(CONTACT), *ContactNumber, fpRead);
				fclose(fpRead);

				unsigned int i;
				//prikaz kontakta
				for (i = 0; i < *ContactNumber; i++)
				{
					printf("%u\t", (sviKontakti + i)->brojac);
					printf("Name: %s ", (sviKontakti + i)->Name);
					printf("Organisation: %s ", (sviKontakti + i)->Organisation);
					printf("E-mail: %s ", (sviKontakti + i)->Email);
					printf("Mobile: %s\n", (sviKontakti + i)->Mobile);

				}
				free(sviKontakti);
			}
		}
	}
}

void traziKontakt(char* datoteka, unsigned int* ContactNumber) {
	FILE* fpSearch = NULL;
	fpSearch = fopen(datoteka, "rb");

	if (fpSearch == NULL) {

		perror("Menu 3 - Search contact");
		return;
		//exit(EXIT_FAILURE);
	}
	//datoteka otvorena
	else {
		CONTACT* sviKontakti = NULL;

		fread(ContactNumber, sizeof(unsigned int), 1, fpSearch);

		if (*ContactNumber == 0) {

			printf("No entered contacts!\n");
			fclose(fpSearch);
			return;
		}
		//ima kontakta
		else {
			sviKontakti = (CONTACT*)calloc(*ContactNumber, sizeof(CONTACT));

			if (sviKontakti == NULL) {

				perror("Reading all contacts");
				exit(EXIT_FAILURE);
			}
			//uspjesno zauzeto
			else {
				fread(sviKontakti, sizeof(CONTACT), *ContactNumber, fpSearch);
				fclose(fpSearch);
				unsigned int search;
				unsigned int i;
				printf("Search by name[1] or by number[2]?\n");
				scanf("%u", &search);

			//ime ili broj
			switch (search) {
				case 1:

					printf("Enter contact name\n");
					char privremenoIme[51] = { '\0' };
					scanf(" %50[^\n]", privremenoIme);
					unsigned int statusIme = 0;
					unsigned int indeksIme = -1;

					//usporedivanje stringa
					for (i = 0; i < *ContactNumber; i++)
					{		
						if (!strcmp((sviKontakti + i)->Name, privremenoIme)) {
							statusIme = 1;
							indeksIme = i;
						}
					}

					if (statusIme) 
					{
						printf("\nContact found!\n");
						
						printf("Name: %s\n", (sviKontakti + indeksIme)->Name);
						printf("Organisation: %s\n", (sviKontakti + indeksIme)->Organisation);
						printf("E-mail: %s\n", (sviKontakti + indeksIme)->Email);
						printf("Mobile: %s\n\n", (sviKontakti + indeksIme)->Mobile);
					}
					else 
					{
						printf("Non-existent contact\n");
					}
					break;

				case 2:

					printf("Enter contact number\n");
					char privremeniBroj[14] = { '\0' };
					scanf("%14s", privremeniBroj);
					unsigned int statusBroj = 0;
					unsigned int indeksBroj = -1;

					//usporedivanje stringa
					for (i = 0; i < *ContactNumber; i++)
					{

						if (!strcmp((sviKontakti + i)->Mobile, privremeniBroj)) {
							statusBroj = 1;
							indeksBroj = i;
						}
					}

					if (statusBroj) 
					{
						printf("\nContact found\n");
				
						printf("Name: %s\n", (sviKontakti + indeksBroj)->Name);
						printf("Organisation: %s\n", (sviKontakti + indeksBroj)->Organisation);
						printf("E-mail: %s\n", (sviKontakti + indeksBroj)->Email);
						printf("Mobile: %s\n\n", (sviKontakti + indeksBroj)->Mobile);
					}
					else 
					{
						printf("Non-existent contact\n");
					}
					break;

				default:
					printf("Wrong option, try again!\n");
				}	
			}
			free(sviKontakti);
		}
	}

}

void fIzlazakIzPrograma(void) {

	printf("Are you sure you want to exit?(Y/N)\n");
	char izbor[2] = { '\0' };
	scanf(" %s", izbor);
	if (!strcmp("Y", izbor)) {
		exit(EXIT_FAILURE);
	}
	return;
}


void editContact(char* datoteka, unsigned int* ContactNumber) {

	FILE* fpEdit = NULL;
	fpEdit = fopen(datoteka, "rb");

	if (fpEdit == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
		//exit(EXIT_FAILURE);
	}
	//datoteka otvorena
	else {
		CONTACT* sviKontakti = NULL;

		fread(ContactNumber, sizeof(unsigned int), 1, fpEdit);

		if (*ContactNumber == 0) {

			printf("No entered contacts!\n");
			fclose(fpEdit);
			return;
		}
		//ima kontakta
		else {
			sviKontakti = (CONTACT*)calloc(*ContactNumber, sizeof(CONTACT));

			if (sviKontakti == NULL) {
				perror("Citanje svih kontakta");
				exit(EXIT_FAILURE);
			}
			//uspjesno zauzimanje memorije
			else {
				CONTACT privremeniKontakt = { 0 };
				fread(sviKontakti, sizeof(CONTACT), *ContactNumber, fpEdit);
								
				unsigned int i;
				
					printf("Enter contact name\n");
					char privremenoIme[51] = { '\0' };
					scanf(" %50[^\n]", privremenoIme);
					unsigned int statusIme = 0;
					unsigned int indeksIme = -1;

                    //usporedivanje stringa
					for (i = 0; i < *ContactNumber; i++)
					{
						if (!strcmp((sviKontakti + i)->Name, privremenoIme)) {
							statusIme = 1;
							indeksIme = i;
						}
					}

					if (statusIme == 1) {
						printf("\nContact found\n");
						
						printf("Name: %s\n", (sviKontakti + indeksIme)->Name);
						printf("Organisation: %s\n", (sviKontakti + indeksIme)->Organisation);
						printf("E-mail: %s\n", (sviKontakti + indeksIme)->Email);
						printf("Mobile: %s\n\n", (sviKontakti + indeksIme)->Mobile);
						fclose(fpEdit);
						//kontakt pronaden ponovo otvaranje datoteke da se moze pisati u nju
						fpEdit = fopen(datoteka, "rb+");
						printf("New name: \n");
						scanf(" %50[^\n]", privremeniKontakt.Name);
						printf("New organisation: \n");
						scanf(" %50[^\n]", privremeniKontakt.Organisation);
						printf("New e-mail: \n");
						scanf(" %50[^\n]", privremeniKontakt.Email);
						printf("New phone number: \n");
						char privremeniBroj[10] = { '\0' };
						scanf("%9s", privremeniBroj);
						strcpy(privremeniKontakt.Mobile, "+385");
						strcat(privremeniKontakt.Mobile, privremeniBroj);
						privremeniKontakt.brojac = indeksIme;

						fseek(fpEdit, sizeof(unsigned int) + ((indeksIme) * sizeof(CONTACT)), SEEK_SET);
						fwrite(&privremeniKontakt, sizeof(CONTACT), 1, fpEdit);
						rewind(fpEdit);
						fwrite(ContactNumber, sizeof(unsigned int), 1, fpEdit);
						fclose(fpEdit);
					}
					else {
						printf("Non-existant contact\n");
					}
					free(sviKontakti);
				}
		}
		}
	fclose(fpEdit);
	}

void sortByName(char* datoteka, unsigned int* ContactNumber) {
	FILE* fpEdit = NULL;
	fpEdit = fopen(datoteka, "rb");

	if (fpEdit == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
		//exit(EXIT_FAILURE);
	}
	//datoteka otvorena
	else {
		CONTACT* sviKontakti = NULL;

		fread(ContactNumber, sizeof(unsigned int), 1, fpEdit);

		if (*ContactNumber == 0) {

			printf("No entered contacts!\n");
			fclose(fpEdit);
			return;
		}
		//ima kontakta
		else {
			sviKontakti = (CONTACT*)calloc(*ContactNumber, sizeof(CONTACT));
			CONTACT temp;
			char prvoSlovo, prvoSlovoPom;
			int min;
			if (sviKontakti == NULL) {
				perror("Citanje svih kontakta");
				exit(EXIT_FAILURE);
			}
			//uspjesno zauzimanje memorije
			else {
				fread(sviKontakti, sizeof(CONTACT), *ContactNumber, fpEdit);
			
				for (int i = 0; i < *ContactNumber - 1; i++) 
				{
					min = i;
					prvoSlovo = (sviKontakti + i)->Name[0] >= 'A' && (sviKontakti + i)->Name[0] <= 'Z' ? (sviKontakti + i)->Name[0] : (sviKontakti + i)->Name[0] - 32;
					for (int j = i + 1; j < *ContactNumber; j++) 
					{
						prvoSlovoPom = (sviKontakti + j)->Name[0] >= 'A' && (sviKontakti + j)->Name[0] <= 'Z' ? (sviKontakti + j)->Name[0] : (sviKontakti + j)->Name[0] - 32;
						if(prvoSlovo<prvoSlovoPom)
						{
							min = j;
							prvoSlovo = (sviKontakti + j)->Name[0] >= 'A' && (sviKontakti + j)->Name[0] <= 'Z' ? (sviKontakti + j)->Name[0] : (sviKontakti + j)->Name[0] - 32;
					    }
				    }
				    //zamjena imena
					strcpy(temp.Name, (sviKontakti + i)->Name);
					strcpy((sviKontakti + i)->Name, (sviKontakti + min)->Name);
					strcpy((sviKontakti + min)->Name, temp.Name);
					//zamjena organizacije
					strcpy(temp.Organisation, (sviKontakti + i)->Organisation);
					strcpy((sviKontakti + i)->Organisation, (sviKontakti + min)->Organisation);
					strcpy((sviKontakti + min)->Organisation, temp.Organisation);
					//zamjena mobitela
					strcpy(temp.Mobile, (sviKontakti + i)->Mobile);
					strcpy((sviKontakti + i)->Mobile, (sviKontakti + min)->Mobile);
					strcpy((sviKontakti + min)->Mobile, temp.Mobile);
					//zamjena emaila
					strcpy(temp.Email, (sviKontakti + i)->Email);
					strcpy((sviKontakti + i)->Email, (sviKontakti + min)->Email);
					strcpy((sviKontakti + min)->Email, temp.Email);
				}

				//ispis na ekran
				for (int i = 0; i < *ContactNumber; i++)
				{
					printf("%u\t", (sviKontakti + i)->brojac);
					printf("Name: %s ", (sviKontakti + i)->Name);
					printf("Organisation: %s ", (sviKontakti + i)->Organisation);
					printf("E-mail: %s ", (sviKontakti + i)->Email);
					printf("Mobile: %s\n", (sviKontakti + i)->Mobile);
				}
				free(sviKontakti);
			}
		}
	}
}

void sortByMobile(char* datoteka, unsigned int* ContactNumber) {
	FILE* fpEdit = NULL;
	fpEdit = fopen(datoteka, "rb");

	if (fpEdit == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
		//exit(EXIT_FAILURE);
	}
	//datoteka otvorena
	else {
		CONTACT* sviKontakti = NULL;

		fread(ContactNumber, sizeof(unsigned int), 1, fpEdit);

		if (*ContactNumber == 0) {

			printf("No entered contacts!\n");
			fclose(fpEdit);
			return;
		}
		//ima kontakta
		else {
			sviKontakti = (CONTACT*)calloc(*ContactNumber, sizeof(CONTACT));
			CONTACT temp;
			char prvoSlovo, prvoSlovoPom;
			int min;
			if (sviKontakti == NULL) {
				perror("Citanje svih kontakta");
				exit(EXIT_FAILURE);
			}
			//uspjesno zauzimanje memorije
			else {
				fread(sviKontakti, sizeof(CONTACT), *ContactNumber, fpEdit);

				for (int i = 0; i < *ContactNumber - 1; i++)
				{
					min = i;
					prvoSlovo = (sviKontakti + i)->Organisation[0] >= 'A' && (sviKontakti + i)->Organisation[0] <= 'Z' ? (sviKontakti + i)->Organisation[0] : (sviKontakti + i)->Organisation[0] - 32;
					for (int j = i + 1; j < *ContactNumber; j++)
					{
						prvoSlovoPom = (sviKontakti + j)->Organisation[0] >= 'A' && (sviKontakti + j)->Organisation[0] <= 'Z' ? (sviKontakti + j)->Organisation[0] : (sviKontakti + j)->Organisation[0] - 32;
						if (prvoSlovo < prvoSlovoPom)
						{
							min = j;
							prvoSlovo = (sviKontakti + j)->Organisation[0] >= 'A' && (sviKontakti + j)->Organisation[0] <= 'Z' ? (sviKontakti + j)->Organisation[0] : (sviKontakti + j)->Organisation[0] - 32;
						}
					}
					//zamjena imena
					strcpy(temp.Name, (sviKontakti + i)->Name);
					strcpy((sviKontakti + i)->Name, (sviKontakti + min)->Name);
					strcpy((sviKontakti + min)->Name, temp.Name);
					//zamjena organizacije
					strcpy(temp.Organisation, (sviKontakti + i)->Organisation);
					strcpy((sviKontakti + i)->Organisation, (sviKontakti + min)->Organisation);
					strcpy((sviKontakti + min)->Organisation, temp.Organisation);
					//zamjena mobitela
					strcpy(temp.Mobile, (sviKontakti + i)->Mobile);
					strcpy((sviKontakti + i)->Mobile, (sviKontakti + min)->Mobile);
					strcpy((sviKontakti + min)->Mobile, temp.Mobile);
					//zamjena emaila
					strcpy(temp.Email, (sviKontakti + i)->Email);
					strcpy((sviKontakti + i)->Email, (sviKontakti + min)->Email);
					strcpy((sviKontakti + min)->Email, temp.Email);
				}

				//ispis na ekran
				for (int i = 0; i < *ContactNumber; i++)
				{
					printf("%u\t", (sviKontakti + i)->brojac);
					printf("Name: %s ", (sviKontakti + i)->Name);
					printf("Organisation: %s ", (sviKontakti + i)->Organisation);
					printf("E-mail: %s ", (sviKontakti + i)->Email);
					printf("Mobile: %s\n", (sviKontakti + i)->Mobile);

				}
				free(sviKontakti);
			}
		}
	}
}



void mainMenu(char* datoteka, unsigned int* ContactNumber) {
	unsigned int izbor = -1;
	while (1) {
		startMenu();

		scanf("%u", &izbor);

		switch (izbor) {

		case 1:
			printf("\n\t\t***************Add contact*****************\n\n");
			dodajKontakt(datoteka, ContactNumber);
			break;

		case 2:
			printf("\n\t\t***************Read all contacts*****************\n\n");
			fProcitajKontakte(datoteka, ContactNumber);
			break;

		case 3:
			printf("\n\t\t***************Search contact*****************\n\n");
			traziKontakt(datoteka, ContactNumber);
			break;

		case 4:
			printf("\n\t\t****************Edit contact*****************\n\n");
			editContact(datoteka, ContactNumber);
			break;

		case 5:
			printf("\n\t\t***************Delete all contacts*****************\n\n");
			printf("Are you sure you want to delete all contacts?(Y/N)\n");
			char izbor[2] = { '\0' };
			scanf(" %s", izbor);
			if (!strcmp("Y", izbor)) {
				remove(datoteka);
				printf("All contacts successfully deleted, making new file");
				kreiranje(datoteka, ContactNumber);
			}
			break;

		case 6:
			printf("\n\t\t*****************Sort by name*****************\n\n");
			int choice;
			printf("Show sorted by name[1] | Show sorted by organisation[2]?\n");
			scanf("%d", &choice);
			switch (choice) {
			case 1:
				sortByName(datoteka, ContactNumber);
				break;
			case 2:
				sortByMobile(datoteka, ContactNumber);
				break;
			default:
				printf("Wrong option, try again!\n");
			}
			break;
		
		case 0:
			printf("\n\t\t********************Exit*********************\n\n");
			fIzlazakIzPrograma();
			break;

		default:
			printf("Wrong option, try again!\n");
		}
	}
}
