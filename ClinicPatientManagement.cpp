#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXSTRING 250
#define MAX_PATIENT 500
struct Patient{
	int id;
	int birth;
	char name[51];
	char symptom[111];
	char doctor[51];
	char date[15];
	float fee;
};
typedef struct Patient Patient;
Patient hospitalData[MAX_PATIENT];
int patientCount = 0;

int importFromFile(const char *fileName){
    FILE *f = fopen(fileName, "r");
    if (f == NULL){ 
    return 1;
}
Patient p;
int count = 0;
while (fscanf(f, "%d|%[^|]|%d|%[^|]|%[^|]|%f|%s", &p.id, p.name, &p.birth, p.symptom, p.doctor, &p.fee, p.date)==7){
        if (patientCount<MAX_PATIENT){
            hospitalData[patientCount]=p;
            patientCount++;
            count++;
        }
        else{
            break;
        }
    }
    fclose(f);
    return 0;
}

int exportToFile(const char *fileName){
    FILE *f = fopen(fileName, "w");
    if (f == NULL){ 
        return 1;
    }
    for (int i = 0; i < patientCount; i++){
        fprintf(f, "%d|%s|%d|%s|%s|%.2f|%s\n",hospitalData[i].id, hospitalData[i].name, hospitalData[i].birth, hospitalData[i].symptom, hospitalData[i].doctor, hospitalData[i].fee, hospitalData[i].date);
    }
    fclose(f);
    return 0;
}

int createNewPatient(Patient p){
    if (patientCount>=MAX_PATIENT){
    return 1;
    }
    hospitalData[patientCount]=p;
    patientCount++;
    return 0;
}
void printHeader(){
    printf("ID|Ten|Nam sinh|Trieu chung|Bac si|Chi phi|Ngay\n");
}
int readItemByID(int id){
    for (int i = 0; i < patientCount; i++){
        if (hospitalData[i].id == id){
            printHeader();
            printf("%d|%s|%d|%s|%s|%.2f|%s\n", hospitalData[i].id, hospitalData[i].name, hospitalData[i].birth, hospitalData[i].symptom, hospitalData[i].doctor, hospitalData[i].fee, hospitalData[i].date);
            return 0;
        }
    }
    return 1;
}
int readDataBase(){
    if (patientCount == 0){
        return 1;
    }
    printHeader();
    for (int i = 0; i < patientCount; i++){
        printf("%d|%s|%d|%s|%s|%.2f|%s\n",hospitalData[i].id, hospitalData[i].name, hospitalData[i].birth, hospitalData[i].symptom, hospitalData[i].doctor, hospitalData[i].fee, hospitalData[i].date);
    }
    return 0;
}
int updatePatientInfo(int id, int field, char *strVal, float numVal){
    for (int i = 0; i < patientCount; i++){
        if (hospitalData[i].id == id){
            switch (field){
                case 1:
                strcpy(hospitalData[i].name, strVal);
                break;
                case 2:
                hospitalData[i].birth=(int)numVal;
                break;
                case 3:
                strcpy(hospitalData[i].symptom, strVal);
                break;
                case 4:
                strcpy(hospitalData[i].doctor, strVal);
                break;
                case 5:
                hospitalData[i].fee=numVal;
                break;
                case 6:
                strcpy(hospitalData[i].date, strVal);
                break;
            }
            return 0;
        }
    }
    return 1;
}
int deletePatient(int id){
    for (int i = 0; i < patientCount; i++){
        if (hospitalData[i].id == id){
            for (int j = i; j < patientCount - 1; j++){
                hospitalData[j] = hospitalData[j+1];
            }
                patientCount--;
                return 0;
            }
        }
        return 1;
}

int main(){
    Patient p;
    int id, field;
    char strVal[250];
    float numVal;
    const char *fileName = "danhsach.txt";

    printf("=== Dang tai du lieu tu file... ===\n");
    importFromFile("Danhsach.txt");
    printf("=== Danh sach hien tai ===\n");
    readDataBase();
    createNewPatient(p);
    return 0;
}
