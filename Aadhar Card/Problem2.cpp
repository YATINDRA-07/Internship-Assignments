
//The concept of Indexed sequential file was used to store the Records of aadhar. The sequential file is named as indexFile, the file holding
// the actual aadhar records is named as mainFile.txt. The Aadhar ID is being used as a key parameter to hold the records in a hashing manner
// hence producing search results in O(1). The file pointers are used to seek to the appropriate positions to reach the desired record.
//
//The telephone records are stored in teleFile.txt. On every delete function of aadhar record care is taken that the corresponding phone number
// is deleted as well. The IDs with blacklist aren't provided with any phone number also when an ID has been added to blacklist, the phone number
// assigned to it, is taken away as well.


#include<iostream>
#include<vector>
#include<cstring>
#include<fstream>
#include<iomanip>
#include<queue>
#include<set>

#define MAX 20
#define openMain open("mainFile.txt")
#define openIndex open("indexFile.txt")

using namespace std;

int getSize(char arr[]);

//A class to store the Aadhar related information
class AadharNode{
	char aadharId[13];
	char firstName[MAX];
	char guardianName[MAX];
	char lastName[MAX];
	bool blackList;
public:
	//Node to initialise the data members
	AadharNode(){
		strcpy(aadharId,"000000000000");
		strcpy(firstName,"00000");
		strcpy(guardianName,"00000");
		strcpy(lastName,"00000");
		blackList=false;
	}
	//Parameterised constructor to copy aadharId
	AadharNode(char ids[]){
		strcpy(aadharId,ids);
	}
	//function to get input of all data members
	void input(){
		try{
			cout<<"Enter Aadhar id:";
			cin>>aadharId;
			if(getSize(aadharId)!=12){
				throw string("Enter a valid 12 digit AadharId");
			}
		}
		catch(string s){
			cout<<s<<endl;
			input();
			return;
		}
		cout<<"Enter first name:";
		cin>>firstName;
		cout<<"Enter last name:";
		cin>>lastName;
		cout<<"Enter guardian name:";
		cin>>guardianName;
	}

	//to display data members
	void display(){
		cout<<setw(14)<<aadharId<<setw(13)<<firstName<<setw(13)<<lastName<<setw(15)<<guardianName<<setw(13)<<blackList<<endl;
	}
	void displayIndividual(){
		cout<<"\nAadhar ID : "<<aadharId<<"\nFirst name : "<<firstName<<"\nLast Name : "<<lastName<<"\nGuardian Name : "<<guardianName<<"\nBlack Listed : ";
		if(blackList)
			cout<<"Yes\n";
		else
			cout<<"No\n";
	}
	//to update data members for Updation of aadhar
	void update(char id[]){
		strcpy(this->aadharId,id);
		cout<<"Enter first name:";
		cin>>firstName;
		cout<<"Enter last name:";
		cin>>lastName;
		cout<<"Enter guardian name:";
		cin>>guardianName;
	}
	//overloaded not equal to compare ID of two objects
	bool operator !=(char ids[]){
    	for(int i=0;i<12;i++){
    		if(ids[i]!=aadharId[i]){
    			return 1;
    		}
    	}
    	return 0;
     }
     bool checkName(char Name[]){
		 int i;
		 for(i=0; firstName[i]!='\0';i++){
			if(Name[i]!=firstName[i]|| Name[i]=='\0'){
				return 1;
			}
    	 }
    	 if(Name[i]!='\0')
    		 return 1;

    	return 0;
     }
     //function that returns the status of a blacklist
     bool getBL(){
     	return blackList;
     }
	friend class AadharSystem;		//using friend to allow access to AadharSystem the data members of
};

//A class to store in sequential data for indexed sequential file
class IndexNode
{
  int pos;
  int chainLink;
  char id[13];
 public:

     IndexNode()
     {
      pos=-1;
      chainLink=-1;
      strcpy(id,"000000000000");
     }

     IndexNode(char ids[]){
    	 strcpy(id,ids);
     }

     //Assignment operator overloaded to equate 2 objects
     void operator =(IndexNode node){
     	pos=node.pos;
     	strcpy(id,node.id);
     	chainLink=node.chainLink;
     }

     //Not equal to operator overloaded to aadharId with the Input ID provided by user(Aadhar system)
     bool operator !=(char ids[]){
    	for(int i=0;i<12;i++){
    		if(ids[i]!=id[i]){
    			return 1;
    		}
    	}
    	return 0;
     }

     int getPos(){
     	return pos;
     }
     friend class AadharSystem;		//using friend to allow access to AadharSystem the data members of
};

//A class to manage aadhar operations
class AadharSystem
{
	int insertPos=0;
	vector<IndexNode> hashTable;
	queue<int> dList;
	fstream file;
	ifstream ifile;
	ofstream ofile;

public:

	AadharSystem(){
		IndexNode empty;
		ifile.open("indexFile.txt" ,ios::binary|ios::in);
		//if a file is not present then we make a file with default index nodes
		if(!ifile){
			ofile.open("indexFile.txt",ios::binary| ios::out);
			for(int i=0;i<1000;i++){
				hashTable.push_back(empty);
				ofile.write((char*)&empty, sizeof(empty));
			}
			insertPos=0;
			ofile.close();
			cout<<"Invalid file!";
		}
		//if present then load the file's objects to vector hashTable
		else{
			IndexNode temp;
			AadharNode Atemp;
			ifile.seekg(0,ios::beg);
			insertPos=0;
			char tryId[13];
			strcpy(tryId,"000000000000");
			int i=0;
			file.open("mainFile.txt",ios::binary | ios::in);

			while(ifile.read((char*)&temp, sizeof(temp))){		//Loop to reading all objects present in sequential file in hashTable
				hashTable.push_back(temp);
				if(insertPos < hashTable.back().pos){	//condition used to find the position of the last inserted object
					insertPos = hashTable.back().pos;
				}
				if(file.read((char*)&Atemp,sizeof(Atemp)) && !(Atemp != tryId)){
					dList.push(i);
				}
				i++;
			}
			insertPos++;								//incrementing position to next available free position
			ifile.close();
		}
		file.open("mainFile.txt",ios::app);
		file.close();
	}

	//destructor to write all the objects back to the sequential file
	~AadharSystem(){
		ofile.open("indexFile.txt");
		for(int i=0;i<hashTable.size();i++){
			ofile.write((char*)&hashTable[i], sizeof(IndexNode));
		}
		ofile.close();
	}
	void insert(AadharNode temp);
	void del(char id[]);
	void update();
	int search(char id[]);
	void search(string);
	int hashFunc(char id[]);
	void addTele();
	void blacklist();
	void display();
	void update(char id[]);
	int SearchOnlyId(char Id[]);
};

//A class for Telephone company data node
class TelephoneNode{
	char Name[MAX];
	char phoneNo[10];
	char Id[13];
public:

	TelephoneNode(){		//constructor for initializing phone no.
		strcpy(phoneNo,"020540000");		//considering some area code for phone no. first digits
		strcpy(Id,"000000000000");
	}
	//Parameterised constructor to get Data related to telephoneNode
	TelephoneNode(char Name[],char id[]){
		strcpy(phoneNo,"020540000");
		strcpy(this->Name,Name);
		strcpy(this->Id,id);
	}
	//Function to Display contents of the Telephone Node
	void Display(){
		cout<<setw(14)<<Id<<setw(13)<<Name<<setw(13)<<phoneNo<<endl;
	}
	//A function to generate random numbers
	void GenerateNumber();

	//overloaded not equal operator to compare IDs of two objects
	bool operator !=(char ids[]){
    	for(int i=0;ids[i]!='\0';i++){
    		if(ids[i]!=Id[i]){
    			return 1;
    		}
    	}
    	return 0;
     }

	friend class TeleOperation;		//friend class to allow access to TeleOperation class
};

//A class for Telephone company related operations
class TeleOperations{
	//AadharSystem Ref;
	fstream file;
	ifstream ifile;
	ofstream ofile;
	vector<TelephoneNode> telelist;
	int Insertpos;
public:
	TeleOperations(){

		Insertpos =0;
		file.open("teleFile.txt",ios::app);
		file.close();
		ifile.open("teleFile.txt",ios::binary|ios::in);
		if(ifile){		//if file already exists then read all objects into a vector object
			TelephoneNode temp;
			while(ifile.read((char*)&temp,sizeof(temp))){
				telelist.push_back(temp);
				Insertpos++;		//Position icremented and object pushed back in vector
			}

		}
		ifile.close();
	}
	//Fucntion which provides unique phone number to given aadhar ID holding personel

	void FindNumber(char[], char[],int);
	void Display();

	//Destructor is used to write the objects back to the file
	~TeleOperations(){
		ofile.open("teleFile.txt");
		for(int i=0;i<telelist.size();i++){
			ofile.write((char*)&telelist[i], sizeof(TelephoneNode));
		}
		ofile.close();
	}
};
//function to display the contents of the TelePhone Directory
void TeleOperations :: Display(){
	TelephoneNode t1;
	cout<<endl;
	cout<<setw(14)<<"ID"<<setw(13)<<"First Name"<<setw(13)<<"PhoneNo\n";
	ifile.open("teleFile.txt",ios::in);
	ifile.seekg(0, ios::beg);
	char tryId[13];
	strcpy(tryId,"000000000000");
	while(ifile.read((char*)&t1,sizeof(t1))){
		if((t1!=tryId)){
			t1.Display();
		}
	}
	ifile.close();

}
//Function to provide number to the user on the basis of aadharID and Name

void TeleOperations :: FindNumber(char Name[], char Id[],int pos){
	TelephoneNode phn(Name,Id);
	AadharNode temp;
	IndexNode inode;
	phn.GenerateNumber();
	ifile.open("indexFile.txt",ios::in|ios::binary);
	ifile.seekg(pos*sizeof(IndexNode),ios::beg);
	ifile.read((char*)&inode,sizeof(inode));
	ifile.close();
	ifile.open("mainFile.txt",ios::binary|ios::in);
	ifile.seekg((inode.getPos())*sizeof(AadharNode),ios::beg);
	ifile.read((char*)&temp,sizeof(temp));
	ifile.close();
	if(temp.checkName(Name)){
		cout<<"\nThe Record provided doesn't match with the aadhar data!";
		return;
	}
	for(int i=0;i<telelist.size();i++){				//Condition checked to see if an ID has already been issued a phone number
		if(!(telelist[i] != Id)){
			cout<<"\nThis AadharId has already been given Telephone No\n\n";//Each ID gets only 1 number
			cout<<setw(14)<<"ID"<<setw(13)<<"First Name"<<setw(13)<<"PhoneNo\n";
			telelist[i].Display();
			return;
		}
	}
	if(pos>0){				//checks only if record is found
		/*ifile.open("indexFile.txt",ios::in|ios::binary);
		ifile.seekg(pos*sizeof(IndexNode),ios::beg);
		ifile.read((char*)&inode,sizeof(inode));
		ifile.close();
		ifile.open("mainFile.txt",ios::binary|ios::in);
		ifile.seekg((inode.getPos())*sizeof(AadharNode),ios::beg);
		ifile.read((char*)&temp,sizeof(temp));
		ifile.close();*/
		if(temp.getBL()==1){
			cout<<"\nThis ID is blacklisted. You can't have a phone number!";
			return;
		}
		ofile.open("teleFile.txt",ios::in|ios::binary|ios::out);
		ofile.seekp(Insertpos*sizeof(TelephoneNode),ios::beg);
		ofile.write((char*)&phn,sizeof(TelephoneNode));
		ofile.close();
		telelist.push_back(phn);
		cout<<"\nThe new number is added to the telephone directory : \n";
		Display();
	}
}

//Used to generate random numbers for Telephone number
void TelephoneNode::GenerateNumber(){
	int i,sum=0,num;
	srand(time(0));			//function is used to produce random numbers according to current time
	for(i=5;i<9;i++){
		num= rand()%10 + 48;
		phoneNo[i] = num;
	}

}

//Function to insert new entry to file containing aadhar info
void AadharSystem::insert(AadharNode temp){
	IndexNode index;
	int pos=insertPos;
	file.open("mainFile.txt",ios::in | ios::out | ios::binary);

	//if a deletion has occured and a possible vacant space is available in Data file

	if(dList.size()!=0){
		pos=dList.front();	//vacant position is stored in a queue
		insertPos--;
		dList.pop();
	}
	file.seekp(pos*sizeof(temp),ios::beg);	//seeking put pointer to get to write object at pos position
	file.write((char*)&temp,sizeof(temp));
	file.close();
	insertPos++;
	index.pos = pos;						//getting a copy of used position for sequential file
	strcpy(index.id,temp.aadharId);			//copying ID to index node

	pos=hashFunc(temp.aadharId);		//Using pos for calculating index from hash function.
	if(hashTable[pos].pos != -1){		//if a record is already present at index of sequential file
		bool flag=1;
		int i=1000;
		while(i<hashTable.size()){		//loop to find the next vacant space in  seq file
			if(hashTable[i].pos==-1){
				hashTable[i]=index;
				flag=0;
				break;
			}
			i++;
		}
		if(flag){
			hashTable.push_back(index);	//inserting index node in hashTable
		}
		int j;
		for(j=pos;hashTable[j].chainLink!=-1;j=hashTable[j].chainLink); //loop to assign a chainlink to keep a track of records
		hashTable[j].chainLink = i;
	}
	else{							//if no collision then you can directly
		hashTable[pos]=index;
	}
}

int AadharSystem::hashFunc(char id[12]){			//hash function for Aadhar id
	int x=100;
	int key=0;
	for(int i=0;i<12;i++){
		key+=id[i]*x;
		if((i+1)%4==0){
			x/=10;
		}
	}
	key=key%1000;
	return key;
}
//Function created to return only the position in the sequential file
int AadharSystem::SearchOnlyId(char id[]){				//function to search any id
	int pos=1000;
	for(pos=hashFunc(id);(IndexNode(id)!=hashTable[pos].id);pos=hashTable[pos].chainLink){
		if(hashTable[pos].chainLink == -1){
			cout<<"Record not found\n";
			return -1;								//return -1 if record not found
		}
	}
	return pos;
}
//Function to search for the ID and also display the data of the record related to it
int AadharSystem::search(char id[]){				//function to search any id
	int pos=1000;
	for(pos=hashFunc(id);(IndexNode(id)!=hashTable[pos].id);pos=hashTable[pos].chainLink){
		if(hashTable[pos].chainLink == -1){
			cout<<"Record not found\n";
			return -1;								//return -1 if record not found
		}
	}
	int indexPos=pos;
	pos=hashTable[pos].pos;
	ifile.openMain;
	ifile.seekg(pos*sizeof(AadharNode), ios::beg);
	AadharNode temp;
	ifile.read((char*)&temp ,sizeof(temp));
	cout<<"\n";
	temp.displayIndividual();
	return indexPos;								//if found return position of object of searched Aadhar ID in main file
}

void AadharSystem::search(string input){			//overloading search function for searching any sub string among whole data
	char name[MAX];
	for(int i=0;input[i]!='\0';i++){
		name[i]=input[i];
	}
	AadharNode t1;
	int i=0;
	ifile.open("mainFile.txt",ios::in);
	ifile.seekg(0, ios::beg);
	bool flag = 0;
	while(ifile.read((char*)&t1,sizeof(t1))){

		//strstr(s1,s2) is a function of cstring class which checks whether s2 is substring or not and if found returns position of first letter else returns null character

		if(strstr(t1.firstName,name) || strstr(t1.lastName,name) || strstr(t1.guardianName,name)){

			if(!flag)
				cout<<"Position"<<"\t    ID"<<"   First Name"<<"    Last Name"<<"  Guardian Name"<<"   Black List\n";
			flag = 1;
			cout<<"       "<<i;
			t1.display();
		}
		i++;
	}
	if(!flag)
		cout<<"\nNo records found! ";
	ifile.close();
}

void AadharSystem::del(char id[]){			//function to delete any data
	int pos = search(id);					//using search function to check if id is present and get its position in main file
	if(pos<0){
		return;
	}
	int mainPos=hashTable[pos].pos;
	int chain=hashTable[pos].chainLink;
	dList.push(mainPos);
	if(chain!=-1){
		hashTable[pos]=hashTable[chain];
		hashTable[chain]=IndexNode();
	}
	else if(chain==-1){
		hashTable[pos]=IndexNode();
	}
	else{
		if(pos>999){
			int i;
			for(i=hashFunc(hashTable[pos].id);hashTable[i].chainLink!=pos;i=hashTable[i].chainLink);
			hashTable[i].chainLink=-1;
		}
		hashTable[pos]=IndexNode();
	}
	AadharNode temp;
	file.open("mainFile.txt",ios::in | ios::out | ios::binary);
	file.seekp(mainPos*sizeof(AadharNode),ios::beg);
	file.write((char*)&temp,sizeof(temp));
	file.close();
	TelephoneNode tele;
	int i=0;
	file.open("teleFile.txt",ios::in|ios::out|ios::binary);
	while(file.read((char*)&tele,sizeof(tele))){				//This loop is used to delete any phone number assigned to the deleted ID
		if(!(tele!=id)){
			TelephoneNode dump;
			file.seekp(i*sizeof(TelephoneNode),ios::beg);
			file.write((char*)&dump,sizeof(TelephoneNode));
			break;
		}
		i++;
	}
	file.close();
}

void AadharSystem::update(char id[]){			//to update an entry
	int pos = search(id);					//getting position in main file
	if(pos<0){
		return;
	}
	int mainPos = hashTable[pos].pos;
	AadharNode temp;
	temp.update(id);							//update function of AadharNode is called to update content of given id
	file.open("mainFile.txt",ios::in | ios::out | ios::binary);
	file.seekp(mainPos*sizeof(AadharNode),ios::beg);

	file.write((char*)&temp,sizeof(temp));
	file.close();
}

void AadharSystem::blacklist(){		//function to manipulate blacklist conditions
	char id[12],choice;
	bool ch;
	cout<<"\nEnter Aadhar ID : ";
	cin>>id;
	int pos = search(id);
	if(pos!=-1){
		cout<<"\nDo you want to add it to blacklist or remove from blacklist (a/r) ? ";
		cin>>choice;
		if(choice == 'a' || choice == 'A')
			ch = 1;
		else
			ch = 0;
		int mainPos = hashTable[pos].pos;
		AadharNode temp;
		file.open("mainFile.txt",ios::in | ios::out | ios::binary);
		file.seekg((mainPos)*sizeof(AadharNode),ios::beg);
		file.read((char*)&temp,sizeof(temp));
		file.close();
		temp.blackList = ch;
		file.open("mainFile.txt",ios::in | ios::out | ios::binary);
		file.seekp(mainPos*sizeof(AadharNode),ios::beg);
		file.write((char*)&temp,sizeof(temp));
		file.close();
		TelephoneNode tele;
		int i=0;
		file.open("teleFile.txt",ios::in|ios::out|ios::binary);
		while(file.read((char*)&tele,sizeof(tele))){				//This loop is used to delete any phone number assigned to the blacklisted ID
			if(!(tele!=id)){
				TelephoneNode dump;
				file.seekp(i*sizeof(TelephoneNode),ios::beg);
				file.write((char*)&dump,sizeof(TelephoneNode));
				break;
			}
			i++;
		}
		file.close();
	}
}

void AadharSystem::display(){
	IndexNode temp;
	cout<<"\nIndexfile.txt : \n";
	cout<<setw(5)<<"\nIndex"<<setw(14)<<"ID"<<"\tPos\tChainLink\n";
	for(int i=0;i<hashTable.size();i++){
		if(hashTable[i].pos != -1)
		cout<<setw(5)<<i<<setw(14)<<hashTable[i].id<<"\t"<<hashTable[i].pos<<"\t"<<hashTable[i].chainLink<<endl;
	}
	AadharNode t1;
	int i=0;
	cout<<"\nMainfile.txt : \n";
	cout<<"\n"<<setw(8)<<"Position"<<setw(14)<<"ID"<<setw(13)<<"First Name"<<setw(13)<<"Last Name"<<setw(15)<<"Guardian Name"<<setw(13)<<"Black List";
	ifile.open("mainFile.txt",ios::in);
	ifile.seekg(0, ios::beg);
	cout<<endl;
	while(ifile.read((char*)&t1,sizeof(t1))){
		cout<<setw(8)<<i;
		t1.display();
		cout<<"\n";
		i++;
	}
	ifile.close();
}

//function which returns size of character array passed
int getSize(char arr[]){
	int i;
	for(i=0;arr[i]!='\0';i++);
	return i;
}




int main(){
	int choice;
	int ch1;
	char ch2;
	do{
		cout<<"\nEnter choice : \n1. Perform operations for Aadhar Card\n2. Perform operations for Telephone Company\n3. Exit\n";
		cin>>choice;
		switch (choice){
			case 1:
				do{
					char Id[13];
					string name;
					AadharNode temp;
					AadharSystem aa;
					cout<<"\nEnter Operation to perform :\n1. Add an entry\n2. Delete an entry\n3. Update an entry\n4. Search by ID \n5. Search by substring\n6. Add or Remove from Blacklist\n7. Display all records in both files\n8. Exit to previous menu\n";
					cin>>ch1;
					switch (ch1){
						case 1:
								temp.input();
								aa.insert(temp);

							break;
						case 2:
							cout<<"\nEnter ID to be deleted : ";
							cin>>Id;
							aa.del(Id);
							break;
						case 3:
							cout<<"\nEnter ID to be Updated : ";
							cin>>Id;
							aa.update(Id);
							break;
						case 4:
							cout<<"\nEnter ID to be searched : ";
							cin>>Id;
							aa.search(Id);
							break;
						case 5:

							cout<<"\nEnter substring to be searched for : ";
							cin>>name;
							aa.search(name);
							break;
						case 6:
							aa.blacklist();
							break;
						case 7:
							aa.display();
							break;
						case 8:
							break;
						default:
							cout<<"\nInvalid Choice!";
							break;
					}
				}while(ch1!=8);
				break;
			case 2:
				cout<<"\n**** Telephone no. Generation ****";
				do{
					AadharSystem aa;
					TeleOperations obj;
					char id[13],firstname[MAX];
					cout<<"\nEnter Aadhar id and first name to check in Database :\n";
					cin>>id>>firstname;
					int pos = aa.SearchOnlyId(id);
					obj.FindNumber(firstname,id,pos);
					cout<<"\nDo you want to continue (y/n) ? ";
					cin>>ch2;
				}while (ch2 == 'y'|| ch2 =='Y');
				break;
			case 3:
				break;
			default:
				cout<<"\nInvalid Choice!";
				break;
			}
		}while(choice!=3);
	return 0;
}

