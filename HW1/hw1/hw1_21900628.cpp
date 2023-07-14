/*
Author: Jang Sechang
Class: Algorithm analysis
HW 1

referecne: https://www.baeldung.com/cs/min-heaps-decrease-key
Remind of overall algorithm
*/

#include <iostream>
#include <string>
#define MAX_SIZE 31
using namespace std;

//Element class having name, key, and school. 
class Element{
    private:
        string name;
        int key;
        string school;
    public:
        void insertName(string name);
        void insertKey(int key);
        void insertSchool(string school);
        string getName();
        int getKey();
        string getSchool();
};
//assign name for the object
void Element::insertName(string name){
    this->name.assign(name);
}
//assign key for the object
void Element::insertKey(int key){
    this->key = key;
}
//assign school for the object
void Element::insertSchool(string school){
    this->school.assign(school);
}
//Get key info of the Element clas
int Element::getKey(){
    return key;
}
//Get name info of the Element clas
string Element::getName(){
    return name;
}
//Get School info of the Element clas
string Element::getSchool(){
    return school;
}
//Implement mean heap
class MinHeap{
    private:
        Element heap[MAX_SIZE];
        int index;
    public:
        MinHeap(); //constructor
        ~MinHeap(); //destructor
        Element* getHeap() {return heap;};
        int getIndex() {return index;}
        int useIndex() {return ++index;}
        int decreaseIndex() {return index--;}
        Element getElement(int i) {return heap[i];}
        bool IsEmpty() {return (index==0);}; //check whether it is empty or not
        bool IsFull() {return (index==MAX_SIZE-1);}; //check whether it is full or not
        void print();
        // void swap(Element *a, Element *b);
};
//Set index as 0
MinHeap::MinHeap(){
    index = 0;
}
//Set index as 0 to destruct
MinHeap::~MinHeap(){
    index = 0;
}

// swap the element value 
void swap(Element *a,Element *b){
    Element temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void MinHeap::print(){ //Print the heap info in following form 
    for(int i=1;i<=index;i++){
        cout << "[" << heap[i].getName() << ", " << heap[i].getKey() << ", " << heap[i].getSchool() << "] " ;
    }
}

int checkId(string id){ //Check the validity of the id. It takes unusual cases away and check whether it is 4 digit. 
    string temp;
    int position = 1;
    int real_id = 0;
    for(int i=0;i<id.length();i++){
        if(id[i]>='0'&& id[i]<='9'){
            temp+=id[i];
        }
        if(id[i]=='o'||id[i]=='O'){
            temp+='0';
        }
    }
    if(temp.length()!=4) return -1;
    if(temp[0]=='0') return -1;
    for(int i = temp.length()-1;i>=0;i--){
        real_id = real_id + ((temp[i]-48) * (position));
        position *= 10;
    }

    return real_id;
}

string trim(string line){ //Take space out. Question: What if name has space between alphabet letter?
    string trimed;

    for(int i=0;i<line.length();i++){
        if(line[i]!=' '){
            trimed += line[i];
        }
    }

    return trimed;
}

MinHeap insert(MinHeap heap, Element item){
    int i = heap.useIndex(); //get index and increment the index value
    
    while((i!=1)&&(item.getKey()<heap.getHeap()[i/2].getKey())){ //insert the value comparing the its node
        heap.getHeap()[i] = heap.getHeap()[i/2];
        i/=2;
    }
    heap.getHeap()[i]=item;
    
    cout << "New element [" << item.getName() << ", " << item.getKey() << ", " << item.getSchool() << "] is inserted" << endl;
    return heap;
}


MinHeap decrease_Key(MinHeap heap, int id_value, int key){
    int temp = heap.getHeap()[id_value].getKey(); // Check it is increasing or decreasing
    int i = heap.getIndex();
    Element item;
    if(temp<key){
        cout << "You can't increase the key" << endl << endl;
        return heap;
    }
    else{ 
        //Swap the first one and last one, use insertion algorithm
        heap.getHeap()[id_value].insertKey(key);
        item = heap.getHeap()[id_value];
        swap(&heap.getHeap()[id_value],&heap.getHeap()[i]);
        while((i!=1)&&(item.getKey()<heap.getHeap()[i/2].getKey())){
            heap.getHeap()[i] = heap.getHeap()[i/2];
            i/=2;
        }
        heap.getHeap()[i]=item;
        return heap;
    }
}

Element minimun(MinHeap *heap){ //Get the root value which is the minimum 
    return heap->getHeap()[1];
}

Element deleteHeap(MinHeap *heap){
    Element smallest = minimun(heap);
    int parent, smaller;//Assume the root is the one will be deleted firstly. 
    int index = heap->decreaseIndex();
    heap->getHeap()[1] = heap->getHeap()[index]; //decrease the index
    parent = 1;
    while (parent*2<=index){ //while there is no anymore leaf
        if(parent*2+1<=index){ //if there is leaf
            if(heap->getHeap()[parent*2].getKey()<heap->getHeap()[parent*2+1].getKey()) // consider whether leaf is smaller or bigger
                smaller = parent*2;
            else
                smaller = parent*2+1;
        }
        else
            smaller = parent*2;   
        if(heap->getHeap()[parent].getKey()<=heap->getHeap()[smaller].getKey()) //if the structure is completely formed
            break;
        swap(&heap->getHeap()[parent],&heap->getHeap()[smaller]); //swap the value
        parent = smaller; //set parent the small one
    }

    return smallest;
}




int main(){
    Element item;

    string key; //keyboard input

    //for insertion
    string name;
    string school;
    string temp_id;
    int id;

    //to choice index
    string string_index;

    // Heap
    int index = MAX_SIZE; //index
    MinHeap minHeap;

    while(1){
        cout << "*********** MENU ****************" << endl;
        cout << "I : Insert new element into queue." << endl;
        cout << "C : Decrease key of element in queue" << endl;
        cout << "D : Delete element with smallest key from queue" << endl;
        cout << "P : Print out all elements in queue" << endl;
        cout << "Q : Quit" << endl << endl;

        cout << "Choose menu: "; 
        getline(cin,key,'\n');
        key = trim(key);
        cin.clear();

        if(key=="I"||key=="i"){ // Getting input from the user
            if(!(minHeap.IsFull())){
                cout << "Enter name of element: ";
                getline(cin,name,'\n'); 
                cin.clear();
                name = trim(name); //delete spaces before/after input 
                
                cout << "Enter id of element: ";
                getline(cin,temp_id,'\n'); 
                cin.clear();

                id = checkId(temp_id); //id validation required
                if(id==-1){
                    cout << "Id is invalid" << endl << endl;
                    continue;
                }

                cout << "Enter school of element: ";
                getline(cin,school,'\n'); 
                cin.clear();
                school = trim(school);

                item.insertName(name);
                item.insertKey(id);
                item.insertSchool(school);

                minHeap = insert(minHeap, item);
            } else{
                cout << "Heap is full! " << endl;
            }
        }

        if(key=="P"||key=="p"){//Print
            minHeap.print();
        }

        if(key=="C"||key=="c"){//Decrease 
            
            cout << "Enter index of element: ";
            getline(cin,string_index,'\n');
            cin.clear();
            
            string_index = trim(string_index);
            index = stoi(string_index);
            
            if(index>minHeap.getIndex()){
                cout << "Element is not present for following index " << endl << endl;
                continue;
            }
            
            cout << "Enter id value: ";
            getline(cin,temp_id,'\n'); 
            cin.clear();
            id = checkId(temp_id);

            if(id==-1){
                cout << "Id is invalid" << endl << endl;
                continue;
            }

            minHeap = decrease_Key(minHeap,index,id);
        }

        if(key=="D"||key=="d"){ //Delete
            
            if(!(minHeap.IsEmpty())){
                item = deleteHeap(&minHeap);
                cout << "[" <<  item.getName() << ", " << item.getKey() << ", " << item.getSchool() << "]" << " is deleted. " << endl;
                
            }
            else {
                cout << "Heap is empty! " << endl;
            }
        }

        if(key=="Q"||key=="q"){ //Quit
            cout << "Thank you. Bye!" << endl;
            break;
        }
        cout << endl << endl;

    }       


    return 0;
}