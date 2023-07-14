#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <iomanip>
#define INF 9999

/*
21900628 Algorithm analysis 
References: Last Homework for creating adjacency list

C++ ifstream: 
https://cypsw.tistory.com/5

Dijkstra algorithm
From Professor's PPT

Floyd algorithm
https://www.geeksforgeeks.org/floyd-warshall-algorithm-dp-16/

*/


using namespace std;

typedef struct g_node { // contents node
    string pointing_city; 
    int weight;
    struct g_node *link;   	
} list_node;

typedef struct h_node { // header of adjacency list
    string city;    
    struct g_node *link;   
} head_node;


class Parser{
    private:
        vector<string> words;
    public:
        Parser(string line);
        int getSize();
        string getWord(int index);
};

Parser::Parser(string line){
  
    string word;
    for(int i = 0 ; i < line.length() ; i++){
        if(isspace(line[i])) {
            words.push_back(word);
            #ifdef _DEBUG
                cout << word << " ";
            #endif
            word.clear();
        }
        else {
            word = word + line[i];
        }
    }
    #ifdef _DEBUG
        cout << endl;
    #endif
}

int Parser::getSize(){
    return words.size();
}

string Parser::getWord(int index) {
    return words.at(index);
}


void insertNode (head_node* listGraph, list_node* node){

	list_node* temp = listGraph->link;

	if(listGraph->link == NULL){
		listGraph->link = node;
		
	} else {
		while (temp->link != NULL){
			temp = temp->link;
		}
		temp -> link = node;
	}
}


head_node* create_linked_list(string file_name, int *number){

    //read the file
    ifstream inputFile(file_name);

    string line;
    head_node* linkedList;

    vector <string> city_mapping;
    int index = 0;
    int i = 0;
    string temp;

    while(getline(inputFile, line)) { // until the end of the file

        while(!isspace(line[i])){
            temp = temp + line[i];
            i++;
        }
        
        city_mapping.push_back(temp);


        #ifdef _DEBUG
            cout << city_mapping.at(index) << endl;
        #endif

        i = 0;
        temp.clear();

        index++;
    }

    *number = index;

    inputFile.clear(); 
    inputFile.seekg(0, ios::beg);

	linkedList = new head_node[index];

    index = 0;
    int converted_int = 0;

	list_node *new_node; 
	
    while(getline(inputFile, line)) {
        
        Parser *parser = new Parser(line);
        
        temp = parser->getWord(i);
        
        i++;

        linkedList[index].city = temp;
        linkedList[index].link = NULL;

        for (i = 1; i < parser->getSize() ; i++){
            
            temp = parser->getWord(i);

            if ( temp == "INF"){
				continue;
			} else {
				converted_int = stoi(temp);
                if(converted_int != 0) {
                    new_node = new list_node;
                    new_node->pointing_city = city_mapping.at(i-1);
                    new_node->weight = converted_int;
                    new_node->link = NULL;
                    insertNode(&linkedList[index], new_node); 
                }
			}
        }

        i = 0;
        index++;
        delete parser;
    }
    
	return linkedList;
}

void Display_Lists(head_node *my_graph , int row){

	for (int i=0; i < row; i++){

		cout  << "From " << my_graph[i].city << ": ";
		list_node *temp = my_graph[i].link;

		while (temp != NULL){
			if(temp->link != NULL) 
				cout << temp->pointing_city << " " << temp->weight <<  " -> ";
			else
				cout << temp->pointing_city << " " << temp->weight;
			temp = temp->link;
		}
		cout << endl;

	}
	cout << endl << endl;
}
vector<string> get_cities(head_node* ori_graph, int number){

    vector<string> cities;

    for (int i = 0 ; i < number ; i++){
        cities.push_back(ori_graph[i].city);
    }

    return cities;
}

typedef struct {
    int weight;
    string city;
} vertex;

bool operator<(const vertex &v1, const vertex &v2){
    return v1.weight < v2.weight;
}

//Dijkstra algorithm
class Digraph{
    private:
        vector <vertex> pq;
        head_node* graph;
        int vertex_number;
        vector <vertex> result;
    public:
        Digraph(head_node* graph, int number);
        void select_source(string city); //build pq
        // ~Digraph();
        vector<vertex>* getPath();
};

Digraph::Digraph(head_node* ori_graph, int number) {

    vertex_number = number;
    graph = ori_graph;
}

void Digraph::select_source(string city){

    vertex new_vertex;

    for(int i = 0 ; i < vertex_number ; i++){

        new_vertex.city = graph[i].city;

        if(new_vertex.city == city) {            
            new_vertex.weight = 0;
        } else {
            new_vertex.weight = INF;
        }
        pq.push_back(new_vertex);
    }


}

int findCityIndex(head_node* graph, string city, int num){

    for(int i = 0 ; i < num ; i++){
        if(city == graph[i].city){
            return i;
        }
    }

    return -1;

}

vector<vertex>* Digraph::getPath(){

    vertex temp;
    int index;
    list_node* temp_node;

    while(!pq.empty())
    {
        sort(pq.begin(), pq.end());
        temp = pq.front(); //vertex will inserted in the set
        result.push_back(temp);
        pq.erase(pq.begin());

        #ifdef _DEBUG
            cout << temp.city << " " << temp.weight << endl;
        #endif

        index = findCityIndex(graph, temp.city, vertex_number);

        temp_node = graph[index].link;

            while( temp_node != NULL ){
                for (int i = 0 ; i < pq.size() ; i++){
                    if( pq.at(i).city == temp_node->pointing_city ){
                        if ( temp_node->weight < 0) {
                            cout << "negative edge found" << endl;
                            return NULL;
                        }

                        if ( pq.at(i).weight > temp.weight + temp_node->weight) {
                            pq.at(i).weight = temp.weight + temp_node->weight;
                        }
                    }
                }
                temp_node = temp_node -> link;
            }
        
    }
    return &result;
}

class Floyd{
    private:
        int** matrix;
        vector<string> cities;
        head_node* ad_list;
        int num_cities;
    public:
        Floyd(head_node* ad_list, vector<string> cities);
        int** getFloyd();
};

Floyd::Floyd(head_node* ad_list, vector<string> cities){

    this->ad_list = ad_list;
    this->cities = cities;
    num_cities = cities.size();

    matrix = new int*[num_cities];
    for (int i = 0 ; i < num_cities ; i++){
        matrix[i] = new int[num_cities];
    }

    list_node *temp;
    int idx;

    //build first matrix / adjacency to matrix
    for (int i = 0 ; i < num_cities ; i++) {

        temp = ad_list[i].link;

        for (int j = 0 ; j < num_cities ; j ++) {
            matrix[i][j] = INF;
        }

        for (int j = 0 ; j < num_cities ; j ++) {
            if ( ad_list[i].city == cities.at(j) ) {
                idx = j;
                break;
            }
        }

        matrix[i][idx] = 0;

        while ( temp != NULL) {

             for (int j = 0 ; j < num_cities ; j++) {

                if ( temp->pointing_city == cities.at(j) ){
                    idx = j;
                    break;
                }
            }

            if (idx != -1) {
                matrix[i][idx] = temp -> weight;
            }

            temp = temp -> link;
        }
    }

    #ifdef _DEBUG
        for (int i = 0 ; i < num_cities ; i++) {
            for (int j = 0 ; j < num_cities ; j++) {
                cout << matrix[i][j] << "\t" ;
            }
            cout << endl;
        }
    #endif

}

int** Floyd::getFloyd(){

    //Odd trials

    for (int i = 0; i < num_cities; i++) {

        for (int j = 0; j < num_cities; j++) {

            for (int k = 0; k < num_cities; k++) {

                if (matrix[j][k] > (matrix[j][i] + matrix[i][k]) && (matrix[i][k] != INF && matrix[j][i] != INF))
                    matrix[j][k] = matrix[j][i] + matrix[i][k];
            }
        }
    }

    return matrix;
}


int main() 
{

    head_node *my_graph;
    vector <vector <vertex> > result_Dijk;

    string filename;
	filename = "hw6-1.txt";

    int number;

    my_graph = create_linked_list(filename, &number);
    vector <string> cities = get_cities(my_graph, number);


    //Display lists
    Display_Lists(my_graph, number);
    vector<vertex> *temp;
    bool neg_or_pos = true;

    for (int i = 0 ; i < number ; i++){
        Digraph *digraph = new Digraph(my_graph, number);
        digraph->select_source(my_graph[i].city);
        if ( (temp = (digraph->getPath())) == NULL) {
            neg_or_pos = false;
            delete digraph;
            break;
        }
        result_Dijk.push_back( *temp );
    }

    //dijkstra algorithm
    if (neg_or_pos == true){

        int index;

        cout << "The followings are shortest distances between cities with Dijkstra's algorithm." << endl << "\t  ";


        for (int i = 0 ; i < result_Dijk.size() ; i++){
            cout << setw(7) << cities.at(i) << "|";
        }
        cout << endl;
            
        for (int i = 0 ; i < result_Dijk.size() ; i++){

            
        cout << setw(10) << result_Dijk.at(i).at(0).city ;

            for (int j = 0 ; j < result_Dijk.at(i).size() ; j++){
                for(int k = 0 ; k < cities.size() ; k++){
                    if(cities.at(j) == result_Dijk.at(i).at(k).city) {
                        index = k;
                        break;
                    }
                }
                cout << setw(8) << result_Dijk.at(i).at(index).weight ;
            }
            cout << endl;
        }
    }

    cout << endl;

    int **matrix;

    Floyd *floyd = new Floyd(my_graph, cities);
    matrix = floyd->getFloyd();

    cout << "The followings are shortest distances between cities with Floyd's algorithm." << endl << "\t  ";
       for (int i = 0 ; i < cities.size() ; i++){
            cout << setw(7) << cities.at(i) << "|";
        }
    cout << endl;
    
    for (int i = 0 ; i < number ; i++) {
        cout << setw(10) << cities.at(i);
        for (int j = 0 ; j < number ; j++) {
            cout << setw(8) << matrix[i][j] ;
        }
        cout << endl;
    }
    
}
