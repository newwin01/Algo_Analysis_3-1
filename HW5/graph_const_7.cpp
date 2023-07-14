#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

/*
21900628 Algorithm analysis 21900628
References: Professor's code on week 10 class

C++ string: 
https://www.digitalocean.com/community/tutorials/string-uppercase-lowercase-c-plus-plus
Pair and vector
https://hydroponicglass.tistory.com/entry/STL-c-sort-%ED%99%9C%EC%9A%A9 

*/

using namespace std;

typedef struct g_node { // contents node
    char alphabet_point; // in this case only alphabet, but may have weight value later
    struct g_node *link;   	
} list_node;

typedef struct h_node { // header of adjacency list
    char alphabet;    
    struct g_node *link;   
} head_node;

int** FileToMatrix(string inputFileName, vector<char>* column, vector<char>* row){
	ifstream inputFile(inputFileName);
	string line;
	int node_count = 0;
	int size;
	int** matrix;
	int row_index = 0;
	int column_index = 0;

	getline(inputFile, line);

	for(int i = 0 ; i < line.length() ; i++){
		if(!isspace(line[i])){
			column->push_back(toupper(line[i]));
		}
	}

	size = column->size();
	if(size > 20 || size <= 0){
		cout << "input error\n" << endl;
		exit(1);
	}

	//initialize the matrix
	matrix = new int*[size];
	for(int i = 0; i < size ; i++){
		matrix[i] = new int[size];
	}

	while(getline(inputFile, line)){
		column_index = 0;
		for(int i = 0 ; i < line.length() ; i++){
			if(!isspace(line[i])){
				if(isalpha(line[i])){
					row->push_back(toupper(line[i]));
				} else {
					matrix[row_index][column_index] = line[i] - 48;
					column_index++;
				}
			}
		}
		row_index++;
	}

	#ifdef _DEBUG
		for(int i = 0 ; i < size ; i++){
			for(int j = 0 ; j < size ; j++){
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
	#endif

	return matrix;
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

bool compare(pair<char, int> a, pair<char, int> b) { //first is char
	return a.first < b.first;
}

void insert(head_node* linkedList, int* matrixRow, vector<char> row){
	//alphabet is in the linkedList
	int linkedListLength = 0;

	vector < pair<char, int> > insert;

	pair<char, int> p1;

	for(int i = 0 ; i < row.size() ; i++){
		insert.push_back(make_pair(row[i], matrixRow[i]));
		if(matrixRow[i]) linkedListLength++;
	}

	sort(insert.begin(), insert.end(), compare);

	#ifdef _DEBUG

		cout << linkedList -> alphabet << " ";

		for(auto i : insert){
			cout << i.first << i.second << " ";
		}
		cout << endl;

	#endif

	list_node *temp;
	list_node *prev;

	for (int i = 0 ; i < insert.size() ; i++) {
		if(insert[i].second == 1){
			temp = new list_node;
			temp->alphabet_point = insert[i].first;
			temp->link = NULL;

			insertNode(linkedList, temp);

		}
	}


}

head_node* create_linked_list(int** matrix, vector<char> column, vector<char> row){

	head_node* linkedList;
	int size = column.size(); //size of matrix
	char alpha = 'A';
	int index ; 

	linkedList = new head_node[size];

	for(int i = 0 ; i < size ; ){

		if(find(row.begin(), row.begin(), alpha) != row.end()){
			index = find(row.begin(), row.end(), alpha) - row.begin();

			linkedList[i].alphabet = alpha;
			linkedList[i].link = NULL;

			insert(&linkedList[i], matrix[index], row);

			i++;
		}	
		alpha++;
	}

	return linkedList;
}




head_node* transpose_Graph(head_node* graph, int row){
	head_node* t_graph;
	list_node* temp;
	list_node* new_node;
	list_node* t_cursor;

	t_graph = new head_node[row];

	for(int i = 0 ; i < row ; i++){
		t_graph[i].alphabet = graph[i].alphabet;
		t_graph[i].link = NULL;
	}

	char find_char;
	char new_char;

	for(int i = 0 ; i < row ; i++){
		find_char = t_graph[i].alphabet;
		t_cursor = t_graph[i].link;
		

		for(int j = 0 ; j < row ; j++){
			temp = graph[j].link;
			new_char = graph[j].alphabet;

			while(temp != NULL){
				if(temp->alphabet_point == find_char){
					new_node = new list_node;
					new_node->alphabet_point = new_char;
					new_node->link = NULL;

					insertNode(&t_graph[i], new_node);

				}
				temp = temp->link;
			}

		} //iterate for all nodes

	}
	
	return t_graph;
}

void Display_Lists(head_node *my_graph , int row){

	for (int i=0; i<row; i++){
		
		cout  << "From " << my_graph[i].alphabet << ": ";
		list_node *temp = my_graph[i].link;

		while (temp != NULL){
			if(temp->link != NULL) 
				cout << temp->alphabet_point <<  " -> ";
			else
				cout << temp->alphabet_point ;
			temp = temp->link;
		}
		cout << endl;
	}

	cout << endl << endl;

}


int main()
{
    head_node *my_graph;
	head_node *t_graph;
	int** matrix;

	vector<char> column;
	vector<char> row;

	string filename;
	filename = "hw5_data.txt";

	matrix = FileToMatrix(filename, &column, &row);

	my_graph = create_linked_list(matrix, column, row);

	Display_Lists(my_graph, row.size());
	
	t_graph = transpose_Graph(my_graph, row.size());

	Display_Lists(t_graph, row.size());


	//need to free
	for(int i = 0 ; i<column.size() ; i++){
		delete[] matrix[i];
		
	}
	delete[] matrix;

	list_node* temp = NULL;
	list_node* head = NULL;

	for(int i = 0 ; i<column.size() ; i++){
		head = my_graph[i].link;
		while (head != NULL){
			temp = head;
			head = head->link;
			free(temp);
		}
	}
	delete[] my_graph;

		for(int i = 0 ; i<column.size() ; i++){
		head = t_graph[i].link;
		while (head != NULL){
			temp = head;
			head = head->link;
			free(temp);
		}
	}
	delete[] t_graph;

}

