#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
using namespace std;

char cubuk[] = { 'A', 'B', 'C' };
vector<stack<int>> yiginlar(3);

struct Sonuc
{
    uint64_t hareketSayisi;
    double sureMs;
};

void durumuYazdir() {
    vector<stack<int>> geciciYiginlar = yiginlar;
    int yukseklikler[3] = { (int)geciciYiginlar[0].size(), (int)geciciYiginlar[1].size(), (int)geciciYiginlar[2].size()};
    int maxYukseklik = max(yukseklikler[0], max(yukseklikler[1], yukseklikler[2]));

    for (int i = maxYukseklik - 1; i >= 0; --i) 
    {
        for (int j = 0; j < 3; ++j) 
        {
            if (i < yukseklikler[j])
            {
                cout << " " << geciciYiginlar[j].top() << " ";
                geciciYiginlar[j].pop();
            }
            else 
            {
                cout << " | ";
            }
        }
        cout << endl;
    }
    cout << "___ ___ ___\n";
    cout << " A   B   C \n\n";
}

void diskHareketi(int a, int b, uint64_t& hareketSayisi, bool adimlariYazdir)
{
    while (!yiginlar[b].empty() && (yiginlar[a].empty() || yiginlar[a].top() > yiginlar[b].top()))
    {
        swap(a, b);
    }
    int disk = yiginlar[a].top();
    if (adimlariYazdir)
    {
        cout << "Disk " << disk << " cubuk " << cubuk[a] << "'den cubuk " << cubuk[b] << "'ye hareket ettiriliyor.\n";
    }
    yiginlar[b].push(disk);
    yiginlar[a].pop();
    ++hareketSayisi;

    if (adimlariYazdir)
    {
        durumuYazdir();
    }
}

Sonuc hanoiKulesiIteratif(int n, bool adimlariYazdir)
{
    yiginlar = vector<stack<int>>(3);
    if (adimlariYazdir)
    {
        cout << "Hanoi Kulesi (Iteratif) " << n << " disk icin:\n";
    }

    int kaynak = 0, yardimci = 1, hedef = 2;
    for (int i = n; i > 0; i--) 
    {
        yiginlar[kaynak].push(i);
    }

    if (adimlariYazdir)
    {
        durumuYazdir();
    }

    uint64_t toplamHareket = (uint64_t(1) << n) - 1;
    uint64_t hareketSayisi = 0;
    auto baslangic = chrono::high_resolution_clock::now();
    if (n % 2 == 0) {
        swap(yardimci, hedef);
    }

    for (uint64_t i = 1; i <= toplamHareket; ++i)
    {
        if (i % 3 == 0) {
            diskHareketi(yardimci, hedef, hareketSayisi, adimlariYazdir);
        }
        else if (i % 3 == 1) {
            diskHareketi(kaynak, hedef, hareketSayisi, adimlariYazdir);
        }
        else {
            diskHareketi(kaynak, yardimci, hareketSayisi, adimlariYazdir);
        }
    }
    auto bitis = chrono::high_resolution_clock::now();
    return { hareketSayisi, chrono::duration<double, milli>(bitis - baslangic).count() };
}

void baslat()
{
    int secim;
    int diskSayisi = 3;

    do {
        cout << "             Iteratif Yontemi        " << endl << endl << endl;
        cout << "1. Sadece 3 diskli Hanoi Kulesi Baslat\n";
        cout << "2. Disk sayisini Giriniz\n";
        cout << "3. Cikis\n";
        cout << "Seciminizi: ";
        cin >> secim;

        switch (secim)
        {
        case 1:
        {
            Sonuc sonuc = hanoiKulesiIteratif(3, true);
            cout << "Toplam hareket: " << sonuc.hareketSayisi << " | Sure: "
                 << fixed << setprecision(3) << sonuc.sureMs << " ms\n";
            break;
        }
        case 2:
            cout << "Disk sayisini girin: ";
            cin >> diskSayisi;
            if (diskSayisi < 1 || diskSayisi > 20)
            {
                cout << "Disk sayisi 1 ile 20 arasinda olmalidir.\n";
                break;
            }
            {
                Sonuc sonuc = hanoiKulesiIteratif(diskSayisi, true);
                cout << "Toplam hareket: " << sonuc.hareketSayisi << " | Sure: "
                     << fixed << setprecision(3) << sonuc.sureMs << " ms\n";
            }
            break;
        case 3:
            cout << "Programdan cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim! Lutfen tekrar deneyin.\n";
            break;
        }
    } while (secim != 3);
}



int main()
{


    baslat();

    system("pause>0");
}
