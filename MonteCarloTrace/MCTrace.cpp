//AUTORE: Filippo Manini
//MATRICOLA: 4798004

#include <iostream>
#include <cstdlib>   
#include <ctime>    
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

//╔══════════════════════════════════════════════════════════════════════╗
//║                           FUNZIONI DI SUPPORTO                       ║
//╚══════════════════════════════════════════════════════════════════════╝

//ritorna una valore double nel range passato in input
double randB(double min, double max) 
{
	return min + (double)(rand()) / ((double)(RAND_MAX/(max - min)));
}

void scritturaFile(vector<long double> v, string nomeFile)
{
	ofstream fout(nomeFile, ios::app); //crea se non esiste e se esiste aggiunge
	
	fout<<endl<<endl<<endl;
    for(int i=0; i<v.size(); ++i)
    {
        fout<<v.at(i)<<endl;
    }
	fout.close();
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                           FUNZIONI STAMPE                            ║
//╚══════════════════════════════════════════════════════════════════════╝

//stampa della matrice
void printMatrix(double **m, int row, int column)
{
	for(int i = 0; i < row; ++i)
	{
		for(int j = 0; j < column; ++j)
		{
            cout << m[i][j] <<"\t";
		}
		cout << endl;
	}
}

//stampa del vettore
void stampaV(vector <long double> v)
{
	for(int i = 0; i < v.size(); ++i)
	{
		cout<<v.at(i)<<endl;
	}
	cout<<endl;
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                           FUNZIONI MATRICI                           ║
//╚══════════════════════════════════════════════════════════════════════╝

//creazione della matrice vuota in memoria
double** createMatrix(int row, int column) 
{
	if(row < 1)
		return NULL;

	double** matrice = new double*[row];

	for(int i = 0; i < row; ++i){
		matrice[i] = new double[column];
    }
	return matrice;
}

// trasposta della matrice m
double** trasposta(double **m, int row, int column)
{
    double **trasposta = createMatrix(column, row);

    for(int i = 0; i < row; ++i)
	{
		for(int j = 0; j < column; ++j)
		{
			trasposta[j][i] = m[i][j];
		}
	}

    return trasposta;
}

/*
moltiplico righe per colonne 
per definizione il numero delle colonne di m1 deve essere uguale al numero delle righe di m2
ritorno la matrice risualtante che avra' tante righe quante m1 e tante colonne quante m2*/
double** prodottoMatrix(double **m1, double **m2, int m1row, int m1column, int m2row, int m2column )
{
	//creo la matrice risultante vuota
	double **mRisult = createMatrix(m1row, m2column);
	
	for(int i = 0; i < m1row; ++i)
	{
		for(int j = 0; j < m2column; ++j)
		{
			for(int x = 0; x < m1column; ++x)
			{
				mRisult[i][j] += m1[i][x] * m2[x][j];
			}
		}
	}
	return mRisult;
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI SPECIFICHE                         ║
//╚══════════════════════════════════════════════════════════════════════╝

//campionato uniformemente nell'intervallo [0,1].
void campionamento(double **m, int row, int column, double min, double max)
{
    for(int i = 0; i < row; ++i)
	{
		for(int j = 0; j < column; ++j)
		{
            m[i][j] = randB(min,max);
		}
	}
}

// inizializzo vettore Rademacher di valori di -1 e 1
void campionamentoRademacher(double **m, int row, int column, int min, int max)
{
	for(int i = 0; i < row; ++i)
	{
		for(int j = 0; j < column; ++j)
		{
			double temp = min+rand()%(max-min+1);
			while (temp == 0){
				temp = min+rand()%(max-min+1);
			}
            m[i][j] =  temp;
		}
	}
}

//Tr(A) è data dalla somma degli elementi diagonali di A e che è invariante per cambio di base.
long double traccia(double **m, int row, int column)
{
	double traccia = 0;

	//scorrere righe o colonne che e' uguale essendo matrice quadrata
	for(int i = 0; i < row; ++i)
	{
		traccia += m[i][i];
	}
	return traccia;
}

//calcolo della norma di Frobenius
long long int normaFrobenius(double **m, int row, int column)
{
	double nf = 0; //norma di Frobenius

	for(int i = 0; i < row; ++i)
	{
		for(int j = 0; j < column; ++j)
		{
			nf += pow ( abs(m[i][j]) , 2); //abs valore assoluto
		}
	}

	return (nf); 
}

//funzione dell'algoritmo MonteCarloTraceAux
void MonteCarloTraceAux(double **a, int row, int column, int M, long double &stimaTRa, long double &varianza)
{
	double **u;
	double **uT;
	double **x_m;
	
	vector<double> vX_m (M); //nuovo <x_m>
	vector<double> vOracolo (M);

	for(int m=1; m < M+1; ++m)
	{
		//PUNTO 1.
		int uRow = column;
		int uColumn = 1;
		u = createMatrix(uRow, uColumn); //per la moltiplicazione tra matrici: numero di colonne della prima matrice deve corrispondere al numero di righe della seconda matrice
		campionamentoRademacher(u, uRow, uColumn, -1, 1); //campiono u come vettore di Rademacher
		//cout<<"stampo vettore 'u' di Rademacher:"<<endl; //DEBUG
		//printMatrix(u, uRow, uColumn); //DEBUG

		//PUNTO 2.
		uT = trasposta(u, uRow, uColumn); //oracolo
		//per uT inverti righe e colonne
		int uTRow = uColumn;
		int uTColumn = uRow; 
		//cout<<"stampo vettore trasposto 'u' di Rademacher:"<<endl; //DEBUG
		//printMatrix(uT, uTRow, uTColumn); //DEBUG
		//cout<<"ORACOLO:"<<endl; //DEBUG
		x_m = prodottoMatrix(uT, a, uTRow, uTColumn, row, column);
		//cout<<"stampo uT * A:"<<endl; //DEBUG
		//printMatrix(x_m, uTRow, column); //DEBUG
		x_m = prodottoMatrix(x_m, u, uTRow, column, uRow, uColumn);
		//cout<<"stampo (uT*A)*u"<<endl; //DEBUG
		//printMatrix(x_m, uTRow, uColumn); //DEBUG
		
		//PUNTO 3.
		//mi salvo i valori dell'oracolo 
		vOracolo[m-1] = x_m[0][0]; //-1 perche parto da 1 e la prima posizione del vettore e' 0
		
		//if per il primo ciclo
		if(m-1 == 0){
			vX_m[m-1] = (vOracolo[m-1] / m);
		}
		else{
			vX_m[m-1] = ( vX_m[m-2] + ( (vOracolo[m-1] - vX_m[m-2]) / m) );
		}
	}
	//stima Tr(A)
	stimaTRa = vX_m[M-1];

	//varianza
	varianza = 0;
	for(int m=1; m < M+1; ++m){
		varianza = (pow(vOracolo[m-1] - stimaTRa, 2)/(M-1));
	}

	//libero la memoria
	delete [] u;
	delete [] uT;
	delete [] x_m;
}

//funzione dell'algoritmo MonteCarloTrace
void MonteCarloTrace(double **a, int row, int column, int M, vector<long double>& vStimaTRa, vector<long double>& vVarianza)
{
	const int run = 100;
	long double stimaTRa = 0;
	long double varianza = 0;

	for(int i=0; i<run; ++i)
	{
		MonteCarloTraceAux(a, row, column, M, stimaTRa, varianza);
		
		vStimaTRa[i] = stimaTRa;
		vVarianza[i] = varianza;
	}
}

//funzione che stima 
long double stimaMediaTRa(vector<long double> v, int M)
{
	int stim = 0;
	for(int i=0; i<v.size(); ++i)
	{
		stim += v.at(i);
	}
	return stim/v.size();
}

//conta quanti valori della varianza sono <= del 2*nF/vM.at(M)
int confVar(vector<long double> vVarianza, long double nFplus)
{
	int cont = 0;
	for(int i=0; i<vVarianza.size(); ++i)
	{
		if(vVarianza.at(i) <= nFplus){
			++cont;
		}
	}
	return cont;
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          MAIN                                        ║
//╚══════════════════════════════════════════════════════════════════════╝

int main()
{
    srand(time(NULL)); //poiche basta chiamarla una sola volta all'interno del programma per fissare il seme 
    system("del test.txt");
	
    int row = 300;
    int column = 300;
    double **b = createMatrix(row, column);
    campionamento(b, row, column, 0, 1);
	//cout<<"stampo matrice B di partenza:"<<endl; //DEBUG
    //printMatrix(b, row, column); //DEBUG
    
	double **bT = trasposta(b, row, column);
	//cout<<"-----------------------------------"<<endl; //DEBUG
	//cout<<"stampo matrice B trasposta:"<<endl; //DEBUG
    //printMatrix(bT, row, column); //DEBUG

	double **a = prodottoMatrix(bT, b, row, column, row, column);
	//cout<<"-----------------------------------"<<endl; //DEBUG
	//cout<<"stampo matrice A = bT*b semidefinita:"<<endl; //DEBUG
	//printMatrix(a, row, column); //DEBUG

	cout<<"-----------------------------------"<<endl;
	cout<<"traccia Tr(A) matrice A  = "<<traccia(a, row, column)<<endl;
	long long int nF = normaFrobenius(a, row, column);
	cout<<"matrice A norma di Frobenius (||A||_F)^2 = "<<nF<<endl;

	vector<int> vM = {5, 10, 25, 100}; //vector di m
	
	for(int M = 0; M < 4; ++M)
	{
		cout<<"-----------------------------------"<<endl;
		const int run = 100;
		vector<long double> vStimaTRa (run);
		vector<long double> vVarianza (run);

		cout<<endl<<"M = "<< vM.at(M) <<endl;
		MonteCarloTrace(a, row, column, vM.at(M), vStimaTRa, vVarianza);

		cout<<"stima di traccia Tr(A): "<<stimaMediaTRa(vStimaTRa, vM.at(M))<<endl;
		scritturaFile(vStimaTRa, "test.txt");
		long double nFplus = static_cast<long double> (2*nF/vM.at(M));

		cout<<confVar(vVarianza, nFplus)<<"% dei valori della varianza e' <= 2*||A||/M"<<endl;
		cout<<"\t"<<"2*||A||/M: "<< nFplus<<endl;
	}
	cout<<"-----------------------------------"<<endl;
	
	//libero la memoria
	delete [] b;
	delete [] bT;
	delete [] a;
    return EXIT_SUCCESS;
}