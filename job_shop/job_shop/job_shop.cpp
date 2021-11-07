// ordonnancement.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include "job_shop.h"

int main()
{
    solution mySolution = {};
    int meilleur = 0;
    srand(123456789);
    initialiser();

     for (int i = 0; i < 1000; i++)
     {
          mySolution = generer_aleatoire(mySolution);
          mySolution = evaluer(mySolution);
          cout << "iteration " << i << "\n\n";
          for (int i = 1; i < myData.nbPiece * myData.nbMachine + 1; i++) {
              cout << mySolution.lambda[i] << " ";
          }
          if (i == 0)
          {
              meilleur = mySolution.cost;
          }

          cout << "\ncout : " << mySolution.cost << "\n";
          mySolution = recherche_locale(mySolution);
          for (int i = 1; i < myData.nbPiece * myData.nbMachine + 1; i++) {
              cout << mySolution.lambda[i] << " ";
          }
          cout << "\ncout : " << mySolution.cost;

          cout << "\n\n";
          if (mySolution.cost < meilleur)
          {
              meilleur = mySolution.cost;
          }
     }

      cout << "\nmeilleur solution : " << meilleur;
     
   grasp(10, 10, 10);
}





