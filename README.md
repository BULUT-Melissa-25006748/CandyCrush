# 🍬 CandyCrush

![C++](https://img.shields.io/badge/C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Qt Creator](https://img.shields.io/badge/Qt_Creator-41CD52?style=for-the-badge&logo=qt&logoColor=white)

Développement en console d'un moteur de jeu d'alignement (type Match-3) réalisé dans le cadre de la SAE 1.01 (BUT Informatique).

## 🛠️ Fonctionnalités Techniques
* **Moteur logique :** Détection d'alignements par algorithme de fenêtre glissante (Lignes/Colonnes) et résolution des réactions en chaîne.
* **Gravité :** Décalage matriciel dynamique (via `std::vector`) pour simuler la chute des éléments.
* **Robustesse :** Sécurisation stricte des limites d'indices de la matrice et purge des flux `std::cin` pour empêcher les crashs de saisie.

## 🕹️ Modes de Jeu
1. **Classique :** Atteindre le meilleur score possible en 10 coups.
2. **PvP Local :** Affrontement au tour par tour sur la même grille.
3. **Apocalypse Zombie (Histoire) :** Scénario de survie avec objectifs progressifs et contrainte d'énergie pour rétablir les systèmes de sécurité.

## 📄 Documentation & Rapport
Le code source est intégralement documenté au format Javadoc. L'arborescence des appels (graphes avec profondeur de 4) a été générée via **Doxygen & Graphviz**.

🔗 **[Consulter le Rapport de Projet complet (PDF)](Rapport%20SAE%201.01.pdf)**
