#include <iostream>
#include <string>
#include <fstream>
#include <stack>
using namespace std;

class Node
{
	string data;
	string antonym, synonym;
	Node *next, *prev;

public:
	Node()
	{
		next = prev = NULL;
	}
	Node(string data, string antonym="",string synonym="")
	{
		setData(data);
		setAntonym(antonym);
		setSynonym(synonym);
		next = prev = NULL;
	}
	void setData(string data)
	{
		this->data = data;
	}
	void setNext(Node *next)
	{
		this->next = next;
	}
	void setPrev(Node *prev)
	{
		this->prev = prev;
	}
	void setSynonym(string synonym)
	{
		this->synonym = synonym;
	}
	void setAntonym(string antonym)
	{
		this->antonym = antonym;
	}

	string getSynonym()
	{
		return synonym;
	}
	string getAntonym()
	{
		return antonym;
	}

	Node *getNext()
	{
		return next;
	}
	Node *getPrev()
	{
		return prev;
	}
	string getData()
	{
		return data;
	}

};

class LinkedList
{
	Node *head, *tail;

public:
	LinkedList()
	{
		head = tail = NULL;
	}

	// Simple inserting function
	void insert(string data)
	{
		Node *N = new Node(data);
		if (head == NULL)
		{
			head = tail = N;
		}
		else
		{
			tail->setNext(N);
			N->setPrev(tail);
			tail = N;
		}
	}

	// Inserting function for Hash Table implementation (antoynym , synonym)
	void insertForLookUp(string word, string antoynym, string synonym)
	{
		Node *N = new Node(word,antoynym,synonym);
		if (head == NULL)
		{
			head = tail = N;
		}
		else
		{
			tail->setNext(N);
			N->setPrev(tail);
			tail = N;
		}
	}

	// Search function to find antoynym and synonym using Hash Table Implementation (Recursive Function)
	Node *searchForLookUP(Node *temp, string word)
	{
		if (temp == NULL)
		{
			return NULL;
		}
		else
		{
			if (temp->getData() == word)
			{
				return temp;
			}

			return searchForLookUP(temp->getNext(), word);
		}

	}

	// simple search function to check if word is found or not (Recursive)
	bool search(Node *temp, string word)
	{
		if (temp == NULL)
		{
			cout << "Word Not Found\n";
			return false;
		}

		if (temp->getData() == word)
		{
			return true;
		}

		return search(temp->getNext(), word);
	}

	// Search function to find same words and returning their positoins for replace and delete purpose (Recursive Function)
	void searchForSpecific(Node* temp, string word, int position = 1)
	{
		if (temp == NULL)
		{
			return;
		}

		if (temp->getData() == word)
		{
			cout << word << " -> found at position : " << position << endl;
		}

		searchForSpecific(temp->getNext(), word, position + 1);
	}

	// Update function to replace specfic word by using its position (Recursive Function)
	bool updateForSpecific(Node *temp, string data,string newWord,int position,int tempPosition=1)
	{
		if (temp == NULL)
		{
			return false;
		}

		if (temp->getData() == data)
		{
			if (tempPosition == position)
			{
				temp->setData(newWord);
				return true;
			}
		}

		 return updateForSpecific(temp->getNext(), data,newWord,position,tempPosition+1);
	}

	// For Undo Redo Function
	void deleteAtLast()
	{
		Node *temp = tail;

		if (temp == head)
		{
			delete temp;
			head = tail = NULL;
		}
		else
		{
			tail = temp->getPrev();
			delete temp;
			tail->setNext(NULL);
		}
	}

	// Delete function for deleting a specific word by using its position (Recursive Function)
	bool deleteForSpecific(Node *temp, string data,int position, int tempPosition = 1)
	{
		if (temp == NULL)
		{
			return false;
		}

		if (temp->getData() == data)
		{
			if (tempPosition == position)
			{
				if (position == 1)
				{
					// if data is at head
					temp = head;
					head = temp->getNext();
					delete temp;
					return true;
				}
				else if(position==totalWords(getHead()))// if data is at tail*/
				{
					tail = temp->getPrev();
					delete temp;
					tail->setNext(NULL);
				}
				else // if data is not at tail or head
				{
					temp->getPrev()->setNext(temp->getNext());
					temp->getNext()->setPrev(temp->getPrev());
				}
				return true;
			}
		}

		return deleteForSpecific(temp->getNext(), data, position, tempPosition + 1);
	}

	// Replace function for replacing all matching words (Recursive Function)
	void replaceAllWords(Node *temp, string oldWord, string newWord, int &notFound)
	{
		if (temp == NULL)
		{
			return;
		}

		if (temp->getData() == oldWord)
		{
			notFound++;
			temp->setData(newWord);
		}

		replaceAllWords(temp->getNext(), oldWord, newWord, notFound);
	}

	Node *getHead()
	{
		return head;
	}

	Node *getTail()
	{
		return tail;
	}

	// Display function (Recursive Function)
	void display(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}

		cout << temp->getData() << " ";
		display(temp->getNext());
	}

	// Funtion to save data in file (Recursive Function)
	void saveInFile(Node *temp,fstream &fout)
	{
		if (temp == NULL)
		{
			return;
		}
		fout << temp->getData() << " ";
		saveInFile(temp->getNext(), fout);
		
	}

	// Function to cound total words (Recursive Function)
	int totalWords(Node *temp,int count=0)
	{
		if (temp == NULL)
		{
			return count;
		}
		totalWords(temp->getNext(), count+1);
	}

	// Function to store data in file (For Sentences)
	void sentenceSeparate(fstream &fin,LinkedList &l)
	{
		bool flag = true;
		string sentence,word;
		if (fin)
		{
			getline(fin, word);

			if (word == "")
			{
				flag = false;
				cout << "No data in File\n";
			}
			else
			{
				int last = word.length();
				int count = 0;
				for (char c : word)
				{
					count++;
					sentence = sentence + c;
					if (c == '.' || count==last)
					{
						l.insert(sentence);
						sentence = "";
					}
				}
			}
			if (flag)
			{
				cout << "Data Added\n";
				fin.close();
			}

		}
		else
		{
			cout << "File not Found\n";
		}
	}

	// Function to find if list is empty or not
	bool isEmpty()
	{
		if (head == NULL)
			return true;
		return false;
	}
	
	// Function to count total words in sentence scenario
	int totalWordsSent(Node *temp)
	{
		int totalWords = 0;
		while (temp != NULL)
		{
			int last = temp->getData().length();
			int count = 0;
			for (char c : temp->getData())
			{
				count++;
				if (c == ' ' || c == '.' || count == last)
				{
					totalWords++;
				}
			}
			temp = temp->getNext();
		}
		return totalWords;
	}
};

class HashTable
{
	LinkedList *list;
	fstream fWord, fAnto, fSyno;

public:
	HashTable()
	{
		list = new LinkedList[10];
	}

	// Function that will return index for string data
	int Hashing(string data)
	{
		int index=0;

		for (int i = 0; i < data.length(); i++)
		{
			index = index + data[i];
		}

		return index % 10;

	}

	void insert()
	{
		fWord.open("Word.txt", ios::in); // opening words file
		fAnto.open("Antonym.txt", ios::in); // opening Antonym file
		fSyno.open("Synonym.txt", ios::in); // opening Synonym file

		if (!fWord && !fSyno && !fAnto)
		{
			cout << "File not Open\n";
		}
		else
		{
			string word, anto, syno;

			while (!fWord.eof())
			{
				fWord >> word;
				fAnto >> anto;
				fSyno >> syno;

				int index=Hashing(word); // Getting index of word 

				list[index].insertForLookUp(word, anto, syno); // inserting data by using index of word
			}
			fWord.close(), fAnto.close(), fSyno.close();
		}
	}

	// Search function to find Synonym and Antonym 
	void search(string word)
	{
		int index = Hashing(word); // Getting the index of searching word

		Node *temp=list[index].searchForLookUP(list[index].getHead(), word); // Getting the address of found word

		if (temp == NULL)
		{
			cout << "Word not Found\n";
		}
		else
		{
			cout << "Word\t\tSynonym\t\t\tAntonym\n-----------------------------------------------------\n";
			cout << temp->getData() << "\t\t" << temp->getSynonym() << "\t\t" << temp->getAntonym() << endl << endl;
		}

	}

};

int main()
{
	fstream fin, fout;
	stack <string> s;
	LinkedList l;
	HashTable h;
	int scenarioChoice, option,position;
	string word,newWord;
	bool flag = true;
	bool Tflag = true;


	cout << "********************************************\n";
	cout << "*                                          *\n";
	cout << "*       Console Based Text Editor          *\n";
	cout << "*                                          *\n";
	cout << "********************************************\n\n";

	cout << " Available Options : \n\n";
	cout << " 1. Word-by-Word Menu\n";
	cout << " 2. Complete Sentence Menu\n\n";

	cout << " Enter your choice (1-2) : ";
	cin >> scenarioChoice;

	system("cls");
	if (scenarioChoice == 1)
	{
		do
		{
		cout << " 1. Insert a word\n 2. Delete a specific word\n 3. Replace a specific word\n 4. Undo\n 5. Redo\n 6. Save in file\n 7. Load from file"
		"\n 8. Find and replace word(all words will be replaced)\n 9. Find total Words\n 10. Antonym and Synonym Lookup\n"
		" 11. Display\n 12. Exit\n Enter your choice : "; cin >> option;
		system("cls");
		switch (option)
		{
		case 1:
			flag = true;
			cin.ignore();
			cout << "Enter Word : ";
			getline(cin, word);

			for (char c : word)
			{
				if (c == ' ')
				{
					flag = false;
					cout << "Words with Space are not Allowed\n";
					break;
				}
			}
			if (flag)
			{
				l.insert(word);
			}
					
			break;

		case 2:
			if (!l.isEmpty())
			{
				l.display(l.getHead());
				cout << "\n-----------------------------------------------\n";
				cout << "Enter Word to Delete : ";
				cin >> word;
				bool flag=l.search(l.getHead(), word);
				if (flag == true)
				{
					l.searchForSpecific(l.getHead(), word);
					cout << "Enter position of the word you want to Delete : ";
					cin >> position;
					
					flag=l.deleteForSpecific(l.getHead(), word, position);
					if (flag)
					{
						cout << "Data Deleted\n";
					}
					else
					{
						cout << "Invalid Position\n";
					}
				}
			}
			else
			{
				cout << "List is Empty\n";
			}
	
			break;

		case 3:
			if (!l.isEmpty())
			{
				l.display(l.getHead());
				cout << "\n-----------------------------------------------\n";
				cout << "Enter Word to Replace : ";
				cin >> word;
				bool flag=l.search(l.getHead(), word);
				
				if (flag == true)
				{
					l.searchForSpecific(l.getHead(), word);
					cout << "Enter New Word : ";
					cin >> newWord;
					cout << "Enter position of the word you want to Replace : ";
					cin >> position;
					flag=l.updateForSpecific(l.getHead(), word, newWord, position);
					if (flag)
					{
						cout << "Data Replaced\n";
					}
					else
					{
						cout << "Invalid Position\n";
					}
				}
			}
			else
			{
				cout << "List is Empty\n";
			}

			break;

		case 4:
		
			if (!l.isEmpty())
			{
				s.push(l.getTail()->getData());
				l.deleteAtLast();
				cout << "Undo Successfully\n";
			}
			else
			{
				cout << "List is Empty\n";
			}
			break;

		case 5:

			if (s.empty())
			{
				cout << "No data for Redo\n";
			}
			else
			{
				word = s.top();
				l.insert(word);
				s.pop();
				cout << "Redo Successfully\n";
			}
			break;

		case 6:

			if (!l.isEmpty())
			{
				fout.open("Save for Words.txt", ios::app);
				if (fout)
				{
					l.saveInFile(l.getHead(), fout);
					fout.close();
					cout << "Data has been Saved in File\n";
				}
				else
				{
					cout << "File not Found\n";
				}
			}
			else
			{
				cout << "List is Empty\n";
			}

			break;
		case 7:
			
			fin.open("Words file.txt", ios::in);
			if (fin)
			{
				while (!fin.eof())
				{
					string word1;
					fin >> word1;
					if (word1 == "")
					{
						cout << "No data in File\n";
						fin.close();
						Tflag = false;
						break;
					}
					else
					{
						l.insert(word1);
						true;
					}
				}
				if (Tflag)
				{
					cout << "Data Added\n";
					fin.close();
				}

			}
			else
			{
				cout << "File not Found\n";
			}

			break;
		case 8:
			if (!l.isEmpty())
			{
				l.display(l.getHead());
				cout << "\n-----------------------------------------------\n";
				cout << "Enter Word to Replace : ";
				cin >> word;
				cout << "Enter New Word : ";
				cin >> newWord;
				int notFound = 0;
				l.replaceAllWords(l.getHead(), word, newWord, notFound);

				if (notFound > 0)
				{
					cout << "Data Replaced\n";
				}
				else
				{
					cout << "Word not found\n";
				}
			}
			else
			{
				cout << "List is Empty\n";
			}
			break;

		case 9:

			if (!l.isEmpty())
			{
				int count=l.totalWords(l.getHead());
				cout << "Total words are : " << count << endl;
			}
			else
			{
				cout << "List is Empty\n";
			}

			break;

		case 10:

			h.insert();
			cout << "Enter Word : ";
			cin >> word;
			system("cls");
			h.search(word);

			break;
		case 11:
			if (!l.isEmpty())
			{
				l.display(l.getHead());
				cout << endl;
			}
			else
			{
				cout << "List is Empty\n";
			}
			break;

		case 12:
			return 0;
			break;

		default:
			cout << "Invalid Input\n";
			break;
		}
		system("pause");
		system("cls");

		} while (option != 12);

	}
	else if (scenarioChoice == 2)
	{
		do
		{
			cout << " 1. Insert a sentence\n 2. Delete a specific sentence\n 3. Replace a specific sentence\n 4. Undo\n 5. Redo"
				"\n 6. Save in file\n 7. Load from file\n 8. Find and replace sentence(all sentences will be replaced)"
				"\n 9. Find total Words\n 10. Antonym and Synonym Lookup\n 11. Display\n 12. Exit\n Enter your choice : "; cin >> option;
			system("cls");
			switch (option)
			{
			case 1:
				cin.ignore();
				cout << "Enter Sentence : ";
				getline(cin, word);
				l.insert(word);

				break;

			case 2:
				if (!l.isEmpty())
				{
					cin.ignore();
					l.display(l.getHead());
					cout << "\n-----------------------------------------------\n";
					cout << "Enter Sentence to Delete : ";
					getline(cin, word);
					bool flag = l.search(l.getHead(), word);
					if (flag == true)
					{
						l.searchForSpecific(l.getHead(), word);
						cout << "Enter position of the Sentence you want to Delete : ";
						cin >> position;
						flag=l.deleteForSpecific(l.getHead(), word, position);
						
						if (flag)
						{
							cout << "Data Deleted\n";
						}
						else
						{
							cout << "Invalid Position\n";
						}
					}
				}
				else
				{
					cout << "List is Empty\n";
				}

				break;

			case 3:
				if (!l.isEmpty())
				{
					cin.ignore();
					l.display(l.getHead());
					cout << "\n-----------------------------------------------\n";
					cout << "Enter Sentence to Replace : ";
					getline(cin, word);
					bool flag=l.search(l.getHead(), word);
					
					if (flag == true)
					{
						l.searchForSpecific(l.getHead(), word);
						cout << "Enter New Sentence : ";
						getline(cin, newWord);
						cout << "Enter position of the Sentence you want to Replace : ";
						cin >> position;
						flag=l.updateForSpecific(l.getHead(), word, newWord, position);
						if (flag)
						{
							cout << "Data Replaced\n";
						}
						else
						{
							cout << "Invalid Position\n";
						}
					}
				}
				else
				{
					cout << "List is Empty\n";
				}

				break;

			case 4:

				if (!l.isEmpty())
				{
					s.push(l.getTail()->getData());
					l.deleteAtLast();
					cout << "Undo Successfully\n";
				}
				else
				{
					cout << "List is Empty\n";
				}
				break;

			case 5:

				if (s.empty())
				{
					cout << "No data for Redo\n";
				}
				else
				{
					word = s.top();
					l.insert(word);
					s.pop();
					cout << "Redo Successfully\n";
				}

				break;

			case 6:

				if (!l.isEmpty())
				{
					fout.open("Save for Sentences.txt", ios::app);
					if (fout)
					{
						l.saveInFile(l.getHead(), fout);
						fout.close();
						cout << "Data has been Saved in File\n";
					}
					else
					{
						cout << "File not Found\n";
					}
				}
				else
				{
					cout << "List is Empty\n";
				}
				break;
			case 7:
				fin.open("Sentences file.txt", ios::in);
				
				l.sentenceSeparate(fin,l);

				break;
			case 8:
				if (!l.isEmpty())
				{
					cin.ignore();
					l.display(l.getHead());
					cout << "\n-----------------------------------------------\n";
					cout << "Enter Sentence to Replace : ";
					getline(cin, word);
					cout << "Enter Sentence Word : ";
					getline(cin, newWord);
					int notFound = 0;
					l.replaceAllWords(l.getHead(), word, newWord, notFound);

					if (notFound > 0)
					{
						cout << "Data Replaced\n";
					}
					else
					{
						cout << "Word not found\n";
					}
				}
				else
				{
					cout << "List is Empty\n";
				}
				break;

			case 9:

				if (!l.isEmpty())
				{
					int count = l.totalWordsSent(l.getHead());
					cout << "Total words are : " << count << endl;
				}
				else
				{
					cout << "List is Empty\n";
				}

				break;

			case 10:

				h.insert();
				cout << "Enter Word : ";
				cin >> word;
				system("cls");
				h.search(word);

				break;
			case 11:
				if (!l.isEmpty())
				{
					l.display(l.getHead());
					cout << endl;
				}
				else
				{
					cout << "List is Empty\n";
				}
				break;

			case 12:
				return 0;
				break;

			default:
				cout << "Invalid Input\n";
				break;
			}
			system("pause");
			system("cls");

		} while (option != 12);
	}
	else
	{
	cout << "Invalid Input\n";
	}


	return 0;
}

