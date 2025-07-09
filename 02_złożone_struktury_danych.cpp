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
struct NODE
{
    NODE *up;
    NODE *left;
    NODE *right;
    int key;
    int bf;
};
vector <int> keys, medians;
vector <int> el={10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};
vector <int> els;
int question, q2, q3;
int tab[10]={5, 8, 9, 2, 1, 10, 3, 7, 6, 4};
//--------------------------------------------------------------------------------ZMIENNE
void createTREE(NODE *&node, int k)
{
    NODE *a, *b;
    a=new NODE;
    a->left=a->right=NULL;
    a->key=k;
    b=node;
    if(!b) node=a;
    else
    {
        while(1)
        {
            if(k<b->key)
            {
                if(!b->left)
                {
                    b->left=a;
                    break;
                }
                else b=b->left;
            }
            else
            {
                if(!b->right)
                {
                    b->right=a;
                    break;
                }
                else b=b->right;
            }
        }
    }
    a->up=b;
}
NODE *findNODE(NODE *node, int k)
{
    cout << "SCIEZKA: ";
    while(node && node->key!=k)
    {
        cout << node->key << " ";
        if(k<node->key) node=node->left;
        else node=node->right;
    }
    return node;
}
NODE *findmin(NODE *node)
{
    if(node) while(node->left) node=node->left;
    return node;
}
NODE *successorNODE(NODE *node)
{
    NODE *a;
    if(node)
    {
        if(node->right) return findmin(node->right);
        else
        {
            a=node->up;
            while(a && (node==a->right))
            {
                node=a;
                a=a->up;
            }
            return a;
        }
    }
    return node;
}
void deleteNODE(NODE *& node, NODE *a)
{
    NODE *b, *c;
    if(a)
    {
        if(!a->left || !a->right) b=a;
        else b=successorNODE(a);
        if(b->left) c=b->left;
        else c=b->right;
        if(c) c->up=b->up;
        if(!b->up) node=c;
        else if(b==b->up->left) b->up->left=c;
        else b->up->right=c;
        if(b!=a) a->key=b->key;
        delete b;
    }
}
void median(vector <int> keys, vector <int> &medians, int a, int b)
{
    if(b==a) return;
    else
    {
        int c=(a+b)/2;
        medians.push_back(keys[c]);
        median(keys, medians, a, c);
        median(keys, medians, c+1, b);
    }
}
void findminNODE(NODE *node)
{
    if(node)
    {
        cout << node->key << " ";
        while(node->left)
        {
            node=node->left;
            cout << node->key << " ";
        }
        cout << endl << "MINIMUM: " << node->key;
    }
    else cout << endl << "NIE MA ZADNEJ WARTOSCI";
}
void findmaxNODE(NODE *node)
{
    if(node)
    {
        cout << node->key << " ";
        while(node->right)
        {
            node=node->right;
            cout << node->key << " ";
        }
        cout << endl << "MAXIMUM: " << node->key;
    }
    else cout << endl << "NIE MA ZADNEJ WARTOSCI";
}
void findminNODE2(NODE *node)
{
    if(node) while(node->left) node=node->left;
}
void findmaxNODE2(NODE *node)
{
    if(node) while(node->right) node=node->right;
}
void inorder(NODE *node)
{
    if(node)
    {
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }
    else return;
}
void inorder2(NODE *node)
{
    if(node)
    {
        inorder2(node->left);
        inorder2(node->right);
    }
    else return;
}
void preorder(NODE *node)
{
    if(node)
    {
        cout << node->key << " ";
        preorder(node->left);
        preorder(node->right);
    }
    else return;
}
void postorder(NODE *&node)
{
    if(node)
    {
        postorder(node->left);
        postorder(node->right);
        cout << node->key << " ";
        node=NULL;
    }
    else return;
}
void preorderfromNODE(NODE *node)
{
    if(node)
    {
        cout << node->key << " ";
        preorder(node->left);
        preorder(node->right);
    }
    else return;
}
void leftrotation(NODE *&node, NODE *a)
{
    cout << a-> key << " ";
    NODE *b=a->right, *c=a->up;
    if(b)
    {
        a->right=b->left;
        if(a->right) a->right->up=a;
        b->left=a;
        b->up=c;
        a->up=b;
        if(c)
        {
            if(c->left==a) c->left=b;
            else c->right=b;
        }
        else node=b;
    }
}
void rightrotation(NODE *&node, NODE *a)
{
    cout << a->key << " ";
    NODE *b=a->left, *c=a->up;
    if(b)
    {
        a->left=b->right;
        if(a->left) a->left->up=a;
        b->right=a;
        b->up=c;
        a->up=b;
        if(c)
        {
            if(c->left==a) c->left=b;
            else c->right=b;
        }
        else node=b;
    }
}
void leftrotation2(NODE *&node, NODE *a)
{
    NODE *b=a->right, *c=a->up;
    if(b)
    {
        a->right=b->left;
        if(a->right) a->right->up=a;
        b->left=a;
        b->up=c;
        a->up=b;
        if(c)
        {
            if(c->left==a) c->left=b;
            else c->right=b;
        }
        else node=b;
    }
}
void rightrotation2(NODE *&node, NODE *a)
{
    NODE *b=a->left, *c=a->up;
    if(b)
    {
        a->left=b->right;
        if(a->left) a->left->up=a;
        b->right=a;
        b->up=c;
        a->up=b;
        if(c)
        {
            if(c->left==a) c->left=b;
            else c->right=b;
        }
        else node=b;
    }
}
unsigned log2(unsigned x)
{
    unsigned y=1;
    while((x>>=1)>0) y<<=1;
    return y;
}
void balanceDSW(NODE *&node)
{
    unsigned a=0, b;
    NODE *c=node;
    while(c)
    {
        if(c->left)
        {
            rightrotation(node, c);
            c=c->up;
        }
        else
        {
            a++;
            c=c->right;
        }
    }
    b=a+1-log2(a+1);
    c=node;
    for(int i=0; i<b; i++)
    {
        leftrotation(node, c);
        c=c->up->right;
    }
    a-=b;
    while(a>1)
    {
        a>>=1;
        c=node;
        for(int i=0; i<a; i++)
        {
            leftrotation(node, c);
            c=c->up->right;
        }
    }
}
void balanceDSW2(NODE *&node)
{
    unsigned a=0, b;
    NODE *c=node;
    while(c)
    {
        if(c->left)
        {
            rightrotation2(node, c);
            c=c->up;
        }
        else
        {
            a++;
            c=c->right;
        }
    }
    b=a+1-log2(a+1);
    c=node;
    for(int i=0; i<b; i++)
    {
        leftrotation2(node, c);
        c=c->up->right;
    }
    a-=b;
    while(a>1)
    {
        a>>=1;
        c=node;
        for(int i=0; i<a; i++)
        {
            leftrotation2(node, c);
            c=c->up->right;
        }
    }
}
//--------------------------------------------------------------------------------FUNKCJE
int main()
{
    srand(time(0));
    cout << fixed << setprecision(9);
    NODE *node=NULL;
    cout << "CO ROBIMY?\n";
    cout << "0. TESTY\n";
    cout << "1. BUDOWANIE DRZEWA AVL METODA PRZESZUKIWANIA BINARNEGO\n";
    cout << "2. KONSTRUOWANIE LOSOWEGO DRZEWA BST\n";
    cin >> question;
    switch(question)
    {
        case 0:
        {
            for(int powt=0; powt<10; powt++)
            {
                for(int i=0; i<12; i++)
                {
                    els.clear();
                    for(int j=0; j<el[i]; j++) els.push_back((rand()*rand())%el[i]*10);
                    sort(els.begin(), els.end());
                    reverse(els.begin(), els.end());

                    node=NULL;
                    auto poc=high_resolution_clock::now();
                    for(int j=0; j<els.size(); j++) createTREE(node, els[j]);
                    auto kon=high_resolution_clock::now();
                    auto rozn=duration_cast<nanoseconds>(kon-poc);
                    float tme=float(rozn.count())/1000000000;
                    cout << "TWORZENIE BST " << el[i] << "\t" << tme << endl;
                    poc=high_resolution_clock::now();
                    findminNODE2(node);
                    kon=high_resolution_clock::now();
                    rozn=duration_cast<nanoseconds>(kon-poc);
                    tme=float(rozn.count())/1000000000;
                    cout << "MINIMUM W BST " << el[i] << "\t" << tme << endl;
                    poc=high_resolution_clock::now();
                    inorder2(node);
                    kon=high_resolution_clock::now();
                    rozn=duration_cast<nanoseconds>(kon-poc);
                    tme=float(rozn.count())/1000000000;
                    cout << "IN-ORDER W BST " << el[i] << "\t" << tme << endl;
                    poc=high_resolution_clock::now();
                    balanceDSW2(node);
                    kon=high_resolution_clock::now();
                    rozn=duration_cast<nanoseconds>(kon-poc);
                    tme=float(rozn.count())/1000000000;
                    cout << "ROWNOWAZENIE BST " << el[i] << "\t" << tme << endl;

                    node=NULL;
                    median(els, medians, 0, el[i]);
                    poc=high_resolution_clock::now();
                    for(int j=0; j<medians.size(); j++) createTREE(node, medians[j]);
                    kon=high_resolution_clock::now();
                    rozn=duration_cast<nanoseconds>(kon-poc);
                    tme=float(rozn.count())/1000000000;
                    cout << "TWORZENIE AVL " << el[i] << "\t" << tme << endl;
                    poc=high_resolution_clock::now();
                    findminNODE2(node);
                    kon=high_resolution_clock::now();
                    rozn=duration_cast<nanoseconds>(kon-poc);
                    tme=float(rozn.count())/1000000000;
                    cout << "MINIMUM W AVL " << el[i] << "\t" << tme << endl;
                    poc=high_resolution_clock::now();
                    inorder2(node);
                    kon=high_resolution_clock::now();
                    rozn=duration_cast<nanoseconds>(kon-poc);
                    tme=float(rozn.count())/1000000000;
                    cout << "IN-ORDER W AVL " << el[i] << "\t" << tme << endl;
                }
            }
            return 0;
        }
        case 1:
        {
            for(int i=0; i<10; i++)
            {
                keys.push_back(tab[i]);
                cout << tab[i] << " ";
            }
            sort(keys.begin(), keys.end());
            median(keys, medians, 0, 10);
            for(int i=0; i<medians.size(); i++) createTREE(node,  medians[i]);
            cout << "\nUTWORZONO DRZEWO AVL\n";
            break;
        }
        case 2:
        {
            for(int i=0; i<10; i++)
            {
                createTREE(node, tab[i]);
                cout << tab[i] << " ";
            }
            cout << "\nUTWORZONO DRZEWO BST\n";
            break;
        }
        default: return 0;
    }
    while(1)
    {
        cout << "CO DALEJ?\n";
        cout << "0. KONIEC PROGRAMU\n";
        cout << "1. ZNAJDZ WARTOSC NAJMNIEJSZA\n";
        cout << "2. ZNAJDZ WARTOSC NAJWIEKSZA\n";
        cout << "3. USUN KLUCZE\n";
        cout << "4. WYPISZ IN-ORDER\n";
        cout << "5. WYPISZ PRE-ORDER\n";
        cout << "6. USUN DRZEWO WYPISUJAC POST-ORDER\n";
        cout << "7. WYPISZ PRE-ORDER PODDRZEWO\n";
        cout << "8. ROWNOWAZENIE ALGORYTMEM DSW\n";
        cout << "9. DODAJ ELEMENT DO DRZEWA\n";
        cin >> question;
        switch(question)
        {
            case 0:
            {
                return 0;
            }
            case 1:
            {
                findminNODE(node);
                cout << endl;
                break;
            }
            case 2:
            {
                findmaxNODE(node);
                cout << endl;
                break;
            }
            case 3:
            {
                cout << "ILE KLUCZY USUWASZ (MAX 10)?\n";
                cin >> q2;
                if(q2>10) q2=10;
                for(int i=0; i<q2; i++)
                {
                    cout << "KLUCZ DO USUNIECIA: ";
                    cin >> q3;
                    deleteNODE(node, findNODE(node, q3));
                    cout << "USUNIETO KLUCZ " << q3 << endl;
                }
                break;
            }
            case 4:
            {
                inorder(node);
                cout << endl;
                break;
            }
            case 5:
            {
                preorder(node);
                cout << endl;
                break;
            }
            case 6:
            {
                postorder(node);
                cout << endl;
                break;
            }
            case 7:
            {
                cout << "PODAJ KLUCZ-KORZEN PODDRZEWA\n";
                cin >> q2;
                preorderfromNODE(findNODE(node, q2));
                cout << endl;
                break;
            }
            case 8:
            {
                balanceDSW(node);
                cout << "ZROWNOWAZONO DRZEWO\n";
                break;
            }
            case 9:
            {
                cout << "PODAJ KLUCZ DO DODANIA\n";
                cin >> q2;
                createTREE(node, q2);
                cout << "DODANO KLUCZ\n";
                break;
            }
            default: return 0;
        }
    }


}
