#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int debugMode;  //global variable for development

typedef struct coordinateStruct{
    int xpos;
    int ypos;
    char val;
    int visited; //0 if false, 1 if true
}coordinate;


/*---------------------------------End of Coordinate struct --------------------------------------------*/


typedef struct mazeStruct
{
 coordinate** darr;//pointer to a dynamically array of coordinate pointers
 int xsize, ysize;           //size of maze, row by column
 coordinate start;      //coordinates for start position
 coordinate end;        //coordinates for end position
} maze;

//use next set of values for row and column if values are less than 0
void checkSize(maze *m, FILE *src){
    if(fscanf (src, "%d %d", &m->xsize, &m->ysize) != EOF){
        if(m->xsize <= 0 || m->ysize <= 0){
            printf("Arguments must be greater than zero. Using next set of values...\n");
            checkSize( m, src);
        }
    }
    else{
        printf("Too few arguments within file to continue. Exiting...\n");
        exit(-1);
    }
}

//use next set of values for Start Coordinate if needed
void checkStart(maze *m, FILE *src){
    if(fscanf (src, "%d %d", &m->start.xpos, &m->start.ypos) != EOF){
        if(m->start.xpos <= 0 || m->start.xpos > m->xsize){
            printf("X coordinate for start position must be between 1 and %d. Using next set of values...\n", m->xsize);
            checkStart( m, src);
        }
        else if(m->start.ypos <= 0 || m->start.ypos > m->ysize){
            printf("Y coordinate for start position must be between 1 and %d. Using next set of values...\n", m->ysize);
            checkStart( m, src);
        }
    }
    else{
        printf("Too few arguments within file to continue. Exiting...\n");
        exit(-1);
    }
}

//use next set of values for End Coordinate if needed
void checkEnd(maze *m, FILE *src){
    if(fscanf (src, "%d %d", &m->end.xpos, &m->end.ypos) != EOF){
        if(m->end.xpos <= 0 || m->end.xpos > m->xsize){
            printf("X coordinate for end position must be between 1 and %d. Using next set of values...\n", m->xsize);
            checkEnd( m, src);
        }
        else if(m->end.ypos <= 0 || m->end.ypos > m->ysize){
            printf("Y coordinate for end position must be between 1 and %d. Using next set of values...\n", m->ysize);
            checkEnd( m, src);
        }
        else if(m->end.xpos == m->start.xpos && m->end.xpos == m->start.ypos){
            printf("End position cannot be the same as start position. Using next set of values...\n");
            checkEnd( m, src);
        }
    }
    else{
        printf("Too few arguments within file to continue. Exiting...\n");
        exit(-1);
    }
}

//use next set of values for Block Coordinates if needed
void checkCoor(maze *m, FILE *src){

    int xpos, ypos;

    while (fscanf (src, "%d %d", &xpos, &ypos) != EOF){
        if(xpos <= 0 || xpos > m->xsize){
            printf("X coordinate for block position must be between 1 and %d. Using next set of values...\n", m->xsize);
            checkCoor( m, src);
        }
        else if(ypos <= 0 || ypos > m->ysize){
            printf("Y coordinate for block position must be between 1 and %d. Using next set of values...\n", m->ysize);
            checkCoor( m, src);
        }
        else if(xpos == m->start.xpos && ypos == m->start.ypos){
            printf("Block position cannot be the same as start position. Using next set of values...\n");
            checkCoor( m, src);
        }
        else if(xpos == m->end.xpos && ypos == m->end.xpos){
            printf("Block position cannot be the same as end position. Using next set of values...\n");
            checkCoor( m, src);
        }
        else{
            m->darr[xpos][ypos].val = '*'; 
            m->darr[xpos][ypos].visited = TRUE; 
        }
    }
}

//creates maze and prints values for row and column as well as start and end coordinates
void initMaze (maze *m,   FILE *src){
    int xpos, ypos;
    int i,j;

    /* read in the size, starting and ending positions in the maze */
    checkSize( m, src);
    checkStart( m, src);
    checkEnd( m, src);
    /* print them out to verify the input */
    printf ("size: %d, %d\n", m->xsize, m->ysize);
    printf ("start: %d, %d\n", m->start.xpos, m->start.ypos);
    printf ("end: %d, %d\n", m->end.xpos, m->end.ypos);

    int row = m->xsize +2;
    int col = m->ysize +2;
    //allocate memory for dynamic array
    m->darr = (coordinate**) malloc(sizeof(coordinate*) * row);
    for(i = 0; i < row; i++){
        m->darr[i]=(coordinate*) malloc(col * sizeof(coordinate));
    }
    
    /* initialize the maze to empty */
        for (i = 0; i < row; i++){
            for (j = 0; j < col; j++){
                m->darr[i][j].val = '.';
                m->darr[i][j].visited = FALSE;
                m->darr[i][j].xpos = i;
                m->darr[i][j].ypos = j;
            }
        }

    /* mark the borders of the maze with *'s */
    for (i=0; i < row; i++) { //left and right
        m->darr[i][0].val = '*';
        m->darr[i][0].visited = TRUE;
        m->darr[i][col-1].val = '*';
        m->darr[i][col-1].visited = TRUE;
    }
    
    for (i=0; i < col; i++) {   //top and bottom
        m->darr[0][i].val = '*';
        m->darr[0][i].visited = TRUE;
        m->darr[row-1][i].val = '*';
        m->darr[row-1][i].visited = TRUE;
    }

    /* mark the starting and ending positions in the maze */
    m->darr[m->start.xpos][m->start.ypos].val = 's';
    m->darr[m->start.xpos][m->start.ypos].visited = TRUE;
    m->darr[m->end.xpos][m->end.ypos].val = 'e';

    /* mark the blocked positions in the maze with *'s */
    checkCoor( m, src);

    /* print out the initial maze */
    for (i = 0; i < m->xsize+2; i++){
        for (j = 0; j < m->ysize+2; j++){
            printf ("%c", m->darr[i][j].val);
            if(j == col - 1){
                printf("\n");
            }
        }
    }
}

//clears maze
void clearMaze(maze *m){
    int row = m->xsize +2;
    int i;
    for(i = 0; i < row; i++){
        free(m->darr[i]);
    }
    free(m->darr);
}

/*---------------------------------End of Maze struct and functions------------------------------*/


typedef struct NodeStruct{
    coordinate c;
    struct NodeStruct* next;
}Node;

typedef struct ListStruct{
    Node * front;
}List;

void initList(List *lst){
    lst->front = NULL;
}

void push (Node ** head, coordinate * xy){
    Node* ptr = (Node*) malloc (sizeof(Node));
    ptr->c = *(xy);
    ptr->c.visited = TRUE;
    ptr->c.val = 'v';
    ptr->next = *head;
    *head = ptr;

    if ( debugMode == TRUE ){
        printf ("(%d, %d) \n", ptr->c.xpos, ptr->c.ypos);
    }
}

int isEmpty (Node * head){
    if(head == NULL){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

void top(Node * head, int *x, int *y){
    *x = head->c.xpos;
    *y = head->c.ypos;
}

void pop(List * lst){
    Node* tmp = lst->front;
    lst->front = lst->front->next;
    if ( debugMode == TRUE ){
    printf (" (%d, %d) \n", tmp->c.xpos, tmp->c.ypos);
    }
    free(tmp);
}

//clears entire Node List
void clear( List* lst){
    while(lst->front != NULL){
        Node* ptr;
        ptr = lst->front;
        lst->front = lst->front->next;
        free(ptr);
        ptr = lst->front;
    }
    lst->front = NULL;
}

//prints reverse of stack
void printStack(Node* ptr){
    if(ptr->next != NULL){
        printStack(ptr->next);
        printf("( %d, %d )\n", ptr->c.xpos, ptr->c.ypos);
    }
    else{
        printf("( %d, %d )\n", ptr->c.xpos, ptr->c.ypos);
    }
}

/*---------------------------------End of Node struct and functions------------------------------*/


int main (int argc, char **argv)
{
  maze m1;

  FILE *src;

    debugMode = FALSE;
    int i;
    for(i = 0; i < argc; i++){
        if( strcmp(argv[i], "-d") == 0 ){
            debugMode = TRUE;
        }
    }

    /* verify the proper number of command line arguments were given */
    if(argc < 2){
        printf("Usage: %s <input file name>\n", argv[0]);
        exit(-1);
    }
    else if(argc > 2){
        printf("Too many file names. Only accepted first file.");
    }
   
    /* Try to open the input file. */
    if ( ( src = fopen( argv[1], "r" )) == NULL ){
        printf ( "Can't open input file: %s \n", argv[1] );
        exit(-1);
    }

    initMaze(&m1, src);

    /*algorithm here*/

    List stack;
    initList(&stack);

    push(&(stack.front), &m1.start); //push start position onto stack

    int x;
    int y;
    int endFound = FALSE;

    while(isEmpty(stack.front)== FALSE && endFound == FALSE){//infinite?
        top(stack.front, &x, &y);

        //if end position is found
        if(x == m1.end.xpos){
            if(y == m1.end.ypos){
                endFound = TRUE;
                break;
            }
        }

        if(m1.darr[x+1][ y].visited == FALSE){ //down
            push(&(stack.front), &m1.darr[x+1][y]);
            m1.darr[x+1][ y].visited = TRUE;
        }

        else if(m1.darr[x][y+1].visited == FALSE){ //right
            push(&(stack.front), &m1.darr[x][y+1]);
            m1.darr[x][y+1].visited = TRUE;
        }

        else if(m1.darr[x-1][ y].visited == FALSE){ //up
            push(&(stack.front), &m1.darr[x-1][ y]);
            m1.darr[x-1][ y].visited = TRUE;
        }

        else if(m1.darr[x][ y - 1].visited == FALSE){ //left
            push(&(stack.front), &m1.darr[x][y-1]);
            m1.darr[x][ y - 1].visited = TRUE;
        }
        else{                                                                                   //backup
            pop(&stack);
        }
    }

    if (stack.front == NULL){
        printf("This maze has no solution. Exiting...\n");
    }
    else{
        printStack(stack.front);
        printf("\n");
        clear(&stack);
    }
    clearMaze(&m1);
    return 0;
}

