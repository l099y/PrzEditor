# NAVIGATIONSHEET

## Plans pour la semaine à venir

- implémenter les actions QUndo non présentes
- implémenter la sauvegarde et le chargement de l'état de la ligne du temps
- changer le comportement des mouvements de shots une fois que le shot inséré figure dans la zone appropriée.

## Résumé d'activité par semaine

## semaine 9 (12 au 16 de 05/21)

- finaliser les uc filesystem et insertion
- commencer à implémenter le framework undo de Qt, et mon propre format de sauvegarde d'application.

### semaine 8(5 au 9 de 05/21) 

- Donner à l'explorateur de fichier les comportements précisé pendant la dernière réunion, implémenter le drag and drop des séquences détectées vers la timeline
- mettre en place la dynamique de l'interface parametre et saisie d'information, qui doit se déclancher sur enter, sur esc annulé la valeur modifiée mais pas validée.

### semaine 7(29 au 02 de 04/21)

- les fichiers prz: il me faut détecter des séquences de prz au sein de dossiers
en créer une représentation comme un fichier etendu pointant vers des adresses.
- mettre en place la dynamique de l'interface parametre et saisie d'information, qui doit se déclancher sur enter, sur esc annulé la valeur modifiée mais pas validée.
- commencer à apprendre les mécaniques du registre d'action intégré à Qt (damn je pensais que j'allais écrire ce registre d'action moi même, where is the fun). 

### semaine 6(23 au 27 de 03/21)

- grippé

### semaine 5(15 au 19 de 03/21)

- lors du déplacement d'un bloc: d'abord il y a une résistance et il reste collé à l'autre bloc, puis il passe au dessus et il s'inverse avec une animation V
- Ne pas changer la longueur d'un bloc dans l’interface timeline: c'est dans l'interface de propriété d'un bloc qu'on le fait
- les autres blocs se décalent en fonction du changement de longeur d'un bloc
- mettre deux modes d'édition: avec un bouton : Aimant: qui est barré ou pas: si on est en mode aimant: il met par défaut les blocs bout à bouts.
- commencer la gestion des fichiers
- commencer la gestion du multiboxage

### semaine 4(8 au 12 de 03/21)

- changer certains comportement dans le dynamisme des blocs v
- implémenter d'autres "mods" de déplacement : magnet, displace. v
- implémenter la graduation v
- implémenter le multiboxage
- gérer l'insertion de nouveaux blocs v
- commencer à gérer le filesystem (certainement pas cette semaine)

### semaine 3(1 au 5 de 03/21)
- avoir un dynamisme entre blocs et placements proche du résultat escompté, mise en place d'une analyse structurelle

### semaine 2 (22 au 26 de 02/21) :
- développement de maquettes précisant le contexte, début d'implémentation UIx

### semaine 1 (15 au 19 de 02/21) :
- développement de maquettes précisant le contexte


## Divers