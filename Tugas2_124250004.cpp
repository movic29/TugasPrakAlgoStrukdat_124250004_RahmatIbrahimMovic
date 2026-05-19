#include <iostream>
#include <cstring>
using namespace std;
struct Karyawan {
    int nip;
    char nama[50];
    char divisi[50];
    int statusCuti;
};
// untuk PTB
struct NodeTree {
    Karyawan info;
    NodeTree *kiri;
    NodeTree *kanan;
};
NodeTree *akar = NULL;

// stack untuk undo
struct InfoUndo {
    int tipe; // 1 ngajuin cuti, 2 selesai cuti
    int nip;
};
struct NodeStack {
    InfoUndo info;
    NodeStack *next;
};
NodeStack *awalstack = NULL;

// queue untuk antrian cuti
struct NodeQueue {
    int nip;
    NodeQueue *next;
};
NodeQueue *depan = NULL;
NodeQueue *belakang = NULL;

// untuk nyari karyawan di PTB
NodeTree* cariKaryawan(int nip) {
    NodeTree *p = akar;
    while (p != NULL) {
        if (nip == p->info.nip) return p;
        else if (nip < p->info.nip) p = p->kiri;
        else p = p->kanan;
    }
    return NULL;
}
// stack
bool stackKosong() {
    return (awalstack == NULL);
}
void pushStack(int tipe, int nip) {
    NodeStack *baru = new NodeStack;
    baru->info.tipe = tipe;
    baru->info.nip = nip;
    baru->next = awalstack;
    awalstack = baru;
}
InfoUndo popStack() {
    InfoUndo data = {-1, -1};
    if (!stackKosong()) {
        NodeStack *hapus = awalstack;
        data = hapus->info;
        awalstack = awalstack->next;
        delete hapus;
    }
    return data;
}
// queue
bool queueKosong() {
    return (depan == NULL);
}
void enqueue(int nip) {
    NodeQueue *baru = new NodeQueue;
    baru->nip = nip;
    baru->next = NULL;
    
    if (queueKosong()) depan = baru;
    else belakang->next = baru;
    
    belakang = baru;
}
int dequeue() {
    if (queueKosong()) return -1;
    NodeQueue *hapus = depan;
    int nip = hapus->nip;
    depan = depan->next;
    
    if (depan == NULL) belakang = NULL;
    delete hapus;
    return nip;
}
// nambah data karyawan PTB
void tambahData() {
    Karyawan data;
    cout << "\nMasukkan NIP    : "; cin >> data.nip;
    cin.ignore();
    cout << "Masukkan Nama   : "; cin.getline(data.nama, 50);
    cout << "Masukkan Divisi : "; cin.getline(data.divisi, 50);

    data.statusCuti = 0; 

    NodeTree *baru = new NodeTree;
    baru->info = data;
    baru->kiri = NULL;
    baru->kanan = NULL;

    if (akar == NULL) {
        akar = baru;
        cout << "Data karyawan berhasil ditambahkan\n";
    } else {
        NodeTree *p = akar;
        NodeTree *b = NULL;
        
        while (p != NULL) {
            b = p;
            if (data.nip < p->info.nip) p = p->kiri;
            else if (data.nip > p->info.nip) p = p->kanan;
            else {
                cout << "NIP udah terdaftar!\n";
                delete baru;
                return;
            }
        }
        
        if (data.nip < b->info.nip) b->kiri = baru;
        else b->kanan = baru;
        cout << "Data karyawan berhasil ditambahkan\n";
    }
}
// nampilin data karyawan inorder PTB
void cetakInOrder(NodeTree *node) {
    if (node != NULL) {
        cetakInOrder(node->kiri);
        
        cout << "\nNIP   : " << node->info.nip 
             << "\nNama  : " << node->info.nama 
             << "\nDivisi: " << node->info.divisi 
             << "\nStatus: ";
             
        if (node->info.statusCuti == 1) {
            cout << "Cuti\n";
        } else {
            cout << "Tidak Cuti\n";
        }
        
        cetakInOrder(node->kanan);
    }
}
void tampilData() {
    cout << "\n= DATA KARYAWAN =\n";
    if (akar == NULL) cout << "Belum ada data karyawan.\n";
    else cetakInOrder;
}
// ngajuin cuti
void pengajuanCuti() {
    int nip;
    cout << "\nMasukkan NIP untuk pengajuan cuti: "; cin >> nip;
    
    NodeTree *k = cariKaryawan(nip);
    if (k == NULL) {
        cout << "Karyawan tidak ditemukan!\n";
    } else {
        // kalau 0 boleh ngajuin cuti
        if (k->info.statusCuti == 0) {
            k->info.statusCuti = 1; // ubah jadi 1 (cuti)
            pushStack(1, nip); // masuk stack
            cout << "Pengajuan cuti berhasil\n";
        } else {
            // kalau udah 1, masuk antrian
            enqueue(nip); 
            cout << "Karyawan dalam antrian cuti\n";
        }
    }
}
// selesai cuti
void selesaiCuti() {
    int nip;
    cout << "\nMasukkan NIP yang selesai cuti: "; cin >> nip;
    
    NodeTree *k = cariKaryawan(nip);
    if (k == NULL) {
        cout << "Karyawan tidak ditemukan!\n";
    } else {
        // ngecek kalau dia cuti
        if (k->info.statusCuti == 1) {
            k->info.statusCuti = 0; // kembaliin ke 0 (nggak cuti)
            pushStack(2, nip); // masukin ke stack
            cout << "Cuti telah diselesaikan\n";
            
            // ngecek antrian 
            if (!queueKosong()) {
                int nipAntrian = dequeue();
                NodeTree *kAntrian = cariKaryawan(nipAntrian);
                if (kAntrian != NULL) {
                    kAntrian->info.statusCuti = 1; // jadiin cuti
                    cout << "Pengajuan cuti berikutnya langsung diproses dari antrian\n";
                }
            }
        } else {
            cout << "Karyawan sedang tidak cuti.\n";
        }
    }
}
// hapus karyawan
void hapusKaryawan() {
    int nip;
    cout << "\nMasukkan NIP yang ingin dipecat: "; cin >> nip;

    NodeTree *p = akar;
    NodeTree *b = NULL;
    int ketemu = 0;

    while (p != NULL) {
        if (nip == p->info.nip) { ketemu = 1; break; }
        b = p;
        if (nip < p->info.nip) p = p->kiri;
        else p = p->kanan;
    }

    if (ketemu == 0) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }
    NodeTree *temp;
    if (p->kiri == NULL && p->kanan == NULL) {
        if (b == NULL) akar = NULL;
        else if (p == b->kiri) b->kiri = NULL;
        else b->kanan = NULL;
        delete p;
    }
    else if (p->kiri != NULL && p->kanan != NULL) {
        temp = p->kiri;
        b = p;
        while (temp->kanan != NULL) {
            b = temp;
            temp = temp->kanan;
        }
        p->info = temp->info; 
        if (b == p) b->kiri = temp->kiri;
        else b->kanan = temp->kiri;
        delete temp;
    }
    else if (p->kiri != NULL && p->kanan == NULL) {
        if (b == NULL) akar = p->kiri;
        else if (p == b->kiri) b->kiri = p->kiri;
        else b->kanan = p->kiri;
        delete p;
    }
    else if (p->kiri == NULL && p->kanan != NULL) {
        if (b == NULL) akar = p->kanan;
        else if (p == b->kiri) b->kiri = p->kanan;
        else b->kanan = p->kanan;
        delete p;
    }
    cout << "Data karyawan berhasil dihapus.karyawan telah dipecat\n";
}
// undo status cuti
void undoAksi() {
    cout << "\n= UNDO status cuti karyawan =\n";
    if (stackKosong()) {
        cout << "Tidak ada aksi untuk di undo\n";
    } else {
        InfoUndo aksi = popStack();
        NodeTree *k = cariKaryawan(aksi.nip);
        
        if (k != NULL) {
            if (aksi.tipe == 1) { 
                k->info.statusCuti = 0; // batalin undo
                cout << "Undo: Status cuti karyawan dibatalkan\n";
            } else if (aksi.tipe == 2) { 
                k->info.statusCuti = 1; // menyelesaikan undo
                cout << "Undo: Status cuti karyawan dikembalikan menjadi cuti\n";
            }
        }
    }
}
// nampilin antrian cuti
void tampilAntrian() {
    cout << "\n= ANTRIAN CUTI =\n";
    if (queueKosong()) {
        cout << "Antrian kosong\n";
    } else {
        NodeQueue *bantu = depan;
        while (bantu != NULL) {
            NodeTree *k = cariKaryawan(bantu->nip);
            if (k != NULL) {
                cout << "- " << k->info.nama << " menunggu giliran cuti\n";
            }
            bantu = bantu->next;
        }
    }
}
int main() {
    int pilihan;
    do {
        cout << "\n=== SISTEM CUTI PT SYUDUDU ===";
        cout << "\n1. Tambah Karyawan";
        cout << "\n2. Tampil Karyawan";
        cout << "\n3. Pengajuan Cuti";
        cout << "\n4. Selesai Cuti";
        cout << "\n5. Hapus Karyawan";
        cout << "\n6. Undo status cuti";
        cout << "\n7. Tampil Antrian Cuti";
        cout << "\n0. Keluar";
        cout << "\nPilih menu: "; cin >> pilihan;

        switch (pilihan) {
            case 1: tambahData(); break;
            case 2: tampilData(); break;
            case 3: pengajuanCuti(); break;
            case 4: selesaiCuti(); break;
            case 5: hapusKaryawan(); break;
            case 6: undoAksi(); break;
            case 7: tampilAntrian(); break;
            case 0: cout << "Keluar dari program..\n"; break;
            default: cout << "Pilihan tidak tersedia!\n";
        }
    } while (pilihan != 0);
    return 0;
}
