@echo off 
cd croswin-src/
echo @JUSTICSOFT - 2026
echo Welcome to the windows version of cr0sfetch!
echo It's a little diffrent due to the limitations of MinTTY (the terminal emulator of choice)
echo The top menubar has been eradicated, in this version everything is controlled via buttons!
pause
cls
echo --- HOW TO READ CPP SOURCE CODE - FOR PROFFESORS ---
echo here are some basic concepts to aid you in ur purusing of my code
echo .dll files - windows library files, are compiled so contain no readable info
echo .h files - files the define the names and arguments of global (codebase-wide) functions
echo .cpp files - the actual logic of the program (handwritten by me)
echo ----------------------------------------------------
echo By the time this project is wrapped up every line of code will be commented, c++ abstractions arent that hard once you notice the patterns
pause
cls
echo --- WINDOWS VERSION CONTROLS ---
echo ` - exit program
echo e - summon file selector menu
echo q - destroy file selector menu
echo w - move up a file
echo s - move down a file
pause
a.exe
echo thank you for testing my program!
pause
