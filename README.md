# Shogi
## with basic rules
* Shogi
* Notation
* Linked-List 

## Instruction
### Compile

### Run
    cd bin
#### Open a new game
    ./shogi -n -s {new_game_file_name}
    ex ./shogi -n -s newgame.txt
#### Reload an old game
    ./shogi -l {ole_game_file_name}
    ex ./shogi -l oldgame.txt
### How to play
1. The koma board will display on the terminal.
   Please enter the coordinates to move the koma.
   (Please follow the koma's rule)
#### Example
player1's turn:
Player1請輸入你要移動的棋子:(若Player2想悔棋請輸入0，存檔則輸入s或S)
5 7
Player1請輸入你要放的位置
5 6

player2's turn:
Player2請輸入你要移動的棋子:(若Player1想悔棋請輸入0，存檔則輸入s或S)
5 3
Player2請輸入你要放的位置
5 4


2. The koma board will display on the terminal again 
   after moving the koma.
3. If you want to regret your last movement, enter 0.
4. When any players' Osho has been eaten, then lose.
5. You can enter 's' or 'S' if you want to save the 
   record as a notation.
6. Playing data will be record in a txt file named as 
   {new_game_file_name}, which user entered while 
    runnig the program

