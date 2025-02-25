#include <stdio.h> //untuk operasi input dan output
#include <windows.h> //untuk fungsi sistem "cls" dan "PAUSE" serta menambahkan tanggal transaksi secara otomatis tersimpan pada SalesHistory.txt
#include <string.h> //untuk operasi string

//Menentukan struktur untuk AccountData
typedef struct{
    char username[12], password[12], type[12];
}AccountData;

//Menentukan struktur untuk ItemData
typedef struct{
    char record[10], ItemName[100], type[20];
    int qty;
    double cost;
}ItemData;

//Menentukan struktur untuk TransHistory
typedef struct {
    char Date[20], CusName[100], ItemName[100];
    int NoteNum, qty;
    double cost, total;
}TransHistory;

//fungsi untuk mencetak baris karakter
void line(char c,int x){
    for(int i=0;i<x;i++)printf("%c",c);
    printf("\n");
}

//fungsi untuk mencetak spasi karakter
void spacer(char c,int x){
    for(int i=0;i<x;i++)printf("%c",c);
}

//fungsi untuk mendapatkan ukuran data Inventory
int GetSizeInv(){
	FILE *fp = fopen("Inventory.txt", "r");
	int count = 0;
	while(!feof(fp)){
		ItemData data;
		fscanf(fp, "%[^;];%[^;];%[^;];%d;%lf;\n", &data.record, &data.ItemName, &data.type, &data.qty, &data.cost);
		count++;
	}
	fclose(fp);
	return count;
}

//fungsi untuk mendapatkan ukuran data Account
int GetSizeAcc(){
	FILE *fp = fopen("User.txt", "r"); //buka file dalam mode baca
	int count = 0; //inisialisasi hitungan ke 0
	while(!feof(fp)){
		AccountData data;
		fscanf(fp, "%[^;];%[^;];%[^;];\n", &data.username, &data.password, &data.type);
		count++;
	}
	fclose(fp); //tutup jumlah pengembalian file
	return count;
}

//fungsi masuk menu
void InitInvData(ItemData data[]){
	FILE *fp = fopen("Inventory.txt", "r"); //buka file mode baca
    int i=0;
	while(!feof(fp)){
		fscanf(fp, "%[^;];%[^;];%[^;];%d;%lf;\n", &data[i].record, &data[i].ItemName, &data[i].type, &data[i].qty, &data[i].cost);
        i++;
	}
	fclose(fp); //tutup file
}

//login Menu
int login(char UIn[10],char PIn[10]){
    FILE *FAcc = fopen("User.txt","r");
    AccountData search;
    int valid=0;
	while(!feof(FAcc)){
		fscanf(FAcc, "%[^;];%[^;];%[^;];\n",search.username,search.password,search.type);
		if(strcmp(search.username,UIn) == 0){
            valid=1;
            break;
        }
	}
    if(valid == 1){
        if (strcmp(search.password,PIn) != 0){
        printf("Wrong Password!\n");
        system("PAUSE");
        return 0;
        }
        else{
            if(strcmp(search.type,"ADMIN") == 0) return 1;
            else if (strcmp(search.type,"SUPERVISOR") == 0) return 2;
            else return 3;
        }
    }
    else{
        printf("Username not exist!\n");
        system("PAUSE");
        return 0;
    }
}

int LoginMenu(){
    char UsernameIn[12],PassIn[12];
    spacer(' ',39); line('-',21);
    spacer(' ',39); spacer('/',8);
    printf("LOGIN");line('/',8);
    spacer(' ',39); line('-',21);
    printf("\n");
    spacer(' ',30); printf("Input Your Username & Password to login\n");
    spacer(' ',39); printf("or input -1 to Exit.\n\n");
    spacer(' ',39); printf("Username : "),scanf(" %s",UsernameIn);
    if (strcmp(UsernameIn,"-1")==0)return -1;
    spacer(' ',39); printf("Password : "),scanf(" %s",PassIn);
    return login(UsernameIn,PassIn);
}

//Transaction Menu
int GetNoteNum(){ //fungsi untuk mendapatkan ukuran data NoteNum
    SYSTEMTIME today;
    int Number,day,month,year;
    GetLocalTime(&today);
    FILE *temp= fopen("NoteNum.txt","r");
    fscanf(temp,"%d;%d;%d;%d;", &Number, &day, &month, &year);
    fclose(temp);
    if (day == today.wDay && month == today.wMonth && year == today.wYear){
        Number++;
        FILE *temp= fopen("NoteNum.txt","w");
        fprintf(temp,"%d;%d;%d;%d;", Number, day, month, year);
        fclose(temp);
        return Number;
    }
    else {
        Number =1;
        FILE *temp= fopen("NoteNum.txt","w");
        fprintf(temp,"%d;%d;%d;%d;", Number, today.wDay, today.wMonth, today.wYear);
        fclose(temp);
        return Number;
    }
}

void Transaction(){ //fungsi untuk input transaksi
    TransHistory Trans[100];
    char repeat='Y';
    int ItemIndex=0;
    Trans[ItemIndex].total = 0;
    Trans[0].NoteNum=GetNoteNum();
    while(repeat=='Y'||repeat=='y'){
        system("cls");
        spacer(' ',8); line('-',35);
        spacer(' ',8); spacer('/',9);
        printf("Transaction Menu");
        line('/',10);
        spacer(' ',8);line('-',35);
        if (ItemIndex==0) {
            printf("\n\nCustomer Name\t\t: "), scanf(" %[^\n]",Trans[ItemIndex].CusName);
        }
        else{
            strcpy(Trans[ItemIndex].CusName,Trans[ItemIndex-1].CusName);
            printf("\n\nCustomer Name\t\t: %s\n",Trans[ItemIndex].CusName);
        }
        printf("Input Item Name\t\t: "); scanf(" %[^\n]",Trans[ItemIndex].ItemName);
        printf("Input Item qty\t\t: "); scanf(" %d",&Trans[ItemIndex].qty);
        FILE *fdata = fopen("Inventory.txt","r");
        ItemData data;
        int Index=0;
        while(!feof(fdata)){
            fscanf(fdata, "%[^;];%[^;];%[^;];%d;%lf;\n", data.record, data.ItemName, data.type, &data.qty, &data.cost);
            if(strcmp(data.ItemName,Trans[ItemIndex].ItemName) == 0) break;
            else Index++;
        }
        fclose(fdata);
        if (Index>=GetSizeInv()){
            printf("Item not found!\n");
            break;
        }
        else if (Trans[ItemIndex].qty>data.qty){
            printf("Item sold out\n");
            break;
            }
        else{
        Trans[ItemIndex].cost = data.cost;
        Trans[ItemIndex].total = 0;
        if(ItemIndex==0) Trans[ItemIndex].total = Trans[ItemIndex].qty * Trans[ItemIndex].cost;
        else Trans[ItemIndex].total = Trans[ItemIndex-1].total + (Trans[ItemIndex].qty * Trans[ItemIndex].cost);
        printf("Item Price per qty\t: %.2lf",Trans[ItemIndex].cost);
        printf("\nTotal Price\t\t: %.2lf",Trans[ItemIndex].total);
        int size=GetSizeInv();
        ItemData change[size];
        InitInvData(change);
        FILE *fdata = fopen("Inventory.txt","w");
        for(int i=0;i<size;i++){
            if(strcmp(data.ItemName,change[i].ItemName) == 0) change[i].qty = change[i].qty - Trans[ItemIndex].qty;
            fprintf(fdata, "%s;%s;%s;%d;%.2lf;\n",change[i].record, change[i].ItemName, change[i].type, change[i].qty, change[i].cost);
        }
	    fclose(fdata);
        ItemIndex++;
        }
        printf("\n\nContinue?(Y/N) ");
        scanf(" %c",&repeat);
    }
    FILE *fhistory = fopen("SalesHistory.txt","a");
    SYSTEMTIME today;
    GetLocalTime(&today);
    for(int i=0;i<ItemIndex;i++){
        fprintf(fhistory, "%d/%d/%d;%s;%d;%s;%d;%.2lf;%.2lf;\n",today.wDay,today.wMonth,today.wYear,Trans[i].CusName,Trans[0].NoteNum,Trans[i].ItemName,Trans[i].qty,Trans[i].cost,Trans[i].total);
    }
    fclose(fhistory);
}

//Entry Menu
void CreateData(){ //fungsi untuk menambah data baru
    char repeat;
    do{
        system("cls");
        line('-',35);
        spacer('/',11);
        printf("Add New Item");
        line('/',12);
        line('-',35);
        ItemData data;
	    printf("Input Record\t: "), scanf(" %[^\n]",data.record);
	    printf("Item Name\t: "), scanf(" %[^\n]",data.ItemName);
	    printf("Item Type\t: "), scanf(" %[^\n]",data.type);
	    printf("Quantity\t: "), scanf(" %d",&data.qty);
	    printf("Cost\t\t: "), scanf(" %lf",&data.cost);
        FILE *fdata = fopen("Inventory.txt", "a");
        fprintf(fdata, "%s;%s;%s;%d;%.2lf;\n",data.record, data.ItemName, data.type, data.qty, data.cost);
	    fclose(fdata);
	    printf("Add Another New Item?(Y/N) ");
        scanf(" %c",&repeat);
    }while(repeat=='Y'||repeat=='y');
}

void UpdateData(){ //fungsi untuk mengubah data
    char repeat;
    do{
        system("cls");
        line('-',35);
        spacer('/',12);
        printf("Update Item");
        line('/',12);
        line('-',35);
        int size=GetSizeInv(),index;
        ItemData data[size];
        InitInvData(data);
        printf("Chose Index		: "), scanf(" %d", &index);
	    printf("Input Record\t: "), scanf(" %[^\n]",data[index-1].record);
	    printf("Item Name\t: "), scanf(" %[^\n]",data[index-1].ItemName);
	    printf("Item Type\t: "), scanf(" %[^\n]",data[index-1].type);
	    printf("Quantity\t: "), scanf(" %d",&data[index-1].qty);
	    printf("Cost\t: "), scanf(" %lf",&data[index-1].cost);
        FILE *fdata = fopen("Inventory.txt", "w");
	    for(int i=0; i<=size; i++) fprintf(fdata, "%s;%s;%s;%d;%.2lf;\n", data[i].record, data[i].ItemName, data[i].type, data[i].qty, data[i].cost);
	    fclose(fdata);
	    printf("Edit Another Item?(Y/N) ");
        scanf(" %c",&repeat);
    }while(repeat=='Y'||repeat=='y');
}

void DeleteData(){ //fungsi untuk menghapus data
    char repeat;
    do{
        system("cls");
        line('-',35);
        spacer('/',12);
        printf("Delete Item");
        line('/',12);
        line('-',35);
        int size=GetSizeInv(),index;
        ItemData data[size];
        InitInvData(data);
	    printf("Select Index: ");
	    scanf(" %d", &index);
	    for(int i=index-1; i<size; i++){
		    data[i] = data[i+1];
	    }
	    FILE *fdata = fopen("Inventory.txt", "w");
	    for(int i=0; i<size-1; i++) fprintf(fdata, "%s;%s;%s;%d;%.2lf;\n", data[i].record, data[i].ItemName, data[i].type, data[i].qty, data[i].cost);
	    fclose(fdata);
	    printf("Delete Another Item?(Y/N) ");
        scanf(" %c",&repeat);
    }while(repeat=='Y'||repeat=='y');
}

void EntryMenu(){ //fungsi menu entry
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',12);
        printf("Entry Menu");
        line('/',13);
        line('-',35);
        printf("\n1. Input New Item Data\n2. Update Item Data\n3. Delete Item Data\n0. Back\n\nInput\t: ");
        scanf(" %d",&MenuInput);
        switch (MenuInput){
        case 1:
            CreateData();
            break;
        case 2:
            UpdateData();
            break;
        case 3:
            DeleteData();
            break;
        case 0:
            printf("\nBack to Main Menu...\n");
            break;
        default:
            printf("\nInvalid Input!!!\n");
            system("PAUSE");
            break;
        }
    }while(MenuInput!=0);
}

//Preview Menu
void QuickPreview(){ //fungsi untuk menampilkan preview cepat
    int size=GetSizeInv();
    ItemData data[size];
    InitInvData(data);
    system("cls");
    spacer(' ',42); line('-',35);
    spacer(' ',42); spacer('/',6);
    printf("Inventory Quick Preview");
    line('/',6);
    spacer(' ',42); line('-',35);
    printf("\n%-10s%-50s%-20s%-20s%-20s\n", "Record #", "Item Name", "Item Type", "Quantity", "Cost");
    line('-', 120);
    for(int i=0;i<size;i++){
        printf("\n%-10s%-50s%-20s%-20d%.2lf\n",data[i].record, data[i].ItemName, data[i].type, data[i].qty, data[i].cost);
    }
    printf("\n\n");
    system("PAUSE");
}

//fungsi untuk mengurutkan berdasarkan kuantitas dalam urutan a-z, 1-9
void AscendingQty(ItemData data[], int n){
    ItemData temp;
    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            if(data[i].qty > data[j].qty){
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

//fungsi untuk mengurutkan berdasarkan kuantitas dalam urutan z-a, 9-1
void DescendingQty(ItemData data[], int n){
    ItemData temp;
    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            if(data[i].qty < data[j].qty){
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

//fungsi untuk pratinjau berdasarkan kuantitas dalam urutan ascending 1-9
void PreviewQtyAsc(){
    int size=GetSizeInv();
    ItemData data[size];
    InitInvData(data);
    system("cls");
    spacer(' ',42); line('-',35);
    spacer(' ',42); spacer('/',5);
    printf("Asc Qty Inventory Preview");
    line('/',5);
    spacer(' ',42); line('-',35);
    printf("\n%-10s%-50s%-20s%-20s%-20s\n", "Record #", "Item Name", "Item Type", "Quantity", "Cost");
    line('-', 120);
    AscendingQty(data, size);
    for(int i=0;i<size;i++){
        printf("\n%-10s%-50s%-20s%-20d%.2lf\n",data[i].record, data[i].ItemName, data[i].type, data[i].qty, data[i].cost);
    }
    printf("\n\n");
    system("PAUSE");
}

//fungsi untuk pratinjau berdasarkan kuantitas dalam urutan ascending 9-1
void PreviewQtyDesc(){
    int size=GetSizeInv();
    ItemData data[size];
    InitInvData(data);
    system("cls");
    spacer(' ',42); line('-',35);
    spacer(' ',42); spacer('/',4);
    printf("Desc Qty Inventory Preview");
    line('/',5);
    spacer(' ',42); line('-',35);
    printf("\n%-10s%-50s%-20s%-20s%-20s\n", "Record #", "Item Name", "Item Type", "Quantity", "Cost");
    line('-', 120);
    DescendingQty(data, size);
    for(int i=0;i<size;i++){
        printf("\n%-10s%-50s%-20s%-20d%.2lf\n",data[i].record, data[i].ItemName, data[i].type, data[i].qty, data[i].cost);
    }
    printf("\n\n");
    system("PAUSE");
}

void PreviewQty(){ //fungsi menampilkan urutan data kuantitas
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',9);
        printf("Sort by Quantity");
        line('/',10);
        line('-',35);
        printf("\n1. Ascending\n2. Descending\n\n0. Back\n\nInput\t: ");
        scanf(" %d",&MenuInput);
        switch (MenuInput){
        case 1:
            PreviewQtyAsc();
            break;
        case 2:
            PreviewQtyDesc();
            break;
        case 0:
            printf("\nBack to Main Menu...\n");
            break;
        default:
            printf("\nInvalid Input!!!\n");
            system("PAUSE");
            break;
        }
    }while(MenuInput!=0);
}

//fungsi untuk pratinjau berdasarkan biaya dalam urutan ascending 1-9
void AscendingCost(ItemData data[], int n){
    ItemData temp;
    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            if(data[i].cost > data[j].cost){
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

//fungsi untuk pratinjau berdasarkan biaya dalam urutan ascending 9-1
void DescendingCost(ItemData data[], int n){
    ItemData temp;
    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            if(data[i].cost < data[j].cost){
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

// fungsi untuk menampilkan berdasarkan biaya dalam urutan ascending 1-9
void PreviewCostAsc(){
    int size=GetSizeInv();
    ItemData data[size];
    InitInvData(data);
    system("cls");
    spacer(' ',42); line('-',35);
    spacer(' ',42); spacer('/',4);
    printf("Asc Cost Inventory Preview");
    line('/',5);
    spacer(' ',42); line('-',35);
    printf("\n%-10s%-50s%-20s%-20s%-20s\n", "Record #", "Item Name", "Item Type", "Quantity", "Cost");
    line('-', 120);
    AscendingCost(data, size);
    for(int i=0;i<size;i++){
        printf("\n%-10s%-50s%-20s%-20d%.2lf\n",data[i].record, data[i].ItemName, data[i].type, data[i].qty, data[i].cost);
    }
    printf("\n\n");
    system("PAUSE");
}

//fungsi untuk menampilkan berdasarkan biaya dalam urutan descending 9-1
void PreviewCostDesc(){
    int size=GetSizeInv();
    ItemData data[size];
    InitInvData(data);
    system("cls");
    spacer(' ',42); line('-',35);
    spacer(' ',42); spacer('/',4);
    printf("Desc Cost Inventory Preview");
    line('/',4);
    spacer(' ',42); line('-',35);
    printf("\n%-10s%-50s%-20s%-20s%-20s\n", "Record #", "Item Name", "Item Type", "Quantity", "Cost");
    line('-', 120);
    DescendingCost(data, size);
    for(int i=0;i<size;i++){
        printf("\n%-10s%-50s%-20s%-20d%.2lf\n",data[i].record, data[i].ItemName, data[i].type, data[i].qty, data[i].cost);
    }
    printf("\n\n");
    system("PAUSE");
}

void PreviewCost(){
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',11);
        printf("Sort by Cost");
        line('/',12);
        line('-',35);
        printf("\n1. Ascending\n2. Descending\n\n0. Back\n\nInput\t: ");
        scanf(" %d",&MenuInput);
        switch (MenuInput){
        case 1:
            PreviewCostAsc();
            break;
        case 2:
            PreviewCostDesc();
            break;
        case 0:
            printf("\nBack to Main Menu...\n");
            break;
        default:
            printf("\nInvalid Input!!!\n");
            system("PAUSE");
            break;
        }
    }while(MenuInput!=0);
}

void PreviewInvMenu(){ //fungsi menampilkan menu preview inventory
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',6);
        printf("Inventory Preview Menu");
        line('/',7);
        line('-',35);
        printf("\n1. Quick Preview\n2. Sorted by Quantity\n3. Sorted by Cost\n0. Back\n\nInput\t: ");
        scanf(" %d",&MenuInput);
        switch (MenuInput){
        case 1:
            QuickPreview();
            break;
        case 2:
            PreviewQty();
            break;
        case 3:
            PreviewCost();
            break;
        case 0:
            printf("\nBack to Main Menu...\n");
            break;
        default:
            printf("\nInvalid Input!!!\n");
            system("PAUSE");
            break;
        }
    }while(MenuInput!=0);
}

void PreviewHistory(){ //fungsi untuk menampilkan menu preview riwayat transaksi
    FILE *fp= fopen("SalesHistory.txt","r");
    int n = 0;
    TransHistory data[100];
    while(!feof(fp)){
        fscanf(fp, "%[^;];%[^;];%d;%[^;];%d;%lf;%lf;\n", &data[n].Date, &data[n].CusName, &data[n].NoteNum, &data[n].ItemName, &data[n].qty, &data[n].cost, &data[n].total);
        n++;
    }
    system("cls");
    spacer(' ',77); line('-',35);
    spacer(' ',77); spacer('/',10);
    printf("Sales Preview");
    line('/',11);
    spacer(' ',77); line('-',35);
    printf("\n\n%-20s%-50s%-20s%-50s%-15s%-20s%-20s\n", "Date", "Customer Name", "Note Number", "Item Name", "Quantity", "Cost", "Total");
    line('-', 190);
    for(int i=0;i<n;i++){
        if (strcmp(data[i].Date,data[i-1].Date)==0) {
            if (strcmp(data[i].Date,data[i-1].Date)==0 && data[i].NoteNum == data[i-1].NoteNum) printf("\n%-20s%-50s%-20s%-50s%-15d%-20.2f%.2lf", " ", " ", " ", data[i].ItemName, data[i].qty, data[i].cost, data[i].total);
            else printf("\n\n%-20s%-50s%-20d%-50s%-15d%-20.2f%.2lf", " ", data[i].CusName, data[i].NoteNum, data[i].ItemName, data[i].qty, data[i].cost, data[i].total);
        }
        else printf("\n\n%-20s%-50s%-20d%-50s%-15d%-20.2f%.2lf", data[i].Date, data[i].CusName, data[i].NoteNum, data[i].ItemName, data[i].qty, data[i].cost, data[i].total);
    }
    printf("\n\n");
    fclose(fp);
    system("PAUSE");
}

void PreviewMenu(){ //fungsi untuk menampilkan menu Preview Data
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',11);
        printf("Preview Menu");
        line('/',12);
        line('-',35);
        printf("\n1. Preview Inventory\n2. Preview Transaction History\n0. Back\n\nInput\t: ");
        scanf(" %d",&MenuInput);
        switch (MenuInput){
        case 1:
            PreviewInvMenu();
            break;
        case 2:
            PreviewHistory();
            break;
        case 0:
            printf("\nBack to Main Menu...\n");
            break;
        default:
            printf("\nInvalid Input!!!\n");
            system("PAUSE");
            break;
        }
    }while(MenuInput!=0);
}

//Search Menu
void SearchMenu(){ //fungsi mencari item
    char repeat;
    do{
        system("cls");
        line('-',35);
        spacer('/',12);
        printf("Search Item");
        line('/',12);
        line('-',35);
        printf("\nPlease Input Item Name or Item Record Number.\nSearch\t: ");
        char CharInput[100];
        scanf(" %[^\n]",CharInput);
        FILE *fdata = fopen("Inventory.txt", "r");
        ItemData data;
        int Index=0;
	    while(!feof(fdata)){
		    fscanf(fdata, "%[^;];%[^;];%[^;];%d;%lf;\n", data.record, data.ItemName, data.type, &data.qty, &data.cost);
		    if(strcmp(data.ItemName,CharInput) == 0 ||strcmp(data.record,CharInput) == 0) break;
            else Index++;
	    }
        if (Index>=GetSizeInv())printf("Item not found!\n");
        else printf("Record Number\t: %s\nItem Name\t: %s\nItem Type\t: %s\nItem Quantity\t: %d\nItem Price\t: %.2lf\n", data.record, data.ItemName, data.type, data.qty, data.cost);
        printf("Search Another Item?(Y/N) ");
        scanf(" %c",&repeat);
    }while(repeat=='Y'||repeat=='y');
}

//Manage User Menu
void InitUserData(AccountData data[]){ //fungsi log in
	FILE *FAcc=fopen("User.txt", "r");
	int index=0;
	while(!feof(FAcc)){
		fscanf(FAcc, "%[^;];%[^;];%[^;];\n",data[index].username,data[index].password,data[index].type);
		index++;
	}
	fclose(FAcc);
}

void PreviewUser(){ //fungsi untuk menampilkan data user
    int size=GetSizeAcc();
    AccountData data[size];
    InitUserData(data);
    system("cls");
    spacer(' ',11); line('-',23);
    spacer(' ',11); spacer('/',5);
    printf("User Preview");
    line('/',6);
    spacer(' ',11); line('-',23);
    printf("\n%-15s%-15s%-15s\n","Username","Password","User Type");
    line('-',45);
    for(int i=0; i<size; i++){
		printf("%-15s%-15s%-15s\n",data[i].username,data[i].password,data[i].type);
	}
    printf("\n\n");
    system("PAUSE");
}

void CreateUser(){ //fungsi untuk membuat akun baru
    system("cls");
    line('-',35);
    spacer('/',9);
    printf("Create New User");
    line('/',10);
    line('-',35);
    int size=(1+GetSizeAcc());
    AccountData data[size];
    InitUserData(data);
	printf("\nInput Nama User\t: "), scanf(" %s",data[size-1].username);
	printf("Input Password\t: "), scanf(" %s",data[size-1].password);
	printf("Input Tipe User\t: "), scanf(" %s",data[size-1].type);
	FILE *FAcc=fopen("User.txt", "w");
    for(int i=0; i<size; i++){
		fprintf(FAcc,"%s;%s;%s;\n",data[i].username,data[i].password,data[i].type);
	}
	fclose(FAcc);
    system("PAUSE");
}

void UpdateUser(){ //fungsi untuk mengedit data user
    system("cls");
    line('-',35);
    spacer('/',10);
    printf("Edit User Data");
    line('/',10);
    line('-',35);
    int index,size=GetSizeAcc();
    AccountData data[size];
    InitUserData(data);
    printf("\nChose index		: "), scanf("%d", &index);
	printf("Input Nama User\t: "), scanf(" %s", data[index-1].username);
	printf("Input Password\t: "), scanf(" %s",data[index-1].password);
	printf("Input Tipe User\t: "), scanf(" %s",data[index-1].type);
	FILE *FAcc=fopen("User.txt", "w");
    for(int i=0; i<size; i++){
		fprintf(FAcc,"%s;%s;%s;\n",data[i].username,data[i].password,data[i].type);
	}
	fclose(FAcc);
    system("PAUSE");
}

void DeleteUser(){ //fungsi untuk menghapus akun user
    system("cls");
    line('-',35);
    spacer('/',9);
    printf("Delete User Data");
    line('/',9);
    line('-',35);
    int index,size=GetSizeAcc();
    AccountData data[size];
    InitUserData(data);
	printf("\nSelect index: ");
	scanf("%d", &index);
	for(int i=index-1; i<size; i++){
		data[i]=data[i+1];
	}
	size--;
	FILE *FAcc=fopen("User.txt", "w");
    for(int i=0; i<size; i++){
		fprintf(FAcc,"%s;%s;%s;\n",data[i].username,data[i].password,data[i].type);
	}
	fclose(FAcc);
}

void ManageUserMenu(){ //fungsi untuk menampilkan menu Manage User
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',9);
        printf("Manage User Menu");
        line('/',10);
        line('-',35);
        printf("\n1. Preview User Data\n2. Create New User Data\n3. Edit User Data\n4. Delete User Data\n0. Back\n\nInput\t: ");
        scanf(" %d",&MenuInput);
        switch (MenuInput){
        case 1:
            PreviewUser();
            break;
        case 2:
            CreateUser();
            break;
        case 3:
            UpdateUser();
            break;
        case 4:
            DeleteUser();
            break;
        case 0:
            printf("\nBack to Main Menu...\n");
            break;
        default:
            printf("\nInvalid Input!!!\n");
            break;
        }
    }while(MenuInput!=0);
}

//Master Menu
int CashierMenu(){ //fungsi untuk menampilkan menu yang didapatkan role kasir
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',13);
        printf("Main Menu");
        line('/',13);
        line('-',35);
        printf("\n1. Transaction\n2. Search Data\n0. Log Out\n\nInput\t: ");
        scanf("%d",&MenuInput);
        switch (MenuInput){
        case 1:
            Transaction();
            break;
        case 2:
            SearchMenu();
            break;
        case 0:
            printf("\nUser Log Out...\n");
            return 0;
        default:
            printf("\nInvalid Input!!!\n");
            break;
        }
    }while(MenuInput!=0);
    system("PAUSE");
}

int SupervisorMenu(){ //fungsi untuk menampilkan menu yang didapatkan role supervisor
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',13);
        printf("Main Menu");
        line('/',13);
        line('-',35);
        printf("\n1. Transaction\n2. Entry Data\n3. Preview Data\n4. Search Data\n0. Log Out\n\nInput\t: ");
        scanf("%d",&MenuInput);
        switch (MenuInput){
        case 1:
            Transaction();
            break;
        case 2:
            EntryMenu();
            break;
        case 3:
            PreviewMenu();
            break;
        case 4:
            SearchMenu();
            break;
        case 0:
            printf("\nUser Log Out...\n");
            return 0;
        default:
            printf("\nInvalid Input!!!\n");
            break;
        }
    }while(MenuInput!=0);
    system("PAUSE");
}

int AdminMenu(){ //fungsi untuk menampilkan menu yang didapatkan role admin
    int MenuInput;
    do{
        system("cls");
        line('-',35);
        spacer('/',13);
        printf("Main Menu");
        line('/',13);
        line('-',35);
        printf("\n1. Transaction\n2. Entry Data\n3. Preview Data\n4. Search Data\n5. Manage User\n0. Log Out\n\nInput\t: ");
        scanf("%d",&MenuInput);
        switch (MenuInput){
        case 1:
            Transaction();
            break;
        case 2:
            EntryMenu();
            break;
        case 3:
            PreviewMenu();
            break;
        case 4:
            SearchMenu();
            break;
        case 5:
            ManageUserMenu();
            break;
        case 0:
            printf("\nUser Log Out...\n");
            return 0;
        default:
            printf("\nInvalid Input!!!\n");
            break;
        }
    }while(MenuInput!=0);
    system("PAUSE");
}

int main(){
    int rank=0;
    do{
    switch (rank){
        case 1:
            system("cls");
            rank = AdminMenu();
            break;
        case 2:
            system("cls");
            rank = SupervisorMenu();
            break;
        case 3:
            system("cls");
            rank = CashierMenu();
            break;
        case 0:
            system("cls");
            rank = LoginMenu();
            break;
        case -1:
            printf("Exiting Program...\n");
            system("PAUSE");
            return 0;
        default:
            printf("Program Error!");
            system("PAUSE");
            break;
        }
    }while(rank!=0||rank!=1||rank!=2||rank!=3);
}
