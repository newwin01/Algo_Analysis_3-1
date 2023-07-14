#include <iostream>
#include <fstream>

#define FILE "data.txt"
const int num_nodes = 5;
using namespace std;

typedef struct g_node{
    int distance;
    int point;
    g_node *link;
} link_node;

typedef struct h_node {
    int data;
    link_node *link;
} head_node;




void Read_from_file(int distance[][num_nodes]){
    ifstream inputFile(FILE);

    if(inputFile.is_open()){
        for(int i=0;i<num_nodes;i++){
            for(int j=0;j<num_nodes;j++){
                inputFile >> distance[i][j];
                cout << distance[i][j] << '\t' ;
            }
            cout << endl;
        }
    }
}

void createLinkedList(head_node** graph){
    *graph = new head_node[num_nodes];

    for(int i=0;i<num_nodes;i++){ //initialize the arrays
        // graph[i]->data=i;
        // graph[i]->link=NULL;

        (*graph)[i].data=i;
        (*graph)[i].link=NULL;
    }
}

void printList(head_node *graph){

    for(int i=0;i<num_nodes;i++){
        cout << graph[i].data << ",";
        cout << graph[i].link << endl;
    }
}

void displayList(head_node *graph){
    link_node *cursor;
    for(int i=0;i<num_nodes;i++){
        
        while()
        
    }
}

void insertGraph(head_node *graph, int distance[][num_nodes]){

    link_node *temp;
    link_node *cursor;

    for(int i=0;i<num_nodes;i++){
        for(int j=0;j<num_nodes;j++){
            if(distance[i][j] > 0){     
                temp = new g_node;

                temp->distance = distance[i][j];
                temp->link = NULL;
                temp->point = j;

                cursor = graph[i].link;

                if(cursor == NULL){
                    graph[i].link = temp;

                } else {
                    g_node *prev = NULL;

                    // while(cursor!=NULL){ //professor version
                    //     prev = cursor;
                    //     cursor = cursor -> link;
                    // }
                    // prev->link = temp;

                    while (cursor->link != NULL){ //my version
                        cursor = cursor->link;
                    }
                    cursor -> link = temp;

                }
            }
        }
    }
}

int main()
{
    
    int distance[num_nodes][num_nodes]; //array
    Read_from_file(distance);

    head_node *graph = NULL;

    createLinkedList(&graph);
    printList(graph);

    
    insertGraph(graph, distance);
    printList(graph);

    
    cout << endl;
}