**************
BASIC MapMaker v1.0
Programmed by MDR Falcon
**************


*Visit my website to download free calculator games, including GAMES THAT CAN'T BE FOUND ANYWHERE ELSE:

www.geocities.com/revolution2032/games


-Contents-
1. Using BASIC MapMaker
2. Using a Saved Map in Your Own Programs
3. Bugs
4. Disclaimer and Legal
5. Contact Info
6. Version History

-------
1. Using BASIC MapMaker

The purpose of BASIC MapMaker is to allow the user to create and store "homescreen maps" using strings.  The program contains five major functions, which are outlined below.

a) Create new map

First, you will be prompted to enter the dimensions of your map (rows and columns).  The number of rows must be at least eight and the number of columns must be at least sixteen.  Next, a screen will come up that will allow you to enter the map row by row.  Press the right and left arrow keys to select a character and [2nd] or [ENTER] to insert the currently selected character into the map.  (Notice that the character selection contains many "hidden" characters that cannot be accessed through the OS.)  Upon completing the map, the program will return to the main menu.

b) Load map

This option will allow you to load a map stored in strings 1-3.  If the string variable that you selected does not exist, an error will occur and the program will be forced to terminate.  If the selected string variable does exist, the length of the string will be displayed and you will be prompted to enter the dimensions of the map.  THE NUMBER OF ROWS MULTIPLIED BY THE NUMBER OF COLUMNS MUST EQUAL THE LENGTH OF THE STRING.  If the dimensions of the map are not valid, a message that says "INVALID DIMENSIONS!" will be displayed and no map will be loaded.  The program will then return to the main menu.

c) Edit a row

This feature allows you to edit a row in the currently selected map.  If no map is currently loaded, a message will appear telling you to create or load a map.  Otherwise, you will be asked which row you would like to edit.  Enter a number corresponding to the row you would like to edit, then press [ENTER].  Next, press the right and left arrow keys to select a character and [2nd] or [ENTER] to insert the currently selected character into the row.  When the row is completed, the program will return to the main menu.

d) View map

This feature allows you to view the currently loaded map.  If the map is larger than 8x16, you can scroll it horizontally and vertically by pressing the arrow keys.  Pressing [CLEAR] will return the program to the main menu.  If no map is currently loaded, a message will tell you to create or load a map.

e) Save current map

This feature allows you to store the currently loaded map to strings 1-3.  If no map is currently loaded, an empty string will be stored to the selected string variable.

-------
2. Using a Saved Map in Your Own Programs

If you want to use your map in a program, save the map to one of the three string variables in BASIC MapMaker, then go to the place in your program where you want the map to appear.  Press [2nd], [STO->], then go to the [VARS] menu, select 7:String..., then select the string variable that your map is stored in.  Press [ENTER] and the string will be pasted into your program.

For information on creating a scrolling homescreen map program, download my tutorials at http://www.ticalc.org/archives/files/fileinfo/391/39193.html

-------
3. Bugs

Editing rows doesn't work yet.

If you try to load a map that doesn't exist, an error will occur, but unfortunately there is no fullproof (or foolproof, whichever you prefer) solution for this using BASIC.

-------
4. Disclaimer and Legal

By downloading BASIC MapMaker, you agree that neither me nor anyone else is responsible for anything that happens to you or your calculator.

My source code for BASIC MapMaker is completely original.  If parts of it are the same or similar to the source code of any other program, it is purely coincidental.  My source code may not be used or reproduced in any program intended for distribution without giving credit to MDR Falcon.

copyright 2007 by MDR Falcon

-------
5. Contact Info

Please email me at revolution2032@gmail.com with any questions, comments, or ideas.

Visit my website for more great games:

www.geocities.com/revolution2032/games

-------
6. Version History

v1.0
First complete version released on 3/11/07.  Released on my website and ticalc.org.  "Edit a row" function now complete.  Substituted tilde for duplicate capital accented I.  Program size: 2273 bytes.

v0.8
First version publicly released (3/3/07).  Released on my website.  "Edit a row" function not complete.  Program size: 2018 bytes.

