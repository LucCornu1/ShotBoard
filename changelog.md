# Changelog - ShotBoard
All notable changes to this project will be documented in this file.

##[0.0.13] - 02-05-2022 - LucC
### Added
- Des assets graphiques

##[0.0.12] - 17-03-2022 - LucC
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

##[0.0.10] - 07-03-2022 - LucC
### Added
- "CreateServer"
- "FindServer"

##[0.0.9] - 03-03-2022 - LucC
### Added
- API Steam à l'aide du plugin "Online Subsystem Steam"

### BugFix
- Changement du point d'origine du Linetrace pour l'alignement du snowboard

##[0.0.8] - 10-02-2022 - LucC
### Added
- Ajout de la fonctionnalité d'accroupissement pour gagner de la vitesse

##[0.0.7] - 31-01-2022 - LucC
### Added
- Ajout de la gravité lors du saut

##[0.0.6] - 31-01-2022 - LucC
### Added
- Ajout du saut lorsque le joueur prend une rampe

### Changed
- Optimisation du déplacement du joueur

##[0.0.5] - 14-12-2021 - LucC
### Added
- Ajout des contrôles manettes

### Changed
- Corrections de bugs : Le joueur pouvait remonter une pente en fonction de son orientation dans le monde
- Changement des dates dans ce changelog pour novembre à decembre

## [0.0.4] - 10-12-2021 - LucC
### Added
- Ajout de l'inclinaison de la planche en fonction du terrain

### Changed
- Changement complet du système de déplacement
- Le joueur peut désormais partir en arrière si la planche se trouve dans le bon sens

## [0.0.3] - 03-12-2021 - LucC
### Added
- Ajout d'un mouvement de caméra libre autour du joueur
- Ajout d'un ajustement de la vitesse maximale en fonction de la pente du terrain

### Changed
- Le joueur utilise désormais le momentum inhérent à Unreal

## [0.0.2] - 02-12-2021 - LucC
### Added
- Création d'un 'Character' avec son Snowboard
- Ajout des fonctions de déplacement, avec momentum

## [0.0.1] - 25-11-2021 - LucC
### Added
- Création et ajout du projet à Github
- Ajout de ce changelog pour garder une trace des changements effectués sur le projet
- Ajout d'un ReadMe qui contiendra un tutoriel écrit pour jouer au jeu (à destination des playtesters)
- Ajout d'un .gitignore type pour les projets sous Unreal Engine 4