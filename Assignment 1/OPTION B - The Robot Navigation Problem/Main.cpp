#include<iostream>
#include<stdio.h>
#include<vector>
#include "Node.h"
#include "Grid.h"
#include "File.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc<3)
    {
        printf("Usage : %s <filename> <method>\n",argv[0]);
        return 0;
    }
    string method = argv[2];

    Grid * Gd = parse_file(argv[1]);
    Gd->p();
    Gd->G[Gd->Start.x][Gd->Start.y].dist = 0;

    if (method == "GBFS") {

        cout << "Greedy Best First Search\n";
        for (int i = 0; i < Gd->Goals.size(); i++)
        {
            Gd->GBFS(&(Gd->Start), &(Gd->Goals[i]));
            Gd->reset_visit();
        }
    }
    else if (method == "BFS") {
        cout << "Breadth First Search\n";
        Gd->BFS(&(Gd->Start));
    }
    else if(method == "DFS") {
        cout<<"Depth First Search\n";
        Gd->G[Gd->Start.x][Gd->Start.y].visit=true;
        Gd->DFS(&(Gd->Start));
    }
    else if(method == "AS"){
        cout<<"A* Search\n";
        for(int i=0;i<Gd->Goals.size();i++)
        {
            Gd->AS(&(Gd->Start),&(Gd->Goals[i]));
            Gd->reset_visit();
        }
    }
    return 0;
}
}