#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSTRING    10000
#define BIGINTEGER   999999
#define LOW_STOCK    5

typedef struct item {
    int id;
    char name[51];
    char category[51];
    int quantity;
} item;

typedef struct tmpItem {
    char *id, *name, *category, *quantity;
} tmpItem;

int itemCount = 0;

/* ===== TI?N ?CH NH?P/XU?T ===== */

void clearScreen(void) {
    for (int i = 0; i < 50; i++) printf("\n");
}

void waitEnter(void) {
    int c;
    printf("Nhan enter de tiep tuc...");
    while ((c = getchar()) != '\n' && c != EOF);
    while ((c = getchar()) != '\n' && c != EOF);
}

void readString(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (!fgets(buffer, size, stdin)) { buffer[0] = '\0'; return; }
    buffer[strcspn(buffer, "\n")] = '\0';
}

int readInt(const char *prompt, int *value) {
    char buf[MAXSTRING];
    readString(prompt, buf, sizeof(buf));
    char *end;
    long v = strtol(buf, &end, 10);
    if (*end || end == buf) return 0;
    *value = (int)v;
    return 1;
}

/* ===== VALIDATE ===== */

int checkIDValid(const char *id) {
    if (!id || strlen(id) != 8) return 0;
    if (id[0] != '9' || id[1] != '7') return 0;
    for (int i = 0; id[i]; i++)
        if (!isdigit((unsigned char)id[i])) return 0;
    return 1;
}

int checkNameValid(const char *name) {
    if (!name) return 0;
    size_t len = strlen(name);
    if (len == 0 || len > 30) return 0;
    int digits = 0;
    for (size_t i = 0; i < len; i++) {
        char c = name[i];
        if (!(isalnum((unsigned char)c) || c == '_')) return 0;
        if (isdigit((unsigned char)c)) digits++;
    }
    return digits != (int)len;
}

int checkCategoryValid(const char *cat) {
    if (!cat) return 0;
    size_t len = strlen(cat);
    if (len == 0 || len > 15) return 0;
    int digits = 0;
    for (size_t i = 0; i < len; i++) {
        char c = cat[i];
        if (!(islower((unsigned char)c) || isdigit((unsigned char)c))) return 0;
        if (isdigit((unsigned char)c)) digits++;
    }
    return digits != (int)len;
}

int checkQuantityValid(const char *q) {
    if (!q) return 0;
    size_t len = strlen(q);
    if (len == 0 || len > 5) return 0;
    for (size_t i = 0; i < len; i++)
        if (!isdigit((unsigned char)q[i])) return 0;
    return 1;
}


int idToIndex(item *d, int id) {
    for (int i = 0; i < itemCount; i++)
        if (d[i].id == id) return i;
    return -1;
}

int nameToIndex(item *d, const char *name) {
    for (int i = 0; i < itemCount; i++)
        if (!strcmp(d[i].name, name)) return i;
    return -1;
}

int findByField(item *d, const char *field, const char *value, int *list) {
    int n = 0;
    if (!strcmp(field, "category")) {
        for (int i = 0; i < itemCount; i++)
            if (!strcmp(d[i].category, value)) list[n++] = i;
    } else if (!strcmp(field, "quantity")) {
        int q = atoi(value);
        for (int i = 0; i < itemCount; i++)
            if (d[i].quantity == q) list[n++] = i;
    }
    return n;
}


void addNewItem(item **d, tmpItem t) {
    item *tmp = (item *)realloc(*d, (itemCount + 1) * sizeof(item));
    if (!tmp) { printf("Loi bo nho!\n"); return; }
    *d = tmp;
    (*d)[itemCount].id = atoi(t.id);
    strncpy((*d)[itemCount].name, t.name, 50);
    (*d)[itemCount].name[50] = '\0';
    strncpy((*d)[itemCount].category, t.category, 50);
    (*d)[itemCount].category[50] = '\0';
    (*d)[itemCount].quantity = atoi(t.quantity);
    itemCount++;
}

void deleteItem(item **d, int idx) {
    if (idx < 0 || idx >= itemCount) return;
    for (int i = idx; i < itemCount - 1; i++)
        (*d)[i] = (*d)[i + 1];
    itemCount--;
    if (itemCount == 0) { free(*d); *d = NULL; return; }
    item *tmp = (item *)realloc(*d, itemCount * sizeof(item));
    if (tmp) *d = tmp;
}


tmpItem splitInputedData(char s[]) {
    tmpItem t;
    if (s[0] == '|') {
        t.id = NULL;
        t.name = strtok(s, "|\n");
        t.category = strtok(NULL, "|\n");
        t.quantity = strtok(NULL, "|\n");
    } else {
        t.id = strtok(s, "|\n");
        t.name = strtok(NULL, "|\n");
        t.category = strtok(NULL, "|\n");
        t.quantity = strtok(NULL, "|\n");
    }
    return t;
}

int importFromFile(item **d, const char *fileName) {
    char buffer[MAXSTRING], fullName[MAXSTRING + 4];
    snprintf(fullName, sizeof(fullName), "%s.txt", fileName);
    FILE *f = fopen(fullName, "r");
    if (!f) return 1;
    while (fgets(buffer, MAXSTRING, f)) {
        tmpItem t = splitInputedData(buffer);
        if (checkIDValid(t.id) && checkNameValid(t.name) &&
            checkCategoryValid(t.category) && checkQuantityValid(t.quantity))
            addNewItem(d, t);
        else { fclose(f); return -(itemCount + 1); }
    }
    fclose(f);
    return 0;
}

void exportToFile(item *d, const char *fileName) {
    char fullName[MAXSTRING + 4];
    snprintf(fullName, sizeof(fullName), "%s.txt", fileName);
    FILE *f = fopen(fullName, "w");
    if (!f) { printf("Khong the mo file de ghi!\n"); return; }
    for (int i = 0; i < itemCount; i++)
        fprintf(f, "%d|%s|%s|%d\n", d[i].id, d[i].name, d[i].category, d[i].quantity);
    fclose(f);
    printf("Xuat file thanh cong.\n");
}


void printHeader(void) {
    printf("   ID   |             Name             |   Category    |Quantity\n");
}

void printItemLine(item *it) {
    printf("%8d|%-30s|%-15s|%-5d\n", it->id, it->name, it->category, it->quantity);
}

void printList(item *d, int *list, int n) {
    if (n == 0) return;
    printHeader();
    for (int i = 0; i < n; i++) printItemLine(&d[list[i]]);
}

void printAllItem(item *d) {
    if (itemCount == 0) { printf("Khong co du lieu trong kho.\n"); return; }
    printHeader();
    for (int i = 0; i < itemCount; i++) printItemLine(&d[i]);
}

void showItem(item *d, int idx) {
    int t[1];
    t[0] = idx;
    printList(d, t, 1);
}

void alarm(item *d) {
    int first = 0;
    for (int i = 0; i < itemCount; i++) {
        if (d[i].quantity < LOW_STOCK) {
            if (!first) {
                printf("\n=== CANH BAO MAT HANG SAP HET (SO LUONG < %d) ===\n", LOW_STOCK);
                printHeader();
                first = 1;
            }
            printItemLine(&d[i]);
        }
    }
}


int findItemByName(item *d, const char *prompt, int *idxOut) {
    char name[MAXSTRING];
    readString(prompt, name, sizeof(name));
    if (!checkNameValid(name)) { printf("Ten khong hop le!\n"); return 0; }
    int idx = nameToIndex(d, name);
    if (idx == -1) { printf("Khong tim thay mat hang trong kho.\n"); return 0; }
    *idxOut = idx;
    return 1;
}

int readQuantity(const char *prompt, int *valueOut) {
    char buf[MAXSTRING];
    readString(prompt, buf, sizeof(buf));
    if (!checkQuantityValid(buf)) { printf("So luong khong hop le!\n"); return 0; }
    *valueOut = atoi(buf);
    return 1;
}


void handleAdd(item *d) {
    int idx;
    if (!findItemByName(d, "Nhap ten mat hang: ", &idx)) return;
    int delta;
    if (!readQuantity("Nhap so luong tang: ", &delta)) return;
    if ((long)d[idx].quantity + delta > BIGINTEGER) {
        printf("So luong vuot qua gioi han cho phep!\n");
        return;
    }
    d[idx].quantity += delta;
    showItem(d, idx);
    alarm(d);
}

void handleDecrease(item *d) {
    int idx;
    if (!findItemByName(d, "Nhap ten mat hang: ", &idx)) return;
    int delta;
    if (!readQuantity("Nhap so luong giam: ", &delta)) return;
    if (delta > d[idx].quantity) {
        printf("So luong giam vuot qua so luong hien co!\n");
        return;
    }
    d[idx].quantity -= delta;
    showItem(d, idx);
    alarm(d);
}

void handleUpdate(item *d) {
    int idx;
    if (!findItemByName(d, "Nhap ten mat hang: ", &idx)) return;

    char newName[MAXSTRING], newCat[MAXSTRING];
    readString("Nhap ten moi: ", newName, sizeof(newName));
    if (!checkNameValid(newName)) { printf("Ten khong hop le!\n"); return; }

    readString("Nhap loai hang moi: ", newCat, sizeof(newCat));
    if (!checkCategoryValid(newCat)) { printf("Loai hang khong hop le!\n"); return; }

    int newQty;
    if (!readQuantity("Nhap so luong moi: ", &newQty)) return;

    strncpy(d[idx].name, newName, 50); d[idx].name[50] = '\0';
    strncpy(d[idx].category, newCat, 50); d[idx].category[50] = '\0';
    d[idx].quantity = newQty;
    printf("Cap nhat mat hang thanh cong.\n");
    showItem(d, idx);
}

void handleDelete(item **dPtr) {
    int idx;
    if (!findItemByName(*dPtr, "Nhap ten mat hang: ", &idx)) return;
    deleteItem(dPtr, idx);
    printf("Xoa du lieu cua mat hang thanh cong.\n");
}

void handleFind(item *d) {
    char field[MAXSTRING];
    readString("Nhap cach tim mat hang (id/name/category/quantity): ", field, sizeof(field));

    if (!strcmp(field, "id")) {
        char id[MAXSTRING];
        readString("Nhap id: ", id, sizeof(id));
        if (!checkIDValid(id)) { printf("Id khong hop le.\n"); return; }
        int idx = idToIndex(d, atoi(id));
        if (idx == -1) { printf("Khong tim thay mat hang trong kho.\n"); return; }
        showItem(d, idx);
    }
    else if (!strcmp(field, "name")) {
        int idx;
        if (!findItemByName(d, "Nhap ten: ", &idx)) return;
        showItem(d, idx);
    }
    else if (!strcmp(field, "category") || !strcmp(field, "quantity")) {
        int isCat = !strcmp(field, "category");
        char value[MAXSTRING];
        readString(isCat ? "Nhap loai hang: " : "Nhap so luong: ", value, sizeof(value));
        if (isCat && !checkCategoryValid(value)) { printf("Loai hang khong hop le.\n"); return; }
        if (!isCat && !checkQuantityValid(value)) { printf("So luong khong hop le.\n"); return; }

        int *list = (int *)malloc(itemCount * sizeof(int));
        if (!list) { printf("Loi bo nho!\n"); return; }
        int n = findByField(d, field, value, list);
        if (n == 0) printf("Khong tim thay mat hang trong kho.\n");
        else printList(d, list, n);
        free(list);
    }
    else printf("Lua chon khong hop le!\n");
}

void handleImport(item **dPtr) {
    char fileName[MAXSTRING];
    readString("Nhap ten file txt: ", fileName, sizeof(fileName));
    int err = importFromFile(dPtr, fileName);
    if (err == 0) { printf("Nhap file thanh cong.\n"); alarm(*dPtr); }
    else if (err == 1) printf("File khong ton tai hoac khong the mo.\n");
    else printf("File co du lieu loi tai dong thu: %d\n", -err);
}

void handleExport(item *d) {
    char fileName[MAXSTRING];
    readString("Nhap ten file txt: ", fileName, sizeof(fileName));
    exportToFile(d, fileName);
}


int main(void) {
    item *storeData = NULL;
    int choose = -1;

    while (choose != 0) {
        printf("---He thong quan ly kho hang---\n");
        printf("1. In ra danh sach hang trong kho\n"
               "2. Tra cuu thong tin hang\n"
               "3. Tang so luong hang\n"
               "4. Giam so luong hang\n"
               "5. Cap nhat thong tin hang\n"
               "6. Xoa thong tin hang\n"
               "7. Nhap thong tin tu file txt\n"
               "8. Xuat thong tin ra file txt\n"
               "0. Thoat\n");

        if (!readInt("Nhap lua chon cua ban: ", &choose)) {
            printf("Lua chon khong hop le!\n");
            waitEnter(); clearScreen(); continue;
        }

        switch (choose) {
            case 0: free(storeData); break;
            case 1: printAllItem(storeData); break;
            case 2: handleFind(storeData); break;
            case 3: handleAdd(storeData); break;
            case 4: handleDecrease(storeData); break;
            case 5: handleUpdate(storeData); break;
            case 6: handleDelete(&storeData); break;
            case 7: handleImport(&storeData); break;
            case 8: handleExport(storeData); break;
            default: printf("Lua chon khong hop le! Vui long nhap lai.\n"); break;
        }

        if (choose != 0) waitEnter();
        clearScreen();
    }
    return 0;
}
