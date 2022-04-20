#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Function declarations
void initialization(int size, int *board);
int position (int num, int size, int *board);
void teardown();
void displayGame(int size,int *board);
int isMoveValid(int num, int size, int *board);
void moveTile(int num, int size, int *board);
int wonGame(int size, int *board);
int saveGame(char *filename, int size, int*board);
int loadGame(char *filename, int *board);

//global variable
int coordinate[]={0,0};



//Creating the 4x4 board in an array.
//The tiles are in inverse order 15 to 1
/*int board[4][4] =
    {{15, 14, 13, 12},
     {11, 10, 9, 8},
     {7, 6, 5, 4},
     {3, 2, 1, 0}};*/
/*
This stores the game state in a 4x4 board and runs through
all 16 positions and adds a ' '
@param board: row and column positions inside the board
*/

void initialization(int size, int *board)
{
    printf("Setting up the game\n");
    int i = 0;
    //Allocating as an array of pointers to arrays 
    board = (int *) malloc(size * size * sizeof(int));
    for (int i =0; i<(size*size); i++) //fill pointers
   // *(board + i) = (int *) malloc(size * sizeof(int))
    *(board + i) = (size * size - 1 - i);

}

int position (int num, int size, int *board)
{
    for (int i = 0; i<size; i++ )
    {
        for(int j = 0; j<size; j++)
        {
            if(*(board+i*size+j)==num)
            {
                coordinate[0]=i;
                coordinate[1]=j;
            }
        }
    }
    return 0;
}

/*
function that ends and quits the game 
*/
void teardown(int *board)
{
    printf("Ending the game");
    // Deallocating memory
    free(board);
}

/*
Prints out the board out in a 4x4 matrix
for loop loops through rows then columns
@param board: row and column
*/
void displayGame(int size, int *board)
{
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (*(board+i*size+j)== 0)
            {
                printf("%4d",' ');
                //continue;
            }
            else
            {
                printf("%4d", *board);
            }
        }
        printf("\n");
    }
}

/*
Checks if the 'move' entered is a valid choice 
@param num: 
@param move: pointer for move 
*/


int isMoveValid(int num, int size, int *board)
{
    if(num<=0 || num>=(size*size))
    {
        printf("invalid tile \n");
        return 0;
    }
    int x = coordinate[0];
    int y = coordinate[1]; 
    position(0,size, *board);

    //loops through row
        for (int i = 0; i < size; i++)
        {//loops through column
            for (int j = 0; j < size; j++)
            {
                if (*(board+i*size+j) == num)
                
                {/*if row-1 is greater or equal to 0 then go to next 
                if statement... if row-1 and col equals 0 then move the tile up
                */
                    if (i - 1 >= 0)
                    {
                        //if (board[i - 1][j] == 0)
                        if(*(board+(i-1)*size+j) == 0)
                        {
                            moveTile(i-1, j, *board);
                            //moveTile(i, j, i - 1, j, *board);
                            return 1;
                        }
                    }
                    if (i + 1 < 4)
                    {
                        //if (board[i + 1][j] == 0)
                        //if(*(board+(i+1)*size+j) == 0)
                        if(*(board+i*size+j)==0)
                        {
                            moveTile(i+1, j, *board);
                            //moveTile(i, j, i + 1, j, *board);
                            return 1;
                        }
                    }
                    if (j - 1 >= 0)
                    {
                        //if (board[i][j - 1] == 0)
                        if (*(board + i *size+(j-1)))
                        {
                            moveTile(i, j-1, *board);
                            //moveTile(i, j, i, j - 1, *board);
                            return 1;
                        }
                    }
                    if (j + 1 < 4)
                    {
                        //if (board[i][j + 1] == 0)
                        if (*(board+i*size+(j+1)))
                        {
                            moveTile(i,j+1, *board);
                            //moveTile(i, j, i, j + 1, *board);
                            return 1;
                         }
                }
                return 0;
            }
        }
        
    }
    return 0;
}

/*
if the function isMoveValid is true, then 
the move is performed and updates the game state. 
if unvalid, then an error message will be printed
@param board: row and column
@param char: pointer to move that stores the move 
*/
void moveTile(int num, int size, int *board)
{
    int valid = isMoveValid(num, size, *board);
    if(valid == 1)
    {
        position(0,size, *board);

        int blankX= coordinate[0];
        int blankY = coordinate[0];

        int tileX = coordinate[0];
        int tileY = coordinate[1];

        int temp = *(board + blankX * size + blankY);
        *(board + blankX*size + blankY) =* (board + tileX * size + tileY);
        *(board + tileX * size + tileY) = temp;
    }
}

int wonGame(int size, int *board)
{
    for( int i=1; i<(size*size-1);i++)
    {
        if(*(board+i)==(*(board+i-1)+1))
        {
            continue;
        }
        else if(*(board+i)==0 && *(board+i+1) == (*(board+i-1)+1))
        {
            i++;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int saveGame(char *filename, int size, int*board)
{
    FILE *file;
    int written;
    if((file = fopen(filename,"write"))==NULL)
    
    written=fwrite(board,sizeof(*board),size,file);
    if (written== 0)
    return 0;
    else
    {
        fclose(file);
        return 1;
    }
}

int loadGame(char *filename, int *board)
{
    FILE *file;
    int *temp;
    if((file=fopen(filename,"re"))==NULL)
    return 0;

    fseek(file,0,SEEK_END);
    int size = ftell(file)/sizeof(board[0]);
    fseek(file,0,SEEK_SET);

    //temp=(int*)malloc(size,sizeof(board[0]));
    temp=(int*)malloc(sizeof(board[0]));

    if(!fread(temp,sizeof(board[0]),size,file))
    return 0;
    else
    {
        int *t = board;
        board = temp;
        temp = t;
        fclose(file);
        return 1;
    }
    free (temp);
}

int main()
{
    int num;
    int size;
    int *board[size][size];
    //setting up the game, calling the initialization function 
    initialization(size, *board);
    char ch;
    char filename[20];
    int tile;
   
    //genreating initial state and displaying board
    displayGame(size,*board);
    //Looping game till user quits
    do
    {
        printf("Menu: [n]ew, [s]ave, [l]oad, [p]rint, [m]ove, [q]uit? ");
        scanf("%s", &ch);
        switch (ch)
        {
        // menu option for print
        case 'n':
        while(1)
        {
            printf("Enter a size for the board[2-10]: \n");
            scanf("%d", &size);
            if(size>= 2 && size<=10)
            {
            break;
            }
            else 
            {
            printf("Invalid size. \n");
            }
        }
            initialization(size, *board);
            break;
        case 's':
            
            printf("Name your file: \n");
            scanf("%s", filename);

            int success = saveGame(filename,size, *board);
            if (success == 1)
            {
            printf("Saved successfully. \n)");
            }
            else
            printf("Could not save file. \n");
            break;
        case 'l':
            printf("What is the name of the file? \n");
            scanf("%s",filename);
            int loading = loadGame(filename, *board);
            if(loading == 1)
            {
                printf("Loaded successfully. \n");
            }
            else
            {
                printf("Could not load file. \n");
            }
        case 'p':
            displayGame(size, *board);
            printf("\n");
            break;
        //menu option for move
        case 'm':
            printf("Which tile to move? ");
            scanf("%d", &tile);
            //if(isMoveValid(tile)==0)
           /* if (isMoveValid(num, size, *board) == 0)
            {
                printf("Invalid Move.\n");
            }
            else{
                printf("Moving %d\n", tile);
            }*/
            moveTile(tile,size, *board);
            //displayGame(size, *board);
            break;
        //menu option for quitting the game 
        case 'q':
            teardown(board);
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (ch != 'q');
    return 0;
}
part2.c
9 KB