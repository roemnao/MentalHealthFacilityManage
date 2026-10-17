#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSTRING 250
#define MAX_PATIENT 500
typedef struct Patient{
	int id;
	int birth;
	char name[51];
	char symptom[111];
	char doctor[51];
	char date[15];
	float fee;
} Patient;

typedef struct TempPatient{
	char *id;
	char *birth;
	char *name;
	char *symptom;
	char *doctor;
	char *date;
	char *fee;
}TempPatient;
Patient hospitalData[MAX_PATIENT];
int patientCount = 0;
