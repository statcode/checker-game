#include <iostream.h>
#include "apmatrix.h"
#include "apvector.h"
#include "apstring.h"
#include <fstream.h>

//Human player functions
void GetMove1(apmatrix<char>&);
bool CheckMove1(const apmatrix<char>&, int, int, int);
void MakeMove1(apmatrix<char>&, int, int, int);
//bool CheckMove1K(const apmatrix<char>&, int, int, int, int);
//void MakeMove1K(apmatrix<char>&, int, int, int, int);


//Computer player    //line 522
void GetMoveRed(const apmatrix<char>&, apvector<int>&);  //work on double jump for king
void TakeMove(apmatrix<char>&, const apvector<int>&);

//Extra functions
void Fill(apmatrix<char>&);
void DispBoard(const apmatrix<char>&);
bool Winner(const apmatrix<char>&);


/*
- Work on the moves for the black piece. (computer generated using the
referee program functions.
- be careful of jumping, double jump, and becoming king.

-On double jump. use board[playerMove[3]+1][playerMove[4]-1] == 'b'

for zig-zag double move. not straight.

*/


int main()
{
/////////////////////////////
apmatrix<char> board(8,8,' ');
apvector<int> playerMove(1,999);
bool noWin = true;  //flag for loop to see it someone win or not
Fill(board); //fills in board with colors
ofstream outFile;

        apstring fileName;

        cout << "Out = " << endl;
        cin >> fileName;

        outFile.open(fileName.c_str());

        cout << "Checker beta. Written by Johnny Philavanh (APCS)" << endl;
        cout << "\nReal Intelligence(bottow row) *red color*" << endl;
        cout << "Wanna-be Intelligence(top row) *Black color*" << endl;
        DispBoard(board);
//////////////////////////////


    for(int i = 0; i < 50; i++)
    {
    cout << "Go Black!" << endl;
    GetMove1(board);
    DispBoard(board);

    //Computer player
    GetMoveRed(board,playerMove);

    for(i=0; i<playerMove.length(); i++)            // output move to file
            outFile << playerMove[i] << " " ;
    outFile << endl << endl;    

    TakeMove(board,playerMove);
    DispBoard(board);
    
    noWin = Winner(board); //returns false if someone win. else return true.

       if(noWin == false)
       {
       i = 51;
       }
    }


    int p;
    cin>>p;

return 0;
}



//ask user which what piece (r or R) to move, asking for coordinate
void GetMove1(apmatrix<char>&board)
{
int URow, UCol, pos, posR;
bool unValid = true;
bool vMove = false;


         //loop will force user to enter a valid space with a red piece
         do
         {
            cout << "\n*Choose a piece to move*" << endl;
            cout << "*Coordinate*" << endl;

            do
            {
            cout << "Row (0-7) = " << endl;
            cin >> URow;
            }while(URow < 0 || URow > 7);

            do
            {
            cout << "Col (0-7) = " << endl;
            cin >> UCol;
            }while(UCol < 0 || UCol > 7);


            //If user pick a small black
            if(board[URow][UCol] == 'b')
            {
               cout << "U are moving an r piece" << endl;
               cout << "Move top-left or top-right" << endl;

                 //check for left-side boundary
                 if(UCol == 0)
                 {
                   do
                   {
                   cout << "Press (1). U can only move right." << endl;
                   cin >> pos;
                   }while(pos != 1);
                  }

               //check for right-side boundary
               else if(UCol == 7)
               {
                  do
                  {
                  cout << "Press (-1). U can only move left." << endl;
                  cin >> pos;
                  }while(pos != -1);
               }

               else
               {

                 do
                 {
                 cout << "-1 for left and 1 for right." << endl;
                 cin >> pos;
                 }while(pos != -1 && pos != 1);

               }

             vMove = CheckMove1(board,URow, UCol,pos);

               if(vMove)
               {
               MakeMove1(board,URow, UCol, pos);
               unValid = false;
               }

               else
               cout << "Invalid! Try again." << endl;

            }

            /*
            //If user pick a red king to move
            else if(board[URow][UCol] == 'R')
            {
            cout << "U are moving an R(king) piece" << endl;
            cout << "Move up(1) and Move down(-1)" << endl;
            cin >> posR;
            cout << "(-1) for left and (1) for right." << endl;
            cin >> pos;

             vMove = CheckMove1K(board, URow, UCol,posR,pos);

               if(vMove)
               {
               MakeMove1K(board, URow, UCol, posR,pos);
               unValid = false;
               }
               else
               cout << "Invalid move. Try again." << endl;
            }
            */

            else //If user choose a wrong piece
            cout << "Invalid! Try again." << endl;

         }while(unValid);
}


//Check to see if "r" have a valid move
bool CheckMove1(const apmatrix<char>&board, int URow, int UCol, int pos)
{
      //check to see if it could move on to an empty space
      if(board[URow-1][UCol+pos] == ' ')
      {
      return true;
      }

      //Check to see if it could jump
      else if ((board[URow-1][UCol+pos] == 'r' ||board[URow-1][UCol+pos] == 'R')
      && (board[URow-2][UCol+pos+1] == ' '||board[URow-2][UCol+pos+-1] == ' ') )
      {
      return true;
      }

      else
      {
      return false;
      }
}


//Moves the "r" piece
void MakeMove1(apmatrix<char>&board, int URow, int UCol, int pos)
{
          //so far dis only work for single jump.

         //moves to an empty space
         if(board[URow-1][UCol+pos] == ' ')
         {
         board[URow][UCol] = ' ';   //original occupy space becomes empty

             //if it moves to the end of other side
             if(URow-1 == 0)
             {
             board[URow-1][UCol+pos] = 'B'; //becomes R (King)
             }
             else
             board[URow-1][UCol+pos] = 'b';
         }

       //if it could jump to the right.
      else if(pos == 1)
      {
       if (((board[URow-1][UCol+pos] == 'r') ||
       (board[URow-1][UCol+pos] == 'R')) && (board[URow-2][UCol+pos+1] == ' '))
       {
       board[URow][UCol] = ' '; //old occupy space becomes ' '
       board[URow-1][UCol+pos] = ' '; //it eats a piece, so now it become ' '

           if(URow-2 == 0) //other side
           {
           board[URow-2][UCol+pos+1] = 'B'; //it's now a king
           }

           else
           board[URow-2][UCol+pos+1] = 'b';
       }

      }
       //if it could jump to the left
       else
       {
       board[URow][UCol] = ' '; //old occupy space becomes " "
       board[URow-1][UCol+pos] = ' '; //it eats a piece, so now it become " "

          if(URow-2 == 0)
          {
          board[URow-2][UCol+pos+-1] = 'B'; //it's now a king
          }

          else
          board[URow-2][UCol+pos+-1] = 'b';

       }
}

//////////////////////////////////////////////////////////
//RED COMPUTER-GENERATED MOVE
void GetMoveRed(const apmatrix<char>&board, apvector<int>&playerMove)
{
int r,c;
int countMove = 0;
bool bJump;

        //loop for moving small my player
        //also check to see if other player can jump u and block it
        bJump = false;
        for(r = 0; r < 8; r++)
        {
              for(c = 0; c < 8; c++)
              {
                     //JUMPING A PLAYER
                     if(r <= 5 && c <= 5) //boundary checking
                     {
                       //jump a 'r' player to the down-right
                       if(board[r][c] == 'r' && (board[r+1][c+1] == 'b' ||
                       board[r+1][c+1] == 'B'))
                       {
                          if(board[r+2][c+2] == ' ')
                          {
                          playerMove.resize(1*2+3);

                          playerMove[1] = r;
                          playerMove[2] = c;
                          playerMove[3] = r+2;
                          playerMove[4] = c+2;

                          //num of moves
                          playerMove[0] = 1;


                          if(r <= 3 && c <= 3) //boundary checking
                          {
                               //check for double jump to down-right
                               if((board[r+3][c+3] == 'b' || board[r+3][c+3] == 'B')
                               && (board[r+4][c+4] == ' '))
                               {
                               playerMove.resize(2*2+3);
                               playerMove[5] = r+4;
                               playerMove[6] = c+4;

                               //num of moves
                               playerMove[0] = 2;
                               }

                           }

                           if(playerMove[4] >= 2)
                           {
                                //check for double jump to down-left
                               if((board[playerMove[3]+1][playerMove[4]-1] == 'b'|| board[playerMove[3]+1][playerMove[4]-1] == 'B')
                               &&(board[playerMove[3]+2][playerMove[4]-2] == ' '))
                               {
                               playerMove.resize(2*2+3);
                               playerMove[5] = playerMove[3]+2;
                               playerMove[6] = playerMove[4]-2;

                               //num of moves
                               playerMove[0] = 2;
                               }
                            }
                          bJump = false;
                          return;//break out
                          }
                       }
                     }


                     //jump a 'r' player to the down-left
                      if(r <= 5 && c >= 2) //boundary
                      {
                         if(board[r][c] == 'r' &&  (board[r+1][c-1] == 'b' ||
                         board[r+1][c-1] == 'B'))
                         {
                            if(board[r+2][c-2] == ' ')
                            {
                            playerMove.resize(1*2+3);

                            playerMove[1] = r;
                            playerMove[2] = c;
                            playerMove[3] = r+2;
                            playerMove[4] = c-2;

                            //num of moves
                            playerMove[0] = 1;


                             if(r <= 3 && c >= 4)
                             {
                               //check for double jump to down-left
                               if((board[r+3][c-3] == 'b' || board[r+3][c-3] == 'B')
                               && (board[r+4][c-4] == ' '))
                               {
                               playerMove.resize(2*2+3);

                               playerMove[5] = r+4;
                               playerMove[6] = c-4;

                               playerMove[0] = 2;
                               }
                             }


                              if(playerMove[4] <= 5)  //boundary for col <= 5
                              {
                                //check for double jump to down-right
                               if((board[playerMove[3]+1][playerMove[4]+1] == 'b' || board[playerMove[3]+1][playerMove[4]+1] == 'B')
                               && (board[playerMove[3]+2][playerMove[4]+2] == ' '))
                               {
                               playerMove.resize(2*2+3);

                               playerMove[5] = playerMove[3]+2;
                               playerMove[6] = playerMove[4]+2;

                               playerMove[0] = 2;
                               }

                               }

                            bJump = false;
                            return;
                            }
                         }
                       }



                  //MOVING TO AN EMPTY SPACE
                 //moving right with a space
                 if(r <= 6 && c <= 6)  //boundary checking
                 {
                   if(board[r][c] == 'r' && board[r+1][c+1] == ' ')
                   {
                   playerMove.resize(1*2+3);

                   playerMove[1] = r;
                   playerMove[2] = c;

                   playerMove[3] = r+1;
                   playerMove[4] = c+1;

                   //resize
                   playerMove[0] = 1;
                   bJump = true;
                   }
                 }

                 if(r <= 6 && c >= 1) //boundary checking
                 {
                    //moving left with space
                    if(board[r][c] == 'r' && board[r+1][c-1] == ' ')
                    {
                    playerMove.resize(1*2+3);

                    playerMove[1] = r;
                    playerMove[2] = c;

                    playerMove[3] = r+1;
                    playerMove[4] = c-1;

                    //resize
                    playerMove[0] = 1;
                    bJump = true;
                    }
                  }

              }
        }



////////////////////////////////////////////////////

         //MOVING THE KING only if small black can't jump
        //also check to see if other player can jump u and block it

if(bJump)
{
        for(r = 0; r < 8; r++)
        {
              for(c = 0; c < 8; c++)
              {
                     //JUMPING A PLAYER
                     if(r <= 5 && c <= 5) //boundary checking
                     {
      //////           //jump a b player to the bottom-right
                       if(board[r][c] == 'R' && (board[r+1][c+1] == 'b' ||
                       board[r+1][c+1] == 'B'))
                       {
                          if(board[r+2][c+2] == ' ')
                          {
                          playerMove.resize(1*2+3);

                          playerMove[1] = r;
                          playerMove[2] = c;
                          playerMove[3] = r+2;
                          playerMove[4] = c+2;

                          //num of moves
                          playerMove[0] = 1;


                          if(r <= 3 && c <= 3) //boundary
                          {
                              //double jump down-right
                              if((board[r+3][c+3] == 'b' || board[r+3][c+3] == 'B')
                               && (board[r+4][c+4] == ' '))
                              {
                              playerMove.resize(2*2+3);

                              playerMove[5] = r+4;
                              playerMove[6] = c+4;

                              playerMove[0] = 2;
                              }
                           }

                           if(playerMove[4] >= 2)
                           {

                             //double jump down-left
                             if((board[playerMove[3]+1][playerMove[4]-1] == 'b' || board[playerMove[3]+1][playerMove[4]-1] == 'B')
                             && (board[playerMove[3]+2][playerMove[4]-2] == 'b'))
                              {
                              playerMove.resize(2*2+3);

                              playerMove[5] = playerMove[3]+2;
                              playerMove[6] = playerMove[4]-2;

                              playerMove[0] = 2;
                              }

                            }

                            if(playerMove[3] >= 2)
                            {
                              //double jump up-right
                              if((board[playerMove[3]-1][playerMove[4]+1] == 'b' || board[playerMove[3]-1][playerMove[4]+1] == 'B')
                               && (board[playerMove[3]-2][playerMove[4]+2] == ' '))
                              {
                              playerMove.resize(2*2+3);

                              playerMove[5] = playerMove[3]-2;
                              playerMove[6] = playerMove[4]+2;

                              playerMove[0] = 2;
                              }
                             }

                          return;//break out
                          }
                       }
                     }

//////////            //jump a 'B' player to the bottom-left
                      if(r <= 5 && c >= 2) //boundary
                      {
                         if(board[r][c] == 'R' && (board[r+1][c-1] == 'b' ||
                         board[r+1][c-1] == 'B'))
                         {
                            if(board[r+2][c-2] == ' ')
                            {
                            playerMove.resize(1*2+3);

                            playerMove[1] = r;
                            playerMove[2] = c;
                            playerMove[3] = r+2;
                            playerMove[4] = c-2;

                            //num of moves
                            playerMove[0] = 1;

                          if(r <= 3 && c >= 4) //boundary
                          {
                              //double jump down-left
                              if((board[r+3][c-3] == 'b' || board[r+3][c-3] == 'B')
                               && (board[r+4][c-4] == ' '))
                              {
                              playerMove.resize(2*2+3);

                              playerMove[5] = r+4;
                              playerMove[6] = c-4;

                              playerMove[0] = 2;
                              }
                           }


                            if(playerMove[4] <= 5)
                            {
                              //double jump down-right
                              if((board[playerMove[3]+1][playerMove[4]+1] == 'b' || board[playerMove[3]+1][playerMove[4]+1] == 'B')
                               && (board[playerMove[3]+2][playerMove[4]+2] == ' '))
                              {
                              playerMove.resize(2*2+3);

                              playerMove[5] = playerMove[3]+2;
                              playerMove[6] = playerMove[4]+2;

                              playerMove[0] = 2;
                              }
                             }


                           if(playerMove[3] >= 2)
                           {
                              //double jump up-left
                              if((board[playerMove[3]-1][playerMove[4]-1] == 'b' || board[playerMove[3]-1][playerMove[4]-1] == 'B')
                               && (board[playerMove[3]-2][playerMove[4]-2] == ' '))
                              {
                              playerMove.resize(2*2+3);

                              playerMove[5] = playerMove[3]-2;
                              playerMove[6] = playerMove[4]-2;

                              playerMove[0] = 2;
                              }

                            }
                            return;
                            }
                         }
                       }


///////////      //jump a  player to the top-right
                      if(r > 1 && c < 6) //boundary
                      {
                         if(board[r][c] == 'R' && (board[r-1][c+1] == 'b' ||
                         board[r-1][c+1] == 'B'))
                         {
                            if(board[r-2][c+2] == ' ')
                            {
                            playerMove.resize(1*2+3);

                            playerMove[1] = r;
                            playerMove[2] = c;
                            playerMove[3] = r-2;
                            playerMove[4] = c+2;

                            //num of moves
                            playerMove[0] = 1;


                         if(r >= 4 && c <= 3)
                         {
                                //double jump top-right
                                if((board[r-3][c+3] == 'b' || board[r-3][c+3] == 'B')&&
                                   (board[r-4][c+4] == ' '))
                                {
                                playerMove.resize(2*2+3);

                                playerMove[5] = r-4;
                                playerMove[6] = c+4;

                                playerMove[0] = 2;
                                }
                            }
                          }


                        if(playerMove[4] >= 2)
                        {
                          //double top-left jump
                          if((board[playerMove[3]-1][playerMove[4]-1] == 'b' || board[playerMove[3]-1][playerMove[4]-1] == 'B')&&
                          (board[playerMove[3]-2][playerMove[4]-2] == ' '))
                          {
                          playerMove.resize(2*2+3);

                          playerMove[5] = playerMove[3]-1;
                          playerMove[6] = playerMove[4]-2;

                          playerMove[0] = 2;
                          }
                         }


                        if(playerMove[3] <= 5)
                        {

                          //double jump down-right
                          if((board[playerMove[3]+1][playerMove[4]+1] == 'b' || board[playerMove[3]+1][playerMove[4]+1] == 'B')&&
                          (board[playerMove[3]+2][playerMove[4]+2] == ' '))
                          {
                          playerMove.resize(2*2+3);

                          playerMove[5] = playerMove[3]+2;
                          playerMove[6] = playerMove[4]+2;

                          playerMove[0] = 2;
                          }

                         }
                         return;
                         }
                       }


  //////               //jump a player to the top-left
                      if(r > 1 && c > 1) //boundary
                      {
                         if(board[r][c] == 'R' && (board[r-1][c-1] == 'b' ||
                         board[r-1][c-1] == 'B'))
                         {
                            if(board[r-2][c-2] == ' ')
                            {
                            playerMove.resize(1*2+3);

                            playerMove[1] = r;
                            playerMove[2] = c;
                            playerMove[3] = r-2;
                            playerMove[4] = c-2;

                            //num of moves
                            playerMove[0] = 1;

                               if(r >= 4 && c >= 4)
                               {
                                 //double jump top-left
                                 if((board[r-3][c-3] == 'b' || board[r-3][c-3] == 'B')
                                 &&(board[r-4][c-4] == ' '))
                                 {
                                 playerMove.resize(2*2+3);

                                 playerMove[5] = r-4;
                                 playerMove[6] = c-4;

                                 playerMove[0] = 2;
                                 }
                                }

                             if(playerMove[4] <= 5)
                             {

                                //double jump top-right
                                if((board[playerMove[3]-1][playerMove[4]+1] == 'b' || board[playerMove[3]-1][playerMove[4]+1] == 'b')
                                 &&(board[playerMove[3]-2][playerMove[4]+2] == ' '))
                                 {
                                 playerMove.resize(2*2+3);

                                 playerMove[5] = playerMove[3]-2;
                                 playerMove[6] = playerMove[4]+2;

                                 playerMove[0] = 2;
                                 }
                              }


                              if(playerMove[3] <= 5)
                              {
                                 //double jump down-left
                                 if((board[playerMove[3]+1][playerMove[4]-1] == 'b' || board[playerMove[3]+1][playerMove[4]-1] == 'b')
                                 &&(board[playerMove[3]+2][playerMove[4]-2] == ' '))
                                 {
                                 playerMove.resize(2*2+3);

                                 playerMove[5] = playerMove[3]+2;
                                 playerMove[6] = playerMove[4]-2;

                                 playerMove[0] = 2;
                                 }
                               }

                            return;
                            }
                         }
                       }



                  //MOVING TO AN EMPTY SPACE
                 //moving bottom-right with a space
                 if(r <= 6 && c <= 6)  //boundary checking
                 {
                   if(board[r][c] == 'R' && board[r+1][c+1] == ' ')
                   {
                   playerMove.resize(1*2+3);

                   playerMove[1] = r;
                   playerMove[2] = c;
                   playerMove[3] = r+1;
                   playerMove[4] = c+1;

                   //resize
                   playerMove[0] = 1;
                   }
                 }

                 if(r <= 6 && c >= 1) //boundary checking
                 {
                    //moving bottom-left with space
                    if(board[r][c] == 'R' && board[r+1][c-1] == ' ')
                    {
                    playerMove.resize(1*2+3);

                    playerMove[1] = r;
                    playerMove[2] = c;

                    playerMove[3] = r+1;
                    playerMove[4] = c-1;

                    //resize
                    playerMove[0] = 1;
                    }
                  }

                  if(r > 0 && c > 0) //boundary checking
                  {
                    //moving top-left with space
                    if(board[r][c] == 'R' && board[r-1][c-1] == ' ')
                    {
                    playerMove.resize(1*2+3);

                    playerMove[1] = r;
                    playerMove[2] = c;

                    playerMove[3] = r-1;
                    playerMove[4] = c-1;

                    //resize
                    playerMove[0] = 1;
                    }
                  }

                  if(r > 0 && c < 7) //boundary checking
                  {
                    //moving top-right with space
                    if(board[r][c] == 'R' && board[r-1][c+1] == ' ')
                    {
                    playerMove.resize(1*2+3);

                    playerMove[1] = r;
                    playerMove[2] = c;

                    playerMove[3] = r-1;
                    playerMove[4] = c+1;

                    //resize
                    playerMove[0] = 1;
                    }
                  }

              }
        }

}


}


void TakeMove(apmatrix<char> &boardForMove, const apvector<int> &move)
{

        int i = 999;
        int num = move[0];
        char piece;
        int len = move.length();
        int midX=999;
        int midY=999;

        for(i=1; i<len-2; i++)
        {
          piece = boardForMove[move[i]][move[i+1]];
          boardForMove[move[i]][move[i+1]] = ' ';
          boardForMove[move[i+2]][move[i+3]] = piece;
          if((abs(move[i]-move[i+2]) ==2 )
             && (abs(move[i+1]-move[i+3]) == 2)  )
             {                                            //check for jump
             midX = (move[i] + move[i+2]) / 2;          //if move is two
             midY = (move[i+1] + move[i+3]) / 2;        //squares then jump
             boardForMove[midX][midY] = ' '; 
             }                                             // delete middle
                                                         // of jump point
          i++;
        }

                                                        //check to see
        if ((piece == 'b') && (move[i] == 0))           //if piece is "KINGED"
             boardForMove[move[i]][move[i+1]]  = 'B';   //at end of turn moving
        if ((piece == 'r') && (move[i] == 7))
             boardForMove[move[i]][move[i+1]]  = 'R';
}




void Fill(apmatrix<char>&board)
{
char fill1 ='r', fill2 ='b';
int r,c;

             //fills in the colors(red & white) to the checkerboard
             for(r = 0; r < board.numrows(); r++)
             {
                 for(c = 0; c < board.numcols(); c++)
                 {
                   /////////////////////////////////////////////
                   //Top row = Wanna-be player
                   if(r == 0) //if first row of 1st
                   {
                   c++;
                   board[r][c] = fill1;
                   }

                   else if(r == 1) //if second row
                   {
                   board[r][c] = fill1;
                   c++;
                   }

                   else if(r == 2)
                   {
                   c++;
                   board[r][c] = fill1;
                   }
                   //////////////////////////////////////////////


                   /////////////////////////////////////////////
                   //Bottow row = Real Player
                   else if(r == board.numrows()-3)
                   {
                   board[r][c] = fill2;
                   c++;
                   }

                   else if(r == board.numrows()-2)
                   {
                   c++;
                   board[r][c] = fill2;
                   }

                   else if(r == board.numrows()-1)
                   {
                   board[r][c] = fill2;
                   c++;
                   }
                   /////////////////////////////////////////////
                 }
             }

}


void DispBoard(const apmatrix<char>&board)
{
int r,c;

        cout << endl << endl;
        cout << "   0 1 2 3 4 5 6 7" << endl;
        cout << "  -----------------" << endl;
        for(r = 0; r < board.numrows(); r++)
        {
           for(c = 0; c < board.numcols(); c++)
           {
              if(c == 0)
              {
              cout << r << " |";
              }
              cout << board[r][c] << "|";
           }
           cout << endl << "  -----------------" << endl;
        }
}


bool Winner(const apmatrix<char>&board)
{
int rCount = 0,  wCount = 0;
int r,c;


    for(r = 0 ; r < board.numrows(); r++)
    {

          for(c = 0; c < board.numcols(); c++)
          {

              if(board[r][c] == 'b' || board[r][c] == 'b')
              {
              wCount++;
              } 

             else if(board[r][c] == 'r' || board[r][c] == 'R')
             {
             rCount++;
             }
          
          }

    }

        if(rCount == 0)
        {
        cout << "White wins! Red is a loser!" << endl;
        return false;
        }
        

           else if(wCount == 0)
           {    
           cout << "Red wins! White is a loser!" << endl;
           return false;
           }
    
           else
           return true;            
}
