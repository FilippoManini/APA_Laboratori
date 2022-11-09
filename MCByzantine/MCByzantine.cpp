//AUTORE: Filippo Manini
//MATRICOLA: 4798004

#include <iostream>
#include <vector>
#include <cstdlib>   
#include <ctime>    
#include <cmath>
#include <map>

using namespace std;

const int t = 1; //generali inaffidabili

//╔══════════════════════════════════════════════════════════════════════╗
//║                           FUNZIONI DI SUPPORTO                       ║
//╚══════════════════════════════════════════════════════════════════════╝

//simula il lancio di una monetta ritornando 0 o 1 
int randB() 
{
    return 0+rand()%(2);
}

//scelta di partenza dei processi
void sceltaStart(int scelta, vector<int>& g1, vector<int>& g2, vector<int>& g3, vector<int>& gInaf)
{
    if(scelta == 0){
        g1[0] = randB();
        g2[0] = g1[0];
        g3[0] = g1[0];
    }
    else{
        g1[0] = randB();
        g2[0] = randB();
        g3[0] = randB();
    }
    gInaf[0] = randB();
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI DI STAMPA                          ║
//╚══════════════════════════════════════════════════════════════════════╝

//stampa dei generali
void printGenereale(vector<int> g, string name)
{
    cout<<name<<": ";
    for(int i=0; i<g.size(); ++i) {
        cout<<g.at(i)<<" ";
    }
    cout<<endl;
}

// passare come parametri i primi 3 affidabili e lultimo inaffidabile
void printGenerali(vector<int> g1, vector<int> g2, vector<int> g3, vector<int> gInaf)
{
    cout<<"STAMPA DEI GENERALI"<<endl;
    printGenereale(g1,"G1");
    printGenereale(g2,"G2");
    printGenereale(g3,"G3");
    printGenereale(gInaf,"GX");
}

void printSingleValue(int g1, int g2, int g3, int gInaf)
{
    cout<<"G1: "<<g1<<endl;
    cout<<"G2: "<<g2<<endl;
    cout<<"G3: "<<g3<<endl;
    cout<<"GX: "<<gInaf<<endl;
}

void stampoFrequenza(map<int, int> freq)
{
	cout << "frequenza del #round:";
	cout << endl<<"#ROUND" <<'\t'<< "FREQUENZA"<<endl;

    for(map<int, int>::iterator it = freq.begin(); it != freq.end(); it++){
        cout << it->first <<'\t'<< it->second << endl;
	}
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI SPECIFICHE                         ║
//╚══════════════════════════════════════════════════════════════════════╝
//funzione di trasmissione ausiliaria 
void trasmissioneAux(vector<int>& g, int g2, int g3, int gInaf)
{
   g[1] = g2;
   g[2] = g3;
   g[3] = gInaf;
}

//i processi affidabili inviano lo steso valore a tutti gli altri processi 
//l'inaffidabile aspetta di ricevere e invia bit 1-b(i).
void trasmissione(vector<int>& g1, vector<int>& g2, vector<int>& g3, vector<int>& gInaf)
{
    //per costruzzione sono tutti della stessa dimensione 
    //inaffidabile invia bit 1-b(i).
    trasmissioneAux(g1, g2.at(0), g3.at(0), 1-g1.at(0));
    trasmissioneAux(g2, g1.at(0), g3.at(0), 1-g2.at(0));
    trasmissioneAux(g3, g1.at(0), g2.at(0), 1-g3.at(0));
    trasmissioneAux(gInaf, g1.at(0), g2.at(0), g3.at(0));
}

//conto il valore maggioritario 
int valoreMaggioritario(vector<int> g)
{
    //allinterno del vector abbiamo solo 0 o 1
    int cont0 = 0;
    int cont1 = 0;

    for(int i=0; i<g.size(); ++i)
    {
        if(g.at(i) == 0){
            ++cont0;
        }else if(g.at(i) == 1){
            ++cont1;
        }
    }

    if(cont0 > cont1){
        return 0;
    }else{
        return 1;
    }
}

//conto quante volte appare maj
tallyFunz(vector<int> g, int maj) 
{
    int cont = 0;
    for(int i=0; i<g.size(); ++i)
    {
        if(maj == g.at(i))
            ++cont;
    }
    return cont;
}

//controllo finale dei valori e inizializzazione dei dati per il prossimo giro
//ritona true se gTally >= (2*1)+1
bool checkData(vector<int>& g, int gMaj, int gTally, int moneta)
{
      if(gTally >= (2*t +1) ){ //(2*1)+1 = 3
            g[0] = gMaj;
            return true;
        }
        else if(moneta == 1) //testa
        {
            g[0] = 1;
            return false;
        }else{
            g[0] = 0;
            return false;
        }
}

//esco quando tutti i generali affidabili sono d'accordo
//questo vuol dire che i generali hanno passato il vincolo gTally >= (2*1)+1 e ritornado false esco dal while
bool checkEnd(bool g1, bool g2, bool g3)
{
    if(g1 == true && g2 == true && g3 == true){
        return false;
    }
    else{
        return true;
    }
}

//calcolo del valore medio 
double valoreMedio(vector<int> v, int numAlg)
{
    double totNumeratore = 0;

    for(int i=0; i<v.size(); ++i)
    {
        totNumeratore += v.at(i);
    }
    return (totNumeratore/numAlg);
}

//calcolo della varianza prendendo come input un vettore e la medie dei volori 
double varianza(vector<int> v, double media)
{
    double varianza = 0;

    for(int i=0; i<v.size(); ++i)
    {
        varianza = varianza + pow( (v.at(i) - media), 2);
    }
    return varianza/v.size();
}

//calcolo la frequenza del #round 
void calcFreq(vector<int> numRound,  map<int, int>& freq)
{
    for (int i=0; i<numRound.size(); i++)
    {
        freq[ numRound[i] ]++;  //utilizzo la posizione della mappa come il #round designato e incremento il valore della medesima chiave 
    } 
}

//conta quante volte volte compaiono valori maggiore di quello passato
double countMedia(vector<int> v, int n)
{
    int res = 0;
    for(int i=0; i<v.size(); ++i)
    {
        if(v.at(i) > n){
            ++res;
        }
    }
    return res;
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          MAIN                                        ║
//╚══════════════════════════════════════════════════════════════════════╝
int main()
{
    srand(time(NULL)); //poiche basta chiamarla una sola volta all'interno del programma per fissare il seme
    int nG = 4; //numero complessivo dei generali
    int gMajTot = 0; //massimo corrente valore maggioritario 
    int numAlg = 100000; //numero per il quale girera' l'algoritmo 10^5
    vector<int> numRound (numAlg);

    int scelta;
    cout<<"scelta se processi affidabili partono allo stesso modo == [0] or [1]: "; 
    cin>>scelta;  

    for(int i=0; i<numAlg; ++i)
    {
        int round = 0;

        //Considera il caso di un sistema distribuito costituito da n=4 processi di cui il quarto è inaffidabile. 
        vector<int> g1 (nG); //generale 1: dove v[0] = spedisce && v[1..3] = riceve
        vector<int> g2 (nG);
        vector<int> g3 (nG);
        vector<int> gInaf (nG); //generale innaffidabile 
        
        //scelta se processi affidabili partono allo stesso modo == 0  
        sceltaStart(scelta, g1, g2, g3, gInaf);

        //cout<<"Dopo scelta iniziale"<<endl;   //DEBUG
        //printGenerali(g1, g2, g3, gInaf);     //DEBUG

        bool loop = true;
        while(loop){

            int monetaGlobale = randB();
            /*cout<<"--------------------"<<endl; //DEBUG
            cout<<"#round: "<<round<<endl;
            printGenerali(g1, g2, g3, gInaf);*/
            
            trasmissione(g1, g2, g3, gInaf);
            //cout<<"--------------------"<<endl; //DEBUG
            //cout<<"dopo trasmissione"<<endl;
            //printGenerali(g1, g2, g3, gInaf);

            vector<int> gMaj (nG); //dove v[0..2] = affidabile && v[3] = inaff
            gMaj[0] = valoreMaggioritario(g1);
            gMaj[1] = valoreMaggioritario(g2);
            gMaj[2] = valoreMaggioritario(g3);
            gMaj[3] = valoreMaggioritario(gInaf);
            //cout<<"--------------------"<<endl;
            //cout<<"VALORI MAGGIORITARI"<<endl;
            //printSingleValue(gMaj.at(0), gMaj.at(1), gMaj.at(2), gMaj.at(3)); //DEBUG
            
            vector<int> gTally (nG);
            gTally[0] = tallyFunz(g1, gMaj[0]);
            gTally[1] = tallyFunz(g2, gMaj[1]);
            gTally[2] = tallyFunz(g3, gMaj[2]);
            gTally[3] = tallyFunz(gInaf, gMaj[3]);
            //cout<<"--------------------"<<endl;
            //cout<<"TALLY: "<<endl;
            //printSingleValue(gTally.at(0), gTally.at(1), gTally.at(2), gTally.at(3)); //DEBUG
            
            //cout<<"--------------------"<<endl;
            bool g1B = checkData(g1, gMaj.at(0), gTally.at(0), monetaGlobale);
            bool g2B = checkData(g2, gMaj.at(1), gTally.at(1), monetaGlobale);
            bool g3B = checkData(g3, gMaj.at(2), gTally.at(2), monetaGlobale);
            //printSingleValue(g1.at(0), g2.at(0), g3.at(0), gInaf.at(0)); //DEBUG

            //controllo finale che ½ b(i) = v, almeno la metà dei generali onesti è d’accordo sul valore giusto 
            loop = checkEnd(g1B, g2B, g3B);

            ++round;
        }
        //cout<<"#round: "<<round<<" - ciclo: "<<i<<endl; //DEBUG

        // inserisco il numero di round di questa esecuzione nella posizione del ciclo che scorre il numero di esecuzione dell'algoritmo corrente
        numRound[i] = round;
    }
    cout<<endl<<"--------------------"<<endl;
    double media = valoreMedio(numRound, numAlg);
    cout<<"MEDIA dei round: "<<media<<endl;
    cout<<"VARIANZA: "<<varianza(numRound, media)<<endl;

    cout<<"--------------------"<<endl;
    map<int, int> freq;
    calcFreq(numRound, freq);
    stampoFrequenza(freq);

    cout<<"--------------------"<<endl;
    cout<<"#round dopo il quale la probabilita' che l'accordo e' raggiunto e' piu' grande del 99.9%"<<endl;
    double prob = 0;
    while(prob < 99.9)
    {
        prob = static_cast<double>(100) - (countMedia(numRound,media) / numAlg);
        cout<<"media #round: "<<media<<"  probabilita': "<<prob<<endl;
        media += 0.1;
    }
    media -= 0.1; //per levare somma del giro superflua 
    cout<<endl<<"#round finale: "<<media<<endl;
    
    return EXIT_SUCCESS;
}