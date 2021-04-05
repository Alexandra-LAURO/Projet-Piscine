#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <queue>

// Nouveau commentaire//
class Sommet
{
private :
    /// Numéro du sommet

    int m_Id;
    std::vector < Sommet* > m_adjacent;


public :

    /// Constructeur
    Sommet(int Id):m_Id{Id} {};

    std::vector < Sommet* > get_adjacent()
    {
        return m_adjacent;
    }


    /// Accesseurs
    int getId()
    {
        return m_Id;
    }

    void nouvArrete( Sommet * x )
    {

        m_adjacent.push_back(x);

        for(int i =0 ; i < x->get_adjacent().size() ; i++)
        {
            m_adjacent.push_back(x->get_adjacent()[i]);
        }

        x->m_adjacent.push_back(this);

        for(int i =0 ; i < get_adjacent().size() ; i++)
        {
            x->m_adjacent.push_back(get_adjacent()[i]);
        }
    }

    int verif_adjacent(Sommet * s)
    {

        for(int i=0 ; i < m_adjacent.size() ; i++ )
        {
            for(int j=0 ; j < s->m_adjacent.size(); j++)
            {
                if(m_adjacent[i] == s->m_adjacent[j] )
                    return 0 ;
            }
        }
        return 1;

    }


    void afficher()
    {
        std::cout<<" sommet "<<m_Id << std::endl;
    }

};

class Arrete
{
private:

    std::vector <Sommet*> m_sommets;
    int m_poids;
    int m_couleur = 0;  /// 1=SOMMET SELECTIONNE   2=SOMMET PAS SELECTIONE


public:

    Arrete(int poids ,Sommet* S1, Sommet * S2)
        :m_poids(poids)
    {
        m_sommets.push_back(S1);
        m_sommets.push_back(S2);
    }

    Sommet* getSommet(int x)
    {
        return m_sommets[x];
    }

    int get_poids()
    {
        return m_poids;
    }

     int get_couleur()
    {
        return m_couleur;
    }

    void set_couleur(int x)
    {
        m_couleur = x;
    }

    void affiche()
    {

        std::cout << m_sommets[0]->getId()<<"<------>"<< m_sommets[1]->getId() <<"    ====POIDS====>   "  <<m_poids<< std::endl;

    }


} ;




class Graphe
{

private :
    ///liste des sommets (vecteur de pointeurs sur Sommet)
    std::vector<Sommet*> m_sommets;
    std::vector <Arrete*> m_arrete;
    int m_ordre;
    int m_taille;


public :

    Graphe(std::string nomFichier)
    {
        std::ifstream ifs{nomFichier};
        if (!ifs)
            throw std::runtime_error("Impossible d'ouvrir en lecture " + nomFichier );


        ifs >> m_ordre;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture ordre du graphe");



        for (int i=0; i<m_ordre; ++i)
        {
            int id;
            ifs >> id;
            m_sommets.push_back( new Sommet{i} );
        }

        ifs >> m_taille;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture taille du graphe");



        for (int i=0 ; i<m_taille ; ++i)
        {
             int n1 ;
             int n2 ;
             int poids ;

            ifs>>n1>>n2>>poids;

            if ( ifs.fail() )
                throw std::runtime_error("Probleme lecture arc");
            m_arrete.push_back(new Arrete{poids,m_sommets[n1],m_sommets[n2]});

        }


        std::cout<<std::endl<<"graphe ";
        std::cout<<"ordre = "<<m_ordre<<std::endl<<"  ";
        std::cout<<"nombre d'arrete = "<< m_taille <<std::endl<<"  ";
        std::cout<<"listes des sommets :"<<std::endl;

        for (int i = 0 ; i < m_sommets.size() ; i++)
        {
            m_sommets[i]->afficher();
            std::cout<<std::endl;
        }

        std::cout<<std::endl << std::endl << "Liste des arretes:\n";


        for(int i =0 ; i < m_taille ; i++)
        {
            m_arrete[i]->affiche();
        }
    }





//    destructeur
    ~Graphe()
    {
        for (auto s : m_sommets)
            delete s;
    }

    //méthode d'affichage



    void kruskal()
    {

        for(int t=0 ; t < (m_ordre-1); t++)
        {

            int MIN = 999999;
            int num;

            for(int i = 0 ; i < m_taille; i++)
            {

                    if( m_arrete[i]->get_poids()  <  MIN   &&   m_arrete[i]->get_couleur()  == 0)
                    {
                        num=i;
                        MIN = m_arrete[i]->get_poids();
                    }

            }


            if(m_arrete[num]->getSommet(0)->verif_adjacent(m_arrete[num]->getSommet(1)) == 1)
            {
                m_arrete[num]->set_couleur(1);
                m_arrete[num]->getSommet(0)->nouvArrete(m_arrete[num]->getSommet(1));

            }

            else
            {
                m_arrete[num]->set_couleur(2);
                t=t-1;
            }

        }




        std::cout  << "\nArrete conservee\n";
        int poids = 0;

        for(int i = 0 ; i < m_taille ; i++)
        {


            if( m_arrete[i]->get_couleur() == 1)
            {
                m_arrete[i]->affiche();

                poids = poids + m_arrete[i]->get_poids();
            }

        }

        std::cout <<std::endl << std::endl<<"poids de l'arbre: "<< poids <<std::endl<< std::endl;

    }




};



int main()
{


    Graphe g{"graphe.txt"};
    g.kruskal();




    return 0;
}
