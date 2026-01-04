/**
 * \file main.cpp
 * \brief Projet Candy Crush - SAE 1.01
 * \author BULUT Mélissa & DAMEZ Jean
 * \date Janvier 2026
 *
 * Ce programme implemente une version console de Candy Crush avec plusieurs modes de jeu.
 */

#include <iostream>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

/** * @brief Constantes globales du jeu
 */
const unsigned KNbCandies = 6; // Nombre de types de chiffres différents (1 à 6)
const unsigned KImpossible = 0; // Représente une case vide après suppression
const unsigned KMaxTimes = 10; // Nombre de coups maximum

/** * @brief Gestion des couleurs et de l'écran
 */
const unsigned KReset   (0); // Réinitialiser la couleur par défaut
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);

// Pour le fond
const unsigned KBGNoir    (40);
const unsigned KBGRouge   (41);
const unsigned KBGVert    (42);
const unsigned KBGCyan    (46);

/**
 * @brief Efface le contenu du terminal.
 *
 * Utilise une séquence d'échappement ANSI pour nettoyer l'écran.
 */
// Effacer l’écran
void clearScreen () {
    cout << "\033[H\033[2J";
}

/**
 * @brief Change la couleur du texte ou du fond dans le terminal.
 *
 * @param coul Le code couleur (ex: 31 pour rouge, 40 pour fond noir).
 */
// Changer la couleur
void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

/** * @brief Types de données pour la grille
 */
typedef vector <unsigned> line; // un type représentant une ligne de la grille
typedef vector <line> mat; // un type représentant la grille

/**
 * @struct maPosition
 * @brief Représente une coordonnée (ligne, colonne) dans la grille.
 */
struct maPosition {
    unsigned abs;
    unsigned ord;
}; // une position dans la grille

/**
 * @brief Initialise la grille de jeu avec des nombres aléatoires.
 *
 * @param grid La matrice à initialiser.
 * @param matSize La taille de la grille (carrée).
 */
// Initialisation de la grille
void initGrid(mat & grid, const size_t & matSize) { // Initialise toutes les cases de la grille des nombres tirés au hasard entre 1 et une constante
    grid.assign(matSize, line(matSize)); // Redimensionne la matrice
    for (size_t i = 0; i < matSize; ++i) { // Boucle pour parcourir chaque ligne i
        for (size_t j = 0; j < matSize; ++j) { // Boucle pour parcourir chaque colonne j dans la ligne i
            grid[i][j] = (rand() % KNbCandies) + 1; // Assigne un nombre aléatoire entre 1 et KNbCandies à la case
        }
    }
}

/**
 * @brief Affiche la grille de jeu dans la console.
 *
 * Gère l'affichage des couleurs pour chaque chiffre et les bordures du tableau.
 *
 * @param grid La matrice représentant la grille de jeu.
 */
// Affichage de la grille
void displayGrid(const mat & grid) {
    couleur(KBGNoir);
    clearScreen();
    couleur(KReset);
    // Affiche les numéros de colonnes en haut
    cout << "   "; // Espacement pour aligner avec les lignes
    for(size_t j=0; j<grid.size(); ++j) cout << j << " "; // Affiche l'index 0, 1, 2...
    cout << endl; // Retour à la ligne après les index
    // Boucle d'affichage du contenu
    for (size_t i = 0; i < grid.size(); ++i) { // Pour chaque ligne i
        cout << i << " |"; // Affiche le numéro de la ligne à gauche + une barre de séparation
        for (size_t j = 0; j < grid[i].size(); ++j) { // Pour chaque colonne j
            if (grid[i][j] >= 1 && grid[i][j] <= KNbCandies) { // Si la case contient un chiffre valide
                switch(grid[i][j]) {
                case 1: couleur(KRouge); break;
                case 2: couleur(KVert); break;
                case 3: couleur(KJaune); break;
                case 4: couleur(KBleu); break;
                case 5: couleur(KMAgenta); break;
                case 6: couleur(KCyan); break;
                default: couleur(KReset); break;
                }
                cout << grid[i][j]; // Affiche le chiffre en couleur
                couleur(KReset); // Remet la couleur normale tout de suite après
            } else {
                cout << " "; // Affiche un espace vide si c'est KImpossible (0)
            }
            cout << "|"; // Affiche la séparation verticale entre les cases
        }
        cout << endl; // Saut de ligne à la fin de la rangée
    }
}

/**
 * @brief Fonctions de mouvements et détections
 */

/**
 * @brief Effectue le déplacement (échange) de deux cases dans la grille.
 *
 * Vérifie si le mouvement reste dans les limites de la grille avant d'échanger.
 *
 * @param grid La matrice du jeu.
 * @param pos La position de départ (ligne, colonne).
 * @param direction La direction du mouvement ('Z', 'Q', 'S', 'D').
 */
// Placement d’un jeton au bon endroit
void makeAMove (mat & grid, const maPosition & pos, const char & direction) {
    unsigned r = pos.ord; // Récupère la ligne de départ
    unsigned c = pos.abs; // Récupère la colonne de départ
    unsigned newR = r; // Variable pour la nouvelle ligne
    unsigned newC = c; // Variable pour la nouvelle colonne
    size_t taille = grid.size(); // Récupère la taille de la grille pour vérifier les bords
    char dir = toupper(direction); // Convertit la direction en majuscule
    // Vérification des mouvements
    if (dir == 'Z' && r > 0) newR--;
    else if (dir == 'S' && r < taille - 1) newR++;
    else if (dir == 'A' && c > 0) newC--;
    else if (dir == 'E' && c < taille - 1) newC++;
    else return;
    // Echange des valeurs
    unsigned temp = grid[r][c]; // Sauvegarde la valeur de la case de départ
    grid[r][c] = grid[newR][newC]; // La case de départ prend la valeur de la case d'arrivée
    grid[newR][newC] = temp; // La case d'arrivée prend l'ancienne valeur de départ
}

/**
 * @brief Détecte un alignement d'au moins 3 chiffres identiques dans une colonne.
 *
 * @param grid La grille de jeu.
 * @param pos [Sortie] La position du début de la suite trouvée.
 * @param howMany [Sortie] Le nombre d'éléments identiques alignés.
 * @return true Si un alignement a été trouvé, false sinon.
 */
// Detection d’une suite de nombres identiques sur la même colonne
bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned & howMany) {
    size_t taille = grid.size(); // Taille de la grille
    for (size_t j = 0; j < taille; ++j) { // Parcourt toutes les colonnes j
        unsigned count = 1; // Initialise le compteur de suite à 1
        for (size_t i = 0; i < taille - 1; ++i) { // Parcourt les lignes i
            if (grid[i][j] == KImpossible) { count = 1; continue; } // Ignore les cases vides
            if (grid[i][j] == grid[i+1][j]) { // Si la case actuelle est égale à celle du dessous
                count++; // Incrémente le compteur
            } else {
                if (count >= 3) { // Si suite terminée
                    howMany = count; // Enregistre la taille de la suite
                    pos.abs = j; // Enregistre la colonne
                    pos.ord = i - count + 1; // Calcule la ligne de début
                    return true;
                }
                count = 1; // Réinitialise le compteur
            }
        }
        // Vérification fin de colonne
        if (count >= 3) {
            howMany = count;
            pos.abs = j;
            pos.ord = taille - count;
            return true;
        }
    }
    return false; // Aucun match trouvé
}

/**
 * @brief Détecte un alignement d'au moins 3 chiffres identiques dans une ligne.
 *
 * @param grid La grille de jeu.
 * @param pos [Sortie] La position du début de la suite trouvée.
 * @param howMany [Sortie] Le nombre d'éléments identiques alignés.
 * @return true Si un alignement a été trouvé, false sinon.
 */
// Detection d’une suite de nombres identiques sur la même ligne
bool atLeastThreeInARow (const mat & grid, maPosition & pos, unsigned & howMany) {
    size_t taille = grid.size(); // Taille de la grille
    for (size_t i = 0; i < taille; ++i) { // Parcourt toutes les lignes i
        unsigned count = 1; // Initalise le ompteur de suite à 1
        for (size_t j = 0; j < taille - 1; ++j) { // Parcourt les colonnes j
            if (grid[i][j] == KImpossible) { count = 1; continue; } // Ignore les cases vides
            if (grid[i][j] == grid[i][j+1]) { // Si la case actuelle est égale à celle de droite
                count++; // Incrémente le compteur
            } else {
                if (count >= 3) { // Si suite terminée
                    howMany = count; // Enregistre la taille
                    pos.ord = i; // Enregistre la ligne
                    pos.abs = j - count + 1; // Calcule la colonne de début
                    return true;
                }
                count = 1; // Réinitialise compteur
            }
        }
        // Vérification fin de ligne
        if (count >= 3) {
            howMany = count;
            pos.ord = i;
            pos.abs = taille - count;
            return true;
        }
    }
    return false; // Aucun match trouvé
}

/**
 * @brief Supprime des éléments dans une colonne et fait tomber les éléments supérieurs.
 *
 * @param grid La grille de jeu.
 * @param pos La position à partir de laquelle supprimer.
 * @param howMany Le nombre d'éléments à supprimer.
 */
// Suppression dans une colonne
void removalInColumn (mat & grid, const maPosition & pos, unsigned howMany) {
    size_t taille = grid.size(); // Taille grille
    for (size_t i = pos.ord; i < taille; ++i) { // On part de l'endroit supprimé vers le bas
        if (i + howMany < taille) { // Tant qu'on ne dépasse pas la grille
            grid[i][pos.abs] = grid[i + howMany][pos.abs]; // Remonte les chiffres du bas
        } else {
            grid[i][pos.abs] = KImpossible; // Les cases restantes sont marquées vides (0)
        }
    }
}

/**
 * @brief Supprime des éléments dans une ligne.
 *
 * Appelle removalInColumn pour chaque élément de la ligne à supprimer.
 *
 * @param grid La grille de jeu.
 * @param pos La position de départ de la suppression.
 * @param howMany Le nombre d'éléments à supprimer.
 */
// Suppression dans une ligne
void removalInRow (mat & grid, const maPosition & pos, unsigned howMany) {
    for (unsigned k = 0; k < howMany; ++k) { // Pour chaque élément de la suite horizontale
        maPosition pTemp; // Position temporaire
        pTemp.abs = pos.abs + k; // Avance horizontalement
        pTemp.ord = pos.ord; // Reste sur la ligne
        removalInColumn(grid, pTemp, 1); // Supprime cet élément comme une colonne de 1
    }
}

/**
 * @brief Gestion des modes de jeux
 */

/**
 * @brief Affiche les règles du jeu à l'utilisateur.
 */
// Affiche les règles du jeu
void reglesDuJeu() {
    couleur(KBGVert);
    clearScreen();
    couleur(KNoir);

    cout << "--- REGLES DU JEU ---" << endl;
    cout << "1. Entrez d'abord le numero de la LIGNE (0 a 7)." << endl;
    cout << "2. Entrez ensuite le numero de la COLONNE (0 a 7)." << endl;
    cout << "3. Entrez enfin la DIRECTION du deplacement :" << endl;
    cout << "   - Z : Haut" << endl;
    cout << "   - S : Bas" << endl;
    cout << "   - A : Gauche" << endl;
    cout << "   - E : Droite" << endl;
    cout << endl;
    cout << "Exemple de saisie : 0 1 Z (Ligne 0, Colonne 1, vers le Haut)" << endl;
    cout << endl;
    cout << "Pour quitter une partie en cours et revenir au menu, tapez 'M'." << endl;
    cout << endl;
    cout << "Bonne chance !" << endl;
    cout << "\nAppuyez sur Entree pour revenir au menu...";
    cin.ignore(100, '\n'); cin.get();
}

/**
 * @brief Fonction principale du gameplay (Classique, PvP, Histoire).
 *
 * Gère l'initialisation, la boucle de jeu, les tours par tour, le score et les conditions de victoire/défaite.
 *
 * @param mode Le mode de jeu choisi (1: Classique, 2: PvP, 3: Histoire).
 */
// Fonction de jeu (pour tous les modes)
void boucleDeJeu(int mode) {
    mat grid; // Déclaration de la matrice de jeu
    const size_t taille = 8; // Taille de la grille fixée à 8x8

    // Variables de jeu remises à zéro
    unsigned score = 0;
    unsigned scoreJ2 = 0;
    unsigned coups = 0;
    bool quitter = false;
    couleur(KBGNoir);
    clearScreen();

    // Mode histoire
    if (mode == 3) {
        clearScreen();
        couleur(KRouge);
        cout << "L'apocalypse a tout detruit. Les rues sont vides, les batiments en ruine..." << endl;
        cout << "et les zombies rodent partout." << endl;
        couleur(KReset);
        cout << endl;
        cout << "Mais partout dans la ville, il reste ces vieux tableaux de controle remplis de chiffres." << endl;
        cout << "En les remettant dans le bon ordre, tu peux rallumer la lumiere, verrouiller des portes," << endl;
        cout << "activer des pieges... et tenir les zombies a distance." << endl;
        cout << "Tu avances, tu combines les chiffres, tu securises les zones." << endl;
        cout << "Plus tu reussis, plus tu survis." << endl;
        couleur(KRouge);
        cout << endl;
        cout << "Et dehors, la horde attend." << endl;
        couleur(KReset);
        cout << "\nAppuyez sur Entree pour commencer l'operation de survie...";
        cin.ignore(100, '\n'); cin.get();
    }

    // Les differents niveaux
    // Mode histoire : 5 niveaux
    int nbNiveaux = (mode == 3) ? 5 : 1;

    for (int niv = 1; niv <= nbNiveaux; ++niv) {
        if (quitter) break; // Si on a demandé à quitter, on sort de la boucle des niveaux

        // Initialisation à chaque niveau
        initGrid(grid, taille);
        score = 0;
        scoreJ2 = 0;
        coups = 0;

        // Pour mode histoire
        unsigned objectifScore = 10000; // Infini par défaut pour classique
        string titreNiveau = "";
        if (mode == 3) {
            clearScreen();
            couleur(KReset);
            if (niv == 1) {
                objectifScore = 20;
                titreNiveau = "Niveau 1 : Le quartier";
                cout << "Tu arrives dans un vieux quartier coupe d'electricite." << endl;
                cout << "Tout est sombre, et on entend parfois des pas au loin..." << endl;
            } else if (niv == 2) {
                objectifScore = 40;
                titreNiveau = "Niveau 2 : Le supermarche";
                cout << "Tu trouves un supermarche ferme." << endl;
                cout << "Il y a surement encore de la nourriture dedans..." << endl;
            } else if (niv == 3) {
                objectifScore = 60;
                titreNiveau = "Niveau 3 : Le metro";
                cout << "Tu descends dans le metro pour traverser la ville." << endl;
                cout << "Sans lumiere, impossible de voir..." << endl;
            } else if (niv == 4) {
                objectifScore = 80;
                titreNiveau = "Niveau 4 : Le centre de secours";
                cout << "Tu trouves un ancien centre de secours abandonne." << endl;
                cout << "Tout est eteint..." << endl;
            } else if (niv == 5) {
                objectifScore = 100;
                titreNiveau = "Niveau 5 : Les remparts";
                cout << "Tu arrives devant une grande zone protegee." << endl;
                cout << "La porte est fermee et tout est hors service." << endl;
            }
            cout << "\nObjectif : Atteindre " << objectifScore << " points de securite." << endl;
            cout << "Appuyez sur Entree pour lancer le niveau...";
            cin.get();
        }

        // Gameplay
        while (coups < KMaxTimes && !quitter) {
            // Mode histoire : victoire si score atteint
            if (mode == 3 && score >= objectifScore) {
                break; // On sort pour valider le niveau
            }
            displayGrid(grid); // Affichage de la grille
            cout << "-----------------------------------" << endl;
            // Affichage des infos
            if (mode == 2) { // Mode PvP
                cout << "Score J1 : " << score << " | Score J2 : " << scoreJ2 << endl;
                cout << "C'est au tour du joueur " << (coups % 2 == 0 ? "1" : "2") << endl;
            } else if (mode == 3) { // Mode histoire
                couleur(KCyan);
                cout << titreNiveau << endl;
                cout << "Systeme de securite : " << score << " / " << objectifScore << endl;
                couleur(KReset);
                if (KMaxTimes - coups <= 3) couleur(KRouge); else couleur(KVert);
                cout << "Energie restante : " << (KMaxTimes - coups) << endl;
                couleur(KReset);
            } else { // Mode classique
                cout << "Score : " << score << " | Coups : " << coups << " / " << KMaxTimes << endl;
            }

            // Commandes
            cout << "Commandes : Ligne Colonne Direction (ex : 0 1 Z)" << endl;
            cout << "Directions: Z (Haut), S (Bas), A (Gauche), E (Droite)" << endl;
            cout << "Ou tapez 'M' pour revenir au Menu." << endl;
            cout << "Votre choix : ";
            char premiereEntree;
            maPosition pChoix;
            char direction;
            if (cin >> premiereEntree) {
                if (toupper(premiereEntree) == 'M') {
                    quitter = true;
                    continue;
                }
                if (isdigit(premiereEntree)) {
                    pChoix.ord = premiereEntree - '0';
                    if (cin >> pChoix.abs >> direction) {
                        makeAMove(grid, pChoix, direction);
                        bool matchFound;
                        do {
                            matchFound = false;
                            maPosition pMatch;
                            unsigned howMany = 0;
                            // Pour le score
                            if (atLeastThreeInARow(grid, pMatch, howMany)) {
                                unsigned points = howMany * grid[pMatch.ord][pMatch.abs]; // Nombre * Valeur
                                if (mode == 2 && coups % 2 != 0) scoreJ2 += points; else score += points;
                                removalInRow(grid, pMatch, howMany);
                                matchFound = true;
                            } else if (atLeastThreeInAColumn(grid, pMatch, howMany)) {
                                unsigned points = howMany * grid[pMatch.ord][pMatch.abs]; // Nombre * Valeur
                                if (mode == 2 && coups % 2 != 0) scoreJ2 += points; else score += points;
                                removalInColumn(grid, pMatch, howMany);
                                matchFound = true;
                            }
                        } while (matchFound);
                        coups++;
                    }
                } else {
                    cin.clear(); cin.ignore(100, '\n');
                }
            }
        } // Fin du gameplay

        // Fin du niveau
        if (!quitter) {
            if (mode == 3) {
                // Pour mode histoire
                if (score >= objectifScore) {
                    // Affiche texte de fin de niveau si victoire
                    couleur(KBGNoir); clearScreen();
                    couleur(KVert);
                    if (niv == 1) {
                        cout << "Tu remets le courant." << endl;
                        cout << "Les lampadaires s'allument, ca eclaire un peu les rues et les zombies s'eloignent." << endl;
                        cout << "Tu peux avancer." << endl;
                    } else if (niv == 2) {
                        cout << "Tu retablis le courant." << endl;
                        cout << "Les portes s'ouvrent et les frigos redemarrent." << endl;
                        cout << "Tu prends ce dont tu as besoin et tu repars vite." << endl;
                    } else if (niv == 3) {
                        cout << "Tu rallumes le systeme." << endl;
                        cout << "Les neons s'allument et un vieux train fonctionne encore un peu." << endl;
                        cout << "Tu t'en sers pour avancer." << endl;
                    } else if (niv == 4) {
                        cout << "Tu reactives le courant." << endl;
                        cout << "Une radio se met a marcher." << endl;
                        cout << "Quelqu'un parle d'un refuge plus loin." << endl;
                        cout << "Ca te donne un objectif." << endl;
                    } else if (niv == 5) {
                        cout << "Tu remets le generateur en marche." << endl;
                        cout << "Les projecteurs s'allument, la porte s'ouvre lentement." << endl;
                        cout << "Tu entres." << endl;
                        cout << "Tu es un peu plus en securite..." << endl;
                    }
                    couleur(KReset);

                    if (niv < 5) {
                        cout << "\nAppuyez sur Entree pour le niveau suivant...";
                        cin.ignore(100, '\n'); cin.get();
                    } else {
                        cout << "\nFIN DE L'HISTOIRE. Appuyez sur Entree pour revenir au menu...";
                        cin.ignore(100, '\n'); cin.get();
                    }
                } else {
                    // Defaite
                    couleur(KBGRouge); clearScreen();
                    couleur(KNoir);
                    cout << "=== ECHEC CRITIQUE ===" << endl;
                    cout << "Plus d'energie. Ils arrivent... C'est la fin." << endl;
                    couleur(KReset);
                    couleur(KBGNoir);
                    cout << "\nAppuyez sur Entree pour revenir au menu...";
                    cin.ignore(100, '\n'); cin.get();
                    break; // Arrête l'histoire
                }
            } else {
                // Fin classique / PvP
                cout << "\nFIN DE PARTIE. Score final : " << score << (mode == 2 ? " / J2 : " + to_string(scoreJ2) : "") << endl;
                cout << "Appuyez sur Entree pour revenir au menu...";
                cin.ignore(100, '\n'); cin.get();
            }
        }
    } // Fin boucle pour niveau
}

/**
 * @brief Point d'entrée du programme.
 *
 * Affiche le menu principal et lance le mode de jeu choisi.
 *
 * @return 0 si le programme se termine correctement.
 */
int main() {
    srand(time(NULL)); // Initialise le générateur de nombres aléatoires avec l'heure
    int choix = -1; // Variable pour le choix du menu
    while (choix != 5) {
        couleur(KBGCyan);
        clearScreen();
        couleur(KNoir);
        cout << "======= MENU DE SELECTION =======" << endl;
        cout << "1. Mode Classique" << endl;
        cout << "2. Mode PvP" << endl;
        cout << "3. Mode Histoire" << endl;
        cout << "4. Regles du jeu" << endl;
        cout << "5. Quitter le programme" << endl;
        cout << "Votre choix : ";
        // Lecture
        if (!(cin >> choix)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Entree invalide. Appuyez sur Entree pour reessayer...";
            cin.get();
            continue;
        }
        // Remet la couleur de base avant de lancer un jeu
        couleur(KReset);
        couleur(KBGNoir);
        switch(choix) {
        case 1: boucleDeJeu(1); break;
        case 2: boucleDeJeu(2); break;
        case 3: boucleDeJeu(3); break;
        case 4: reglesDuJeu(); break;
        case 5: cout << "Au revoir !" << endl; break;
        default:
            cout << "Choix inconnu. Appuyez sur Entree...";
            cin.ignore(100, '\n');
            cin.get();
            break;
        }
    }
    return 0; // Fin du programme
}
