# GameOfLife

Asta e implementarea mea pentru Game of Life. 

Taskul 1: am folosit matrici ca sa simulez schimbarile unei celule in GoL

Taskul 2: am pus coordonatele celulelor care se schimba intr-o stiva 

Taskul 3: am creat un arbore. fiecare arbore retine in nod o stiva de liste cu celulele care s au schimbat. fiecare subarbore se schimba dupa o anumita regula (doi vecini vii sau standard). am parcurs arborele in preordine. pentru fiacrea subarbore, in functie de celulele modificate, cream matricea cu starea fiecarei celule si o afisam.

Taskul 4: am creat un graf ale carui noduri sunt celulele vii din fiecare matrice afisata la taskul 3. am folosit functiile din task3 pentru a crea nodul. cu ajutorul matricei de adiacenta, am incercat sa o sortare topologica cu DFS ca sa aflu cel mai lung lant hamiltonian din graf

[Pt detalii](https://site-pa.netlify.app/proiecte/game_of_life/) 

## Cum se executa: 
- gcc gameoflife.c
- make clean
- make
- Datele de intrare si de iesire se gasesc in folderul 'InputData'
- Datele de referinta se gasesc in folderul 'RefData'




