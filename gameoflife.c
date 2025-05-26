//GoL de la capat
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 


struct Coord{
    int line, column;
    struct Coord *next;
};
typedef struct Coord Cell;

struct Stack{
    Cell *changedcell; 
    struct Stack *next;
};
typedef struct Stack NodeStack;

struct Tree{
    Cell *changedcell;
    struct Tree *left, *right;
};
typedef struct Tree NodeTree;

struct Graph{
    Cell *changedcell;
    int nrnode;
    struct Graph* next;
    int **a;
}Graph;
typedef struct Graph NodeGraph;
 
struct VectorHelp{
    int line, column;
};
typedef struct VectorHelp Vector;

void reead(FILE *finput, char ***grid, int n, int m, int k)
{

    int i,j;
    //fac un border si ii aloc spatiu
    int bordern, borderm;
    bordern=(n)+2;
    borderm=(m)+2;

    *grid=(char**)malloc(bordern * sizeof(char*));
    for(i=0;i<bordern;i++)
    {   
        (*grid)[i]=(char*)malloc((borderm+1) * sizeof(char));
    }
    //initializez borderul si matricea
    for(i=0;i<bordern;i++)
        for(j=0;j<borderm;j++)
    {
        (*grid)[i][j]='+';
    }

    for(i=0;i<bordern;i++)
        for(j=0;j<borderm;j++)
        {
            (*grid)[0][j]='+';
            (*grid)[bordern-1][j]='+';
            (*grid)[i][0]='+';
            (*grid)[i][borderm-1]='+';
        }
    
    //citesc restul valorilor in interiorul borderului

    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
    {   
        fscanf(finput, " %c", &((*grid)[i][j]));
    }
}
void writee(FILE *foutput,char **grid, int n, int m)
{
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=m;j++)
            fprintf(foutput, "%c", grid[i][j]);
        fprintf(foutput,"\n");
    }
}
void freespace( char **grid, int n)
{
    int i,bordern;
    if(grid==NULL) return;
    bordern=(n)+2;
    for(i=0;i<bordern;i++)
        free(grid[i]);
    free(grid);
}
int alive_neighbours(char **grid, int i, int j)
{
    int how_many=0;
    if(grid[i-1][j]=='X') how_many++;
    if(grid[i+1][j]=='X') how_many++;
    if(grid[i-1][j-1]=='X') how_many++;
    if(grid[i-1][j+1]=='X') how_many++;
    if(grid[i+1][j-1]=='X') how_many++;
    if(grid[i+1][j+1]=='X') how_many++;
    if(grid[i][j+1]=='X') how_many++;
    if(grid[i][j-1]=='X') how_many++;
    return how_many;
}
void new_gen(char **grid, int n, int m, char ***newgen)
{
    int i, j, alive;
    //aloc memorie pt noua generatie cu tot cu border
    int bordern, borderm;
    bordern=n+2;
    borderm=m+2;

    *newgen=(char**)malloc(bordern * sizeof(char*));
    for(i=0;i<bordern;i++)
    {   
        (*newgen)[i]=(char*)malloc((borderm+1) * sizeof(char));
    }
    //initializez matricea cu celulele din grid
    for(i=1;i<bordern-1;i++)
        for(j=1;j<borderm-1;j++)
        {
            (*newgen)[i][j]=grid[i][j];
        }
    //initializez borderul
    for(i=0;i<bordern;i++)
        for(j=0;j<borderm;j++)
        {
            (*newgen)[0][j]='+';
            (*newgen)[bordern-1][j]='+';
            (*newgen)[i][0]='+';
            (*newgen)[i][borderm-1]='+';
        }
    //aplic regulile GoL 
    //parcurg veche generatie si salvez modificarile in noua generatie
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            alive=alive_neighbours(grid,i,j);
            if((grid)[i][j]=='X')
            {
                if(alive<2 || alive>3) 
                (*newgen)[i][j]='+';
                else
                    if(alive==2 || alive==3) 
                        (*newgen)[i][j]='X';
            }
            else
            {
                if(alive==3)  (*newgen)[i][j]='X';
                else  
                (*newgen)[i][j]='+';
            }
               
        }

}
/*void swap_gen(char ***grid, char ***newgen)
{
    //swap la generatii astfel incat cea mai noua generatie sa fie luata ca referinta
    //look into it nu merge pe T4 
    char **aux;
    aux=*grid;
    *grid=*newgen;
    *newgen=aux;
}*/

void push(NodeStack** stackTop, Cell *changedcell) {
    NodeStack* newNode = (NodeStack*)malloc(sizeof(NodeStack));
    newNode->changedcell= changedcell;
    newNode->next = *stackTop;
    *stackTop = newNode;
}

void addAtEndList(Cell **cellHead, int l, int c)
{
    Cell *aux=*cellHead;
    Cell *newCell=(Cell*)malloc(sizeof(Cell));
    newCell->line=l-1; //ca incepe de la 0 in matrice
    newCell->column=c-1;

    if(*cellHead==NULL)
    {
        newCell->next=*cellHead;
        *cellHead=newCell;
    }
    else
    {
        while(aux->next!=NULL)
            aux=aux->next;
        aux->next=newCell;
        newCell->next=NULL;
    }
}
NodeStack* StackedCells(char **grid, int n, int m, char ***newgen)
{
    NodeStack *stackTop=NULL;
    Cell *cellHead=NULL;
    
    new_gen(grid,n,m,newgen);
    int i,j;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            if(grid[i][j]!=(*newgen)[i][j]) //daca celula veche e diferita de cea noua dau push la coordonate in stiva
            addAtEndList(&cellHead,i,j);
        }
    push(&stackTop,cellHead);
    return stackTop;
}
void writeStack(FILE *foutput, NodeStack* stackTop, int generation)
{
    fprintf(foutput,"%d ",generation);
    while(stackTop!=NULL)
    {
        Cell *cellHead=stackTop->changedcell;
        while(cellHead!=NULL)
        {
            fprintf(foutput,"%d %d ",cellHead->line, cellHead->column);
            cellHead=cellHead->next;
        }
        stackTop=stackTop->next;
    }
    fprintf(foutput,"\n");
}
void deleteCell(Cell *cellHead)
{
    Cell *temp;
    while ((cellHead) != NULL) {  
        temp = cellHead;
        cellHead = (cellHead)->next;
        free(temp);
    }
}
void deleteStack(NodeStack** stackTop) {
    NodeStack* temp;
    while ((*stackTop) != NULL) { 
        deleteCell((*stackTop)->changedcell); 
        temp = *stackTop;
        *stackTop = (*stackTop)->next;
        free(temp);
    }
}
void new_gen_rule_b(char **grid, int n, int m, char ***newgen)
{
    int i, j, alive;
    //aloc memorie pt noua generatie cu tot cu border
    int bordern, borderm;
    bordern=n+2;
    borderm=m+2;

    *newgen=(char**)malloc(bordern * sizeof(char*));
    for(i=0;i<bordern;i++)
    {   
        (*newgen)[i]=(char*)malloc((borderm+1) * sizeof(char));
    }
    //initializez matricea cu celulele din grid
    for(i=1;i<bordern-1;i++)
        for(j=1;j<borderm-1;j++)
        {
            (*newgen)[i][j]=grid[i][j];
        }
    //initializez borderul
    for(i=0;i<bordern;i++)
        for(j=0;j<borderm;j++)
        {
            (*newgen)[0][j]='+';
            (*newgen)[bordern-1][j]='+';
            (*newgen)[i][0]='+';
            (*newgen)[i][borderm-1]='+';
        }
    //aplic regulile GoL 
    //parcurg veche generatie si salvez modificarile in noua generatie
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            alive=alive_neighbours(grid,i,j);
           if(alive==2) 
                (*newgen)[i][j]='X';
            else 
                (*newgen)[i][j]='+';
               
        }

}
void copyGrid(char **sursa, char ***dest, int n, int m)
{
    int i,j,bordern,borderm;
    bordern=n+2, borderm=m+2;
    *dest=(char**)malloc(bordern*sizeof(char*));
    for(i=0;i<bordern;i++)
    {
        (*dest)[i]=(char*)malloc((borderm+1)*sizeof(char));
    }
    for(i=0;i<bordern;i++)
        for(j=0;j<borderm;j++)
            (*dest)[i][j]=sursa[i][j];
}
void addToTree(NodeTree *rootTree, char **grid,int n, int m, int k, int lvl)
{
    int i,j;
    if(lvl>k) return;
    //subarborele stang cu regula B
    char **newgenB=NULL;
    new_gen_rule_b(grid,n,m,&newgenB);

    Cell *cellHeadB=NULL;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            if(grid[i][j]!=newgenB[i][j]) //daca celula veche e diferita de cea noua dau push la coordonate in stiva
            addAtEndList(&cellHeadB,i,j);
        }

    NodeTree* leftNode=(NodeTree*)malloc(sizeof(NodeTree));
    leftNode->changedcell=cellHeadB;
    leftNode->left=NULL;
    leftNode->right=NULL;
    rootTree->left=leftNode;

    addToTree(leftNode,newgenB,n,m,k,lvl+1);
    freespace(newgenB,n);

    //subarborele din dreapta cu regulule normale
    char **newgenN=NULL;
    new_gen(grid,n,m,&newgenN);

    Cell *cellHeadN=NULL;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            if(grid[i][j]!=(newgenN)[i][j]) //daca celula veche e diferita de cea noua 
            addAtEndList(&cellHeadN,i,j);
        }

    NodeTree* rightNode=(NodeTree*)malloc(sizeof(NodeTree));
    rightNode->changedcell=cellHeadN;
    rightNode->left=NULL;
    rightNode->right=NULL;
    rootTree->right=rightNode;

    addToTree(rightNode,newgenN,n,m,k,lvl+1);
    freespace(newgenN,n);

}
void buildRootList(NodeTree **rootTree, char **grid, int n, int m,int k)
{
    //initializez root ul arborelui cu o lista cu celulele vii din generatia 0;
    //le salvez ca si cum sunt celule schimbate fata de un grid gol
    int i,j;
    *rootTree=(NodeTree*)malloc(sizeof(NodeTree));
    Cell *cellHead=NULL;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        if(grid[i][j]=='X')
            addAtEndList(&cellHead,i,j);

    (*rootTree)->changedcell=cellHead;
    (*rootTree)->left=NULL;
    (*rootTree)->right=NULL;
    //construiesc nodurile arborelui cu liste
    addToTree((*rootTree),grid,n,m,k,0);

}

NodeGraph* createGraph(NodeGraph* graph)
{
    graph=(NodeGraph*)malloc(sizeof(NodeGraph));
    graph->nrnode=0;
    graph->changedcell=0;
    graph->next=NULL;
    return graph;
}
void addToGraph(NodeGraph *graph, char **grid, int n, int m)
{
    int i,j;
    Cell *cellGraph=NULL;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            if(grid[i][j]=='X') 
            addAtEndList(&cellGraph,i-1,j-1); //numerotarea incepe de la 0
            graph->nrnode++;
        }

    NodeGraph* newNode=(NodeGraph*)malloc(sizeof(NodeGraph));
    newNode->changedcell=cellGraph;
    newNode->nrnode=graph->nrnode;
    newNode->next=graph->next;
    graph->next=newNode;
}

int isEdge(int x1, int y1, int x2, int y2)
{
    //daca diferenta in modul dintre coord e 0 atunci exista muchie si o pun in matricea de adiacenta
    if(x1==x2) 
        if(abs(y1-y2)==1) return 1;

    if(y1==y2) 
        if(abs(x1-x2)==1) return 1;

    if(abs(x1-x2)==1)
        if(abs(y1-y2)==1) return 1;

    if(abs(y1-y2)==1)
        if(abs(x1-x2)==1) return 1;
    
    return 0;
}

void preorderBuildTreeGrid(FILE* foutput, NodeTree* rootTree, /*Cell *changedcell, */char **newgridTree,/*char ***oldGrid,*/ int n, int m,int lvl)
{
    /*deci eu am un arbore. fiecare nod contine o lista de celule care s au schimbat
    eu stiu ca celulele din root sunt toate vii, deci pot sa construiesc matricea dupa ele.la generatia 1,
    stiu starea fiecarei celule pt ca stiu starea din gen 0 si care sunt modificate. si tot asa.
    parcurg arborele in preordine. cand merg pe stanga stiu ca s-au modificat dupa regula b, cand 
    merg pe dreapta stiu ca s au modificat dupa regulile standard. lessgoo*/
    int i,j,l,c;

    if(rootTree==NULL) return;
    writee(foutput,newgridTree,n,m);  

    fprintf(foutput,"\n");//spatiu ca nu ii place fara
    if(rootTree->left!=NULL)
    {
        char **leftGrid=NULL;
        copyGrid(newgridTree,&leftGrid,n,m);
        Cell *auxCell=rootTree->left->changedcell;
        while(auxCell!=NULL)
        {
            l=auxCell->line+1; 
            c=auxCell->column+1; 
            if(newgridTree[l][c]=='+') leftGrid[l][c]='X'; //nu le omoara doar le invie
            auxCell=auxCell->next;
        }
        copyGrid(leftGrid,&newgridTree,n,m);
        preorderBuildTreeGrid(foutput,rootTree->left,leftGrid,n,m,lvl+1);
        freespace(leftGrid,n);
    }

    if(rootTree->right!=NULL)
    {
        char **rightGrid=NULL;
        copyGrid(newgridTree,&rightGrid,n,m);
        Cell *auxCell=rootTree->right->changedcell;
        while(auxCell!=NULL)
        {
            l=auxCell->line+1; 
            c=auxCell->column+1; 
            if(newgridTree[l][c]=='X') rightGrid[l][c]='+';
            else
            if(newgridTree[l][c]=='+') rightGrid[l][c]='X';
            auxCell=auxCell->next;
        }
        copyGrid(rightGrid,&newgridTree,n,m);
        preorderBuildTreeGrid(foutput,rootTree->left,rightGrid,n,m,lvl+1);
        freespace(rightGrid,n);
    }

}
int howManyNodes(NodeGraph *graph)
{
    int N=0;
    NodeGraph *auxGraph=graph;
    while(auxGraph!=NULL)
    {
        N++;
        auxGraph=auxGraph->next;
    }
    return N;
}
void createMatrix(NodeGraph *graph,int ***adjmat,char **grid, int n, int m)
{
    //grid e ala din tree, adjmat e matricea mea semi adiacenta
    int i,j,N=0;
    NodeGraph *auxGraph=graph;
    while(auxGraph!=NULL)
    {
        N++;
        auxGraph=auxGraph->next;
    }
    //am aflat nr de noducri din graf
    //aloc memorie pt matrice
    *adjmat=(int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
    {
        (*adjmat)[i]=(int*)malloc((N)*sizeof(int));
    }

    //pun nodurile din graf intr-un vector de structuri
    Vector *auxVector=(Vector*)malloc(N*sizeof(Vector));
    auxGraph=graph;
    i=0;
    while(auxGraph!=NULL)
    {
        auxVector[i].line=auxGraph->changedcell->line;
        auxVector[i].column=auxGraph->changedcell->column;
        i++;
        auxGraph=auxGraph->next;
    }

    //parcurg vectorul si fac matricea
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        {
            (*adjmat)[i][j]=isEdge(auxVector[i].line,auxVector[i].column,auxVector[j].line,auxVector[j].column);
        }

}

void DFS_scan(NodeGraph *graph, int visited[],int i, Cell* changedcell, NodeStack** stackTop) {
    int V=howManyNodes(graph);
    int j;
    visited[i] = 1;
    Cell* auxCell=graph->changedcell;
    j=0; 
    while(graph!=NULL){
    if (graph->a[i][j] == 1 && visited[j] == 0)
        DFS_scan(graph, visited, j,auxCell,stackTop);

    j++;
    }

    push(stackTop,auxCell);
}
void writeStackGraph(FILE *foutput, NodeStack* stackTop)
{
    while(stackTop!=NULL)
    {
        Cell *cellHead=stackTop->changedcell;
        while(cellHead!=NULL)
        {
            fprintf(foutput,"(%d,%d)",cellHead->line, cellHead->column);
            cellHead=cellHead->next;
        }
        stackTop=stackTop->next;
    }
    fprintf(foutput,"\n");
}
void preorderTreeGraph(FILE* foutput, NodeTree* rootTree, char **newgridTree, int n, int m,int lvl,NodeGraph *graph,NodeStack **stackTop)
{
    int i,j,l,c;
    int N=howManyNodes(graph);
    if(rootTree==NULL) return;

    addToGraph(graph,newgridTree,n,m); 
    //creez matricea de adiacenta pt fiecare graf 
    int **adjmat;
    createMatrix(graph,&adjmat,newgridTree,n,m);

    graph->a=(int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
        graph->a[i]=(int*)malloc(N*sizeof(int));
    
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        graph->a[i][j]=adjmat[i][j];

    Cell* auxCell=graph->changedcell;
    int length=0;
    int *visited=(int*)malloc(N*sizeof(int));
    for(i=0;i<N;i++) visited[i]=0;

        for(i=0;i<N;i++){
            if(visited[i]==0)
            {
                DFS_scan(graph,visited,i,auxCell,&stackTop);
                length++;
            }
        }
        //afisez lantul 
    printf(foutput, "%d\n", length); writeStackGraph(foutput,stackTop);

    fprintf(foutput,"\n");//spatiu ca nu ii place fara
    if(rootTree->left!=NULL)
    {
        char **leftGrid=NULL;
        copyGrid(newgridTree,&leftGrid,n,m);
        Cell *auxCell=rootTree->left->changedcell;
        while(auxCell!=NULL)
        {
            l=auxCell->line+1; 
            c=auxCell->column+1; 
            if(newgridTree[l][c]=='+') leftGrid[l][c]='X'; //nu le omoara doar le invie
            auxCell=auxCell->next;
        }
        copyGrid(leftGrid,&newgridTree,n,m);
        preorderTreeGraph(foutput,rootTree->left,leftGrid,n,m,lvl+1,graph,stackTop);
        freespace(leftGrid,n);
    }

    if(rootTree->right!=NULL)
    {
        char **rightGrid=NULL;
        copyGrid(newgridTree,&rightGrid,n,m);
        Cell *auxCell=rootTree->right->changedcell;
        while(auxCell!=NULL)
        {
            l=auxCell->line+1; 
            c=auxCell->column+1; 
            if(newgridTree[l][c]=='X') rightGrid[l][c]='+';
            else
            if(newgridTree[l][c]=='+') rightGrid[l][c]='X';
            auxCell=auxCell->next;
        }
        copyGrid(rightGrid,&newgridTree,n,m);
        preorderTreeGraph(foutput,rootTree->left,rightGrid,n,m,lvl+1,graph,stackTop);
        freespace(rightGrid,n);
    }
}

int main(int argc, const char* argv[])
{    
   for(int i=1;i<argc;i++)
   {
    FILE *finput=fopen(argv[i],"r");

    char OutputFile[50];
    strcpy(OutputFile, argv[i]);
    char *dot = strrchr(OutputFile, '.');
        if(dot) {
            strcpy(dot, ".out");
    } else {
        strcat(OutputFile, ".out");
    }

    FILE *foutput=fopen(OutputFile,"w");

        if(finput==NULL)
    {
        puts("Fisierul input nu poate fi deschis!\n");
        exit(1);
    }
        if(foutput==NULL)
    {
        puts("Fisierul output nu poate fi deschis!\n");
        exit(1);
    }
    int n,m,k,task;
    //fac citirea aici ca fct reead face figuri
    fscanf(finput, "%d", &task); //citirea taskului 

    if(task==1)
    {
        fscanf(finput, "%d %d %d", &n,&m,&k);
        char **grid=NULL;
        reead(finput,&grid,n,m,k);
    
        for(int generation=0;generation<=k;generation++)
            {
                writee(foutput,grid,n,m);
                char **newgen=NULL;
                new_gen(grid,n,m,&newgen);

                freespace(grid,n);
                grid=newgen;
                fprintf(foutput,"\n");
            }
    freespace(grid,n);
    }

    if(task==2)
    {
        //taskul 2 lessgoo
        fscanf(finput, "%d %d %d", &n,&m,&k);
        char **grid=NULL;
        reead(finput,&grid,n,m,k);
        for(int generation=1;generation<=k;generation++)
    {
        char **newgen=NULL;

        NodeStack* stackTop=StackedCells(grid,n,m,&newgen);
        writeStack(foutput,stackTop,generation);

        freespace(grid,n);
        grid=newgen;
        newgen=NULL;

        deleteStack(&stackTop);
    }
    freespace(grid,n);

    }

    if(task==3)
    {
        //task3 da ma daa
        fscanf(finput, "%d %d %d", &n,&m,&k);
        char **grid=NULL;
        reead(finput,&grid,n,m,k);
        //for(int generation=1;generation<=k;generation++)
   // {
       // char **newgen=NULL,**oldGrid=NULL;
       // int lvl=0;

        NodeTree *rootTree=NULL;// Cell *changedcell=NULL;
        buildRootList(&rootTree,grid,n,m,/*generation*/k);
        preorderBuildTreeGrid(foutput,rootTree,/*rootTree->changedcell,*/grid,/*newgen,*/n,m,0);
   // }
    freespace(grid,n);
    }
    if(task==4)
    {
        //task 4 ish
        fscanf(finput, "%d %d %d", &n,&m,&k);
        char **grid=NULL;
        reead(finput,&grid,n,m,k);
        NodeGraph *graph=(NodeGraph*)malloc(sizeof(NodeGraph));
        NodeTree *rootTree=NULL;
        NodeStack *stackTop=NULL;
        buildRootList(&rootTree,grid,n,m,k);
        preorderTreeGraph(foutput,rootTree,grid,n,m,0,graph,stackTop);

        freespace(grid,n);
    }
    

    fclose(finput); 
    fclose(foutput);
    }
    return 0;
}
