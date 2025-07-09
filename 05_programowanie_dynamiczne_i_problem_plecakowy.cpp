#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <chrono>
#include <stack>
using namespace std;
using namespace std::chrono;
//--------------------------------------------------------------------------------BIBLIOTEKI
struct item
{
    int waga, cena, id;
    double stosunek;
    bool wybrany;
};
int q1, q2;
int N, C, S, M; // N-number; C-capacity; S-sum; M-mass
int elementy=15, powtorzenia=50, /*3*/ pojemnosci=5;
long long iter, iter2;
int ciag[100001];
double tme;
vector <item> PRZEDMIOTY, przedmioty;
vector <vector <int>> macierz;
vector <vector <vector <float>>> czasy;
vector <int> el={100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
vector <int> poj={50, 75, 100, 125, 150};
ifstream wejscie;
ofstream wyniki;
//--------------------------------------------------------------------------------ZMIENNE
bool sort2(item &a, item &b)
{
    return a.stosunek>b.stosunek;
}
long long potega(int n)
{
    long long p=1;
    while(n--) p=p*2;
    return p;
}
void programowaniedynamiczne()
{
    przedmioty=PRZEDMIOTY;
    S=0;
    M=0;
    macierz.resize(N+1);
    for(int i=0; i<=N; i++) macierz[i].resize(C+1);
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=C; j++)
        {
            if(przedmioty[i].waga>j) macierz[i][j]=macierz[i-1][j];
            else if(macierz[i-1][j-przedmioty[i].waga]+przedmioty[i].cena>macierz[i-1][j]) macierz[i][j]=macierz[i-1][j-przedmioty[i].waga]+przedmioty[i].cena;
            else macierz[i][j]=macierz[i-1][j];
        }
    }
    S=macierz[N][C];
    iter=N;
    iter2=C;
    while(iter!=0)
    {
        if(macierz[iter][iter2]>macierz[iter-1][iter2])
        {
            przedmioty[iter].wybrany=1;
            M=M+przedmioty[iter].waga;
            iter2=iter2-przedmioty[iter].waga;
            iter--;
        }
        else iter--;
    }
    macierz.clear();
}
void zachlanny()
{
    przedmioty=PRZEDMIOTY;
    S=0;
    M=0;
    sort(przedmioty.begin(), przedmioty.end(), sort2);
    iter=0;
    while(iter<N && M<C)
    {
        if(przedmioty[iter].waga+M<=C)
        {
            M=M+przedmioty[iter].waga;
            S=S+przedmioty[iter].cena;
            przedmioty[iter].wybrany=1;
        }
        iter++;
    }
}
void silowy()
{
    przedmioty=PRZEDMIOTY;
    S=0;
    int Stym;
    long long X;
    for(long long i=1; i<potega(N); i++)
    {
        iter=i;
        iter2=0;
        Stym=0;
        M=0;
        while(iter>0)
        {
            if(iter%2==1)
            {
                M=M+przedmioty[iter2].waga;
                Stym=Stym+przedmioty[iter2].cena;
            }
            if(M>C) break;
            iter2++;
            iter=iter/2;
        }
        if(M<=C && Stym>S)
        {
            S=Stym;
            X=i;
        }
    }
    iter=0;
    M=0;
    while(X>0)
    {
        if(X%2==1)
        {
            przedmioty[iter].wybrany=1;
            M=M+przedmioty[iter].waga;
        }
        iter++;
        X=X/2;
    }
}
//--------------------------------------------------------------------------------FUNKCJE
int main()
{
    srand(time(0));
    czasy.resize(3);
    for(int i=0; i<3; i++) czasy[i].resize(pojemnosci);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<pojemnosci; j++) czasy[i][j].resize(elementy);
    }
    cout << "0. TESTY" << endl;
    cout << "1. DANE Z PLIKU input.txt - PLIK POWINIEN MIEC POPRAWNE DANE" << endl;
    cout << "2. KONIEC PROGRAMU" << endl;
    cin >> q1;
    switch(q1)
    {
        case 0:
        {
            for(int ile_el=0; ile_el<elementy; ile_el++)
            {
                for(int jaka_poj=0; jaka_poj<pojemnosci; jaka_poj++)
                {
                    for(int pwt=0; pwt<powtorzenia; pwt++)
                    {
                        PRZEDMIOTY.clear();
                        N=el[ile_el];
                        C=N*poj[jaka_poj]/100;
                        cout << pwt << " " << N << " " << C << " ";
                        for(int i=1; i<=N; i++)
                        {
                            item X;
                            X.waga=(rand()%N)/2+1;
                            X.cena=(rand()%N)/2+1;
                            X.id=i;
                            X.wybrany=0;
                            X.stosunek=double(X.cena)/double(X.waga);
                            PRZEDMIOTY.push_back(X);
                        }
                        for(int petla=0; petla<1; petla++)
                        {
                            auto poc=high_resolution_clock::now();
                            programowaniedynamiczne();
                            auto kon=high_resolution_clock::now();
                            auto rozn=duration_cast<nanoseconds>(kon-poc);
                            tme=double(rozn.count())/1000000000;
                            czasy[0][jaka_poj][ile_el]=czasy[0][jaka_poj][ile_el]+tme;
                        }
                        for(int petla=0; petla<1; petla++)
                        {
                            auto poc=high_resolution_clock::now();
                            zachlanny();
                            auto kon=high_resolution_clock::now();
                            auto rozn=duration_cast<nanoseconds>(kon-poc);
                            tme=double(rozn.count())/1000000000;
                            czasy[1][jaka_poj][ile_el]=czasy[1][jaka_poj][ile_el]+tme;
                        }
                        N=(ile_el+1)*2;
                        C=N*poj[jaka_poj]/100;
                        cout << N << " " << C << endl;
                        PRZEDMIOTY.clear();
                        for(int i=1; i<=N; i++)
                        {
                            item X;
                            X.waga=(rand()%N)/2+1;
                            X.cena=(rand()%N)/2+1;
                            X.id=i;
                            X.wybrany=0;
                            X.stosunek=double(X.cena)/double(X.waga);
                            PRZEDMIOTY.push_back(X);
                        }
                        for(int petla=0; petla<1; petla++)
                        {
                            auto poc=high_resolution_clock::now();
                            silowy();
                            auto kon=high_resolution_clock::now();
                            auto rozn=duration_cast<nanoseconds>(kon-poc);
                            tme=double(rozn.count())/1000000000;
                            czasy[2][jaka_poj][ile_el]=czasy[2][jaka_poj][ile_el]+tme;
                        }
                    }
                }
            }
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<pojemnosci; j++)
                {
                    for(int k=0; k<elementy; k++) czasy[i][j][k]=czasy[i][j][k]/double(powtorzenia);
                }
            }
            wyniki.open("wyniki.txt");
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<pojemnosci; j++)
                {
                    for(int k=0; k<elementy; k++) wyniki << czasy[i][j][k] << "\t";
                    wyniki << endl;
                }
                wyniki << endl << endl << endl;
            }
            wyniki.close();
            return 0;
        }
//--------------------------------------------------------------------------------TESTY
        case 1:
        {
            wejscie.open("input.txt");
            wejscie >> N >> C;
            for(int i=1; i<=N; i++)
            {
                item X;
                wejscie >> X.waga;
                wejscie >> X.cena;
                X.id=i;
                X.wybrany=0;
                X.stosunek=double(X.cena)/double(X.waga);
                PRZEDMIOTY.push_back(X);
            }
            wejscie.close();
            cout << "WCZYTANO DANE Z PLIKU" << endl;
            while(1)
            {
                cout << "1. ALGORYTM PROGRAMOWANIA DYNAMICZNEGO" << endl;
                cout << "2. ALGORYTM ZACHLANNY" << endl;
                cout << "3. ALGORYTM SILOWY" << endl;
                cout << "4. KONIEC PROGRAMU" << endl;
                cin >> q2;
                switch(q2)
                {
                    case 1:
                    {
                        programowaniedynamiczne();
                        break;
                    }
                    case 2:
                    {
                        zachlanny();
                        break;
                    }
                    case 3:
                    {
                        silowy();
                        break;
                    }
                    default: return 0;
                }
                cout << endl << "WYBRANE PRZEDMIOTY:" << endl;
                for(int i=0; i<N; i++) if(przedmioty[i].wybrany) cout << przedmioty[i].id << ".\tWAGA: " << przedmioty[i].waga << "\tCENA: " << przedmioty[i].cena << endl;
                cout << endl << "SUMA: " << S << endl << "WAGA: " << M << endl << endl;
            }
        }
//--------------------------------------------------------------------------------DANE Z PLIKU
        default: return 0;
    }
}
