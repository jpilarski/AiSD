#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <chrono>
#include <stack>
#include <windows.h>
using namespace std;
using namespace std::chrono;
//--------------------------------------------------------------------------------BIBLIOTEKI
int q1, q2;
int V, E;
int elementy=17, nasycenia=9, powtorzenia=3, licznik, iter, iter_a, iter_b;
int ciag[101];
bool cykl;
double tme, czas, limit=30;
vector <pair <int, int>> krawedzie;
vector <vector <int>> macierz, nastepnik;
vector <int> lista, wierzcholki;
vector <vector <vector <double>>> czasy;
vector <vector <vector <int>>> ilosci;
vector <int> el={20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
vector <int> nas={10, 20, 30, 40, 50, 60, 70, 80, 90};
vector <vector <int>> kraweul;
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
        macierz[krawedzie[i].second][krawedzie[i].first]=1;
    }
}
void listanastepnikow()
{
    nastepnik.clear();
    nastepnik.resize(V+1);
    for(int i=0; i<E; i++) nastepnik[krawedzie[i].first].push_back(krawedzie[i].second);
}
bool Hamilton(int v, bool type)
{
    if((clock()-czas)/CLOCKS_PER_SEC>limit) // LIMIT
    {
        return 0;
    }
    wierzcholki[v]=1;
    iter++;
    if(type)
    {
        if(!nastepnik[v].empty())
        {
            for(int i=0; i<nastepnik[v].size(); i++)
            {
                if(nastepnik[v][i]==1 && iter==V)
                {
                    lista.push_back(v);
                    return 1;
                }
                if(wierzcholki[nastepnik[v][i]]==0)
                {
                    if(Hamilton(nastepnik[v][i], type))
                    {
                        lista.push_back(v);
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        for(int i=1; i<=V; i++)
        {
            if(macierz[v][i])
            {
                if(i==1 && iter==V)
                {
                    lista.push_back(v);
                    return 1;
                }
                if(wierzcholki[i]==0)
                {
                    if(Hamilton(i, type))
                    {
                        lista.push_back(v);
                        return 1;
                    }
                }
            }
        }
    }
    wierzcholki[v]=0;
    iter--;
    return 0;
}
void HamiltonN()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    lista.push_back(1);
    iter=0;
    cykl=Hamilton(1, 0);
    if(cykl==0) cout << "\tERROR\t";
    reverse(lista.begin(), lista.end());
}
void HamiltonS()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    lista.push_back(1);
    iter=0;
    cykl=Hamilton(1, 1);
    if(cykl==0) cout << "\tERROR\t";
    reverse(lista.begin(), lista.end());
}
void Euler(int v, bool type)
{
    if(type)
    {
        if(!nastepnik[v].empty())
        {
            while(!nastepnik[v].empty())
            {
                iter=nastepnik[v][0];
                nastepnik[v].erase(nastepnik[v].begin());
                Euler(iter, type);
            }
        }
    }
    else
    {
        for(int i=1; i<=V; i++)
        {
            if(macierz[v][i]==1)
            {
                iter=i;
                macierz[v][i]=0;
                macierz[i][v]=0;
                Euler(i, type);
            }
        }
    }
    lista.push_back(v);
}
void EulerN()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    cykl=1;
    for(int i=1; i<=V; i++)
    {
        iter=0;
        for(int j=1; j<=V; j++)
        {
            if(macierz[i][j]==1) iter++;
        }
        if(iter%2==1) cykl=0;
    }
    odwiedzone.push(1);
    while(!odwiedzone.empty())
    {
        iter=odwiedzone.top();
        wierzcholki[iter]=1;
        odwiedzone.pop();
        for(int i=1; i<=V; i++)
        {
            if(macierz[iter][i]==1 && wierzcholki[i]==0)
            {
                wierzcholki[i]=1;
                odwiedzone.push(i);
            }
        }
    }
    for(int i=1; i<=V; i++) if(wierzcholki[i]==0) cykl=0;
    if(cykl) Euler(1, 0);
    if(cykl==0) cout << "\tERROR\t";
    reverse(lista.begin(), lista.end());
}
void EulerS()
{
    wierzcholki.clear();
    lista.clear();
    wierzcholki.resize(V+1);
    cykl=1;
    for(int i=1; i<=V; i++)
    {
        if(!nastepnik[i].empty())
        {
            for(int j=0; j<nastepnik[i].size(); j++) wierzcholki[nastepnik[i][j]]++;
        }
    }
    for(int i=1; i<=V; i++)
    {
        if(!nastepnik.empty())
        {
            if(wierzcholki[i]!=nastepnik[i].size()) cykl=0;
        }
        else if(wierzcholki[i]>0) cykl=0;
    }
    wierzcholki.clear();
    wierzcholki.resize(V+1);
    odwiedzone.push(1);
    while(!odwiedzone.empty())
    {
        iter=odwiedzone.top();
        wierzcholki[iter]=1;
        odwiedzone.pop();
        if(!nastepnik[iter].empty())
        {
            for(int i=0; i<nastepnik[iter].size(); i++)
            {
                if(wierzcholki[nastepnik[iter][i]]==0)
                {
                    wierzcholki[nastepnik[iter][i]]=1;
                    odwiedzone.push(nastepnik[iter][i]);
                }
            }
        }
    }
    for(int i=1; i<=V; i++) if(wierzcholki[i]==0) cykl=0;
    if(cykl) Euler(1, 1);
    if(cykl==0) cout << "\tERROR\t";
    reverse(lista.begin(), lista.end());
}
//--------------------------------------------------------------------------------FUNKCJE
int main()
{
    srand(time(0));
    czasy.resize(4);
    ilosci.resize(4);
    for(int i=0; i<4; i++)
    {
        czasy[i].resize(nasycenia);
        ilosci[i].resize(nasycenia);
    }
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<nasycenia; j++)
        {
            czasy[i][j].resize(elementy);
            ilosci[i][j].resize(elementy);
        }
    }
    cout << fixed << setprecision(3);
    cout << "0. TESTY" << endl;
    cout << "1. GRAF Z PLIKU input.txt - PLIK POWINIEN MIEC POPRAWNE DANE" << endl;
    cout << "2. KONIEC PROGRAMU" << endl;
    cin >> q1;
    switch(q1)
    {
        case 0:
        {
            kraweul.resize(17);
            kraweul[0]={20, 38, 57, 76, 95, 113, 133, 152, 171};
			kraweul[1]={30, 60, 90, 119, 150, 180, 210, 239, 268};
			kraweul[2]={43, 87, 130, 174, 217, 261, 304, 347, 390};
			kraweul[3]={59, 119, 178, 238, 297, 356, 416, 477, 538};
			kraweul[4]={78, 156, 233, 312, 390, 468, 545, 624, 700};
			kraweul[5]={99, 197, 297, 396, 495, 594, 694, 792, 893};
			kraweul[6]={122, 245, 367, 489, 613, 735, 857, 979, 1102};
			kraweul[7]={148, 297, 445, 594, 742, 891, 1039, 1187, 1331};
			kraweul[8]={177, 354, 531, 708, 885, 1062, 1239, 1417, 1593};
			kraweul[9]={208, 416, 623, 832, 1040, 1248, 1455, 1664, 1869};
			kraweul[10]={241, 483, 724, 966, 1207, 1449, 1690, 1932, 2170};
			kraweul[11]={277, 555, 833, 1109, 1387, 1665, 1943, 2219, 2497};
			kraweul[12]={316, 632, 948, 1264, 1579, 1896, 2211, 2527, 2844};
			kraweul[13]={357, 714, 1071, 1428, 1785, 2141, 2498, 2857, 3212};
			kraweul[14]={400, 801, 1201, 1602, 2002, 2403, 2803, 3204, 3604};
			kraweul[15]={446, 892, 1339, 1786, 2232, 2679, 3125, 3572, 4014};
			kraweul[16]={495, 989, 1485, 1979, 2475, 2969, 3465, 3959, 4444};
            for(int ile_el=0; ile_el<elementy; ile_el++)
            {
                V=el[ile_el];
                for(int jakie_nas=0; jakie_nas<nasycenia; jakie_nas++)
                {
                    for(int pwt=0; pwt<powtorzenia; pwt++)
                    {
                        cout << V << "\t" << nas[jakie_nas] << "\t";
                        for(int petla=0; petla<1; petla++)
                        {
                            cout << "HN\t";
                            E=V*(V-1)/2;
                            E=E*nas[jakie_nas]/100;
                            if(E<V) E=V;
                            macierz.clear();
                            krawedzie.clear();
                            macierz.resize(V+1);
                            for(int i=0; i<=V; i++) macierz[i].resize(V+1);
                            for(int i=1; i<=V; i++) ciag[i]=i;
                            for(int i=0; i<E; i++)
                            {
                                iter_a=rand()%V+1;
                                iter_b=rand()%V+1;
                                swap(ciag[iter_a], ciag[iter_b]);
                            }
                            licznik=0;
                            for(int i=1; i<V; i++)
                            {
                                krawedzie.push_back(make_pair(ciag[i], ciag[i+1]));
                                macierz[ciag[i]][ciag[i+1]]=1;
                                macierz[ciag[i+1]][ciag[i]]=1;
                                licznik++;
                            }
                            krawedzie.push_back(make_pair(ciag[V], ciag[1]));
                            macierz[ciag[V]][ciag[1]]=1;
                            macierz[ciag[1]][ciag[V]]=1;
                            licznik++;
                            iter_a=1, iter_b=1;
                            while(licznik<E)
                            {
                                do
                                {
                                    iter_b++;
                                    if(iter_b>V) iter_b=1;
                                }
                                while(macierz[ciag[iter_a]][ciag[iter_b]]==1);
                                krawedzie.push_back(make_pair(ciag[iter_a], ciag[iter_b]));
                                macierz[ciag[iter_a]][ciag[iter_b]]=1;
                                macierz[ciag[iter_b]][ciag[iter_a]]=1;
                                licznik++;
                                iter_a=iter_b;
                            }
                            sort(krawedzie.begin(), krawedzie.end(), sort2);
                            sort(krawedzie.begin(), krawedzie.end());
                            macierzsasiedztwa();
                            czas=clock();
                            auto poc=high_resolution_clock::now();
                            HamiltonN();
                            auto kon=high_resolution_clock::now();
                            auto rozn=duration_cast<nanoseconds>(kon-poc);
                            cout << (clock()-czas)/CLOCKS_PER_SEC << "\t";
                            tme=double(rozn.count())/1000000000;
                            if(cykl)
                            {
                                czasy[0][jakie_nas][ile_el]=czasy[0][jakie_nas][ile_el]+tme;
                                ilosci[0][jakie_nas][ile_el]++;
                            }
                        }
//--------------------------------------------------------------------------------HAMILTON NIESKIEROWANY
                        for(int petla=0; petla<1; petla++)
                        {
                            cout << "HS\t";
                            E=V*(V-1);
                            E=E*nas[jakie_nas]/100;
                            if(E<V) E=V;
                            macierz.clear();
                            krawedzie.clear();
                            macierz.resize(V+1);
                            for(int i=0; i<=V; i++) macierz[i].resize(V+1);
                            for(int i=1; i<=V; i++) ciag[i]=i;
                            for(int i=0; i<E; i++)
                            {
                                iter_a=rand()%V+1;
                                iter_b=rand()%V+1;
                                swap(ciag[iter_a], ciag[iter_b]);
                            }
                            licznik=0;
                            for(int i=1; i<V; i++)
                            {
                                krawedzie.push_back(make_pair(ciag[i], ciag[i+1]));
                                macierz[ciag[i]][ciag[i+1]]=1;
                                licznik++;
                            }
                            krawedzie.push_back(make_pair(ciag[V], ciag[1]));
                            macierz[ciag[V]][ciag[1]]=1;
                            licznik++;
                            iter_a=1, iter_b=1;
                            while(licznik<E/2)
                            {
                                do
                                {
                                    iter_b++;
                                    if(iter_b>V) iter_b=1;
                                }
                                while(macierz[ciag[iter_a]][ciag[iter_b]]==1);
                                krawedzie.push_back(make_pair(ciag[iter_a], ciag[iter_b]));
                                macierz[ciag[iter_a]][ciag[iter_b]]=1;
                                licznik++;
                                iter_a=iter_b;
                            }
                            iter_a=1, iter_b=1;
                            while(licznik<E)
                            {
                                do
                                {
                                    iter_b--;
                                    if(iter_b<1) iter_b=V;
                                }
                                while(macierz[ciag[iter_a]][ciag[iter_b]]==1);
                                krawedzie.push_back(make_pair(ciag[iter_a], ciag[iter_b]));
                                macierz[ciag[iter_a]][ciag[iter_b]]=1;
                                licznik++;
                                iter_a=iter_b;
                            }
                            sort(krawedzie.begin(), krawedzie.end(), sort2);
                            sort(krawedzie.begin(), krawedzie.end());
                            listanastepnikow();
                            czas=clock();
                            auto poc=high_resolution_clock::now();
                            HamiltonS();
                            auto kon=high_resolution_clock::now();
                            auto rozn=duration_cast<nanoseconds>(kon-poc);
                            cout << (clock()-czas)/CLOCKS_PER_SEC << "\t";
                            tme=double(rozn.count())/1000000000;
                            if(cykl)
                            {
                                czasy[1][jakie_nas][ile_el]=czasy[1][jakie_nas][ile_el]+tme;
                                ilosci[1][jakie_nas][ile_el]++;
                            }
                        }
//--------------------------------------------------------------------------------HAMILTON SKIEROWANY
                        for(int petla=0; petla<1; petla++)
                        {
                            cout << "EN\t";
                            E=kraweul[ile_el][jakie_nas];
                            macierz.clear();
                            krawedzie.clear();
                            macierz.resize(V+1);
                            for(int i=0; i<=V; i++) macierz[i].resize(V+1);
                            for(int i=1; i<=V; i++) ciag[i]=i;
                            for(int i=0; i<E; i++)
                            {
                                iter_a=rand()%V+1;
                                iter_b=rand()%V+1;
                                swap(ciag[iter_a], ciag[iter_b]);
                            }
                            licznik=0;
                            for(int i=1; i<V; i++)
                            {
                                krawedzie.push_back(make_pair(ciag[i], ciag[i+1]));
                                macierz[ciag[i]][ciag[i+1]]=1;
                                macierz[ciag[i+1]][ciag[i]]=1;
                                licznik++;
                            }
                            krawedzie.push_back(make_pair(ciag[V], ciag[1]));
                            macierz[ciag[V]][ciag[1]]=1;
                            macierz[ciag[1]][ciag[V]]=1;
                            licznik++;
                            iter_a=1, iter_b=1;
                            while(licznik+2<E)
                            {
                                do
                                {
                                    iter_b++;
                                    if(iter_b>V) iter_b=1;
                                }
                                while(macierz[ciag[iter_a]][ciag[iter_b]]==1);
                                krawedzie.push_back(make_pair(ciag[iter_a], ciag[iter_b]));
                                macierz[ciag[iter_a]][ciag[iter_b]]=1;
                                macierz[ciag[iter_b]][ciag[iter_a]]=1;
                                licznik++;
                                iter_a=iter_b;
                            }
                            for(int i=2; i<=V; i++)
                            {
                                if(macierz[ciag[iter_a]][ciag[i]]==0 && macierz[ciag[i]][ciag[1]]==0 && i!=iter_a && licznik<E)
                                {
                                    krawedzie.push_back(make_pair(ciag[iter_a], ciag[i]));
                                    macierz[ciag[iter_a]][ciag[i]]=1;
                                    macierz[ciag[i]][ciag[iter_a]]=1;
                                    licznik++;
                                    krawedzie.push_back(make_pair(ciag[i], ciag[1]));
                                    macierz[ciag[i]][ciag[1]]=1;
                                    macierz[ciag[1]][ciag[i]]=1;
                                    licznik++;
                                    break;
                                }
                            }
                            sort(krawedzie.begin(), krawedzie.end(), sort2);
                            sort(krawedzie.begin(), krawedzie.end());
                            macierzsasiedztwa();
                            auto poc=high_resolution_clock::now();
                            EulerN();
                            auto kon=high_resolution_clock::now();
                            auto rozn=duration_cast<nanoseconds>(kon-poc);
                            tme=double(rozn.count())/1000000000;
                            if(cykl)
                            {
                                czasy[2][jakie_nas][ile_el]=czasy[2][jakie_nas][ile_el]+tme;
                                ilosci[2][jakie_nas][ile_el]++;
                            }
                        }
//--------------------------------------------------------------------------------EULER NIESKIEROWANY
                        for(int petla=0; petla<1; petla++)
                        {
                            cout << "ES\t";
                            E=kraweul[ile_el][jakie_nas]*2;
                            macierz.clear();
                            krawedzie.clear();
                            macierz.resize(V+1);
                            for(int i=0; i<=V; i++) macierz[i].resize(V+1);
                            for(int i=1; i<=V; i++) ciag[i]=i;
                            for(int i=0; i<E; i++)
                            {
                                iter_a=rand()%V+1;
                                iter_b=rand()%V+1;
                                swap(ciag[iter_a], ciag[iter_b]);
                            }
                            licznik=0;
                            for(int i=1; i<V; i++)
                            {
                                krawedzie.push_back(make_pair(ciag[i], ciag[i+1]));
                                krawedzie.push_back(make_pair(ciag[i+1], ciag[i]));
                                macierz[ciag[i]][ciag[i+1]]=1;
                                macierz[ciag[i+1]][ciag[i]]=1;
                                licznik++;
                                licznik++;
                            }
                            krawedzie.push_back(make_pair(ciag[V], ciag[1]));
                            krawedzie.push_back(make_pair(ciag[1], ciag[V]));
                            macierz[ciag[V]][ciag[1]]=1;
                            macierz[ciag[1]][ciag[V]]=1;
                            licznik++;
                            licznik++;
                            iter_a=1, iter_b=1;
                            while(licznik+4<E)
                            {
                                do
                                {
                                    iter_b++;
                                    if(iter_b>V) iter_b=1;
                                }
                                while(macierz[ciag[iter_a]][ciag[iter_b]]==1);
                                krawedzie.push_back(make_pair(ciag[iter_a], ciag[iter_b]));
                                krawedzie.push_back(make_pair(ciag[iter_b], ciag[iter_a]));
                                macierz[ciag[iter_a]][ciag[iter_b]]=1;
                                macierz[ciag[iter_b]][ciag[iter_a]]=1;
                                licznik++;
                                licznik++;
                                iter_a=iter_b;
                            }
                            for(int i=2; i<=V; i++)
                            {
                                if(macierz[ciag[iter_a]][ciag[i]]==0 && macierz[ciag[i]][ciag[1]]==0 && i!=iter_a && licznik<E)
                                {
                                    krawedzie.push_back(make_pair(ciag[iter_a], ciag[i]));
                                    krawedzie.push_back(make_pair(ciag[i], ciag[iter_a]));
                                    macierz[ciag[iter_a]][ciag[i]]=1;
                                    macierz[ciag[i]][ciag[iter_a]]=1;
                                    licznik++;
                                    licznik++;
                                    krawedzie.push_back(make_pair(ciag[i], ciag[1]));
                                    krawedzie.push_back(make_pair(ciag[1], ciag[i]));
                                    macierz[ciag[i]][ciag[1]]=1;
                                    macierz[ciag[1]][ciag[i]]=1;
                                    licznik++;
                                    licznik++;
                                    break;
                                }
                            }
                            sort(krawedzie.begin(), krawedzie.end(), sort2);
                            sort(krawedzie.begin(), krawedzie.end());
                            listanastepnikow();
                            auto poc=high_resolution_clock::now();
                            EulerS();
                            auto kon=high_resolution_clock::now();
                            auto rozn=duration_cast<nanoseconds>(kon-poc);
                            tme=double(rozn.count())/1000000000;
                            if(cykl)
                            {
                                czasy[3][jakie_nas][ile_el]=czasy[3][jakie_nas][ile_el]+tme;
                                ilosci[3][jakie_nas][ile_el]++;
                            }
                            cout << endl;
                        }
//--------------------------------------------------------------------------------EULER SKIEROWANY
                    }
                }
            }
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<nasycenia; j++)
                {
                    for(int k=0; k<elementy; k++)
                    {
                        if(ilosci[i][j][k]!=0) czasy[i][j][k]=czasy[i][j][k]/double(ilosci[i][j][k]);
                        else czasy[i][j][k]=limit;
                    }
                }
            }
            wyniki.open("wyniki.txt");
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<nasycenia; j++)
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
                cout << "1. CYKL HAMILTONA W GRAFIE NIESKIEROWANYM (MACIERZ SASIEDZTWA)" << endl;
                cout << "2. CYKL HAMILTONA W GRAFIE SKIEROWANYM (LISTA NASTEPNIKOW)" << endl;
                cout << "3. CYKL EULERA W GRAFIE NIESKIEROWANYM (MACIERZ SASIEDZTWA)" << endl;
                cout << "4. CYKL EULERA W GRAFIE SKIEROWANYM (LISTA NASTEPNIKOW)" << endl;
                cout << "5. KONIEC PROGRAMU" << endl;
                cin >> q2;
                switch(q2)
                {
                    case 1:
                    {
                        macierzsasiedztwa();
                        HamiltonN();
                        break;
                    }
                    case 2:
                    {
                        listanastepnikow();
                        HamiltonS();
                        break;
                    }
                    case 3:
                    {
                        macierzsasiedztwa();
                        EulerN();
                        break;
                    }
                    case 4:
                    {
                        listanastepnikow();
                        EulerS();
                        break;
                    }
                    default: return 0;
                }
                if(cykl) for(int i=0; i<lista.size(); i++) cout << lista[i] << " ";
                else
                {
                    cout << "GRAF WEJSCIOWY NIE ZAWIERA CYKLU";
                    return 0;
                }
                cout << endl;
            }
        }
//--------------------------------------------------------------------------------GRAF Z PLIKU
        default: return 0;
    }
}
