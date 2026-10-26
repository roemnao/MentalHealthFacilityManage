#include <stdio.h>
//du lieu
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXSTRING 250
#define MAX_PATIENT 500

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

//validation

int isduplicateid(int id){
	for (int i = 0; i < patientCount;i++){
		if(atoi(hospitalData[i].id) == id){
			return 1;
		}
	}
	return 0;
}
int validateid(char*id){
	int len = strlen(id);
	if (len <8 || len>12){
		return 0;
	}
	for (int i = 0; i < len; i++){
		if (!isdigit(id[i])){
			return 0;
		}
	}
	int cvtid = atoi(id);
	if (isduplicateid(cvtid)){
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
	int len = strlen(date);
	if(len != 10){
		return 0;
	}
		if (date[2]!='/'||date[5]!='/'){
			return 0;
		}
		for (int i = 0; i< 10 ; i++){
			if (i !=2 && i != 5){
				if(!isdigit(date[i])){
					return 0;
				}
			}
		}
		int d = (date[0]-'0')*10 + (date[1]-'0');
		int m = (date[3]-'0')*10 + (date[4]-'0');
        int y = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');
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
int validateHospitalFee(int fee) {
    if (fee >= 0) {
        return 1;
    } else {
        return 0;
    }
}
int main(){
	printf("xong");
    return 0;
}

//crud
int importFromFile(const char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (!f) return 1;
    Patient p;
    while (fscanf(f, "%s %s %s %s %s %s %s", 
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

int exportToFile(const char *fileName) {
    FILE *f = fopen(fileName, "w");
    if (!f) return 1;
    for (int i = 0; i < patientCount; i++) {
        fprintf(f, "%s %s %s %s %s %s %s\n",
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

void printHeader() {
    printf("%-15s|%-20s|%-10s|%-15s|%-15s|%-12s|%-12s\n", "ID","TEN","NAM SINH","TRIEU CHUNG","BAC SI","CHI PHI","NGAY");
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

int readDataBase() {
    if (patientCount == 0){ 
        return 1;
    }
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
            switch (tolower(typeChange[0])){
                case 'i': 
                strncpy(hospitalData[i].id, newVal, sizeof(hospitalData[i].id)-1); 
                hospitalData[i].id[sizeof(hospitalData[i].id)-1]= '\0';
                break;
                case 'n': 
                strncpy(hospitalData[i].name, newVal, sizeof(hospitalData[i].name)-1); 
                hospitalData[i].name[sizeof(hospitalData[i].name)-1]= '\0';
                break;
                case 'b': 
                strncpy(hospitalData[i].birth, newVal, sizeof(hospitalData[i].birth)-1); 
                hospitalData[i].birth[sizeof(hospitalData[i].birth)-1]= '\0';
                break;
                case 's': 
                strncpy(hospitalData[i].symptom, newVal, sizeof(hospitalData[i].symptom)-1); 
                hospitalData[i].symptom[sizeof(hospitalData[i].symptom)-1]= '\0';
                break;
                case 'd': 
                strncpy(hospitalData[i].doctor, newVal, sizeof(hospitalData[i].doctor)-1);
                hospitalData[i].doctor[sizeof(hospitalData[i].doctor)-1]= '\0';
                break;
                case 't': 
                strncpy(hospitalData[i].date, newVal, sizeof(hospitalData[i].date)-1); 
                hospitalData[i].date[sizeof(hospitalData[i].date)-1]= '\0';
                break;
                case 'f': 
                strncpy(hospitalData[i].fee, newVal, sizeof(hospitalData[i].fee)-1); 
                hospitalData[i].fee[sizeof(hospitalData[i].fee)-1]= '\0';
                break;
                default:
                return 1;
            }
            return 0;
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

void inputOnePatient(Patient *p) {
    printf("Nhap ID: "); 
    scanf("%s", p->id);
    printf("Nhap Ho ten: "); 
    getchar();
    fgets(p->name, sizeof(p->name), stdin);
    p->name[strcspn(p->name, "\n")] = '\0';
    printf("Nhap Nam sinh: "); 
    scanf("%s", p->birth);
    printf("Nhap Trieu chung: "); 
    getchar();
    fgets(p->symptom, sizeof(p->symptom), stdin);
    p->symptom[strcspn(p->symptom, "\n")] = '\0';
    printf("Nhap Bac si: "); 
    getchar();
    fgets(p->doctor, sizeof(p->doctor), stdin);
    p->doctor[strcspn(p->doctor, "\n")] = '\0';
    printf("Nhap Vien phi: "); 
    scanf("%s", p->fee);
    printf("Nhap Ngay kham: "); 
    scanf("%s", p->date);
}


//cac ham khac


int main(){
	int choice, field;
    char id[15], typeChange[2], newVal[250];
    float numVal;
    Patient p;
    const char *fileName = "danhsach.txt";
    importFromFile(fileName);

    while (1) {
        printf("\n===== QUAN LY BENH NHAN =====\n");
        printf("1. Doc file\n");
        printf("2. Xem danh sach\n");
        printf("3. Tim theo ID\n");
        printf("4. Them benh nhan\n");
        printf("5. Cap nhat thong tin\n");
        printf("6. Xoa benh nhan\n");
        printf("7. Luu ra file\n");
        printf("0. Thoat\n");
        printf("=============================\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (importFromFile(fileName) == 0){
                    printf("Doc file thanh cong! So benh nhan: %d\n", patientCount);}
                else{
                    printf("Khong mo duoc file!\n");}
                break;
        
            case 2:
                if (readDataBase() == 1){
                    printf("\nDanh sach trong!\n");}
                break;

            case 3:
                printf("Nhap ID can tim: ");
                scanf("%s", id);
                if (readItemByID(id) == 0){
                    printf("Tim thay!\n");}
                else{
                    printf("Khong tim thay!\n");}
                break;

            case 4:
                inputOnePatient(&p);
			    if (!validateid(p.id)) {
			        printf("Loi: ID khong hop le hoac bi trung!\n");
			        break;
			    }
			    if (!validatename(p.name)) {
			        printf("Loi: Ten khong hop le!\n");
			        break;
			    }
			    if (!validatebirth(p.birth)) {
			        printf("Loi: Nam sinh khong hop le!\n");
			        break;
			    }
			    if (!validateDoctor(p.doctor)) {
			        printf("Loi: Ten bac si khong hop le!\n");
			        break;
			    }
			    if (!validateHospitalFee(atoi(p.fee))) {
			        printf("Loi: Vien phi khong hop le!\n");
			        break;
			    }
			    if (!validateDate(p.date)) {
			        printf("Loi: Ngay kham khong hop le!\n");
			        break;
			    }
                if (createNewPatient(p) == 0){
                    printf("THEM THANH CONG!\n");}
                else{
                    printf("Loi khi them!\n");}
                break;

            case 5:
                printf("Nhap ID can cap nhat: ");
                scanf("%s", id);
                printf("Chon truong: i-ID n-Ten b-NamSinh s-TrieuChung d-BacSi t-Ngay f-Phi\n");
                printf("Nhap ky tu: ");
                scanf("%s", typeChange);
                printf("Nhap gia tri moi: ");
                scanf(" %[^\n]", newVal);
				int isValid = 1;
			    switch (tolower(typeChange[0])) {
			        case 'i': isValid = validateid(newVal); break;
			        case 'n': isValid = validatename(newVal); break;
			        case 'b': isValid = validatebirth(newVal); break;
			        case 'd': isValid = validateDoctor(newVal); break;
			        case 'f': isValid = validateHospitalFee(atoi(newVal)); break;
			        case 't': isValid = validateDate(newVal); break;
			    }
				if (!isValid) {
				        printf("Loi: Gia tri moi khong hop le!\n");
				break;
				    }
                if (updatePatientInfo(id, typeChange, newVal) == 0){
                    printf("CAP NHAT THANH CONG!\n");}
                else{
                    printf("Khong tim thay!\n");}
            	break;

            case 6:
                printf("Nhap ID can xoa: ");
                scanf("%s", id);
                if (deletePatient(id) == 0){
                    printf("XOA THANH CONG!\n");}
                else{
                    printf("Khong tim thay!\n");}
                break;

            case 7:
                if (exportToFile(fileName) == 0){
                    printf("LUU FILE THANH CONG!\n");}
                else{
                    printf("Khong ghi duoc file!\n");}
                break;

            case 0:
                printf("Tam biet!\n");
                return 0;

            default:
                printf("Lua chon khong hop le!\n");
        }
        getchar();
        char a[100]; 
        gets(a);
    }
    return 0;
}
