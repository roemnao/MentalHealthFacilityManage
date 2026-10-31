#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define MAXSTRING 10001
#define MAX_PATIENT 10000
char error[][2][MAXSTRING] = {{"100","File khong ton tai"},{"101","File khong hop le"},{"102","So benh nhan vuot gioi han luu tru"},
                            {"200","Du lieu nhap vao bi trong"},{"201","Du lieu nhap vao khong hop le"},{"202","Danh sach benh nhan trong"},
                            {"301","Ngay sinh sau ngay kham"},
                            {"404","Khong tim thay benh nhan trong danh sach"}};
int errorCout = sizeof(error) / sizeof(error[0]);
typedef struct Patient {
    char id[MAXSTRING];
    char birth[MAXSTRING];
    char name[MAXSTRING];
    char symptom[MAXSTRING];
    char doctor[MAXSTRING];
    char date[MAXSTRING];
    char fee[MAXSTRING];
}Patient;

typedef struct Date {
    int day;
    int month;
    int year;
}Date;
Patient hospitalData[MAX_PATIENT];
int patientCount = 0;

void printError(int errorID);
void printHeader();
Date splitDate(char *date);
void normalizeDate(char *date);
int isduplicateid(char*id){
	for (int i = 0; i < patientCount;i++){
		if(strcmp(hospitalData[i].id, id) == 0){
			return 1;
		}
	}
	return 0;
}

int validateID(char*id){
	int len = strlen(id);
	if (len <8 || len > 12){
		return 0;
	}
	for (int i = 0; i < len; i++){
		if (!isdigit((unsigned char)id[i])){
			return 0;
		}
	}
	return 1;
}
int validateName(char*name){
	int len = strlen(name);
	if (len == 0 || len >50){
		return 0;
	}
	for (int i = 0 ; i< len;i++){
		if (!isalpha((unsigned char)name[i]) && name[i]!= ' '){
			return 0;
		}
	}
	return 1;
}
int cmpDate(Date bigger, Date smaller){
    if (bigger.year > smaller.year) {
        return 1;
    }
    else if (bigger.year < smaller.year) {
        return -1;
    }
    if (bigger.month > smaller.month) {
        return 1;
    }
    else if (bigger.month < smaller.month) {
        return -1;
    }
    if (bigger.day > smaller.day) {
        return 1;
    }
    else if (bigger.day < smaller.day) {
        return -1;
    }
    return 0;
}
int validateDate(char *date){
    int len = strlen(date);
    int slashes = 0;
    for (int i = 0; i < len; i++){
        if (date[i] == '/') slashes++;
        else if (!isdigit((unsigned char)date[i])) return 0;
    }
    if (len < 8 || len > 10 || slashes != 2) return 0;
    int d, m, y, n = 0;
    if (sscanf(date, "%d/%d/%d%n", &d, &m, &y, &n) != 3 || date[n] != '\0') return 0;
    if (strlen(strrchr(date, '/') + 1) != 4) return 0;
    if (y < 1900 || m < 1 || m > 12 || d < 1 || d > 31){
        return 0;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11){
        if (d > 30) return 0;
    }
    if (m == 2) {
        int isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        if (isLeap && d > 29) return 0;
        if (!isLeap && d > 28) return 0;
    }
    return 1;
}

int validateFee(char *fee){
    for(size_t i = 0; i < strlen(fee); i++){
        if(!isdigit((unsigned char)fee[i])){
            return 0;
        }
    }   
    return 1; 
}
int validateSymptom(char *symptom){
    for(size_t i = 0; i < strlen(symptom); i++){
        if(!isdigit((unsigned char)symptom[i]) && !isalpha((unsigned char)symptom[i]) && !(symptom[i] == ' ')){
            return 0;
        }
    }
    return 1;
}
int validateChoice(int firstChoice, int lastChoice, char *userChoice){
    if(strlen(userChoice) != 1) return 0;
    else if(!isdigit((unsigned char)userChoice[0])) return 0;
    else if(atoi(userChoice) < firstChoice || atoi(userChoice) > lastChoice) return 0;
    return 1;
}
int isPatientExist(char value[], char valueType){
    for(int i = 0; i < patientCount; i++){
        if(valueType == 'n'){
            if(strcmp(hospitalData[i].name,value) == 0) return 1;
        }
        else if(valueType == 'i'){
            if(strcmp(hospitalData[i].id,value) == 0) return 1;
        }
    }
    return 0;
}
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void printHeader() {
    printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n", "ID","TEN","NAM SINH","TRIEU CHUNG","BAC SI","CHI PHI","NGAY");
}
int importFromFile(const char *fileName) {
    char path[MAXSTRING];
    snprintf(path, sizeof(path), "%s.txt", fileName);
    FILE *f = fopen(path, "r");
    if (!f) return 100;

    Patient p;
    patientCount = 0;
    while (1) {
        int n = fscanf(f, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|\n]",
                        p.id, p.name, p.birth, p.symptom, p.doctor, p.fee, p.date);
        if (n == EOF) break;
        if (n != 7
            || !validateID(p.id) || isduplicateid(p.id) || !validateName(p.name) || !validateDate(p.birth) || !validateSymptom(p.symptom)
            || !validateName(p.doctor) || !validateFee(p.fee) || !validateDate(p.date) || cmpDate(splitDate(p.date), (splitDate(p.birth))) == -1){
            fclose(f);
            patientCount = 0;
            return 101;
        }
        if (patientCount + 1 > MAX_PATIENT){
            fclose(f);
            patientCount = 0;
            return 102;
        }
        normalizeDate(p.birth);
        normalizeDate(p.date);
        hospitalData[patientCount++] = p;
    }
    fclose(f);
    return 0;
}

int exportToFile(const char *fileName) {
    char path[MAXSTRING];
    snprintf(path, sizeof(path), "%s.txt", fileName);
    FILE *f = fopen(path, "w");
    if (!f) return 100;
    for (int i = 0; i < patientCount; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%s|%s\n",
                hospitalData[i].id, hospitalData[i].name,
                hospitalData[i].birth, hospitalData[i].symptom,
                hospitalData[i].doctor, hospitalData[i].fee,
                hospitalData[i].date);
    }
    fclose(f);
    return 0;
}
void printError(int errorID){
    for(int i = 0; i < errorCout; i++){
        if(errorID == atoi(error[i][0])) printf("%s.\n", error[i][1]);
    }
}
void promptTillValidChoice(int firstChoice, int lastChoice, int *userChoice){
    char butter[MAXSTRING];
    while(1){
    	fgets(butter, sizeof(butter), stdin);
        butter[strcspn(butter, "\n")] = '\0';
        if(strcmp(butter,"") == 0){
            printError(200);
            printf("Xin hay nhap lai: ");
        } 
        else if(!validateChoice(firstChoice, lastChoice, butter)){
            printError(201);
            printf("Xin hay nhap lai: ");
        }
        else if(patientCount == 0 && (atoi(butter) == 2 || atoi(butter) == 3 || atoi(butter) == 5 || atoi(butter) == 6 || atoi(butter) == 9)){
            printError(202);
            printf("Xin hay nhap lai: ");
        }
        else{
            *userChoice = atoi(butter);
            break;
        }
    }
}

void normalizeDate(char *date){
    int d, m, y;
    if(sscanf(date, "%d/%d/%d", &d, &m, &y) == 3){
        snprintf(date, MAXSTRING, "%d/%d/%d", d, m, y);
    }
}
void promptTillValidValue(char valueType, char *value){
    char butter[MAXSTRING];
    while(1){
    	fgets(butter, sizeof(butter), stdin);
        butter[strcspn(butter, "\n")] = '\0';
        if(valueType == 'd' && validateDate(butter)) normalizeDate(butter);
        if(strcmp(butter,"") == 0){
            printError(200);
            printf("Xin hay nhap lai: ");
        } 
        else if(!validateID(butter) && valueType == 'i'){
            printError(201);
            printf("Xin hay nhap lai: ");
        }
        else if(!validateName(butter) && valueType == 'n'){
            printError(201);
            printf("Xin hay nhap lai: ");
        }
        else if(!validateSymptom(butter) && valueType == 'y'){
            printError(201);
            printf("Xin hay nhap lai: ");
        }
        else if(!validateFee(butter) && valueType == 'f'){
            printError(201);
            printf("Xin hay nhap lai: ");
        }
        else if(!validateDate(butter) && valueType == 'd'){
            printError(201);
            printf("Ngay phai dung dinh dang d/m/yyyy.\n");
            printf("Xin hay nhap lai: ");
        }
        else{
            snprintf(value, MAXSTRING, "%s", butter);
            break;
        }
    }
}
void addNewPatient(Patient p) {
    hospitalData[patientCount++] = p;
}
Patient createPatient(){
    char valueType[8] = "idnyndf";
    const char *label[] = {"ID","ngay sinh (d/m/yyyy)","ten","trieu chung","bac si phu trach","ngay kham (d/m/yyyy)","vien phi"};
    Patient tmp;
    char *value[] = {tmp.id,tmp.birth,tmp.name,tmp.symptom,tmp.doctor,tmp.date,tmp.fee};
    for(int i = 0; i<7; i++){
        printf("Nhap %s: ", label[i]);
        promptTillValidValue(valueType[i],value[i]);
        while(i == 0 && isduplicateid(tmp.id)){
            printError(201);
            printf("ID da ton tai. Xin hay nhap lai: ");
            promptTillValidValue('i',tmp.id);
        }
        while(i == 5 && cmpDate(splitDate(tmp.date),splitDate(tmp.birth)) == -1){
            printError(301);
            printf("Ngay kham khong duoc truoc ngay sinh (%s). Vui long nhap lai ngay kham: ", tmp.birth);
            promptTillValidValue('d',tmp.date);
        }
    }
    return tmp;
}
void readItemByID(char *id) {
    printHeader();
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(hospitalData[i].id, id) == 0) {
            printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n",
                hospitalData[i].id, hospitalData[i].name,
                hospitalData[i].birth, hospitalData[i].symptom,
                hospitalData[i].doctor, hospitalData[i].fee,
                hospitalData[i].date);
        }
    }
}

void readItemByName(char *name) {
    printHeader();
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(hospitalData[i].name, name) == 0) { 
            printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n",
                hospitalData[i].id, hospitalData[i].name,
                hospitalData[i].birth, hospitalData[i].symptom,
                hospitalData[i].doctor, hospitalData[i].fee,
                hospitalData[i].date);
        }
    }
}
int convertIDToIndex(char *id){
    for(int i = 0; i < patientCount; i++){
        if(strcmp(id,hospitalData[i].id) == 0) return i;
    }
    return -1;
}
void readDataBase() {
    printHeader();
    for (int i = 0; i < patientCount; i++) {
        printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n",
               hospitalData[i].id, hospitalData[i].name,
               hospitalData[i].birth, hospitalData[i].symptom,
               hospitalData[i].doctor, hospitalData[i].fee,
               hospitalData[i].date);
    }
}

void updatePatientInfo(char *id, int typeChange, char *newVal) {
    int index = convertIDToIndex(id);
    char *value[] = {hospitalData[index].id,hospitalData[index].birth,hospitalData[index].name,
                    hospitalData[index].symptom,hospitalData[index].doctor,hospitalData[index].date,
                    hospitalData[index].fee};
    snprintf(value[typeChange - 1], MAXSTRING, "%s", newVal);
}

void deletePatient(char *id) {
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(hospitalData[i].id, id) == 0) {
            for (int j = i; j < patientCount - 1; j++)
                hospitalData[j] = hospitalData[j + 1];
            patientCount--;
        }
    }
}



Date splitDate(char *date){
    Date tmp;
    sscanf(date, "%d/%d/%d", &tmp.day, &tmp.month, &tmp.year);
    return tmp;
}
int main() {
    int choice;
    Patient tmp;
    int loadErr = importFromFile("patientList");
    if(loadErr != 0) printError(loadErr);
    while (1) {
        printf("\n===== QUAN LY BENH NHAN =====\n");
        printf("1. Doc file\n");
        printf("2. Xem danh sach\n");
        printf("3. Tim kiem benh nhan\n");
        printf("4. Them benh nhan\n");
        printf("5. Cap nhat thong tin\n");
        printf("6. Xoa benh nhan\n");
        printf("7. Luu ra file\n");
        printf("8. Tong so benh nhan\n");
        printf("9. Tong chi phi benh vien\n");
        printf("0. Thoat\n");
        printf("=============================\n");
        printf("Nhap lua chon: ");
        promptTillValidChoice(0, 9, &choice);

        switch (choice) {
            case 1:{
                int err = importFromFile("patientList");
                if(err != 0) printError(err);
                else printf("Doc file thanh cong!\n");
                break;
            }
            case 2:
                readDataBase();
                break;
            case 3:{
                printf("1. Tim theo ID\n");
                printf("2. Tim theo TEN\n");
                printf("Nhap lua chon: ");
                promptTillValidChoice(1, 2, &choice);
                switch(choice){
                    case 1:{
                        printf("Nhap id benh nhan: ");
                        promptTillValidValue('i',tmp.id);
                        while (isPatientExist(tmp.id,'i') == 0){
                            printError(404);
                            printf("Vui long nhap lai: ");
                            promptTillValidValue('i',tmp.id);
                        }

                        readItemByID(tmp.id);
                        break;
                    }
                    case 2:{
                        printf("Nhap ten benh nhan: ");
                        promptTillValidValue('n',tmp.name);
                        while (isPatientExist(tmp.name,'n') == 0){
                            printError(404);
                            printf("Vui long nhap lai: ");
                            promptTillValidValue('n',tmp.name);
                        }

                        readItemByName(tmp.name);
                        break;
                    }
                }
                break;
            }
            case 4:{
                if(patientCount >= MAX_PATIENT){
                    printError(102);
                    break;
                }
                addNewPatient(createPatient());
                printf("Da them benh nhan vao danh sach.\n");
                exportToFile("patientList");
                break;
            }
            case 5:{
                char valueType[8] = "idnyndf";
                char newValue[MAXSTRING];

                printf("Nhap id benh nhan: ");
                promptTillValidValue('i',tmp.id);
                while(!isPatientExist(tmp.id,'i')){
                    printError(404);
                    printf("Vui long nhap lai: ");
                    promptTillValidValue('i',tmp.id);
                }

                printf("1.id    2.ngay sinh    3.ten    4.trieu chung\n"
                        "5.bac si phu trach     6.ngay kham    7.vien phi\n"
                        "Nhap loai gia tri muon thay doi: ");
                promptTillValidChoice(1,7,&choice);

                printf("Nhap gia tri moi: ");
                promptTillValidValue(valueType[choice - 1], newValue);
                while(choice == 1 && isduplicateid(newValue)){
                    printError(201);
                    printf("ID da ton tai. Vui long nhap lai: ");
                    promptTillValidValue('i',newValue);
                }
                while(choice == 2 && cmpDate(splitDate(hospitalData[convertIDToIndex(tmp.id)].date),splitDate(newValue)) == -1){
                    printError(301);
                    printf("Vui long nhap lai: ");
                    promptTillValidValue(valueType[choice-1],newValue);
                }
                while(choice == 6 && cmpDate(splitDate(hospitalData[convertIDToIndex(tmp.id)].birth),splitDate(newValue)) == 1){
                    printError(301);
                    printf("Vui long nhap lai: ");
                    promptTillValidValue(valueType[choice-1],newValue);
                }
                updatePatientInfo(tmp.id, choice, newValue);
                printf("Cap nhat file thanh cong\n");
                exportToFile("patientList");
                break;
            }
            case 6:{
                printf("Nhap id benh nhan: ");
                promptTillValidValue('i',tmp.id);
                while(!isPatientExist(tmp.id,'i')){
                    printError(404);
                    printf("Vui long nhap lai: ");
                    promptTillValidValue('i',tmp.id);
                }

                deletePatient(tmp.id);
                printf("Da xoa benh nhan khoi danh sach.\n");
                exportToFile("patientList");
                break;
            }
            case 7:{
                char butter[MAXSTRING];
                printf("Nhap ten file (.txt): ");
                promptTillValidValue('s', butter);
                int err = exportToFile(butter);
                if(err != 0) printError(err);
                else printf("Xuat file thanh cong!\n");
                break;
            }
            case 8:
                printf("Tong so benh nhan hien co: %d.\n", patientCount);
                break;

            case 9: {
                long long sum = 0;
                for (long long i = 0;i<patientCount;i++){
                    sum += atoll(hospitalData[i].fee);
                }
                printf("Tong vien phi cua tat ca benh nhan: %lld.\n", sum);
                break;
            }
            case 0:
                printf("Tam biet!\n");
                return 0;
        }
        printf("Nhan enter de tiep tuc...");
        fgets(tmp.name,sizeof(tmp.name),stdin);
        clearScreen();

    }
}
