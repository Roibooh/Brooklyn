Nous sommes 3 étudiants en S4 à EPITA Lyon. Dans le cadre de notre S4 nous travaillons sur un projet qui intègre de l’algorithmie vu en cours durant les semestres précédant. 
Nous intégrons donc les graphes et les parcours de graphes pour notre projet décrit ci-dessous.
Ce projet consiste à reconduire un Google maps très simplifié. Nous ne travaillons pas sur une carte réelle mais sur une grille (d’où le nom Brooklyn avec la distance Brooklyn facile à calculer), et nous ne sommes pas aussi précis sur les horaires des transports en communs et d’autre détails que nous faisons moins bien que Google… 
Le résultat final serait de sélectionner plusieurs points sur la grille où l’on aimerait se rendre, et que notre programme renvoi l’itinéraire le plus rapide pour se rendre à tout ce point et puis l’afficher a l’utilisateur.
Vous pouvez retrouver le projet en téléchargeable sur notre site, et les ressources utilisés pour le réaliser.

site web : https://roibooh.github.io/Brooklyn/index.html

Dans graphs il y a des graphs de test pour ne pas avoir à en générer, graph1M ne contient qu'un le graph piétond de 1000x1000 et graph300 contient un graph complet de 15x20

Pour installer le projet lancer simplement l'instalateur brooklyn.exe et suivez les étapes.
Il fois installer vous pouvez soit utiliser l'interface grahpique, soit le fichier main
Le fichier main s'utilise de la facon suivante:
../main path/to/graph/folder/ a b c d e f g ect..
path/to/graph/folder/ is the path to a graph stored with the 5 files, one for each transport, files can be empty
a b c d e f g ect.. are the node to be seen in the graph, minimum 2 and no maximum
The project will show you the most efficient path is there are less than 10 nodes
If there is more the project will show a path close to the best.
