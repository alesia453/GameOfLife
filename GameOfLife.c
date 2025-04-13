#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord{
    int line, column;
};
typedef struct Coord Cell;

struct Elem{
    int index;
    Cell *change;
};
typedef struct Elem Generation;

void reead(FILE *finput, char ***grid, int *n, int *m, int *k)
{

    int i,j;
    int task; 
    
    fscanf(finput, "%d", &task); //citirea taskului 
    fscanf(finput, "%d %d %d", n,m,k);
    //fac un border si ii aloc spatiu
    int bordern, borderm;
    bordern=(*n)+2;
    borderm=(*m)+2;

    *grid=(char**)malloc(bordern * sizeof(char*));
    for(i=0;i<bordern;i++)
    {   
        (*grid)[i]=(char*)malloc((borderm+1) * sizeof(char));
    }
    //initializez borderul
    for(i=0;i<bordern;i++)
        for(j=0;j<borderm;j++)
        {
            (*grid)[i][j]='+';
            (*grid)[0][j]='?';
            (*grid)[bordern-1][j]='?';
            (*grid)[i][0]='?';
            (*grid)[i][borderm-1]='?';
        }
    
    //citesc restul valorilor in interiorul borderului

    for(i=1;i<=*n;i++)
        for(j=1;j<=*m;j++)
    {   
        fscanf(finput, " %c", &((*grid)[i][j]));
    }
    
}
void writee(FILE *foutput,char **grid, int n, int m)
{
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=m;j++){
            //printf("%c",grid[i][j]);
            fprintf(foutput, "%c", grid[i][j]);
        }
        //printf("\n");
        fprintf(foutput,"\n");
    }
}
void WriteStack(FILE *foutput, const Generation *stack, int k)
{
    int i,j;
    for(i=0;i<k;i++)
    {
        fprintf(foutput, "%d: ",i+1);
        for(j=0;j<stack[i].index;j++)
        {
            fprintf(foutput, " (%d,%d)", stack[i].change[j].line,stack[i].change[j].column);
        }
        fprintf(foutput,"\n");    
    }
}
void freespace( char **grid, int n)
{
    int i,bordern;
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
void new_gen(char **grid, int n, int m, char ***newgen, Generation *stack, int GenIndex)
{
    int i, j, alive;
    //aloc memorie pt noua generatie cu tot cu border
    int bordern, borderm;
    bordern=(n)+2;
    borderm=(m)+2;

    *newgen=(char**)malloc(bordern * sizeof(char*));
    for(i=0;i<bordern;i++)
    {   
        (*newgen)[i]=(char*)malloc((borderm + 1) * sizeof(char));
    }
    //initializez borderul
    for(i=0;i<bordern;i++)
        for(j=0;j<borderm;j++)
        {
            (*newgen)[i][j]='+';
            (*newgen)[0][j]='?';
            (*newgen)[bordern-1][j]='?';
            (*newgen)[i][0]='?';
            (*newgen)[i][borderm-1]='?';
        }

    //initializez elementele stivei
    stack[GenIndex].index=0;
    stack[GenIndex].change=NULL;

    //parcurg veche generatie si salvez modificarile in noua generatie
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            alive=alive_neighbours(grid,i,j);
            char new_state=grid[i][j];

            if((grid)[i][j]=='X')
            {
                if(alive<2 || alive>3) new_state='+';
                else
                   // if(alive==2 || alive==3) 
                        new_state='X';
            }else
                if(alive==3) new_state='X';
                    else new_state=(grid)[i][j];
            

            (*newgen)[i][j]=new_state;

            //verififc daca starea celulei s a schimbat de la generatia anterioara
            if(new_state != grid[i][j])
            {
                stack[GenIndex].index++; //numar cate celule s au schimbat
                stack[GenIndex].change=(Cell*)realloc(stack[GenIndex].change, stack[GenIndex].index*sizeof(Cell)); //aloc spatiu pt inca o celula
                //salvez coordonatele celulei schimbate si pt ca am bordat matricea initiala, scad 1
                int coordinate=stack[GenIndex].index - 1;
                stack[GenIndex].change[coordinate].line=i-1;
                stack[GenIndex].change[coordinate].column=j-1;
            }
        }

}
void swap_gen(char ***grid, char ***newgen)
{
    //swap la generatii astfel incat cea mai noua generatie sa fie luata ca referinta
    char **aux;
    aux=*grid;
    *grid=*newgen;
    *newgen=aux;
}

int main(int argc, const char* argv[])
{
   
    int *n,*m,*k,generation,i,j;
    char **grid=NULL,**newgen=NULL;
    n=(int*)malloc(sizeof(int));
    m=(int*)malloc(sizeof(int));
    k=(int*)malloc(sizeof(int));

    for(i=1;i<argc;i+=2)
   {
    FILE *finput=fopen(argv[i],"r");
    FILE *foutput=fopen(argv[i+1],"w");
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

    reead(finput,&grid,n,m,k);
    
    //aloc memorie pt stack
    Generation *stack=(Generation *)malloc((*k)*sizeof(Generation));

        for(generation=0;generation<*k;generation++)
            {
                new_gen(grid,*n,*m,&newgen,stack,generation);
                //writee(foutput,newgen,*n,*m);
                swap_gen(&grid,&newgen);
                freespace(newgen,*n);
            }

    writee(foutput,grid,*n,*m);
    WriteStack(foutput,stack,*k);
    //eliberez memoria din stack
    for(j=0;j<*k;j++)
    {
        free(stack[j].change);
    }
    free(stack);

    freespace(grid,*n);
    
    
    fclose(finput); 
    fclose(foutput);

    }
    free(n); free(m); free(k);
    return 0;
}