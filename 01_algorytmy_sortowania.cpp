#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;
//--------------------------------------------------------------------------------BIBLIOTEKI
int tab[100001];
int orig[100001];
long long por, sca;
int zmienna_do_petli;
float tme;
bool qck;
int elementy=13, ile_na_typ=5, ile_w_poczatkowym=10;
vector <vector <vector <float>>> czasy;
vector <vector <vector <long long>>> operacje;
vector <int> el={10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000};
vector <int> pivoty_przyrosty;
//--------------------------------------------------------------------------------ZMIENNE
void merg(int lew, int pra, int sro, int t[], long long &por, long long &sca)
{
    sca++;
    int a=lew, b=sro, c=lew, p[100001];
    while(a<sro && b<=pra)
    {
        por++;
        if(t[a]<t[b]) p[c++]=t[a++];
        else p[c++]=t[b++];
    }
    while(a<sro) p[c++]=t[a++];
    while(b<=pra) p[c++]=t[b++];
    for(c=lew; c<=pra; c++) t[c]=p[c];
}
void merge_sort(int first, int last, int t[], long long &por, long long &sca)
{
    int middle=(first+last)/2;
    if(first<last)
    {
        merge_sort(first, middle, t, por, sca);
        merge_sort(middle+1, last, t, por, sca);
        merg(first, last, middle+1, t, por, sca);
    }
}
//--------------------------------------------------------------------------------MERGE SORT
void heap(int first, int last, int t[], long long &por)
{
    int a, b, c;
    for(a=last/2; a>=first; a--)
    {
        b=a;
        c=2*b;
        while(c<=last)
        {
            if(c+1<=last)
            {
                if(t[c+1]>t[c] && t[c+1]>t[b])
                {
                    swap(t[c+1], t[b]);
                    c++;
                }
                else if(t[c]>t[b]) swap(t[c], t[b]);
            }
            else if(t[c]>t[b]) swap(t[c], t[b]);
            b=c;
            c=2*b;
        }
    }
}
void heap_sort(int first, int last, int t[], long long &por)
{
    heap(first, last, t, por);
    int a, b, c;
    for(a=last; a>first; a--)
    {
        swap(t[a], t[first]);
        por++;
        b=1;
        c=2*b;
        while(c<a)
        {
            por++;
            if(c+1<a)
            {
                por++;
                if(t[c+1]>t[c] && t[c+1]>t[b])
                {
                    swap(t[c+1], t[b]);
                    por++;
                    c++;
                }
                else if(t[c]>t[b])
                {
                    swap(t[c], t[b]);
                    por++;
                }
            }
            else if(t[c]>t[b])
            {
                swap(t[c], t[b]);
                por++;
            }
            b=c;
            c=2*b;
        }
    }
}
//--------------------------------------------------------------------------------HEAP SORT
void bubble_sort(int first, int last, int t[], long long &por)
{
    int a, b;
    bool c=true;
    for(a=first; a<=last-1 && c; a++)
    {
        c=false;
        for(b=first; b<=last-a; b++)
        {
            por++;
            if(t[b]>t[b+1])
            {
                por++;
                swap(t[b], t[b+1]);
                c=true;
            }
        }
    }
}
//--------------------------------------------------------------------------------BUBBLE SORT
void selection_sort(int first, int last, int t[], long long &por)
{
    int a, b, mini;
    for(a=first; a<=last-1; a++)
    {
        mini=a;
        for(b=a+1; b<=last; b++)
        {
            por++;
            if(t[b]<t[mini]) mini=b;
        }
        swap(t[mini], t[a]);
        por++;
    }
}
//--------------------------------------------------------------------------------SELECTION SORT
void insertion_sort(int first, int last, int t[], long long &por)
{
    int a, b, c;
    for(a=first+1; a<=last; a++)
    {
        c=t[a];
        for(b=a-1; b>=first && t[b]>c; b--, por++)
        {
            t[b+1]=t[b];
            por++;
        }
        t[b+1]=c;
        por++;
    }
}
//--------------------------------------------------------------------------------INSERTION SORT
int quick(int lew, int pra, int t[], long long &por, vector <int> &pivoty_przyrosty, bool qck)
{
    int a=lew, b=pra, c=t[pra];
    if(qck) pivoty_przyrosty.push_back(c);
    while(1)
    {
        while(a<=pra && t[a]<c)
        {
            a++;
            por++;
        }
        while(t[b]>c && b>=lew)
        {
            b--;
            por++;
        }
        if(a<=b)
        {
            swap(t[a], t[b]);
            por++;
            a++;
            b--;
        }
        else return b;
    }
}
void quick_sort(int first, int last, int t[], long long &por, vector <int> &pivoty_przyrosty, bool qck)
{
    while(first<last)
    {
        int middle=quick(first, last, t, por, pivoty_przyrosty, qck);
        if((middle-first)<=(last-middle))
        {
            quick_sort(first, middle, t, por, pivoty_przyrosty, qck);
            first=middle+1;
        }
        else
        {
            quick_sort(middle+1, last, t, por, pivoty_przyrosty, qck);
            last=middle;
        }
    }
}
//--------------------------------------------------------------------------------QUICK SORT
int power(int a, int b)
{
    int c=1;
    while(b--) c=c*a;
    return c;
}
void shell_sort(int first, int last, int t[], long long &por, vector <int> &pivoty_przyrosty, bool qck)
{
    int k=1, p, a, b, c, d;
    while((power(3, k)-1)/2<last) k++;
    k--;
    for(k=k; k>=1; k--)
    {
        p=(power(3, k)-1)/2;
        if(qck) pivoty_przyrosty.push_back(p);
        for(a=0; a<p; a++)
        {
            for(b=first+a+p; b<=last; b=b+p)
            {
                c=t[b];
                for(d=b-p; d>=first && t[d]>c; d=d-p, por++)
                {
                    t[d+p]=t[d];
                    por++;
                }
                t[d+p]=c;
                por++;
            }
        }
    }
}
//--------------------------------------------------------------------------------SHELL SORT
void wypisz_wyniki(int first, int last, int t[], float tme, long long por, long long sca, vector <int> pivoty_przyrosty, int typ_sortowania)
{
    cout << "CIAG WYJSCIOWY:";
    for(int i=first; i<=last; i++) cout << " " << tab[i];
    cout << endl << "CZAS: " << tme;
    if(typ_sortowania==1)
    {
        cout << endl << "LICZBA POROWNAN: " << por;
        cout << endl << "LICZBA SCALEN: " << sca << endl;
    }
    else
    {
        cout << endl << "LICZBA POROWNAN I ZAMIAN: " << por << endl;
    }
    if(typ_sortowania==6)
    {
        cout << "KOLEJNE PIVOTY:";
        for(int i=0; i<pivoty_przyrosty.size(); i++) cout << " " << pivoty_przyrosty[i];
        cout << endl;
    }
    if(typ_sortowania==7)
    {
        cout << "KOLEJNE PRZYROSTY:";
        for(int i=0; i<pivoty_przyrosty.size(); i++) cout << " " << pivoty_przyrosty[i];
        cout << endl;
    }
}
//--------------------------------------------------------------------------------WYNIKI
void poprawnosc(int first, int last, int t[])
{
    bool check=1;
    for(int i=first+1; i<=last; i++) if(tab[i]<tab[i-1]) check=0;
    if(check) cout << "SORTOWANIE POPRAWNE" << endl;
    else cout << "BLAD SORTOWANIA" << endl;
}
//--------------------------------------------------------------------------------POPRAWNOSC SORTOWANIA
int main()
{
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        srand(time(0));
        cout << fixed << setprecision(9);
        operacje.resize(7);
        czasy.resize(7);
        for(int i=0; i<7; i++) operacje[i].resize(5);
        for(int i=0; i<7; i++) czasy[i].resize(5);
        for(int i=0; i<35; i++) operacje[i%7][i%5].resize(el.size());
        for(int i=0; i<35; i++) czasy[i%7][i%5].resize(el.size());
    }
//--------------------------------------------------------------------------------SRAND, SETPRECISION, TABLICA WYNIKOW
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        for(int i=1; i<=ile_w_poczatkowym; i++) orig[i]=(rand()*rand())%(10*ile_w_poczatkowym)+1;
        cout << "CIAG POCZATKOWY:";
        for(int i=1; i<=ile_w_poczatkowym; i++) cout << " " << orig[i];
        cout << endl << endl << "WYNIK SORTOWANIA" << endl;
    }
//--------------------------------------------------------------------------------CIAG POCZATKOWY
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        cout << endl << "MERGE SORT" << endl;
        por=0; sca=0;
        for(int i=1; i<=ile_w_poczatkowym; i++) tab[i]=orig[i];
        auto poc=high_resolution_clock::now();
        merge_sort(1, ile_w_poczatkowym, tab, por, sca);
        auto kon=high_resolution_clock::now();
        auto rozn=duration_cast<nanoseconds>(kon-poc);
        tme=float(rozn.count())/1000000000;
        wypisz_wyniki(1, ile_w_poczatkowym, tab, tme, por, sca, pivoty_przyrosty, 1);
        poprawnosc(1, ile_w_poczatkowym, tab);
    }
//--------------------------------------------------------------------------------MERGE SORT WYNIK
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        cout << endl << "HEAP SORT" << endl;
        por=0;
        for(int i=1; i<=ile_w_poczatkowym; i++) tab[i]=orig[i];
        auto poc=high_resolution_clock::now();
        heap_sort(1, ile_w_poczatkowym, tab, por);
        auto kon=high_resolution_clock::now();
        auto rozn=duration_cast<nanoseconds>(kon-poc);
        tme=float(rozn.count())/1000000000;
        wypisz_wyniki(1, ile_w_poczatkowym, tab, tme, por, sca, pivoty_przyrosty, 2);
        poprawnosc(1, ile_w_poczatkowym, tab);
    }
//--------------------------------------------------------------------------------HEAP SORT WYNIK
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        cout << endl << "BUBBLE SORT" << endl;
        por=0;
        for(int i=1; i<=ile_w_poczatkowym; i++) tab[i]=orig[i];
        auto poc=high_resolution_clock::now();
        bubble_sort(1, ile_w_poczatkowym, tab, por);
        auto kon=high_resolution_clock::now();
        auto rozn=duration_cast<nanoseconds>(kon-poc);
        tme=float(rozn.count())/1000000000;
        wypisz_wyniki(1, ile_w_poczatkowym, tab, tme, por, sca, pivoty_przyrosty, 3);
        poprawnosc(1, ile_w_poczatkowym, tab);
    }
//--------------------------------------------------------------------------------BUBBLE SORT WYNIK
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        cout << endl << "SELECTION SORT" << endl;
        por=0;
        for(int i=1; i<=ile_w_poczatkowym; i++) tab[i]=orig[i];
        auto poc=high_resolution_clock::now();
        selection_sort(1, ile_w_poczatkowym, tab, por);
        auto kon=high_resolution_clock::now();
        auto rozn=duration_cast<nanoseconds>(kon-poc);
        tme=float(rozn.count())/1000000000;
        wypisz_wyniki(1, ile_w_poczatkowym, tab, tme, por, sca, pivoty_przyrosty, 4);
        poprawnosc(1, ile_w_poczatkowym, tab);
    }
//--------------------------------------------------------------------------------SELECTION SORT WYNIK
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        cout << endl << "INSERTION SORT" << endl;
        por=0;
        for(int i=1; i<=ile_w_poczatkowym; i++) tab[i]=orig[i];
        auto poc=high_resolution_clock::now();
        insertion_sort(1, ile_w_poczatkowym, tab, por);
        auto kon=high_resolution_clock::now();
        auto rozn=duration_cast<nanoseconds>(kon-poc);
        tme=float(rozn.count())/1000000000;
        wypisz_wyniki(1, ile_w_poczatkowym, tab, tme, por, sca, pivoty_przyrosty, 5);
        poprawnosc(1, ile_w_poczatkowym, tab);
    }
//--------------------------------------------------------------------------------INSERTION SORT WYNIK
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        cout << endl << "QUICK SORT" << endl;
        por=0; pivoty_przyrosty.clear(); qck=1;
        for(int i=1; i<=ile_w_poczatkowym; i++) tab[i]=orig[i];
        auto poc=high_resolution_clock::now();
        quick_sort(1, ile_w_poczatkowym, tab, por, pivoty_przyrosty, qck);
        auto kon=high_resolution_clock::now();
        auto rozn=duration_cast<nanoseconds>(kon-poc);
        tme=float(rozn.count())/1000000000;
        wypisz_wyniki(1, ile_w_poczatkowym, tab, tme, por, sca, pivoty_przyrosty, 6);
        poprawnosc(1, ile_w_poczatkowym, tab);
    }
//--------------------------------------------------------------------------------QUICK SORT WYNIK
    for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
    {
        cout << endl << "SHELL SORT" << endl;
        por=0; pivoty_przyrosty.clear();
        for(int i=1; i<=ile_w_poczatkowym; i++) tab[i]=orig[i];
        auto poc=high_resolution_clock::now();
        shell_sort(1, ile_w_poczatkowym, tab, por, pivoty_przyrosty, qck);
        auto kon=high_resolution_clock::now();
        auto rozn=duration_cast<nanoseconds>(kon-poc);
        tme=float(rozn.count())/1000000000;
        wypisz_wyniki(1, ile_w_poczatkowym, tab, tme, por, sca, pivoty_przyrosty, 7);
        poprawnosc(1, ile_w_poczatkowym, tab);
        qck=0;
    }
//--------------------------------------------------------------------------------SHELL SORT WYNIK
    for(int ile_el=0; ile_el<elementy; ile_el++)
    {
        for(int losowy=0; losowy<ile_na_typ; losowy++)
        {
            for(int i=1; i<=el[ile_el]; i++) orig[i]=(rand()*rand())%(10*el[ile_el])+1;
//--------------------------------------------------------------------------------GENEROWANIE LOSOWEGO
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0; sca=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                merge_sort(1, el[ile_el], tab, por, sca);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[0][0][ile_el]=czasy[0][0][ile_el]+tme;
                operacje[0][0][ile_el]=operacje[0][0][ile_el]+por;
            }
//--------------------------------------------------------------------------------MERGE SORT LOSOWY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                heap_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[1][0][ile_el]=czasy[1][0][ile_el]+tme;
                operacje[1][0][ile_el]=operacje[1][0][ile_el]+por;
            }
//--------------------------------------------------------------------------------HEAP SORT LOSOWY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                bubble_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[2][0][ile_el]=czasy[2][0][ile_el]+tme;
                operacje[2][0][ile_el]=operacje[2][0][ile_el]+por;
            }
//--------------------------------------------------------------------------------BUBBLE SORT LOSOWY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                selection_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[3][0][ile_el]=czasy[3][0][ile_el]+tme;
                operacje[3][0][ile_el]=operacje[3][0][ile_el]+por;
            }
//--------------------------------------------------------------------------------SELECTION SORT LOSOWY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                insertion_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[4][0][ile_el]=czasy[4][0][ile_el]+tme;
                operacje[4][0][ile_el]=operacje[4][0][ile_el]+por;
            }
//--------------------------------------------------------------------------------INSERTION SORT LOSOWY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                quick_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[5][0][ile_el]=czasy[5][0][ile_el]+tme;
                operacje[5][0][ile_el]=operacje[5][0][ile_el]+por;
            }
//--------------------------------------------------------------------------------QUICK SORT LOSOWY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                shell_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[6][0][ile_el]=czasy[6][0][ile_el]+tme;
                operacje[6][0][ile_el]=operacje[6][0][ile_el]+por;
            }
//--------------------------------------------------------------------------------SHELL SORT LOSOWY
        }
//--------------------------------------------------------------------------------CIAG LOSOWY
        for(int rosnacy=0; rosnacy<ile_na_typ; rosnacy++)
        {
            for(int i=1; i<=el[ile_el]; i++) orig[i]=(rand()*rand())%(10*el[ile_el])+1;
            sort(orig+1, orig+el[ile_el]+1);
//--------------------------------------------------------------------------------GENEROWANIE ROSNACEGO
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0; sca=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                merge_sort(1, el[ile_el], tab, por, sca);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[0][1][ile_el]=czasy[0][1][ile_el]+tme;
                operacje[0][1][ile_el]=operacje[0][1][ile_el]+por;
            }
//--------------------------------------------------------------------------------MERGE SORT ROSNACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                heap_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[1][1][ile_el]=czasy[1][1][ile_el]+tme;
                operacje[1][1][ile_el]=operacje[1][1][ile_el]+por;
            }
//--------------------------------------------------------------------------------HEAP SORT ROSNACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                bubble_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[2][1][ile_el]=czasy[2][1][ile_el]+tme;
                operacje[2][1][ile_el]=operacje[2][1][ile_el]+por;
            }
//--------------------------------------------------------------------------------BUBBLE SORT ROSNACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                selection_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[3][1][ile_el]=czasy[3][1][ile_el]+tme;
                operacje[3][1][ile_el]=operacje[3][1][ile_el]+por;
            }
//--------------------------------------------------------------------------------SELECTION SORT ROSNACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                insertion_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[4][1][ile_el]=czasy[4][1][ile_el]+tme;
                operacje[4][1][ile_el]=operacje[4][1][ile_el]+por;
            }
//--------------------------------------------------------------------------------INSERTION SORT ROSNACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                quick_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[5][1][ile_el]=czasy[5][1][ile_el]+tme;
                operacje[5][1][ile_el]=operacje[5][1][ile_el]+por;
            }
//--------------------------------------------------------------------------------QUICK SORT ROSNACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                shell_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[6][1][ile_el]=czasy[6][1][ile_el]+tme;
                operacje[6][1][ile_el]=operacje[6][1][ile_el]+por;
            }
//--------------------------------------------------------------------------------SHELL SORT ROSNACY
        }
//--------------------------------------------------------------------------------CIAG ROSNACY
        for(int malejacy=0; malejacy<ile_na_typ; malejacy++)
        {
            for(int i=1; i<=el[ile_el]; i++) orig[i]=(rand()*rand())%(10*el[ile_el])+1;
            sort(orig+1, orig+el[ile_el]+1);
            reverse(orig+1, orig+el[ile_el]+1);
//--------------------------------------------------------------------------------GENEROWANIE MALEJACEGO
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0; sca=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                merge_sort(1, el[ile_el], tab, por, sca);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[0][2][ile_el]=czasy[0][2][ile_el]+tme;
                operacje[0][2][ile_el]=operacje[0][2][ile_el]+por;
            }
//--------------------------------------------------------------------------------MERGE SORT MALEJACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                heap_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[1][2][ile_el]=czasy[1][2][ile_el]+tme;
                operacje[1][2][ile_el]=operacje[1][2][ile_el]+por;
            }
//--------------------------------------------------------------------------------HEAP SORT MALEJACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                bubble_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[2][2][ile_el]=czasy[2][2][ile_el]+tme;
                operacje[2][2][ile_el]=operacje[2][2][ile_el]+por;
            }
//--------------------------------------------------------------------------------BUBBLE SORT MALEJACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                selection_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[3][2][ile_el]=czasy[3][2][ile_el]+tme;
                operacje[3][2][ile_el]=operacje[3][2][ile_el]+por;
            }
//--------------------------------------------------------------------------------SELECTION SORT MALEJACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                insertion_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[4][2][ile_el]=czasy[4][2][ile_el]+tme;
                operacje[4][2][ile_el]=operacje[4][2][ile_el]+por;
            }
//--------------------------------------------------------------------------------INSERTION SORT MALEJACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                quick_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[5][2][ile_el]=czasy[5][2][ile_el]+tme;
                operacje[5][2][ile_el]=operacje[5][2][ile_el]+por;
            }
//--------------------------------------------------------------------------------QUICK SORT MALEJACY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                shell_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[6][2][ile_el]=czasy[6][2][ile_el]+tme;
                operacje[6][2][ile_el]=operacje[6][2][ile_el]+por;
            }
//--------------------------------------------------------------------------------SHELL SORT MALEJACY
        }
//--------------------------------------------------------------------------------CIAG MALEJACY
        for(int A_ksztaltny=0; A_ksztaltny<ile_na_typ; A_ksztaltny++)
        {
            for(int i=1; i<=el[ile_el]/2; i++) orig[i]=(rand()*rand())%(10*el[ile_el])+1;
            for(int i=el[ile_el]/2+1; i<=el[ile_el]; i++) orig[i]=(rand()*rand())%(10*el[ile_el])+1;
            sort(orig+1, orig+el[ile_el]/2+1);
            sort(orig+el[ile_el]/2+1, orig+el[ile_el]+1);
            reverse(orig+el[ile_el]/2+1, orig+el[ile_el]+1);
//--------------------------------------------------------------------------------GENEROWANIE A KSZTALTNEGO
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0; sca=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                merge_sort(1, el[ile_el], tab, por, sca);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[0][3][ile_el]=czasy[0][3][ile_el]+tme;
                operacje[0][3][ile_el]=operacje[0][3][ile_el]+por;
            }
//--------------------------------------------------------------------------------MERGE SORT A KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                heap_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[1][3][ile_el]=czasy[1][3][ile_el]+tme;
                operacje[1][3][ile_el]=operacje[1][3][ile_el]+por;
            }
//--------------------------------------------------------------------------------HEAP SORT A KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                bubble_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[2][3][ile_el]=czasy[2][3][ile_el]+tme;
                operacje[2][3][ile_el]=operacje[2][3][ile_el]+por;
            }
//--------------------------------------------------------------------------------BUBBLE SORT A KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                selection_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[3][3][ile_el]=czasy[3][3][ile_el]+tme;
                operacje[3][3][ile_el]=operacje[3][3][ile_el]+por;
            }
//--------------------------------------------------------------------------------SELECTION SORT A KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                insertion_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[4][3][ile_el]=czasy[4][3][ile_el]+tme;
                operacje[4][3][ile_el]=operacje[4][3][ile_el]+por;
            }
//--------------------------------------------------------------------------------INSERTION SORT A KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                quick_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[5][3][ile_el]=czasy[5][3][ile_el]+tme;
                operacje[5][3][ile_el]=operacje[5][3][ile_el]+por;
            }
//--------------------------------------------------------------------------------QUICK SORT A KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                shell_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[6][3][ile_el]=czasy[6][3][ile_el]+tme;
                operacje[6][3][ile_el]=operacje[6][3][ile_el]+por;
            }
//--------------------------------------------------------------------------------SHELL SORT A KSZTALTNY
        }
//--------------------------------------------------------------------------------CIAG A KSZTALTNY
        for(int V_ksztaltny=0; V_ksztaltny<ile_na_typ; V_ksztaltny++)
        {
            for(int i=1; i<=el[ile_el]/2; i++) orig[i]=(rand()*rand())%(10*el[ile_el])+1;
            for(int i=el[ile_el]/2+1; i<=el[ile_el]; i++) orig[i]=(rand()*rand())%(10*el[ile_el])+1;
            sort(orig+1, orig+el[ile_el]/2+1);
            reverse(orig+1, orig+el[ile_el]/2+1);
            sort(orig+1+el[ile_el]/2, orig+el[ile_el]+1);
//--------------------------------------------------------------------------------GENEROWANIE V KSZTALTNEGO
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0; sca=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                merge_sort(1, el[ile_el], tab, por, sca);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[0][4][ile_el]=czasy[0][4][ile_el]+tme;
                operacje[0][4][ile_el]=operacje[0][4][ile_el]+por;
            }
//--------------------------------------------------------------------------------MERGE SORT V KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                heap_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[1][4][ile_el]=czasy[1][4][ile_el]+tme;
                operacje[1][4][ile_el]=operacje[1][4][ile_el]+por;
            }
//--------------------------------------------------------------------------------HEAP SORT V KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                bubble_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[2][4][ile_el]=czasy[2][4][ile_el]+tme;
                operacje[2][4][ile_el]=operacje[2][4][ile_el]+por;
            }
//--------------------------------------------------------------------------------BUBBLE SORT V KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                selection_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[3][4][ile_el]=czasy[3][4][ile_el]+tme;
                operacje[3][4][ile_el]=operacje[3][4][ile_el]+por;
            }
//--------------------------------------------------------------------------------SELECTION SORT V KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0;
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                insertion_sort(1, el[ile_el], tab, por);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[4][4][ile_el]=czasy[4][4][ile_el]+tme;
                operacje[4][4][ile_el]=operacje[4][4][ile_el]+por;
            }
//--------------------------------------------------------------------------------INSERTION SORT V KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                quick_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[5][4][ile_el]=czasy[5][4][ile_el]+tme;
                operacje[5][4][ile_el]=operacje[5][4][ile_el]+por;
            }
//--------------------------------------------------------------------------------QUICK SORT V KSZTALTNY
            for(zmienna_do_petli=0; zmienna_do_petli<1; zmienna_do_petli++)
            {
                por=0, pivoty_przyrosty.clear();
                for(int i=1; i<=el[ile_el]; i++) tab[i]=orig[i];
                auto poc=high_resolution_clock::now();
                shell_sort(1, el[ile_el], tab, por, pivoty_przyrosty, qck);
                auto kon=high_resolution_clock::now();
                auto rozn=duration_cast<nanoseconds>(kon-poc);
                tme=float(rozn.count())/1000000000;
                czasy[6][4][ile_el]=czasy[6][4][ile_el]+tme;
                operacje[6][4][ile_el]=operacje[6][4][ile_el]+por;
            }
//--------------------------------------------------------------------------------SHELL SORT V KSZTALTNY
        }
//--------------------------------------------------------------------------------CIAG V KSZTALTNY
    }
    ofstream wyj;
    wyj.open("wyniki2.txt");
    for(int i=0; i<7; i++)
    {
        for(int j=0; j<5; j++)
        {
            for(int k=0; k<elementy; k++)
            {
                wyj << czasy[i][j][k]/ile_na_typ << " " << operacje[i][j][k]/ile_na_typ << "\t";
            }
            wyj << endl;
        }
        wyj << endl << endl << endl;
    }
    wyj.close();
}
