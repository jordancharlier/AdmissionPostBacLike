#include <vector>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <random>
#include <algorithm>
#include <queue>

using std::vector;
using std::unordered_map;
using std::cout;
using std::iostream;
using std::queue;

enum { NB_ETABLISSEMENT = 5000, NB_ETUDIANT = 1000000, NB_MAX_CHOIX = 50, NB_MAX_CHOIX_ETABLISSEMENT = NB_ETUDIANT / NB_ETABLISSEMENT};

struct Etudiant
{
	int num_etudiant;
	int num_voeux;
  	int pris_ou_pas=0;
};

struct lEtud
{
  	vector<int> listEtab;
  	vector<double> poids;
  	double classement;
};

struct lEtab
{
	vector<Etudiant> etudiant;
    int etudiant_preselec=0;
};

typedef unordered_map<int, lEtud> myListEtud;
typedef unordered_map<int, lEtab> myListEtab;

struct myListEtud_et_pris
{
	myListEtab mListEtab;
	unordered_map<int, int> pris;
};

typedef myListEtud_et_pris cloporte;

double rdm_double() {
    double min = 4.0;
    double max = 20.0;
    return ((double)rand() * (max - min)) / (double)RAND_MAX + min;
}

myListEtud init(int nb)
{
    myListEtud l;
    int rdm;
    double rdm_classement, poids = 0.0;

    // Générateur de nombres pseudo-aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dis(0, NB_ETABLISSEMENT - 1);

    for(int i = 0; i < nb; i++) 
    {
    	rdm_classement = rdm_double();
        for(int j = 0; j < NB_MAX_CHOIX; j++) 
        {
        	rdm = dis(gen);
        	poids = (NB_MAX_CHOIX - j) + rdm_classement;

        	//Si l'element n'existe pas déja
        	if (!(std::find(l[i].listEtab.begin(), l[i].listEtab.end(),rdm) != l[i].listEtab.end()))
        	{
        		l[i].poids.push_back(poids);
        		l[i].listEtab.push_back(rdm);
        		l[i].classement = rdm_classement;
        	}
        }
    }
    return l;
}

void affiche_lEtud (myListEtud &l)
{
	for(auto it=l.begin() ; it != l.end() ; ++it)
	{
		cout << "num étudiant : " << it->first << "\n";
		lEtud tmp = it->second;
		cout << "Liste d'établissement : ";
    	for(auto pair_it = tmp.listEtab.begin() ; pair_it != tmp.listEtab.end() ; ++pair_it)
    	{
    		cout << " " << *pair_it << " ";
    	}
		cout << "\nPoids par établissement : ";
    	for(auto pair_it = tmp.poids.begin() ; pair_it != tmp.poids.end() ; ++pair_it)
    	{
    		cout << " " << *pair_it << " ";
    	}
    	cout << "\n" << "Classement : " << tmp.classement << "\n";
	}
}

void affiche_lEtab (myListEtab &l, myListEtud &lE)
{
	int i;
	for(auto it=l.begin() ; it != l.end() ; ++it)
	{
        cout << "\n";

		cout << "num établissement : " << it->first << "\n";
		
		lEtab tmp = it->second;
		cout << "Liste d'étudiant : ";
		for (i = 0; (unsigned int) i < tmp.etudiant.size(); ++i)
		{
			cout << " " << tmp.etudiant[i].num_etudiant << " ";
		}
		cout << "\n" << "Liste voeux : ";
		for (i = 0; (unsigned int) i < tmp.etudiant.size(); ++i)
		{
			cout << " " << tmp.etudiant[i].num_voeux << " ";
		}
		cout << "\n" << "Liste de note : ";
		for (i = 0; (unsigned int) i < tmp.etudiant.size(); ++i)
		{
			cout << " " << lE[tmp.etudiant[i].num_etudiant].classement << " ";
		}
        cout << "\n" << "Les poids d'obèse : ";
        for (i = 0; (unsigned int) i < tmp.etudiant.size(); ++i)
        {
            cout << " " << lE[tmp.etudiant[i].num_etudiant].poids[tmp.etudiant[i].num_voeux] << " ";
        }
        cout << "\n" << "Pris ou pas: ";
        for (i = 0; (unsigned int) i < tmp.etudiant.size(); ++i)
        {
            cout << " " << tmp.etudiant[i].pris_ou_pas << " ";
        }
        
		cout << "\n";
	}
}

myListEtab affectation (myListEtud &listEtud)
{
	myListEtab listEtabli;
	Etudiant etudtmp;

	//Affectation de tous les élèves dans les établissements
	for(int i = 0; i < NB_MAX_CHOIX; i++)
	{
    	for(auto it=listEtud.begin() ; it != listEtud.end() ; ++it)
		{
		  lEtud tmp = it->second;

    		if((unsigned int) i < tmp.listEtab.size())
    		{
    			//listEtabli[tmp.listEtab[i]].etudiant.reserve(NB_ETUDIANT); //malloc // A vérifier sometimes segf sans cette ligne...
    			etudtmp.num_etudiant = it->first;
    			etudtmp.num_voeux = i;
    			//etudtmp.classement = tmp.classement;
          		
                // etudtmp.poids = (NB_MAX_CHOIX - i) + tmp.classement;
    			listEtabli[tmp.listEtab[i]].etudiant.push_back(etudtmp);
    		}
    	}	
	}

	return listEtabli;
}

bool mieux_ou_pas (cloporte &c, int i, int iter, myListEtud &listEtud)
{
    double poids_tmp = listEtud[c.mListEtab[i].etudiant[iter].num_etudiant].poids[c.mListEtab[i].etudiant[iter].num_voeux];

    //cout << poids_tmp << " / " << listEtud[c.mListEtab[i].etudiant[iter].num_etudiant].poids[0] << "\n";
    if(listEtud[c.mListEtab[i].etudiant[iter].num_etudiant].poids[0] > poids_tmp)
        return 1;
   	return 0;
}

cloporte PrisOuPasPris (cloporte &c, myListEtud &listEtud)
{
	int compteur = 0;
	int timer = 10;

    for(int i = 0; i < NB_ETABLISSEMENT; i++)
    {
    	//cout << i << "\n";
    	//Affichage pourcentage
    	if(i * 100 / NB_ETABLISSEMENT >= timer)
    	{
    		timer += 10;
	    	cout << i * 100 / NB_ETABLISSEMENT << "% ...\n";
    	}
    	//Fin Affichage


        for (int iter = 0; iter<c.mListEtab[i].etudiant.size(); ++iter)
        {
        	//Si l'étudiant n'est pas déja pris
        	if (c.pris[c.mListEtab[i].etudiant[iter].num_etudiant] == 0)
        	{
        		if(compteur > NB_MAX_CHOIX_ETABLISSEMENT)
        			break;
        		if(!mieux_ou_pas(c, i, iter,listEtud))
        		{
                	c.mListEtab[i].etudiant[iter].pris_ou_pas=1;
                	c.pris[c.mListEtab[i].etudiant[iter].num_etudiant] = 1;
                	compteur++;
                    c.mListEtab[i].etudiant_preselec+=1;
                    //cout << iter << "\n";
                }
        	}
        }
        compteur = 0;
    }
    cout << "100 %\n";
    return c;
}

// int etudiant_pas_perdu_dans_les_limbes (myListEtab listEtab, int i)
// {
//     int nb = 0;

//     for (int iter = 0; iter<listEtab[i].etudiant.size(); ++iter)
//     {    
//         if(listEtab[i].etudiant[iter].pris_ou_pas == 1)
//         {
//             nb++;
//         }
//     }
//     return nb;
// }



cloporte bouche_trou (cloporte &c, myListEtud &listEtud)
{
    for(int i = 0; i < NB_ETABLISSEMENT; i++)
    { 
        for (int iter = 0; iter<c.mListEtab[i].etudiant.size(); ++iter)
        {
        	//cout << c.pris[c.mListEtab[i].etudiant[iter].num_etudiant];
        	if (c.pris[c.mListEtab[i].etudiant[iter].num_etudiant] == 0)
        	{
            	if(c.mListEtab[i].etudiant_preselec < NB_MAX_CHOIX_ETABLISSEMENT)
            	{
            	    c.mListEtab[i].etudiant[iter].pris_ou_pas=1;
            	    c.pris[c.mListEtab[i].etudiant[iter].num_etudiant] = 1;
            	}
            }
        }
    }
    return c;
}


int nb_violation (cloporte &c, myListEtud &listEtud)
{
	double poids_tmp = 0.0;
	int compteur = 0;
	int nb = 0;
	for(int i = 0; i < NB_ETABLISSEMENT; i++)
    { 
        for (int iter = 0; iter<c.mListEtab[i].etudiant.size(); ++iter)
        {
        	if(c.mListEtab[i].etudiant[iter].pris_ou_pas == 1)
        	{
        		poids_tmp = listEtud[c.mListEtab[i].etudiant[iter].num_etudiant].poids[c.mListEtab[i].etudiant[iter].num_voeux];
        		for (auto it = listEtud[c.mListEtab[i].etudiant[iter].num_etudiant].poids.begin(); it != listEtud[c.mListEtab[i].etudiant[iter].num_etudiant].poids.end(); ++it)
        		{
        			if(*it > poids_tmp)
        			{
        				nb++;
        				break;
        			}
        		}
        	}
        }
    }
    return nb;
}

int etudiant_perdu_dans_les_limbes (cloporte &mListEtab)
{
	int nb = 0;
	for(int i = 0; i < NB_ETUDIANT; i++)
    {
        if(mListEtab.pris[i] == 1)
            nb++;
    }
	return NB_ETUDIANT - nb;
}

void check (cloporte &c, myListEtud &listEtud)
{
	cout << "Nb de violation : " << nb_violation(c, listEtud) << "\n";
	cout << "Nb d'étudiant non placé : " << etudiant_perdu_dans_les_limbes(c) << "\n";
}

int main ()
{
	cout << "Jeu de données : \n";
	cout << "NB_ETUDIANT = " << NB_ETUDIANT << "\n";
	cout << "NB_ETABLISSEMENT = " << NB_ETABLISSEMENT << "\n";
	cout << "NB_CHOIX_PAR_ETUDIANT = " << NB_MAX_CHOIX << "\n\n";

	myListEtud listEtud = init(NB_ETUDIANT);
	cout << "Init étudiant, fait.\n";
  	myListEtab listEtab = affectation(listEtud);
	cout << "Init établissement, fait.\n\n";
	//affiche_lEtud(listEtud);
	
	cloporte c;
	c.mListEtab = listEtab;
  	
  	cout << "1er traitement, en cours ...\n";
    c = PrisOuPasPris(c, listEtud);
    cout << "1er traitement, fait.\n\n";
    check(c, listEtud);

    cout << "\n\n";

   	cout << "2eme traitement, en cours ...\n";
	c = bouche_trou (c, listEtud);
    cout << "2ème traitement, fait.\n\n";
    check(c, listEtud);

	//affiche_lEtab(listEtab, listEtud);


	return 0;
}