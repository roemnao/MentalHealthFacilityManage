#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#define MAXSTRING 10000
#define MAX_PATIENT 10000

struct Patient {
    char id[20];
    char birth[20];
    char name[51];
    char symptom[111];
    char doctor[51];
    char date[20];
    char fee[20];
};
typedef struct Patient Patient;

Patient hospitalData[MAX_PATIENT];
int patientCount = 0;

//Khoi tao truoc
void printHeader();
//validation

int isduplicateid(const char *id){
	for (int i = 0; i < patientCount;i++){
		if(strcmp(hospitalData[i].id, id) == 0){
			return 1;
		}
	}
	return 0;
}
int validateid(const char* id){
	int len = strlen(id);
	if (len <8 || len>12){
		return 0;
	}
	for (int i = 0; i < len; i++){
		if (!isdigit(id[i])){
			return 0;
		}
	}
	if (isduplicateid(id)){
		return 0;
	}
	return 1;
}
int validatename(char*name){
	int len = strlen(name);
	if (len == 0 || len >50){
		return 0;
	}
	for (int i = 0 ; i< len;i++){
		if (!isalpha(name[i]) && name[i]!= ' '){
			return 0;
		}
	}
	return 1;
}
int validatebirth(char *birth){
	int len = strlen(birth);
	if(len==0){
		return 0;
	}
	for(int i = 0 ; i < len ;i++){
		if(!isdigit(birth[i])){
			return 0;
		}
	}
	int year = atoi(birth);
	time_t t = time(NULL);
	struct tm * current_time = localtime(&t);
	int current_year = current_time -> tm_year +1900;
	if (year < 1900 || year > current_year) {
		return 0;
	}
	return 1;
}
int validateDate(char*date){
	
    char tmpDate[3][10000];
    if(sscanf(date,"%s/%s/%s",tmpDate[0], tmpDate[2], tmpDate[3]) != 3) return 0;
    int d = atoi(tmpDate[0]);
    int m = atoi(tmpDate[1]);
    int y = atoi(tmpDate[2]);

    if (y<1900 || m<1 ||m>12||d<1||d>31){
        return 0;
    }
    if(m==4||m==6||m==9||m==11){
        if (d>30){
            return 0;
        }
    }
        if (m == 2) {
    int isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (isLeap && d > 29) {
        return 0;
    }
    if (!isLeap && d > 28) {
        return 0;
    }
    }time_t t = time(NULL);
    struct tm *current_time = localtime(&t);
    int current_d = current_time->tm_mday;
    int current_m = current_time->tm_mon + 1;
    int current_y = current_time->tm_year + 1900;
    if (y > current_y) {
        return 0;
    }
    if (y < current_y) {
        return 1;
    }
    if (m > current_m) {
        return 0;
    }
    if (m < current_m) {
        return 1;
    }
    if (d > current_d) {
        return 0;
    }
    return 1;
}
int validateDoctor(char *doctor){
    int len = strlen(doctor);
    if (len == 0 || len > 50){
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (!isalpha(doctor[i]) && doctor[i] != ' '){
            return 0;
        }
    }
    int inWord = 0;
    int wordCount = 0;
    for (int i = 0; i < len; i++){
        if (doctor[i] != ' '){
            if (!inWord) {
                inWord = 1;
                wordCount++;
            }
        } else {
            inWord = 0;
        }
    }
    if (wordCount == 0) {
        return 0;
    }
    return 1;
}

int validateFee(const char *fee) {
	if (fee[0] == '\0') {
		return 0;
	}
	for (int i = 0; fee[i] != '\0'; i++) {
		if (!isdigit((unsigned char)fee[i])) {
			return 0;
		}
	}
	errno = 0;
	char *end;
	long value = strtol(fee, &end, 10);
	return errno != ERANGE && *end == '\0' && value <= INT_MAX && atol(fee) > 0;
}
//crud
int importFromFile(char *fileName) {

    char tmp[10000];

    strcpy(tmp, fileName);

    sprintf(tmp,"%s.txt",tmp);
    printf("%s", tmp);
    

    FILE *f = fopen("tmp", "r");

    if (!f){
     return 1;
     }

    Patient p;

    patientCount = 0;

    while (fscanf(f, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|\n]",
                  p.id, p.name, p.birth, p.symptom, 
                  p.doctor, p.fee, p.date) == 7) {
        if (patientCount < MAX_PATIENT){
            hospitalData[patientCount] = p;
            patientCount++;
        }

        else{
            break;
        }

    }

    fclose(f);

    return 0;

}

int exportToFile(char *fileName) {
    char tmp[10000];
    strcpy(tmp, fileName);
    sprintf(fileName,"%s.txt",tmp);
    FILE *f = fopen(fileName, "w");
    if (!f) return 1;
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

int createNewPatient(Patient p) {
    if (patientCount >= MAX_PATIENT){
        return 1;
    }
    hospitalData[patientCount++] = p;
    return 0;
}



int readItemByID(const char *id) {
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(hospitalData[i].id, id) == 0) {
            printHeader();
            printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n",
                hospitalData[i].id, hospitalData[i].name,
                hospitalData[i].birth, hospitalData[i].symptom,
                hospitalData[i].doctor, hospitalData[i].fee,
                hospitalData[i].date);
            return 0;
        }
    }
    return 1;
}

int readItemByName(const char *name) {
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(hospitalData[i].name, name) == 0) {
            printHeader();
            printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n",
                hospitalData[i].id, hospitalData[i].name,
                hospitalData[i].birth, hospitalData[i].symptom,
                hospitalData[i].doctor, hospitalData[i].fee,
                hospitalData[i].date);
            return 0;
        }
    }
    return 1;
}

int readDataBase() {
    printHeader();
    for (int i = 0; i < patientCount; i++) {
        printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n",
               hospitalData[i].id, hospitalData[i].name,
               hospitalData[i].birth, hospitalData[i].symptom,
               hospitalData[i].doctor, hospitalData[i].fee,
               hospitalData[i].date);
    }
    return 0;
}

int updatePatientInfo(const char *id, char *typeChange, char *newVal) {
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(hospitalData[i].id, id) == 0) {
            char *patientValues[] = {hospitalData[i].id, hospitalData[i].name, hospitalData[i].birth, 
                            hospitalData[i].symptom, hospitalData[i].doctor, hospitalData[i].date, 
                            hospitalData[i].fee};
            char changes[] = "inbsdtf";
            for(int j = 0; j < 7; j++){
                if(tolower(typeChange[0]) == changes[j]){
                    strcpy(patientValues[j], newVal);
                    return 0;
                }
            }
        }
    }
    return 1;
}

int deletePatient(const char *id) {
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(hospitalData[i].id, id) == 0) {
            for (int j = i; j < patientCount - 1; j++)
                hospitalData[j] = hospitalData[j + 1];
            patientCount--;
            return 0;
        }
    }
    return 1;
}

void promptIDTillValid(char *id);
void promptNameTillValid(char *name);
void promptBirthTillValid(char *birth);
void promptSymptomTillValid(char *symptom);
void promptDoctorTillValid(char *doctor);
void promptDateTillValid(char *date);
void promptFeeTillValid(char *fee);

void inputOnePatient(Patient *p) {
    promptIDTillValid(p->id);
    promptNameTillValid(p->name);
    promptBirthTillValid(p->birth);
    promptSymptomTillValid(p->symptom);
    promptDoctorTillValid(p->doctor);
    promptFeeTillValid(p->fee);
    promptDateTillValid(p->date);
}

//cli
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
void promptIDTillValid(char *id){
    while (1){
        printf("Nhap ID (8-12 chu so): ");
        if(scanf("%s", id) != 1){
            continue;
        }
        else if(validateid(id) == 0){
            printf("ID khong hop le hoac bi trung! Vui long nhap lai.\n");
            continue;
        }
        break;
    }
}
void promptNameTillValid(char *name){
    while (1){
        printf("Nhap ho ten: ");
        if(scanf(" %[^\n]", name) != 1){
            continue;
        }
        else if(validatename(name) == 0){
            printf("Ten khong hop le! Vui long nhap lai.\n");
            continue;
        }
        break;
    }
}

void promptBirthTillValid(char *birth){
    while (1){
        printf("Nhap nam sinh: ");
        if(scanf("%s", birth) != 1){
            continue;
        }
        if(validatebirth(birth) == 0){
            printf("Nam sinh khong hop le! Vui long nhap lai.\n");
            continue;
        }
        break;
    }
}

void promptSymptomTillValid(char *symptom){
    while (1){
        printf("Nhap trieu chung: ");
        if(scanf(" %[^\n]", symptom) != 1){
            continue;
        }
        else if(validatename(symptom) == 0){
            printf("Ten trieu chung khong hop le! Vui long nhap lai.\n");
            continue;
        }
        break;
    }
}

void promptDoctorTillValid(char *doctor){
    while (1){
        printf("Nhap bac si: ");
        if(scanf(" %[^\n]", doctor) != 1){
            continue;
        }
        if(validateDoctor(doctor) == 0){
            printf("Ten bac si khong hop le! Vui long nhap lai.\n");
            continue;
        }
        break;
    }
}

void promptDateTillValid(char *date){
    while (1){
        printf("Nhap ngay kham (dd/mm/yyyy): ");
        if(scanf("%", date) != 1){
            continue;
        }
        if(validateDate(date) == 0){
            printf("Ngay kham khong hop le! Vui long nhap lai.\n");
            continue;
        }
        break;
    }
}

void promptFeeTillValid(char *fee){
    while (1){
        printf("Nhap vien phi: ");
        if(scanf("%s", fee) != 1){
            continue;
        }
        if(validateFee(fee) == 0){
            printf("Vien phi khong hop le! Vui long nhap lai.\n");
            continue;
        }
        break;
    }
}
    
    
int main(){
	int choice, choose;
    char id[20], typeChange[2], newVal[250], name[51];
    long long sum;
    Patient tmp;
    char fileName[260] = "patientList";
    importFromFile(fileName);
    char menuBuffer[10000];

    while(1) {
        printf("\n===== QUAN LY BENH NHAN =====\n");
        printf("1. Nhap danh sach tu file\n");
        printf("2. Xem danh sach benh nhan\n");
        printf("3. Tim kiem\n");
        printf("4. Them benh nhan\n");
        printf("5. Cap nhat thong tin\n");
        printf("6. Xoa benh nhan\n");
        printf("7. Xuat danh sach ra file\n");
        printf("8. Tong so benh nhan\n");
        printf("9. Tong chi phi benh vien\n");
        printf("0. Thoat\n");
        printf("=============================\n");
        while (1){
            printf("nhap lua chon (0-9): ");
            if(scanf("%d", &choice) != 1 || choice > 9 || choice < 0){
                printf("Lua chon khong hop le");
                continue;
            }
            break;
        }

        if (patientCount == 0 &&
            (choice == 2 || choice == 3 || choice == 5 || choice == 6)) {
            printf("Danh sach trong!\n");
        } 
        else {
            switch (choice) {
                case 0:
                    printf("Tam biet!\n");
                    return 0;

                case 1:
                    printf("Nhap file (.txt): ");
                    scanf("%s", fileName);
                    if (importFromFile(fileName) == 0){
                        printf("Doc file thanh cong! So benh nhan: %d\n", patientCount);}
                    else{
                        printf("Khong mo duoc file!\n");}
                    break;
            
                case 2:
                    readDataBase();
                    break;

                case 3:
                    printf("1. Tim theo ID\n");
                    printf("2. Tim theo TEN\n");
                    printf("Nhap lua chon: ");
                    if (choose == 1) {
                        printf("Nhap ID can tim: ");
                        promptIDTillValid(tmp.id);
                        if (readItemByID(tmp.id) == 0) {
                            printf("Tim thay!\n");
                        } else {
                            printf("Khong tim thay!\n");
                        }
                    } else if (choose == 2) {
                        promptNameTillValid(tmp.name);
                        if (readItemByName(tmp.name) == 0) {
                            printf("Tim thay!\n");
                        } else {
                            printf("Khong tim thay!\n");
                        }
                    } else {
                        printf("Lua chon khong hop le!\n");
                    }
                    break;

                case 4:
                    inputOnePatient(&tmp);
                    if (createNewPatient(tmp) == 0){
                        printf("THEM THANH CONG!\n");}
                    else{
                        printf("Loi khi them!\n");}
                    break;

                case 5: {
                    printf("Nhap ID can cap nhat: ");
                    promptIDTillValid(tmp.id);
                    printf( "Chon truong: 1-ID 2-Ten    3-NamSinh   4-TrieuChung\n"
                            "             5-BacSi       6-Ngay      7-Phi\n");
                    printf("Nhap ky tu: ");
                    scanf("%s", typeChange);
                    printf("Nhap gia tri moi: ");
                    switch (typeChange[0]) {
                        case 1:
                            promptIDTillValid(newVal);
                            break;
                        case 2:
                            promptNameTillValid(newVal);
                            break;
                        case 3:
                            promptBirthTillValid(newVal);
                            break;
                        case 4:
                            promptSymptomTillValid(newVal);
                            break;
                        case 5:
                            promptDoctorTillValid(newVal);
                            break;
                        case 6:
                            promptDateTillValid(newVal);
                            break;
                        case 7:
                            promptFeeTillValid(newVal);
                            break;
                        default:
                            printf("Truong duoc chon khong hop le!\n");
                            break;
                    }
                    int isValid = 1;
                    switch (typeChange[0]) {
                        case 1:isValid = validateid(newVal);break;
                        case 2: isValid = validatename(newVal); break;
                        case 3: isValid = validatebirth(newVal); break;
                        case 4: isValid = validateDoctor(newVal); break;
                        case 5: isValid = validateFee(newVal); break;
                        case 6: isValid = validateDate(newVal); break;
                        case 7: isValid = newVal[0] != '\0'; break;
                        default: isValid = 0; break;
                    }
                    if (!isValid) {
                        printf("Gia tri moi khong hop le voi truong da chon!\n");
                        break;
                    }
                    if (updatePatientInfo(id, typeChange, newVal) == 0){
                        printf("CAP NHAT THANH CONG!\n");}
                    else{
                        printf("Khong tim thay ID benh nhan!\n");}
                    break;
                }

                case 6:
                    printf("Nhap ID can xoa: ");
                    scanf("%s", id);
                    if (deletePatient(id) == 0){
                        printf("XOA THANH CONG!\n");}
                    else{
                        printf("Khong tim thay!\n");}
                    break;

                case 7:
                    printf("Nhap file (.txt): ");
                    scanf("%s", fileName);
                    if (exportToFile(fileName) == 0){
                        printf("LUU FILE THANH CONG!\n");}
                    else{
                        printf("Khong ghi duoc file!\n");}
                    break;

                case 8:
                    printf("Tong benh nhan hien co: %d\n", patientCount);
                    break;

                case 9:
                    sum = 0;
                    for (int i = 0; i < patientCount; i++) {
                        sum += atoll(hospitalData[i].fee);
                    }
                    printf("Tong chi phi la: %lld", sum);
                    break;
            }
        }
        getchar();
        char enter[10000];
        printf("Nhan enter de tiep tuc...");
        fgets(enter,sizeof(enter),stdin);
        clearScreen();
    }
    return 0;
}
