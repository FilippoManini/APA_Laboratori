//AUTORE: Filippo Manini
//MATRICOLA: 4798004

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

typedef string Label;

struct vertexNode
{
    Label label;
    bool visited;
    vector <Label> adjList;

}typedef Graph;

//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI DEL GRAFO                          ║
//╚══════════════════════════════════════════════════════════════════════╝

Graph getVertice(vector<Graph> g, Label l)
{
    int i =0;
    while(g.at(i).label.compare(l) != 0 ){
        ++i;
    }
    return g.at(i);
}

//restituisce true se il vertice e' nel grafo
bool verticeInGrafo(Label l, vector<Graph> g)
{
    for(int i = 0; i < g.size(); ++i)
    {
        if(g.at(i).label.compare(l) == 0){
            return true;
        }
    }
    return false;
}

//aggiunta del vertice
bool aggVertice(Label l, vector<Graph>& g)
{
    if(verticeInGrafo(l, g)){
        return false;
    }

    Graph v;
    v.label = l;

    vector<Label> aList;
    v.adjList = aList;
    v.visited = false; //inizilizzo i vertici a falso 

    g.push_back(v); //inserimento in fondo 
    return true;
}

//ritorna il numero di vertici nel grafo passato 
int numVertice(vector<Graph>& g)
{
    return g.size();
}

//funzione che cancella il vertice passato con Label l
bool cancVertice(vector<Graph>& g, Label l)
{
    for(int i = 0; i < g.size(); ++i)
    {
        if(!verticeInGrafo(g.at(i).label, g)){
            return false;
        }
        else if(g.at(i).label.compare(l) == 0)
        {
            g.erase(g.begin()+i);
            return true;
        }
    }
    return false;
}

//ritorna il numero di archi nel grafo passato 
int numArchi(vector<Graph>& g)
{
    int cont = 0;
    for(int i = 0; i < g.size(); ++i)
    {
        cont += g.at(i).adjList.size();
    }
    return cont/2;
}

//Aggiunge un arco tra i due archi from e to
bool aggArco(Label from, Label to, vector<Graph>& g)
{
    if(from.compare(to) == 0){
        return false;
    }
    if(!verticeInGrafo(from,g) || !verticeInGrafo(to,g)){
        return false;
    }
    for(int i = 0; i < g.size(); ++i)
    {
        if(g.at(i).label.compare(from) == 0)
        {
            g.at(i).adjList.push_back(to);
        }
        else if(g.at(i).label.compare(to) == 0)
        {
            g.at(i).adjList.push_back(from);
        }
    }
    return true;
}

//cancello gli archi con Label = l
bool cancArchi(vector<Graph>& g, Label l)
{
    for(int i = 0; i < g.size(); ++i)
    {
        for(int j = 0; j < g.at(i).adjList.size() ; j++)
        {
            if( g.at(i).adjList.at(j).compare(l) == 0 ) //sono uguali
            {
                g.at(i).adjList.erase(g.at(i).adjList.begin()+j);
                
                j = -1; //RIPARTO
            }
        }
    }
    return true;
}

bool cancella(vector<Graph>& g, Label lCanc)
{
    cancArchi(g, lCanc); 
    cancVertice(g, lCanc);
    return true;
}

//Scelta casuale di un vertice
Label verticeCasuale(vector<Graph> g)
{
    int vCas = rand()%g.size();
    return g.at(vCas).label;
}

//setto a true il vertice visitato
bool setVerticeVisitato(vector<Graph>& g, Label l)
{
    for(int i = 0; i < g.size(); ++i)
    {
        if(g.at(i).label == l){
            g.at(i).visited = true;
            return true;
        }
    }
    return false;
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                           FUNZIONI AUSILIARE                         ║
//╚══════════════════════════════════════════════════════════════════════╝

//funzione ausialire per il controllo dell'arco minimo 
void controlloArchi(int nArchi, int& nArchiMin)
{
    if(nArchiMin > nArchi){
        nArchiMin = nArchi;
    }
}

//calcolo della frequenza 
int frequenza(vector<int> v, int nArchiMin)    
{
    int frq = 0;
    for(int i = 0; i < v.size(); ++i)
    {
        if(v.at(i) == nArchiMin)
        {
            ++frq;
        }
    }
    return frq;
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                           FUNZIONI DI STAMPA                         ║
//╚══════════════════════════════════════════════════════════════════════╝
void stampaListaDiAdiacenza(vector<Label> vAdiacenza)
{
    for(int i = 0; i < vAdiacenza.size(); ++i)
    {
        cout << vAdiacenza.at(i) << "; ";
    }
    cout<<endl;
}

void stampaGrafo(vector<Graph> g)
{
    if(g.size() < 1){
        cout<<"Grafo vuoto"<<endl;
    }
    
    for(int i = 0; i < g.size(); ++i)
    {
        cout << g.at(i).label<<" (" << g.at(i).visited << "): ";

        stampaListaDiAdiacenza(g.at(i).adjList);
    }
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI SPECIFICHE                         ║
//╚══════════════════════════════════════════════════════════════════════╝
//Creazione grafo di Fritsch
void creazioneGrafoFritsch(vector<Graph>& g)
{
    vector<Label> vLabel = {"A","B","C","D","E","F","G","H","I"}; //lista dei vertici per il grafo di Fritsch
    
    //inserimento dei vertici per il grafo di Fritsch
    for(int i = 0; i < vLabel.size(); ++i){
        aggVertice(vLabel.at(i), g);
    }

    //Creazione archi
    aggArco("A","G", g);
    aggArco("A","I", g);
    aggArco("A","B", g);
    aggArco("A","C", g);
    aggArco("A","D", g);

    aggArco("B","C", g);
    aggArco("B","G", g);
    aggArco("B","E", g);

    aggArco("C","D", g);
    aggArco("C","E", g);
    aggArco("C","F", g);

    aggArco("D","I", g);
    aggArco("D","F", g);
    
    aggArco("E","F", g);
    aggArco("E","G", g);
    aggArco("E","H", g);
    
    aggArco("F","H", g);
    aggArco("F","I", g);

    aggArco("G","H", g);
    aggArco("G","I", g);

    aggArco("H","I", g);
}

//controlla se tutti i vertici adiacenti al label passato sono stati visitati ritornando true
bool listaDiAdiacenzaVisistata(vector<Graph>& g, Label l)
{
    bool vVisitato = true;
    for(int i = 0; i < g.size(); ++i)
    {
        if(g.at(i).label.compare(l) == 0) //mi posiziono sul vertice 
        {
            for(int j = 0; j < g.at(i).adjList.size(); ++j)
            {
                if(!(getVertice(g, g.at(i).adjList.at(j)).visited)){
                    vVisitato = false;
                }
            }
        }
    }
    return vVisitato;
}

//controllo se tutti i vertici adiacenti di quel vertice sono visitati 
Graph verticeNonVisistato(vector<Graph>& g, Label l)
{
    Graph vR;
    //rimango dentro finche tutta la lista di adiacenza non e' stata visitata
    while(!listaDiAdiacenzaVisistata(g, l) ) 
    {
        //prendo un vertice casuale dalla lista di adiacenza
        vector<Label> v = getVertice(g, l).adjList;
        int vCas = rand()%v.size();
        vR = getVertice(g, v.at(vCas));

        if(!vR.visited){
            return vR;
        }
    }

    return getVertice(g, l);
}

//aggiunge arco/i, unisco la lista di adiacenza del vertice che vado a tagliare al nuovo vertice  
bool unisciArci(vector<Graph>& g, Label lCanc, Label lPrev, vector<Label> vListAdiacenza)
{
    //scorro la lista di adiacenza del vertice da cancellare 
    for(int i = 0; i < vListAdiacenza.size(); ++i)
    {
        if(!verticeInGrafo(vListAdiacenza.at(i), g)){
            return false;
        }
        //se vertice della lista di adiacenza != dal precedente 
        else if(vListAdiacenza.at(i).compare(lPrev) != 0)
        {
            //aggiungo un arco tra l'ex collegamento del vertice eliminato e il vertice precedente
            aggArco(vListAdiacenza.at(i), lPrev, g);
        }
    }
    return true;
}

//rinomino la nuova label 
bool rinominoLabel(vector<Graph>& g, Label lCanc, Label lPrev)
{
    Label nuovoLabel = lPrev + lCanc; 
    //se il vertice non e' presente dal grafo ritorno false
    if(!verticeInGrafo(lCanc, g) || !verticeInGrafo(lPrev, g)){
        return false;
    }

    for(int i = 0; i < g.size(); ++i)
    {
        //controllo se il vertice esiste 
        if(!verticeInGrafo(g.at(i).label, g)){
            return false;
        }
        else if(g.at(i).label.compare(lPrev) == 0)
        {
            g.at(i).label = nuovoLabel;
        }
        else
        {
            for(int j = 0; j < g.at(i).adjList.size(); j++)
            {
                if(g.at(i).adjList.at(j).compare(lPrev) == 0)
                {
                    g.at(i).adjList.at(j) = nuovoLabel;
                }
            }
        }
    }
}

bool taglio(vector<Graph>& g, Label lCanc, Label lPrev)
{
    
    //mi salvo la lista di adiacenza del vertice da eliminare
    vector<Label> vListAdiacenza = getVertice(g, lCanc).adjList;
    //cancello vertice e archi
    cancella(g, lCanc);
    //unisco la lista di adiacenza del vertice che vado a tagliare 
    unisciArci(g, lCanc, lPrev, vListAdiacenza);
    //rinomino la label 
    rinominoLabel(g, lCanc, lPrev);
    return true;
}

//setto i vertici come non visistato 
bool setNonVisitato(vector<Graph>& g)
{
    for(int i = 0; i < g.size(); ++i)
    {
        g.at(i).visited = false;
    }
    return true;
}

bool taglioMinimo(vector<Graph>& g, Label randV, Label lPrev)
{
    //se il vertice non e' presente dal grafo ritorno false
    if(!verticeInGrafo(randV, g)){
        return false;
    }

    setVerticeVisitato(g, randV);
    Graph vCanc = verticeNonVisistato(g, randV);
    //cout<<"verticeNonVisistato: " << vCanc.label <<endl; //DEBUG

    //taglio quando nodo partenza == verticeNonVisistato
    if(vCanc.label.compare(randV) != 0)
    {
        lPrev = randV;
        taglioMinimo(g, vCanc.label, lPrev);
    }
    else
    {
        /*cout<< "taglio: "<<endl;
        cout<<"\tlCanc: "<<vCanc.label<<endl;
        cout<<"\tlPrev: "<<lPrev<<endl;*/ //DEBUG
        taglio(g, vCanc.label, lPrev);
        setNonVisitato(g);
    }
    return true;
}

//╔════════════════════════════════════════════════════════════════════════════════════════╗
//║                                       MAIN                                             ║
//╚════════════════════════════════════════════════════════════════════════════════════════╝
int main()
{
    srand(time(NULL)); //basta chiamarla una sola volta all'interno del programma per fissare il seme
    vector<Graph> g; //grafo di partenza 
    Label lPrev = "";
    int run = 100000; //numero di run 10^5
    
    vector<int> vTaglioMino (run); //vettore che conterra' il taglio minimo per ogni run

    creazioneGrafoFritsch(g);
    int nArchi = numArchi(g);
    int nArchiMin = nArchi; //inizializzo il numero di archi minimo con il numero di archi con cui parte il grafo 
    int nVertici = 0;
    cout<<"-----------------------------------"<<endl;
    cout <<"Grafo Fritsch iniziale:"<<endl;
    cout<<"numVertice: "<<numVertice(g)<<endl;
    cout<<"numArchi: "<<nArchi<<endl;
    stampaGrafo(g);
    
    for(int i = 0; i < run; ++i)
    {
        creazioneGrafoFritsch(g);
        
        for(int i = numVertice(g); i > 2; --i)
        {
            //cout<<"-----------------------------------"<<endl; //DEBUG
            Label vCasuale = verticeCasuale(g);
            //cout<<"vCasuale: "<<vCasuale<<endl; //DEBUG
            taglioMinimo(g, vCasuale, lPrev);
            /*cout<<"DOPO TAGLIO: "<<endl;
            cout<<"numVertice: "<<numVertice(g)<<endl;
            cout<<"numArchi: "<<numArchi(g)<<endl;
            stampaGrafo(g);*/ //DEBUG
        }
        //cout<<"-----------------------------------"<<endl; //DEBUG
        //cout<<"TAGLIO MINIMO: "<<endl; //DEBUG
        //nVertici = numVertice(g); //DEBUG
        //cout<<"numVertice: "<<nVertici<<endl; //DEBUG
        nArchi = numArchi(g);
        vTaglioMino[i] = nArchi; //mi salvo il taglio minimo
        //cout<<"numArchi: "<<nArchi<<endl; //DEBUG
        //stampaGrafo(g); //DEBUG

        if(i == 0){
            nArchiMin = nArchi;
        }
        controlloArchi(nArchi, nArchiMin);

        g.clear(); //elimino tutti gli elemneti in preparazione al nuovo ciclo
    }

    cout<<"-----------------------------------"<<endl;
    cout<<"MinCut del grafo dopo "<<run<<" run e' "<<nArchiMin<<" archi"<<endl;
    cout<<"stima p (2/n^2): "<< 2/pow(9,2) <<endl; //9 numero iniziale di vertici 
    double freqEmpirica = static_cast<double> (frequenza(vTaglioMino, nArchiMin))/vTaglioMino.size();
    cout<<"Frequenza empirica del taglio minimo: "<< freqEmpirica <<endl;
    cout<<"-----------------------------------"<<endl;
    return EXIT_SUCCESS;
}