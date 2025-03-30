#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *finput, *foutput;
void reead(char ***grid, int *n, int *m, int *k)
{

    int i,j;

    int task; fscanf(finput, "%d", &task); //citirea taskului 
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
void writee(char **grid, int n, int m)
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
void new_gen(char **grid, int n, int m, char ***newgen)
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
    //parcurg veche generatie si salvez modificarile in noua generatie
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            alive=alive_neighbours(grid,i,j);
            if((grid)[i][j]=='X')
            {
                if(alive<2 || alive>3) (*newgen)[i][j]='+';
                else
                    if(alive==2 || alive==3) 
                        (*newgen)[i][j]='X';
            }else
                if(alive==3)(*newgen)[i][j]='X';
                    else (*newgen)[i][j]=(grid)[i][j];
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
    int i,*n,*m,*k,generation;
    char **grid=NULL,**newgen=NULL;
    n=(int*)malloc(sizeof(int));
    m=(int*)malloc(sizeof(int));
    k=(int*)malloc(sizeof(int));
    for(i=1;i<argc;i+=2)
    {
        finput=fopen(argv[i],"r");
        foutput=fopen(argv[i+1],"w");
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

    reead(&grid,n,m,k);
    
        for(generation=0;generation<*k;generation++)
            {
                new_gen(grid,*n,*m,&newgen);
                writee(newgen,*n,*m);
                swap_gen(&grid,&newgen);
                //freespace(newgen,*n);
            }

    writee(grid,*n,*m);
    freespace(grid,*n);
    
    

    fclose(finput); 
    fclose(foutput);

    }
    free(n); free(m); free(k);
    return 0;
}