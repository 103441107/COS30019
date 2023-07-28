#include "Grid.h"
/**
 * Constructor
 * @param a No. of rows
 * @param b No.of Columns
*/
Grid::Grid(int a,int b)
{
    N=a;
    M=b;
    G = new Node*[N];
    for(int i = 0; i < N; ++i){
        G[i] = new Node[M];
        // #pragma omp parallel for
        for(int j = 0; j < M; ++j) {
            G[i][j].x=i;
            G[i][j].y=j;
        }
    }   
}

/**
 * Prints out the board onto the terminal
*/
void Grid::p()
{
    for(int i=0 ;i<N ;i++)
    {
        for(int j=0 ;j<M;j++)cout<<G[i][j].col<<" ";
        cout<<endl;
    }
}

/**
 * Helper function to check whether a cell exist 
 * or is not a wall
 * or has not been visited before
 * @param x x-coordinate / row number
 * @param y y-coordinate / column number
 * @return true if cell is valid false otherwise
*/
bool Grid::is_Valid(int x,int y)
{
    if(x<0)return false;
    if(x>=N) return false;
    if(y<0)return false;
    if(y>=M)return false;
    if(G[x][y].col=='W') return false;
    if(G[x][y].visit == true) return false;
    return true;
}

/**
 * Helper function to check whether a cell exist 
 * or is not a wall
 * or can be relaxed for djikstra's algorithm
 * @param x x-coordinate / row number
 * @param y y-coordinate / column number
 * @return true if cell is valid false otherwise
*/
bool Grid::is_Valid_1(int x,int y,Node * curr)
{
    if(x<0)return false;
    if(x>=N) return false;
    if(y<0)return false;
    if(y>=M)return false;
    if(G[x][y].col=='W') return false;
    if(G[x][y].dist <= curr->dist+1) return false;
    return true;
}

/**
 * Resets the visit values of all nodes to false
*/
void Grid::reset_visit(){
    // #pragma omp parallel for
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++){
            G[i][j].visit=false;
            G[i][j].cost=-1;
            G[i][j].dist=-1;
            G[i][j].prev=NULL;
        }
    }
}

/**
 * Estimates the cost to travel to destination from source
 * @param src  Source node 
 * @param dest Destination node
*/
int Grid::cost(Node * src, Node * dest)
{
    return abs(src->x - dest->x) + abs(src->y - dest->y);
}

/**
 * Prints out a path from source to destination
 * @param tail Node pointer to dest which can be traversed reversely to reach source
 * @param path String to store the path
*/
void Grid::print_path(Node * tail,stringstream & S)
{
    if(tail->prev==NULL) return; 
    print_path(tail->prev, S);
    int dx = tail->prev->x - tail->x ;
    int dy = tail->prev->y - tail->y ;

    if(tail->prev->col=='R')S << "Start : ( "<<tail->prev->x<<", "<<tail->prev->y<<")\t ";
    

    if(dx == -1) S << "down; ";
    else if(dx == 1) S <<"up; ";
    else if (dy == 1) S << "left; ";
    else if (dy == -1) S << "right; ";

    if(tail->col=='G')S << "Goal : ( "<<tail->x<<", "<<tail->y<<") ";
}

/*
 Depth First Search Algorithm
*/
void Grid::DFS(Node * curr)
{
    curr->visit = true;
    if(curr->col == 'G')
    {
        stringstream path;
        print_path(curr,path);
        cout<<path.str()<<endl;
        return;
    } 
    
    if(is_Valid(curr->x-1,curr->y)) {

        G[curr->x-1][curr->y].prev = curr;
        DFS(&G[curr->x-1][curr->y]);
        G[curr->x-1][curr->y].prev = NULL;
    }
    if(is_Valid(curr->x,curr->y-1)){

        G[curr->x][curr->y-1].prev = curr;
        DFS(&G[curr->x][curr->y-1]);
        G[curr->x][curr->y-1].prev = NULL;
    }
    if(is_Valid(curr->x+1,curr->y)){

        G[curr->x+1][curr->y].prev = curr;
        DFS(&G[curr->x+1][curr->y]);
        G[curr->x+1][curr->y].prev = NULL;
    }
    if(is_Valid(curr->x,curr->y+1)){

        G[curr->x][curr->y+1].prev = curr;
        DFS(&G[curr->x][curr->y+1]);
        G[curr->x][curr->y+1].prev = NULL;
    }
    curr->visit = false;
}

/*
 Breadth First Search Algorithm
*/
void Grid::BFS(Node * curr)
{
    queue<Node *> Search;
    curr->visit = true;
    Search.push(curr);
    
    while(!Search.empty())
    {
        Node * temp = Search.front();
        Search.pop();

        if(temp->col == 'G')
        {
            stringstream path;
            print_path(temp,path);
            cout<<path.str()<<endl;
        }
        if(is_Valid(temp->x-1,temp->y))
        {
            G[temp->x-1][temp->y].prev = temp;
            G[temp->x-1][temp->y].visit = true;
            Search.push(&G[temp->x-1][temp->y]);
        }
        if(is_Valid(temp->x,temp->y-1))
        {
            G[temp->x][temp->y-1].prev = temp;
            G[temp->x][temp->y-1].visit = true;
            Search.push(&G[temp->x][temp->y-1]);
        }
        if(is_Valid(temp->x+1,temp->y))
        {
            G[temp->x+1][temp->y].prev = temp;
            G[temp->x+1][temp->y].visit = true;
            Search.push(&G[temp->x+1][temp->y]);
        }
        if(is_Valid(temp->x,temp->y+1))
        {
            G[temp->x][temp->y+1].prev = temp;
            G[temp->x][temp->y+1].visit = true;
            Search.push(&G[temp->x][temp->y+1]);
        }

    }
}

//Comparator for the priority queue for GBFS
struct compare
{
    bool operator()(const Node * a, const Node * b)
    {
        return a->cost > b->cost;
    }
};

/*
 Greedy Best First Search Algorithm
*/
void Grid::GBFS(Node* src, Node* dest)
{
    priority_queue<Node*, vector<Node*>, compare > pq;
    src->cost = cost(src,dest);
    pq.push(src);
    src->visit =true;
    
 
    while (!pq.empty()) {
        Node* temp = pq.top();
        pq.pop();
        
        if(temp->x==dest->x && temp->y == dest->y)
        {
            stringstream path;
            print_path(temp,path);
            cout<<path.str()<<endl;
            break;
        }
        int tx=temp->x;
        int ty=temp->y;

        if(is_Valid(tx-1,ty))
        {
            G[tx-1][ty].prev = temp;
            G[tx-1][ty].cost = cost(&G[tx-1][ty],dest);
            G[tx-1][ty].visit = true;
            pq.push(&G[tx-1][ty]);
        }
        if(is_Valid(tx,ty-1))
        {
            G[tx][ty-1].prev = temp;
            G[tx][ty-1].cost = cost(&G[tx][ty-1],dest);
            G[tx][ty-1].visit = true;
            pq.push(&G[tx][ty-1]);
        }
        if(is_Valid(tx+1,ty))
        {
            G[tx+1][ty].prev = temp;
            G[tx+1][ty].cost = cost(&G[tx+1][ty],dest);
            G[tx+1][ty].visit = true;
            pq.push(&G[tx+1][ty]);
        }
        if(is_Valid(tx,ty+1))
        {
            G[tx][ty+1].prev = temp;
            G[tx][ty+1].cost = cost(&G[tx][ty+1],dest);
            G[tx][ty+1].visit = true;
            pq.push(&G[tx][ty+1]);
        }
    }    
}

//Comparator for the priority queue for AS
struct compare_1
{
    bool operator()(const Node * a, const Node * b)
    {
        return (a->cost + a->dist) > (b->cost + b->dist);
    }
};

/*A* search Algortihm
*/
void Grid::AS(Node* src, Node* dest)
{

    priority_queue<Node*, vector<Node*>, compare_1> pq;

    src->cost = cost(src,dest);
    src->dist = 0;
    src->visit =true;
    pq.push(src);
    
    
    while (!pq.empty()) {
        Node* temp = pq.top();
        pq.pop();
        
        if(temp->x==dest->x && temp->y == dest->y)
        {
            stringstream path;
            print_path(temp,path);
            cout<<path.str()<<endl;
            break;
        }
        int tx=temp->x;
        int ty=temp->y;

        if(is_Valid(tx-1,ty))
        {
            G[tx-1][ty].prev = temp;
            G[tx-1][ty].dist = temp->dist + 1;
            G[tx-1][ty].cost = cost(&G[tx-1][ty],dest);
            G[tx-1][ty].visit = true;
            pq.push(&G[tx-1][ty]);
        }
        if(is_Valid(tx,ty-1))
        {
            G[tx][ty-1].prev = temp;
            G[tx][ty-1].dist = temp->dist + 1;
            G[tx][ty-1].cost = cost(&G[tx][ty-1],dest);
            G[tx][ty-1].visit = true;
            pq.push(&G[tx][ty-1]);
        }
        if(is_Valid(tx+1,ty))
        {
            G[tx+1][ty].prev = temp;
            G[tx+1][ty].dist = temp->dist + 1;
            G[tx+1][ty].cost = cost(&G[tx+1][ty],dest);
            G[tx+1][ty].visit = true;
            pq.push(&G[tx+1][ty]);
        }
        if(is_Valid(tx,ty+1))
        {
            G[tx][ty+1].prev = temp;
            G[tx][ty+1].dist = temp->dist + 1;
            G[tx][ty+1].cost = cost(&G[tx][ty+1],dest);
            G[tx][ty+1].visit = true;
            pq.push(&G[tx][ty+1]);
        }
    }    
}