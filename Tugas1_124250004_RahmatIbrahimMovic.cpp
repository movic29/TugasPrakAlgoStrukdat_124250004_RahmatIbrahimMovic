#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Film {
    char name[100];
    double price;
    int year;
    double rating;
};
Film arr[100];
int n = 0;
// Simpan Data Ke File
void SimpanData() {
    FILE *file = fopen("netfilm_db.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(file,"%s;%.0lf;%d;%.1lf\n",
        arr[i].name,
        arr[i].price,
        arr[i].year,
        arr[i].rating);
    }
    fclose(file);
}
// Ngecek Data Yang Ada Di File
void MuatData() {
    FILE *file = fopen("netfilm_db.txt", "r");
    
    if (file == NULL);
    return;
    while (fscanf(file, " %[^;];%lf;%d;%lf\n",
        arr[n].name,
        &arr[n].price,
        &arr[n].year,
        &arr[n].rating) != EOF) {
        n++;
    }
    fclose(file);
}
// Buat Nampilin Data Film
void LihatFilm() {
    cout << "\n NETFLIX CONTENT KATALOG \n\n";
    
    for(int i = 0; i < n; i++){
        cout<<"["<< i + 1 <<"] "
        <<arr[i].name<<" Rp "
        <<arr[i].price<<" "
        <<arr[i].year<<" "
        <<arr[i].rating<<endl;
    }
}
// Bubble Sort Untuk Urutin Dari Abjad
void BubbleSort(){
    for(int i = 0; i < n - 1;i++){
        for(int j = 0; j < n - 1 - i;j++){
            if(strcmp(arr[j].name,arr[j + 1].name) > 0){
                swap(arr[j],arr[j + 1]);
            }
        }
    }
    cout<<"\n Katalog Berhasil Diurutkan Berdasarkan Abjad\n";
}
// Quick Sort Untuk Urutin Dari Rating
int rating(int low,int high){
    double pivot=arr[high].rating;
    int i= low - 1;

    for(int j = low; j < high; j++){
        if(arr[j].rating > pivot){
            i++;
            swap(arr[i],arr[j]);
        }
    }
    swap(arr[i + 1],arr[high]);
    return i + 1;
}
void QuickSort(int low,int high){
    if(low < high){
        int R=rating(low,high);
        
        QuickSort(low,R - 1);
        QuickSort(R + 1,high);
    }
    cout<<" Katalog Film Berhasil diurutkan Berdasarkan Rating Tertinggi\n";
}
void CariFilm(){
    char key[100];

    cin.ignore();
    cout<<"Masukkan Judul Film: ";
    cin.getline(key,100);
    
    // Data Yang Kurang Dari 30 Pakai Linear Search
    if(n < 30){
        cout<<"Menggunakan Linear Search \n";

        for(int i = 0; i < n; i++){
            if(strcmp(arr[i].name,key)){
                cout<<"Ketemu: "
                <<arr[i].name<<" ("<<arr[i].year<<") \n";
          return;
            }
        }
        cout<<"Judul tidak ditemukan\n";
    }
    // Data Yang Lebih Dari 30 Pakai Binary Search
    else{
        cout<<"Menggunakan Binary Search\n";

        BubbleSort();
        int left = 0;
        int right = n - 1;

        while(left <= right){
            int mid = (left + right) / 2;
            int bs = strcmp(arr[mid].name,key);
            
            if(bs == 0){
                cout<<"Ketemu: "
                <<arr[mid].name<<" ("<<arr[mid].year<<")\n";
                return;
            }
            else if(bs < 0)
                left = mid + 1;
            else
                right = mid - 1;
        }
        cout<<"Judul tidak ditemukan\n";
    }
}
// Tambahin Film
void TambahFilm(){
    cin.ignore();
    
    cout<<"Input Judul : ";
    cin.getline(arr[n].name,100);
    
    cout<<"Input Harga : ";
    cin>>arr[n].price;
    
    cout<<"Input Tahun : ";
    cin>>arr[n].year;
    
    cout<<"Input Rating: ";
    cin>>arr[n].rating;
    n++;
    
    SimpanData();
    cout<<"\n Film Berhasil Ditambahkan!! \n";
}
int main(){
    
    MuatData();
    int menu;
    
    do{
        cout<<"\n NETFILM SISTEM DASHBOARD \n\n";
        cout<<"1. Lihat Katalog Film \n";
        cout<<"2. Urutkan Sesuai abjad \n";
        cout<<"3. Urutkan Sesuai rating \n";
        cout<<"4. Cari Film \n";
        cout<<"5. Tambah Film Baru \n";
        cout<<"0. Keluar Dan Simpan \n";
        
        cout<<"\n Pilih Menu = ";
        cin>>menu;
        
        switch(menu){
            
            case 1:
            LihatFilm();
            break;
            
            case 2:
            BubbleSort();
            break;
            
            case 3:
            QuickSort(0,n - 1);
            break;
            
            case 4:
            CariFilm();
            break;
            
            case 5:
            TambahFilm();
            break;
            
            case 0:
            SimpanData();
            cout<<"\n Data Berhasil Disinkronkan Ke netfilm_db.txt \n";
            cout<<" Sistem Berakhir. Terimakasih!! \n";
            break;
        }
    } while(menu != 0);
}
