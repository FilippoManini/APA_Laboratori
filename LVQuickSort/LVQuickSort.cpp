#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cmath>

using namespace std;

static int DimSequenza = 10000; //10^4 = 10000
static int nConfronti = 100000; //10^5 = 100000
int contConfronti = 0; //conta il numero N(j) di confronti effettuati in ogni singolo run j per ordinare la sequenza S.


//╔══════════════════════════════════════════════════════════════════════╗
//║                           FUNZIONI DI SUPPORTO                       ║
//╚══════════════════════════════════════════════════════════════════════╝
void scambia(vector<int>& v, int i, int j)
{
  int tmp = v[j]; 
  v[j] = v[i];
  v[i] = tmp;
}

void scritturaFile(vector<double> v)
{
	ofstream fout("dataNormalizzati.txt"); //crea se non esiste e se esiste sovrascrive il contenuto

    for(int i=0; i<v.size(); ++i)
    {
        fout<<v.at(i)<<endl;
    }
	fout.close();
}


//╔══════════════════════════════════════════════════════════════════════╗
//║                          QUICKSORT RANDOMIZZATO                      ║
//╚══════════════════════════════════════════════════════════════════════╝
int partizionaInPlace(vector<int>& v, int inizio, int fine)
{
   int pivotIndex = inizio+rand()%(fine-inizio+1); // scelgo un indice a caso tra inizio e fine: sara' il mio pivot
   scambia(v, pivotIndex, inizio); // metto il pivot all'inizio della sequenza da riordinare
   int i = inizio+1;

    for (int j=inizio+1; j<=fine; ++j)
    {
        if (v[j] < v[inizio]) // confronto con il pivot che e' all'inizio
        {
            scambia(v, i, j);
            ++i;

            ++contConfronti; //<-----------------
        }
    }

    scambia(v, inizio, i-1);
    return i-1;
}

void qs(vector<int>& v, int inizio, int fine)
{
    if (inizio < fine)
    {
        int pivot_index=partizionaInPlace(v, inizio, fine);
        qs(v, inizio, pivot_index-1);
        qs(v, pivot_index+1, fine);
    }
}

void quickSortRandom(vector<int>& v)
{
   qs(v, 0, v.size()-1);
}


//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI DI STAMPA                          ║
//╚══════════════════════════════════════════════════════════════════════╝

void stampaVector(vector<double> v)
{
    for(int i=0; i<v.size(); ++i){
        cout<<v.at(i)<<'\t';
    }
    cout<<endl;
}


//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI SPECIFICHE                         ║
//╚══════════════════════════════════════════════════════════════════════╝

//riepo la sequenza di numeri casuali, costruendo una sequenza S di numeri con |S| = 10^4.
void riempiSequenzaCauale(vector<int>& v)
{
    for(int i=0; i<v.size(); ++i)
    {
        v[i] = rand()%(DimSequenza);
    }
}

//Calcola il valore medio del numero di confronti effettuati su 10^5 run.
double valoreMedio(vector<double> v)
{
    double totNumeratore = 0;

    for(int i=0; i<v.size(); ++i)
    {
        totNumeratore += v.at(i);
    }
    return (totNumeratore/nConfronti);
}

//calcolo della varianza prendendo come input un vettore e la medie dei volori 
double varianza(vector<double> v, double media)
{
    double varianza = 0;

    for(int i=0; i<v.size(); ++i)
    {
        varianza = varianza + pow( (v.at(i) - media), 2);
    }
    return varianza/v.size();
}

//probabilità con la quale LVQuickSort effettua il doppio e il quintuplo del valor medio dei confronti. 
void checkMediaValori(vector<double> v, double media)
{
    double media2 = media*2;
    double media5 = media*5;

    cout<<"valore della media X 2 -> "<<media2<<endl;
    cout<<"valore della media X 5 -> "<<media5<<endl;

    for(int i=0; i<v.size(); ++i)
    {
        if(v.at(i) == media2){
            cout<<"valore "<< v.at(i) <<" maggiore della media X 2"<<endl;

            if(v.at(i) == media5){
                cout<<"valore "<< v.at(i) <<" maggiore della media X 5"<<endl;
            }
        }
    }
}

//Normalizza a 1 l'area dell'istogramma
vector<double> normalizzazione(const vector<double>& v)
{
    vector<double> v_normalizzato(v.size());

    int max = v.at(0);
    int min = v.at(0);
    
    for(int i=0; i<v.size(); ++i)
    {
        if(max < v.at(i)){
            max = v.at(i);
        }
        if(min > v.at(i)){
            min = v.at(i);
        }
    } 
    int denominatore = max-min;

    for(int i=0; i<v.size(); ++i)
    {
        v_normalizzato[i] = (v.at(i) - min) / denominatore;
    }
    return v_normalizzato;
}


//╔══════════════════════════════════════════════════════════════════════╗
//║                          MAIN                                        ║
//╚══════════════════════════════════════════════════════════════════════╝
int main()
{
    srand(time(NULL)); // srand basta chiamarla una sola volta all'interno del programma per fissare il seme della generazione pseudo-casuale
    vector<double> v_confronti(nConfronti);

    for(int i=0; i<nConfronti; ++i)
    {
        vector<int> v_elem(DimSequenza); //Costruisci una sequenza S di numeri con |S| = 10^4.
        riempiSequenzaCauale(v_elem);
        
        quickSortRandom(v_elem);

        v_confronti[i] = contConfronti;

        contConfronti = 0; //azzero la variabile per il prossimo giro
        v_elem.clear(); //pulisco il vettore dai valori precedenti
    }
    cout<<"----------------------------------------------------"<<endl;
    cout<<"sequenza S di numeri con |S| = "<<DimSequenza<<endl;
    cout<<"#run = "<<nConfronti<<endl;
    cout<<"----------------------------------------------------"<<endl;

    double media = valoreMedio(v_confronti);
    cout<<"Valore medio: "<< media <<endl;
    checkMediaValori(v_confronti, media);
    cout<<"Varianza: "<< varianza(v_confronti,media) <<endl;
    cout<<"----------------------------------------------------"<<endl;

    vector<double> v_normalizzato( normalizzazione(v_confronti) );
    cout<<"Valori normalizzati: "<<endl;
    media = valoreMedio(v_normalizzato);
    cout<<"Valore medio: "<< media <<endl;
    checkMediaValori(v_normalizzato, media);
    cout<<"Varianza: "<< varianza(v_normalizzato,media) <<endl;
    cout<<"----------------------------------------------------"<<endl;

    cout <<"scrittura su file in corso..."<<endl;
    scritturaFile(v_normalizzato);
    cout <<"scrittura su file OK"<<endl;
    cout<<"----------------------------------------------------"<<endl;
    return EXIT_SUCCESS;
}
