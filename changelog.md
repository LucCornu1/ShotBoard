# Changelog - ShotBoard
All notable changes to this project will be documented in this file.

##[0.1.1] - 15-05-2022
### Added
- Controles :
	- Virage avec le Joystick gauche ou Q/D
	- Déccélération avec la touche B (manette) ou S (clavier)
	- Saut avec la touche A (manette) ou espace (clavier)

### Tests
- Tests effectués sur les tremplins (bug)

##[0.1.0] - 15-05-2022
### Changed
- Tout

##[0.0.18] - 12-05-2022
### Changed
- Le calcul de la vitesse

##[0.0.17] - 11-05-2022
### Added
- La planche se déplace en fonction de la pente

### BugFix
- La planche ne s'arrête plus au moindre dénivelé

##[0.0.16] - 10-05-2022
### Added
- Rotation de la planche sur la pente

##[0.0.15] - 05-05-2022
### Changed
- Rework en cours de la physique du joueur
	- Gravité ajoutée (incomplète)

##[0.0.14] - 03-05-2022
### Changed
- Le level de test est modifié pour ajouter une partie à bosse sur la pente lisse
- Légère modification des mouvements du joueur

##[0.0.13] - 02-05-2022
### Added
- Des assets graphique pour le décor
- Des planches de snow ainsi que des personnages

### Changed
- Ajout des assets sur le personnage et la planche de snow
- Modifications sur le level de test, afin de comparer la méthode de sculptage avec l'utilisation de StaticMesh
	--> Test effectués sur la descente et les rampes de saut
- Ajout d'un effet de "lag" sur la caméra : Celle-ci met un peu plus de temps à suivre le personnage désormais

##[0.0.12] - 17-03-2022
### Added
- Les calculs s'effectuent désormais du côté serveur, puis sont dupliqués côté client, qui ajoute la vitesse
- Duplication de la rotation du joueur

##[0.0.11] - 09-03-2022 - LucC & JoffreyZ
### Added
- Création d'une session fonctionnant en LAN
- D'autres joueurs peuvent rejoindre en LAN la session créée
- Lorsque le joueur arrive au bout de la piste, il est téléporter en haut de la piste

### Changed
- Ajout des commentaires au code

### Removed
- Le mode "Online" avec STEAM OSS, en raison d'un bug connu avec l'AppID 480

##[0.0.10] - 07-03-2022
### Added
- "CreateServer"
- "FindServer"

##[0.0.9] - 03-03-2022
### Added
- API Steam à l'aide du plugin "Online Subsystem Steam"

### BugFix
- Changement du point d'origine du Linetrace pour l'alignement du snowboard

##[0.0.8] - 10-02-2022
### Added
- Ajout de la fonctionnalité d'accroupissement pour gagner de la vitesse

##[0.0.7] - 31-01-2022
### Added
- Ajout de la gravité lors du saut

##[0.0.6] - 31-01-2022
### Added
- Ajout du saut lorsque le joueur prend une rampe

### Changed
- Optimisation du déplacement du joueur

##[0.0.5] - 14-12-2021
### Added
- Ajout des contrôles manettes

### Changed
- Corrections de bugs : Le joueur pouvait remonter une pente en fonction de son orientation dans le monde
- Changement des dates dans ce changelog pour novembre à decembre

## [0.0.4] - 10-12-2021
### Added
- Ajout de l'inclinaison de la planche en fonction du terrain

### Changed
- Changement complet du système de déplacement
- Le joueur peut désormais partir en arrière si la planche se trouve dans le bon sens

## [0.0.3] - 03-12-2021
### Added
- Ajout d'un mouvement de caméra libre autour du joueur
- Ajout d'un ajustement de la vitesse maximale en fonction de la pente du terrain

### Changed
- Le joueur utilise désormais le momentum inhérent à Unreal

## [0.0.2] - 02-12-2021
### Added
- Création d'un 'Character' avec son Snowboard
- Ajout des fonctions de déplacement, avec momentum

## [0.0.1] - 25-11-2021
### Added
- Création et ajout du projet à Github
- Ajout de ce changelog pour garder une trace des changements effectués sur le projet
- Ajout d'un ReadMe qui contiendra un tutoriel écrit pour jouer au jeu (à destination des playtesters)
- Ajout d'un .gitignore type pour les projets sous Unreal Engine 4