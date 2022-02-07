# PPP_Project-Hospital_Appointments_Manager
- [x] Finished

## Index
- [Description](#description)
- [Technologies used](#technologies-used)
- [To run this project](#to-run-this-project)
- [Notes important to read](#notes-important-to-read)
- [Authors](#authors)

## Description
This project was developed for Introduction to Procedural Programming subject @University of Coimbra, Informatics Engineering <br>
Consists in develop a program that manages hospital appointments
#### Main Languages:
![](https://img.shields.io/badge/-C-333333?style=flat&logo=C%2B%2B&logoColor=5459E2) 

## Technologies used:
1. C ([documentation](https://devdocs.io/c/))
2. [CLion](https://www.jetbrains.com/clion/)(for the first way to run)

## To run this project:
[WARNING] CLion must be installed (for the first way to run)<br>
You have two ways to run this project:
1. Using CLion
    * Download the folder "Project" that is inside "#clion_folder"
    * Open this project with CLion<br>
      ![image](https://i.imgur.com/omTIa1E.png) ![image](https://i.imgur.com/faeZmPV.png) <br>
    * Run the program<br>
      ![image](https://i.imgur.com/AJgteWA.png)

2. Using Terminal:
    * Download the folder "src"
    * Compile the program
      ```shellscript
      [your-disk]:[name-path]> gcc -o program -Wall main.c Funcoes.c StructsListasLigadas.c 
      ```
      OR (to facilitate you can just run the file compile.sh, that is inside the "src" folder, to compile
      ```shellscript 
      [your-disk]:[name-path]> sh compile.sh main
      ```
    * Finally just run it<br>
      [WINDOWS]
      ```shellscript 
      [your-disk]:[name-path]> main.exe
      ```
      [LINUX]
      ```shellscript 
      [your-disk]:[name-path]> ./main
      ```

## Notes important to read
- The files that are on folder "resources" must be on the folder of "Project" (1st way to run) or on the same folder of *.c files (2nd way to run)

## Authors:
- [João Silva](https://github.com/ikikara)
