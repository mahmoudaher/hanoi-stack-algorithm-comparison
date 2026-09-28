#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <limits>
#include <cstdint>
using namespace std;

char cubuk[] = { 'A', 'B', 'C' };
vector<stack<int>> yiginlar(3);

struct Sonuc
{
    uint64_t hareketSayisi;
    double sureMs;
};


void durumuYazdir() 
{
    vector<stack<int>> geciciYiginlar = yiginlar;
    int yukseklikler[3] = { (int)geciciYiginlar[0].size(), (int)geciciYiginlar[1].size(), (int)geciciYiginlar[2].size() };
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
    if (yiginlar[b].empty() || (!yiginlar[a].empty() && yiginlar[a].top() < yiginlar[b].top()))
    {
        int disk = yiginlar[a].top();
        if (adimlariYazdir)
        {
            cout << "Disk " << disk << " cubuk " << cubuk[a] << "'den cubuk " << cubuk[b] << "'ye hareket ettiriliyor.\n";
        }
        yiginlar[b].push(yiginlar[a].top());
        yiginlar[a].pop();
        ++hareketSayisi;
    }
    else {
        diskHareketi(b, a, hareketSayisi, adimlariYazdir);
    }

    if (adimlariYazdir)
    {
        durumuYazdir();
    }
}

void hanoiOzyinelemeli(int disk, char source, char target, char auxiliary, uint64_t& hareketSayisi, bool adimlariYazdir)
{
    if (disk == 1)
    {
        diskHareketi(source - 'A', target - 'A', hareketSayisi, adimlariYazdir);
        return;
    }
    hanoiOzyinelemeli(disk - 1, source, auxiliary, target, hareketSayisi, adimlariYazdir);
    diskHareketi(source - 'A', target - 'A', hareketSayisi, adimlariYazdir);
    hanoiOzyinelemeli(disk - 1, auxiliary, target, source, hareketSayisi, adimlariYazdir);
}

Sonuc hanoiKulesiOzyinelemeli(int n, bool adimlariYazdir)
{
    yiginlar = vector<stack<int>>(3);
    for (int i = n; i > 0; --i)
    {
        yiginlar[0].push(i);
    }

    if (adimlariYazdir)
    {
        cout << "Hanoi Kulesi (Ozyinelemeli) " << n << " disk icin:\n";
        durumuYazdir();
    }

    uint64_t hareketSayisi = 0;
    auto baslangic = chrono::high_resolution_clock::now();
    hanoiOzyinelemeli(n, 'A', 'C', 'B', hareketSayisi, adimlariYazdir);
    auto bitis = chrono::high_resolution_clock::now();

    return { hareketSayisi, chrono::duration<double, milli>(bitis - baslangic).count() };
}

void diskHareketiIteratif(vector<stack<int>>& kuleler, int a, int b, uint64_t& hareketSayisi)
{
    while (!kuleler[b].empty() && (kuleler[a].empty() || kuleler[a].top() > kuleler[b].top()))
    {
        swap(a, b);
    }

    kuleler[b].push(kuleler[a].top());
    kuleler[a].pop();
    ++hareketSayisi;
}

Sonuc hanoiKulesiIteratif(int n)
{
    vector<stack<int>> kuleler(3);
    for (int i = n; i > 0; --i)
    {
        kuleler[0].push(i);
    }

    int kaynak = 0;
    int yardimci = 1;
    int hedef = 2;
    if (n % 2 == 0)
    {
        swap(yardimci, hedef);
    }

    uint64_t hareketSayisi = 0;
    auto baslangic = chrono::high_resolution_clock::now();
    uint64_t toplamHareket = (uint64_t(1) << n) - 1;
    for (uint64_t i = 1; i <= toplamHareket; ++i)
    {
        if (i % 3 == 0)
        {
            diskHareketiIteratif(kuleler, yardimci, hedef, hareketSayisi);
        }
        else if (i % 3 == 1)
        {
            diskHareketiIteratif(kuleler, kaynak, hedef, hareketSayisi);
        }
        else
        {
            diskHareketiIteratif(kuleler, kaynak, yardimci, hareketSayisi);
        }
    }
    auto bitis = chrono::high_resolution_clock::now();

    return { hareketSayisi, chrono::duration<double, milli>(bitis - baslangic).count() };
}

bool diskSayisiGecerli(int n)
{
    return n >= 1 && n <= 20;
}

void sonucuYazdir(const char* yontem, const Sonuc& sonuc)
{
    cout << left << setw(18) << yontem
         << right << setw(15) << sonuc.hareketSayisi
         << setw(18) << fixed << setprecision(3) << sonuc.sureMs << " ms\n";
}

void karsilastir(int n)
{
    Sonuc ozyinelemeli = hanoiKulesiOzyinelemeli(n, false);
    Sonuc iteratif = hanoiKulesiIteratif(n);

    cout << "\n" << n << " disk icin karsilastirma\n";
    cout << left << setw(18) << "Yontem" << right << setw(15) << "Hareket" << setw(18) << "Sure\n";
    cout << string(51, '-') << "\n";
    sonucuYazdir("Ozyinelemeli", ozyinelemeli);
    sonucuYazdir("Iteratif", iteratif);
    cout << "Teorik hareket sayisi: " << ((uint64_t(1) << n) - 1) << "\n\n";
}

void baslat()   
{
    int secim;
    int diskSayisi;

    do {
        cout << "             Hanoi Karsilastirma        " << endl << endl;
        cout << "1. Ozyinelemeli yontemi adimlarla calistir\n";
        cout << "2. Disk sayisini girip ozyinelemeli yontemi calistir\n";
        cout << "3. Ozyinelemeli ve iteratif yontemleri karsilastir\n";
        cout << "4. Cikis\n";
        cout << "Seciminizi: ";
        cin >> secim;

        switch (secim) {
        case 1:
            hanoiKulesiOzyinelemeli(3, true);
            break;
        case 2:
            cout << "Disk sayisini girin: ";
            cin >> diskSayisi;
            if (diskSayisiGecerli(diskSayisi))
            {
                hanoiKulesiOzyinelemeli(diskSayisi, true);
            }
            else
            {
                cout << "Disk sayisi 1 ile 20 arasinda olmalidir.\n";
            }
            break;
        case 3:
            cout << "Disk sayisini girin (1-20): ";
            cin >> diskSayisi;
            if (diskSayisiGecerli(diskSayisi))
            {
                karsilastir(diskSayisi);
            }
            else
            {
                cout << "Disk sayisi 1 ile 20 arasinda olmalidir.\n";
            }
            break;
        case 4:
            cout << "Programdan cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim! Lutfen tekrar deneyin.\n";
            break;
        }
    } while (secim != 4);
}



int main() 
{
    baslat();

    system("pause>0");
}
