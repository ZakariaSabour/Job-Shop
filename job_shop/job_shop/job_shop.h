#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

const int maxp = 20;
const int maxm = 20;
const int max_lambda = 10;


typedef struct {
    int nbMachine;
    int nbPiece;
    int mach[maxp][maxm];
    int d[maxp][maxm];
}Data;

extern Data myData;

typedef struct pere {
    int i;
    int j;
}pere;

typedef struct opMach {
    int piece;
    int rang;
}opMach;

typedef struct solution {
    int m[maxp][maxm];
    pere peres[maxp][maxm];
    pere pere_fin;
    int np[maxp];
    opMach nm[maxm];
    int cost;
    int lambda[maxp * maxm];

}solution;


void initialiser();
solution generer_aleatoire(solution mySolution);
solution initialisation_evaluer(solution mySolution);
solution evaluer(solution mySolution);
int  position(int lambda[], int piece, int etape);
int* permute_lambda(int lambda[], int i, int j);
solution recherche_locale(solution mySolution);
int hash_calcul(solution mySolution);
int* gnenerer_voisin(int lambda[]);
void grasp(int nb_voisins, int nb_els, int nb_iter);