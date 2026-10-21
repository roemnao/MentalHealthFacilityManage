#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
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

Patient hospitalData[MAX_PATIENT];
int patientCount = 0;
int isduplicateid(int id){
	for (int i = 0; i < patientCount;i++){
		if(hospitalData[i].id == id){
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
