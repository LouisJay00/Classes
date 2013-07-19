/*
Define a Date structure with month, day and year and Class with title (string), units (integer) and grade (character).  
Then, define a Student structure with ID (integer) and name (string), doe (date of enrollment),
a Class pointer pointing to an array of Classes (an array of Class structures the student has completed to date) 
created dynamically at run time, and a Student pointer called next. 
*/


#include <iostream>
#include<string>
#include<iomanip>
using namespace std;


struct Date
{
  int month;
	int day;
	int year;
};

struct Class
{
	string title;
	int units;
	char grade;
};

struct Student
{	
	int classlimit;
	int ID;
	string name;
	Date doe;
	Class *classes[50];
	Student* next;
};
int input(Student*[]);
void display(Student*[],int);
void nameSort(Student*[],int);
void IDSort(Student*[],int);
void makelist(Student*[],int);
void displayLink(Student *[],int);

void menu(Student*[],int&,bool&,int);
void del(Student*[],int&);
void add(Student*[],int&,int);
void displayOne(Student*[],int);
void edit(Student*[],int,int);
void Testdisplay(Student*[],int);

int main()
{
	
	Student* ptr[100]= {0};

	
	int limit = 0;
	int choice = 0;
	bool loop = true;

	limit = input(ptr);
	
 //To make testing faster:
	/*for(int i=0;i<limit;i++)
	{
		ptr[i]->doe.day = 0;
		ptr[i]->doe.month = 0;
		ptr[i]->doe.year = 0;
		ptr[i]->ID = 0;
	}
	*/

	cout << "\nBefore Sorting \n\n";
	display(ptr,limit);
	
	system("pause");

	cout << "\nWould you prefer to sort by the student's \n1. Name \n2. Student ID \nChoose one.\n";
	cin >> choice;
	cin.ignore();

	switch(choice)
	{
		case 1: nameSort(ptr,limit);
			break;
		case 2: IDSort(ptr,limit);
			break;
		default: cout << "\nPlease enter a valid choice.\n";
	}
	
	cout << "\n\nAfter Sorting \n\n";
	display(ptr,limit);
	
	system("pause");
	
	if(limit > 1)
	{
		makelist(ptr,limit);

		displayLink(ptr,limit);
	}
	
	
	do{
		menu(ptr,limit,loop,choice);
	}while(loop);


	//DELETING:
	for (int i=0; i < limit;i++)
	{
		for(int j=0; j < ptr[i]->classlimit; j++)
		delete ptr[i]->classes[j];
	}
	
	//this is the only syntax that will allow me to end the program.
	for(int i=0; i< limit; i++)
	delete ptr[i];			
	
	system("pause");
	return 0;
}

void menu(Student* p[],int& limit, bool& loop, int choice)
{
	int option = 0;

	cout << "1. Add student \n"
		 << "2. Edit student \n"
		 << "3. Delete \n"
		 << "4. Display a student \n"
		 << "5. Display all students \n"
		 << "6. Quit\n"
		 << "\nPlease choose an option. \n";
	cin >> option;
	cin.ignore();

	switch(option)
	{
		case 1: add(p,limit,choice);
			break;
		case 2: edit(p,limit,choice);
			break;
		case 3: del(p,limit);
			break;
		case 4: displayOne(p,limit);
			break;
		case 5: display(p,limit);
			break;
		case 6: loop = false;
			break;
		default: cout << "\nEnter a valid choice.\n";
	}

}


void add(Student* p[],int& limit,int choice)
{
	int L = limit + 1;
	string enterKey = "";
	
	char slash ='/';

	cout << "Enter Students and their classes. When finished, \npress [Enter] for the student name. \n\n";


		p[L] = new Student;
		
		cout << "Enter student name: ";
		getline(cin, p[L]->name);

		
		cout << "\nEnter " << p[L]->name << "'s ID number: ";
		cin >> p[L]->ID;
		cin.ignore();
		
		cout << endl;
	
		cout << "Enter " << p[L]->name << "'s date of enrollment in this format: [MM/DD/YY] \n";
		cin >> p[L]->doe.month >> slash >> p[L]->doe.day >> slash >> p[L]->doe.year;
		cin.ignore();
	
			cout << "\nEnter the information for " << p[L]->name << "'s classes. \nWhen finished, press [Enter] for the name of the class. \n ";

	
		for(int numClass = 0; numClass < 50; numClass++)
		{
			p[L]->classes[numClass] = new Class;

			cout << "\nEnter class title. \n";
			getline (cin,p[L]->classes[numClass]->title);

			if(p[L]->classes[numClass]->title == enterKey)
			{
				p[L]->classlimit = numClass;
				break;
			}

			cout << "Enter " << p[L]->classes[numClass]->title << "'s units. \n";
			cin >> p[L]->classes[numClass]->units;
			cin.ignore();


			cout << "Enter the grade for " << p[L]->classes[numClass]->title << ". \n";
			cin >> p[L]->classes[numClass]->grade;
			cin.ignore();
		}


	

	limit += 1;

	
	switch(choice)
	{
		case 1: nameSort(p,limit);
			break;
		case 2: IDSort(p,limit);
			break;
	}

}


void del(Student* p[], int& limit)
{
	Student* temp = p[0];
	int number = 0;

	cout << "Which student would you like to delete? \n"
		<< "Available record numbers are [1-" << limit << "] \n";
	cin >> number;
	cin.ignore();

	number -= 1;

		temp = p[number];

		p[number] = p[number + 1];

		for(int i = number; i < limit; i++)
		 p[i] = p[i + 1];

	for(int i = 0; i< temp->classlimit; i++)
		delete temp->classes[i];

	delete temp;

	temp = 0;

	limit -=1;

}

void edit(Student* p[],int limit,int choice)
{
	int number;
	char slash = '/';
	string enterKey = "";

	cout << "Which student would you like to edit? \n"
		<< "Available record numbers are [1-" << limit << "] \n";
	cin >> number;
	number -=1;

		for(int i =0; i< p[number]->classlimit; i++)
				delete p[number]->classes[i];

	cin.ignore();
		
		cout << "Enter new student name: ";
		getline(cin, p[number]->name);
		
		cout << "\nEnter " << p[number]->name << "'s ID number: ";
		cin >> p[number]->ID;
		cin.ignore();
		
		cout << endl;
	
		cout << "Enter " << p[number]->name << "'s date of enrollment in this format: [MM/DD/YY] \n";
		cin >> p[number]->doe.month >> slash >> p[number]->doe.day >> slash >> p[number]->doe.year;
		cin.ignore();
	
			cout << "\nEnter the information for " << p[number]->name << "'s classes. \nWhen finished, press [Enter] for the name of the class. \n ";

	
		for(int numClass = 0; numClass < 50; numClass++)
		{
		
		  p[number]->classes[numClass] = new Class;

			cout << "\nEnter class title. \n";
			getline (cin,p[number]->classes[numClass]->title);

				if(p[number]->classes[numClass]->title == enterKey)
			{
				p[number]->classlimit = numClass;
				break;
			}

				
			cout << "Enter " << p[number]->classes[numClass]->title << "'s units. \n";
			cin >> p[number]->classes[numClass]->units;
			cin.ignore();


			cout << "Enter the grade for " << p[number]->classes[numClass]->title << ". \n";
			cin >> p[number]->classes[numClass]->grade;
			cin.ignore();
		}

		
	switch(choice)
	{
		case 1: nameSort(p,limit);
			break;
		case 2: IDSort(p,limit);
			break;
	}
}


void displayOne(Student* p[],int limit)
{	
	int number;
	char slash = '/';

	cout << "Which student would you like to display? \n"
		<< "Available record numbers are [1-" << limit << "] \n";
	cin >> number;

	number -= 1;

	cout << endl << p[number]->name << "'s Student file. \n"
		 << "ID: " << p[number]->ID << endl
		 << "Date of Enrollment: " << p[number]->doe.month << slash << p[number]->doe.day << slash << p[number]->doe.year << endl
		 << "Classes Completed: " << endl;

	for(int j =0; j < p[number]->classlimit; j++)
		cout << p[number]->classes[j]->title << endl
		<< "Units: " << p[number]->classes[j]->units << endl
		<< "Grade: " << p[number]->classes[j]->grade << endl;
	
	cout << endl << endl;
}


void makelist(Student* p[],int limit)
{
	cout<< "\ninside makelist().\n";
	
	Student* current;
	
	current = *p;
	//current is equal to the address that p holds (the first element of the array)
	//NOT the address of the pointer itself.

	
	for(int i = 0; i < limit; i++)
	{
		current = p[i + 1];

		p[i]->next = current;

	}		
	
	cout << "\nLink list assignment functioning normally. \n";

	limit -=1;
	p[limit]->next = 0;

	delete current;
}

void displayLink(Student* p[], int limit)
{
	cout << "\nDisplaying using the linked lists:\n";

	char slash = '/';
	
	limit -= 1;
	
	for(int i = 0; i < limit; i++)
	{	
	
	cout << "\n The data for person # " << (i + 2) << " is: \n";

	cout << endl << p[i]->next->name << "'s Student file. \n"
		 << "ID: " << p[i]->next->ID << endl
		 << "Date of Enrollment: " << p[i]->next->doe.month << slash << p[i]->next->doe.day << slash << p[i]->next->doe.year << endl
		 << "Classes Completed: " << endl;
	

	for(int j =0; j < p[i]->next->classlimit; j++)
		cout << p[i]->next->classes[j]->title << endl
		<< "Units: " << p[i]->next->classes[j]->units << endl
		<< "Grade: " << p[i]->next->classes[j]->grade << endl <<endl;
	
	}

}

void IDSort(Student* p[], int limit)
{
	Student *temp;
	bool swap;

	do
	{
		swap = false;
		for(int i =0; i < (limit - 1); i++)
		{
			if(p[i]->ID > p[i + 1]->ID)
			{
				temp = p[i];
				p[i] = p[i+1];
				p[i+1] = temp;
				swap = true;
			}
		}

	}while(swap);
}

void nameSort(Student* p[], int limit)
{
	Student* temp;
	bool swap;

	do
	{
		swap = false;
		for(int i =0; i < (limit - 1); i++)
		{
			if(p[i]->name > p[i + 1]->name)
			{
				temp = p[i];
				p[i] = p[i+1];
				p[i+1] = temp;
				swap = true;
			}
		}

	}while(swap);

}

int input(Student* p[])
{
	int limit = 0;
	string enterKey = "";
	
	char slash ='/';

	cout << "Enter Students and their classes. When finished, \nPress [Enter] for the student name. \n\n";

	for(int i = 0; i < 101; i++)
		{	
		p[i] = new Student;
		
		cout << "Enter student name: ";
		getline(cin, p[i]->name);

		if(p[i]->name == enterKey && i >= 1)
		{
			limit = i;
			break;
		}

		
		
		cout << "\nEnter " << p[i]->name << "'s ID number: ";
		cin >> p[i]->ID;
		cin.ignore();
		
		cout << endl;

			if(p[i]->ID < 0 && i >= 1)
			{
				if (i < 1)
				{
				cout << "\nError: Must enter at least one Student.\n";
				continue;
				}

				else
				{
				limit = i;
				break;
				}
			}
	
		cout << "Enter " << p[i]->name << "'s date of enrollment in this format: [MM/DD/YY] \n";
		cin >> p[i]->doe.month >> slash >> p[i]->doe.day >> slash >> p[i]->doe.year;
		cin.ignore();
	
			cout << "\nEnter the information for " << p[i]->name << "'s classes. \nWhen finished, press [Enter] for the name of the class. \n ";

	
		for(int numClass = 0; numClass < 50; numClass++)
		{
			p[i]->classes[numClass] = new Class;

			cout << "\nEnter class title. \n";
			getline (cin,p[i]->classes[numClass]->title);

			if(p[i]->classes[numClass]->title == enterKey)
			{
				p[i]->classlimit = numClass;
				break;
			}

			cout << "Enter " << p[i]->classes[numClass]->title << "'s units. \n";
			cin >> p[i]->classes[numClass]->units;
			cin.ignore();


			cout << "Enter the grade for " << p[i]->classes[numClass]->title << ". \n";
			cin >> p[i]->classes[numClass]->grade;
			cin.ignore();
		}


	}	

	return limit;
}


void display(Student* ptr[], int limit)
{
	cout << "\nlimit: " << limit << endl;
	char slash = '/';

	for(int i = 0; i < limit; i++)
	{
		cout << endl << ptr[i]->name << "'s Student file. \n"
		 << "ID: " << ptr[i]->ID << endl
		 << "Date of Enrollment: " << ptr[i]->doe.month << slash << ptr[i]->doe.day << slash << ptr[i]->doe.year << endl
		 << "Classes Completed: " << endl;

	for(int j =0; j < ptr[i]->classlimit; j++)
		cout << ptr[i]->classes[j]->title << endl
		<< "Units: " << ptr[i]->classes[j]->units << endl
		<< "Grade: " << ptr[i]->classes[j]->grade << endl;
	}

}
