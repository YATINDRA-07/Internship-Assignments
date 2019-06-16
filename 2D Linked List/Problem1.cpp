/*
	The 3x3 matrix is taken as input. 
	Then it was converted into a 2 D linklist. 
	The element in right of any number in matrix is in its right link.
	The element at below of any number in matrix is in its down link.
	Then right link of 3rd column elements is null and down link of 3rd row is null.
	This results in 2-D matrix formation.
	For perimeter sum of all elements is calculated and middle element is subtracted.
	For transpose, same count of right and down traverse is done and swapped.
	In sum of diagonals we have showed sum of major diagonal, minor diagonal, and addition of both ( central elements is added twice )
*/
#include <iostream>
#include <iomanip>

using namespace std;

class Node{
	Node *right,*down;
	int data;
public:
	Node(){							//Constructor to intialize data members
		down = right = NULL;

	}
	Node(int data){						//A parametrized constructor
		this->data = data;
		down = right = NULL;
	}
	friend class LinkList;                                  //friend class to access data members of LinkList
};

class LinkList{
	Node *head;
	int Mat[3][3];
public:
	LinkList(){						//Constructor to initialize data member
		head =NULL;
	}
	void getData();
	void Convert(int ,int);
	void sumDiagonals();
	void Perimeter();
	void Transpose();
	Node* TraverseRight(Node* , int);
	Node* TraverseDown(Node* , int);
	void display();
};

void LinkList :: getData(){					//Function to get 2-D matrix
	head = NULL;
	cout<<"\nEnter the Matrix(3x3)\n";
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			cin>>Mat[i][j];
			Convert(i,j);
		}
	}
}

void LinkList :: Convert(int i,int j){				//Function to convert 2-D matrix to 2-D linklist
	Node *new1 = new Node(Mat[i][j]);
	if(head == NULL){
		head = new1;
		return;
	}
	if(j>0){						//logic for creating right link
		Node *t2 = TraverseDown(head,i);
		t2= TraverseRight(t2,j-1);
		t2->right=new1;
	}
	if(i>0){						//logic for creating down link
		Node *t1 = TraverseRight(head,j);
		t1 = TraverseDown(t1,i-1);
		t1->down = new1;
	}	
}

Node* LinkList :: TraverseRight(Node* temp,int r )		//Function for traversing to the right of current node
{
	for(int i=r;i>0;i--){
		temp=temp->right;	
	}
	return temp;
}

Node* LinkList :: TraverseDown(Node* temp,int d)		//Function for traversing to the bottom of current node
{
	for(int i=d;i>0;i--){
		temp=temp->down;
	}
	return temp;
}

void LinkList::sumDiagonals(){					//Function to calculate sum of diagonals of 2-D linklist
	Node *temp = head;
	int majorSum = 0;
	int minorSum = 0;
	while(temp){						//code for getting major diagonal
		majorSum += temp->data;
		temp = temp->right;
		if(temp){
			temp = temp->down;
		}
	}
	cout<<"\nSum of Major Diagonal Elements is : "<<majorSum;
	int N=2;
	temp = head;
	while(N+1>0){
		temp = TraverseRight(temp,N);			//code for getting minor daigonal
		minorSum += temp->data;
		temp = TraverseDown(head,3-N);
		N--;
	}
	
	cout<<"\nSum of Minor Diagonal Elements is : "<<minorSum;
	
	cout<<"\nSum of Major and Minor Diagonal Elements is (Considering center element twice) : "<<majorSum + minorSum;
}

void LinkList::Perimeter(){				//Function to calculate perimeter 
	Node *temp =head,*temp2=head;
	cout<<endl;
	int sum=0;
	while(temp!=NULL){					//Loop to calculate sum of all elements of matrix
		temp2=temp;
		while(temp2!=NULL){
			sum+=temp2->data;
			temp2=temp2->right;
		}
		temp=temp->down;
	}
	sum -= head->right->down->data;			//subtraction middle element for getting perimeter
	cout<<"\n"<<sum;
}

void LinkList::Transpose(){				//Function for obtaining transpose of 2-D linklist
	Node *temp1,*temp2,*tempd = head;
	int temp;
	while (tempd){
		temp1=tempd->right;
		temp2=tempd->down;
		while(temp1 && temp2){
			temp = temp1->data;		//logic for swapping data
			temp1->data = temp2->data;
			temp2->data = temp;
			temp1 = temp1->right;
			temp2 = temp2->down;
		}
		tempd = tempd->right;
		if(tempd){
			tempd = tempd->down;
		}
	}
	cout<<"\nThe Transpose Matrix is : \n";
	display();
}

void LinkList::display(){				//Function to display 2-D LinkList
	Node *temp =head,*temp2=head;
	cout<<endl;
	while(temp!=NULL){
		temp2=temp;
		while(temp2!=NULL){
			cout<<setw(3)<<temp2->data<<setw(3)<<"->";
			temp2=temp2->right;
		}
		cout<<" NULL";
		cout<<endl<<setw(3)<<"|"<<setw(6)<<"|"<<setw(6)<<"|"<<"\n";
		temp=temp->down;
	}
	cout<<setw(6)<<"NULL"<<setw(6)<<"NULL"<<setw(6)<<"NULL";
}

int main() {
	LinkList obj;
	int flag =0,ch;
	do{
		if(flag==0){
			cout<<"\n\t\t\t2-D LinkList\n";
			obj.getData();
			obj.display();
			flag =1;
		}
		else{
			cout<<"\nEnter the number for operation : "
				"\n1. Perimeter "
				"\n2. Sum of Diagonal Elements"
				"\n3. Transpose "
				"\n4. Make new Matrix"
				"\n5. Exit\n";
			cin>>ch;
			switch(ch){
				case 1 : 
					obj.Perimeter();
					break;
				case 2 :
					obj.sumDiagonals();
					break;
				case 3 :
					obj.Transpose();
					break;
				case 4 :
					flag =0;
					continue;
				case 5 :
					break;
				default :
					cout<<"\nWrong Input";
					break;
			}
		}
		
	}while(ch != 5);
	return 0;
}

