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
int q1, q2;
int V, E;
int elementy=15, powtorzenia=50, licznik, iter, iter2, iter3;
int ciag[100001];
bool check, cykl;
double tme;
vector <pair <int, int>> krawedzie;
vector <vector <int>> macierz, nastepnik, poprzednik, brak;
vector <int> lista, wierzcholki;
vector <vector <float>> czasy;
vector <int> el={100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
stack <int> odwiedzone;
ifstream wejscie;
ofstream wyniki;
//--------------------------------------------------------------------------------ZMIENNE
bool sort2(pair <int, int> &a, pair <int, int> &b)
{
    return a.second<b.second;
}
void macierzsasiedztwa()
{
    macierz.clear();
    macierz.resize(V+1);
    for(int i=0; i<=V; i++) macierz[i].resize(V+1);
    for(int i=0; i<E; i++)
    {
        macierz[krawedzie[i].first][krawedzie[i].second]=1;
        macierz[krawedzie[i].second][krawedzie[i].first]=-1;
    }
}
void macierzgrafu()
{
    macierz.clear();
    nastepnik.clear();
    poprzednik.clear();
    brak.clear();
    wierzcholki.clear();
    macierz.resize(V+1);
    nastepnik.resize(V+1);
    poprzednik.resize(V+1);
    brak.resize(V+1);
    for(int i=0; i<=V; i++) macierz[i].resize(V+4);
    for(int i=0; i<E; i++)
    {
        nastepnik[krawedzie[i].first].push_back(krawedzie[i].second);
        poprzednik[krawedzie[i].second].push_back(krawedzie[i].first);
    }
    for(int i=1; i<=V; i++)
    {
        wierzcholki.resize(V+1);
        for(int j=0; j<nastepnik[i].size(); j++) wierzcholki[nastepnik[i][j]]=1;
        for(int j=0; j<poprzednik[i].size(); j++) wierzcholki[poprzednik[i][j]]=1;
        for(int j=1; j<=V; j++) if(wierzcholki[j]==0) brak[i].push_back(j);
        wierzcholki.clear();
    }
    for(int i=1; i<=V; i++)
    {
        if(nastepnik[i].size()!=0) macierz[i][V+1]=nastepnik[i][0];
        else macierz[i][V+1]=0;
        if(poprzednik[i].size()!=0) macierz[i][V+2]=poprzednik[i][0]+V;
        else macierz[i][V+2]=0;
        if(brak[i].size()!=0) macierz[i][V+3]=brak[i][0]*(-1);
        else macierz[i][V+3]=0;
        if(!nastepnik[i].empty())
        {
            iter2=nastepnik[i][0];
            for(int j=0; j<nastepnik[i].size()-1; j++)
            {
                iter=nastepnik[i][j];
                iter2=nastepnik[i][j+1];
                macierz[i][iter]=iter2;
            }
            macierz[i][iter2]=iter2;
        }
        if(!poprzednik[i].empty())
        {
            iter2=poprzednik[i][0];
            for(int j=0; j<poprzednik[i].size()-1; j++)
            {
                iter=poprzednik[i][j];
                iter2=poprzednik[i][j+1];
                macierz[i][iter]=iter2+V;
            }
            macierz[i][iter2]=iter2+V;
        }
        if(!brak[i].empty())
        {
            iter2=brak[i][0];
            for(int j=0; j<brak[i].size()-1; j++)
            {
                iter=brak[i][j];
                iter2=brak[i][j+1];
                macierz[i][iter]=iter2*(-1);
            }
            macierz[i][iter2]=iter2*(-1);
        }
    }
}
void DFSmsasiedztwa()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    for(int i=1; i<=V; i++)
    {
        check=0;
        if(wierzcholki[i]==0)
        {
            odwiedzone.push(i);
            while(!odwiedzone.empty())
            {
                while(wierzcholki[odwiedzone.top()]==2) odwiedzone.pop();
                iter=odwiedzone.top();
                wierzcholki[iter]=1;
                cykl=1;
                for(int j=V; j>=1; j--)
                {
                    if(macierz[iter][j]==1 && wierzcholki[j]==0)
                    {
                        odwiedzone.push(j);
                        cykl=0;
                    }
                    else if(macierz[iter][j]==1 && wierzcholki[j]==1) check=1;
                }
                if(cykl)
                {
                    lista.push_back(iter);
                    odwiedzone.pop();
                    wierzcholki[iter]=2;
                }
                if(check) break;
            }
            if(check) break;
        }
        if(check) break;
    }
    reverse(lista.begin(), lista.end());
}
void DFSmgrafu()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    for(int i=1; i<=V; i++)
    {
        check=0;
        if(wierzcholki[i]==0)
        {
            odwiedzone.push(i);
            while(!odwiedzone.empty())
            {
                while(wierzcholki[odwiedzone.top()]==2) odwiedzone.pop();
                iter=odwiedzone.top();
                wierzcholki[iter]=1;
                cykl=1;
                iter2=V+1;
                if(macierz[iter][iter2]!=0)
                {
                    while(macierz[iter][iter2]!=iter2)
                    {
                        if(wierzcholki[macierz[iter][iter2]]==0)
                        {
                            odwiedzone.push(macierz[iter][iter2]);
                            cykl=0;
                        }
                        else if(wierzcholki[macierz[iter][iter2]]==1) check=1;
                        iter2=macierz[iter][iter2];
                    }
                }
                if(cykl)
                {
                    lista.push_back(iter);
                    odwiedzone.pop();
                    wierzcholki[iter]=2;
                }
                if(check) break;
            }
            if(check) break;
        }
        if(check) break;
    }
    reverse(lista.begin(), lista.end());
}
void DELmsasiedztwa()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    for(int v=0; v<V; v++)
    {
        for(int i=1; i<=V; i++)
        {
            if(wierzcholki[i]==0)
            {
                check=1;
                for(int j=1; j<=V; j++) if(macierz[i][j]==-1) check=0;
                if(check)
                {
                    lista.push_back(i);
                    wierzcholki[i]=1;
                    for(int j=1; j<=V; j++)
                    {
                        macierz[j][i]=0;
                        macierz[i][j]=0;
                    }
                }
            }
        }
    }
}
void DELmgrafu()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    for(int v=0; v<V; v++)
    {
        for(int i=1; i<=V; i++)
        {
            if(macierz[i][V+2]==0 && wierzcholki[i]==0)
            {
                lista.push_back(i);
                wierzcholki[i]=1;
                iter=V+1;
                if(macierz[i][iter]!=0)
                {
                    while(macierz[i][iter]!=iter)
                    {
                        iter=macierz[i][iter];
                        if(macierz[iter][V+2]==i+V && macierz[iter][i]==i+V)
                        {
                            macierz[iter][V+2]=0;
                            macierz[iter][i]=i*(-1);
                        }
                        else if(macierz[iter][V+2]==i+V)
                        {
                            macierz[iter][V+2]=macierz[iter][i];
                            macierz[iter][i]=i*(-1);
                        }
                        else
                        {
                            iter2=i-1;
                            while(macierz[iter][iter2]!=i+V) iter2--;
                            if(macierz[iter][i]==i+V) macierz[iter][iter2]=iter2+V;
                            else
                            {
                                iter3=macierz[iter][i];
                                macierz[iter][iter2]=iter3;
                            }
                            macierz[iter][i]=i*(-1);
                        }
                    }
                }
                macierz[i][V+1]=0;
                macierz[i][V+3]=-1;
                for(int j=1; j<V; j++) macierz[i][j]=(j+1)*(-1);
                macierz[i][V]=V*(-1);
            }
        }
    }
}
//--------------------------------------------------------------------------------FUNKCJE
int main()
{
    srand(time(0));
    czasy.resize(4);
    for(int i=0; i<4; i++) czasy[i].resize(elementy);
    cout << fixed << setprecision(9);
    cout << "0. TESTY" << endl;
    cout << "1. GRAF Z PLIKU input.txt - PLIK POWINIEN MIEC POPRAWNE DANE" << endl;
    cout << "2. KONIEC PROGRAMU" << endl;
    cin >> q1;
    switch(q1)
    {
        case 0:
        {
            for(int ile_el=0; ile_el<elementy; ile_el++)
            {
                V=el[ile_el];
                E=V*(V-1)/4;
                for(int pwt=0; pwt<powtorzenia; pwt++)
                {
                    macierz.clear();
                    krawedzie.clear();
                    macierz.resize(V+1);
                    for(int i=0; i<=V; i++) macierz[i].resize(V+1);
                    for(int i=1; i<=V; i++) ciag[i]=i;
                    for(int i=0; i<V; i++)
                    {
                        int a1=rand()%V+1;
                        int a2=rand()%V+1;
                        swap(ciag[a1], ciag[a2]);
                    }
                    licznik=0;
                    while(licznik<E)
                    {
                        int a1=rand()%V+1;
                        int a2=rand()%V+1;
                        if(a1>a2) swap(a1, a2);
                        if(a1!=a2 && macierz[a1][a2]==0)
                        {
                            macierz[a1][a2]=1;
                            licznik++;
                            krawedzie.push_back(make_pair(ciag[a1], ciag[a2]));
                        }
                    }
                    sort(krawedzie.begin(), krawedzie.end(), sort2);
                    sort(krawedzie.begin(), krawedzie.end());
                    for(int i1=0; i1<1; i1++)
                    {
                        macierzsasiedztwa();
                        auto poc=high_resolution_clock::now();
                        DFSmsasiedztwa();
                        auto kon=high_resolution_clock::now();
                        auto rozn=duration_cast<nanoseconds>(kon-poc);
                        tme=float(rozn.count())/1000000000;
                        czasy[0][ile_el]=czasy[0][ile_el]+tme;
                    }
                    for(int i2=0; i2<1; i2++)
                    {
                        macierzgrafu();
                        auto poc=high_resolution_clock::now();
                        DFSmgrafu();
                        auto kon=high_resolution_clock::now();
                        auto rozn=duration_cast<nanoseconds>(kon-poc);
                        tme=float(rozn.count())/1000000000;
                        czasy[1][ile_el]=czasy[1][ile_el]+tme;
                    }
                    for(int i3=0; i3<1; i3++)
                    {
                        macierzsasiedztwa();
                        auto poc=high_resolution_clock::now();
                        DELmsasiedztwa();
                        auto kon=high_resolution_clock::now();
                        auto rozn=duration_cast<nanoseconds>(kon-poc);
                        tme=float(rozn.count())/1000000000;
                        czasy[2][ile_el]=czasy[2][ile_el]+tme;
                    }
                    for(int i4=0; i4<1; i4++)
                    {
                        macierzgrafu();
                        auto poc=high_resolution_clock::now();
                        DELmgrafu();
                        auto kon=high_resolution_clock::now();
                        auto rozn=duration_cast<nanoseconds>(kon-poc);
                        tme=float(rozn.count())/1000000000;
                        czasy[3][ile_el]=czasy[3][ile_el]+tme;
                    }
                }
                for(int i=0; i<4; i++) czasy[i][ile_el]=czasy[i][ile_el]/powtorzenia;
            }
            wyniki.open("wyniki.txt");
            for(int i=0; i<elementy; i++)
            {
                for(int j=0; j<4; j++) wyniki << czasy[j][i] << "\t";
                wyniki << endl;
            }
            wyniki.close();
            return 0;
        }
//--------------------------------------------------------------------------------TESTY
        case 1:
        {
            wejscie.open("input.txt");
            wejscie >> V >> E;
            for(int i=0; i<E; i++)
            {
                int v1, v2;
                wejscie >> v1;
                wejscie >> v2;
                krawedzie.push_back(make_pair(v1, v2));
            }
            wejscie.close();
            sort(krawedzie.begin(), krawedzie.end(), sort2);
            sort(krawedzie.begin(), krawedzie.end());
            cout << "WCZYTANO DANE Z PLIKU" << endl;
            while(1)
            {
                cout << "1. MACIERZ SASIEDZTWA I SORTOWANIE TARJANA (DFS)" << endl;
                cout << "2. MACIERZ GRAFU I SORTOWANIE TARJANA (DFS)" << endl;
                cout << "3. MACIERZ SASIEDZTWA I SORTOWANIE KAHNA (DEL)" << endl;
                cout << "4. MACIERZ GRAFU I SORTOWANIE KAHNA (DEL)" << endl;
                cout << "5. KONIEC PROGRAMU" << endl;
                cin >> q2;
                switch(q2)
                {
                    case 1:
                    {
                        macierzsasiedztwa();
                        DFSmsasiedztwa();
                        break;
                    }
                    case 2:
                    {
                        macierzgrafu();
                        DFSmgrafu();
                        break;
                    }
                    case 3:
                    {
                        macierzsasiedztwa();
                        DELmsasiedztwa();
                        break;
                    }
                    case 4:
                    {
                        macierzgrafu();
                        DELmgrafu();
                        break;
                    }
                    default: return 0;
                }
                if(lista.size()==V) for(int i=0; i<V; i++) cout << lista[i] << " ";
                else
                {
                    cout << "GRAF ZAWIERA CYKL. SORTOWANIE NIEMOZLIWE";
                    return 0;
                }
                cout << endl;
            }
        }
//--------------------------------------------------------------------------------GRAF Z PLIKU
        default: return 0;
    }
}
