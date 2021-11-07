#include "job_shop.h"

int T[max_lambda] = { 0 };

Data myData;

void initialiser() {
    string const fichier("la01.txt");
    ifstream flux(fichier.c_str());

    flux >> myData.nbPiece;
    flux >> myData.nbMachine;

    for (int i = 1; i < myData.nbPiece + 1; i++) {
        for (int j = 1; j < myData.nbMachine + 1; j++) {
            flux >> myData.mach[i][j];
            flux >> myData.d[i][j];
        }
    }
}

solution generer_aleatoire(solution mySolution) {
    int pieces[maxp];
    int etapes[maxp];
    int taille = myData.nbPiece;
    int random;
    int cpt = 1;


    for (int i = 1; i < myData.nbPiece + 1; i++)
    {
        pieces[i] = i;
        etapes[i] = myData.nbMachine;
    }

    for (int i = 1; i <= myData.nbMachine * myData.nbPiece; i++) {
        random = (rand() % taille) + 1;
        mySolution.lambda[i] = pieces[random];
        etapes[random]--;

        if (etapes[random] == 0) {
            etapes[random] = etapes[taille];
            pieces[random] = pieces[taille];
            taille--;
        }
    }

    return mySolution;

}

solution initialisation_evaluer(solution mySolution) {
    for (int i = 0; i <= myData.nbPiece; i++)
    {
        mySolution.np[i] = 0;
    }

    for (int i = 0; i <= myData.nbMachine; i++)
    {
        mySolution.nm[i] = { 0, 0 };
    }

    for (int i = 1; i <= myData.nbPiece; i++)
    {
        mySolution.peres[i][1] = { 0,0 };
    }

    mySolution.cost = 0;
    return mySolution;
}

solution evaluer(solution mySolution) {

    mySolution = initialisation_evaluer(mySolution);

    for (int i = 1; i <= myData.nbMachine * myData.nbPiece; i++)
    {
        int j = mySolution.lambda[i];
        mySolution.np[j]++;
        int etape = mySolution.np[j];
        int machine = myData.mach[j][etape];

        // arcs horizontaux
        if (etape == 1)
        {
            mySolution.m[j][etape] = 0;
        }
        else {
            mySolution.m[j][etape] = mySolution.m[j][etape - 1] + myData.d[j][etape - 1];
            mySolution.peres[j][etape] = { j, etape - 1 };
        }


        //arcs disjonctifs
        if (mySolution.nm[machine].piece != 0 && mySolution.nm[machine].rang != 0)
        {
            int p_prec = mySolution.nm[machine].piece;
            int r_prec = mySolution.nm[machine].rang;
            int val = mySolution.m[p_prec][r_prec] + myData.d[p_prec][r_prec];
            if (val > mySolution.m[j][etape])
            {
                mySolution.peres[j][etape] = { p_prec,r_prec };
                mySolution.m[j][etape] = val;
            }

        }

        if (etape == myData.nbMachine)
        {
            int val = mySolution.m[j][etape] + myData.d[j][etape];
            if (val > mySolution.cost)
            {
                mySolution.cost = val;
                mySolution.pere_fin = { j, etape };
            }
        }

        mySolution.nm[machine] = { j,etape };

    }
    return mySolution;
}

int* permute_lambda(int lambda[], int i, int j)
{
    int nouveau_lambda[maxm * maxp];

    for (int k = 1; k <= myData.nbMachine * myData.nbPiece; k++)
    {
        nouveau_lambda[k] = lambda[k];
    }
    nouveau_lambda[i] = lambda[j];
    nouveau_lambda[j] = lambda[i];

    return nouveau_lambda;
    ;
}

int position(int lambda[], int piece, int etape)
{
    int size = myData.nbMachine * myData.nbPiece;
    int cpt = 0;
    for (int k = size; k > 0; k--)
    {
        if (lambda[k] == piece)
        {
            cpt++;
            if (cpt == myData.nbMachine - etape + 1)
                return k;
        }
    }
}

solution recherche_locale(solution mySolution) {
    int itr = 0;
    solution other_solution;

    mySolution = evaluer(mySolution);
    pere x = mySolution.pere_fin,
        pred_x = mySolution.peres[x.i][x.j];
    while (pred_x.i != 0 && pred_x.j != 0 && itr < 5 * myData.nbPiece)
    {
        itr++;
        if (myData.mach[x.i][x.j] == myData.mach[pred_x.i][pred_x.j])
        {
            int* lambda_permute = permute_lambda(mySolution.lambda, position(mySolution.lambda, x.i, x.j), position(mySolution.lambda, pred_x.i, pred_x.j));
            for (int i = 1; i <= myData.nbMachine * myData.nbPiece; i++) {
                other_solution.lambda[i] = lambda_permute[i];
            }


            other_solution = evaluer(other_solution);
            if (other_solution.cost < mySolution.cost)
            {
                mySolution = other_solution;
                x = mySolution.pere_fin;
                pred_x = mySolution.peres[x.i][x.j];
            }
            else
            {
                x = pred_x;
                pred_x = mySolution.peres[x.i][x.j];
            }
        }
        else
        {
            x = pred_x;
            pred_x = mySolution.peres[x.i][x.j];
        }
    }
    return mySolution;
}

int hash_calcul(solution mySolution) {
    int hash_code = 0;
    for (int i = 1; i <= myData.nbPiece; i++)
    {
        for (int j = 1; j <= myData.nbMachine; j++)
        {
            hash_code += mySolution.m[i][j];
        }
    }


    return hash_code % max_lambda;
}

int* gnenerer_voisin(int lambda[]) {
    int p1, p2;
    int size = myData.nbMachine * myData.nbPiece;
    p1 = rand() % size + 1;
    do
    {
        p2 = rand() % size + 1;
    } while (lambda[p1] == lambda[p2]);
    int* voisin = permute_lambda(lambda, p1, p2);

    return voisin;
}

void afficher_lambda(int lambda[]) {
    for (int i = 1; i <= myData.nbMachine * myData.nbPiece; i++)
    {
        cout << lambda[i] << " ";
    }
    cout << "\n";

}

void grasp(int nb_voisins, int nb_els, int nb_iter)
{
    solution bestSolution = {}, voisin = {}, best_voisin = {};
    int hash_code, best_cost = INT32_MAX;

    for (int i = 0; i < nb_iter; i++)
    {
        do
        {
            bestSolution = generer_aleatoire(bestSolution);
            bestSolution = recherche_locale(bestSolution);

        } while (T[hash_calcul(bestSolution)] == 1);
        T[hash_calcul(bestSolution)] = 1;

        cout << "lambda aleatoire: ";
        afficher_lambda(bestSolution.lambda);
        cout << "cout: " << bestSolution.cost << "\n";

        for (int j = 1; j <= nb_els; j++)
        {
            for (int k = 1; k <= nb_voisins; k++)
            {
                int* lambda_voisin = gnenerer_voisin(bestSolution.lambda);

                for (int l = 1; l <= myData.nbMachine * myData.nbPiece; l++)
                {
                    voisin.lambda[l] = lambda_voisin[l];
                }

                voisin = recherche_locale(voisin);
                hash_code = hash_calcul(voisin);
                if (T[hash_code] != 1)
                {
                    if (voisin.cost < best_cost)
                    {
                        best_cost = voisin.cost;
                        best_voisin = voisin;
                    }
                }
            }
            bestSolution = best_voisin;
        }
        cout << "best lambda voisin : ";
        afficher_lambda(bestSolution.lambda);
        cout << "best voisin cost : " << bestSolution.cost << "\n";
    }
}