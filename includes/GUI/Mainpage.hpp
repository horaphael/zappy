/**
 * @mainpage 🎮 Zappy - Jeu Réseau Multijoueur Inspiré de Zaphod Beeblebrox
 *
 * @section objectif Objectif du projet
 * Créer un jeu multijoueur en réseau où plusieurs équipes s'affrontent sur une carte pour atteindre l’élévation maximale.
 * L’équipe gagnante est celle où au moins 6 joueurs atteignent le niveau 8.
 *
 * @section composants 🔧 Composants à développer
 * - **zappy_server (en C)** : Serveur qui gère le monde et les communications.
 * - **zappy_gui (en C++)** : Interface graphique pour observer le jeu.
 * - **zappy_ai (langage libre)** : Client autonome contrôlant les joueurs.
 *
 * @section monde 🌍 Monde du jeu : Trantor
 * - Carte torique (rebouclée sur les bords).
 * - Ressources : nourriture + 6 types de pierres (linemate, deraumere, sibur, mendiane, phiras, thystame).
 * - Ressources réparties uniformément, avec des densités définies.
 *
 * @section joueurs 🧍‍♂️ Joueurs et gameplay
 * Chaque joueur est un drone contrôlé par un client.
 * Doit se nourrir (1 unité de nourriture = 126 unités de temps).
 * Interagit avec l’environnement (ramasser/déposer des objets, regarder autour, se déplacer).
 * Peut effectuer un rituel d’élévation avec d’autres joueurs du même niveau en réunissant les ressources nécessaires.
 *
 * @section rituel 📈 Rituel d’Élévation
 * Nécessite des ressources spécifiques + un nombre précis de joueurs du même niveau sur une case.
 * Tous les joueurs participants montent de niveau si la condition est remplie au début et à la fin du rituel.
 *
 * @section vision 👁️ Vision et communication
 * Champ de vision augmente avec le niveau.
 * Communication via des messages sonores diffusés (direction du son incluse).
 *
 * @section commandes 💬 Commandes
 * Les joueurs peuvent :
 * Se déplacer, tourner, regarder, ramasser/déposer, incanter, forker (reproduire), éjecter d’autres joueurs, envoyer des messages.
 *
 * @section interface 🖥️ Interface Graphique
 * Doit afficher la carte et les éléments en 2D (SFML recommandée).
 * Peut être étendue en 3D.
 * Doit être réactive aux événements du serveur (ex. changements de tuiles).
 */
