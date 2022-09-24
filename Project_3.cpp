#include <iostream>
#include <string>
#include <list>
#include <cstring>
#include <vector>
#include <stack>
#include <map>
#include <iterator>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <fstream>
using namespace std;

class Player
{
private:
	string name;            // Player's name
	string position;        // Position on the floor
	int age;                // Age of player
	int games;              // Number of games played in the current season
	int minPlayed;          // Minutes of playtime
	float PER;              // Player efficiency rating
	float DPM;              // Daily Plus-Minus
	float TSp;              // True shooting %
	float threePAR;         // 3 point attempt rate
	float FTR;              // Free throw attempting
	float ORBp;             // Offensive Rebound per game
	float DRBp;             // Defensive Rebound per game
	float ASTp;             // Assist per game
	float STLp;             // Steal per game
	float BLKp;             // Block per game
	float TOVp;             // Turnover per game
public:
	Player();
	Player(string name, string position, int age, int games, int minPlayed, float PER, float DPM, float TSp, float threePAR, float FTR, float ORBp, float DRBp, float ASTp, float STLp, float BLKp, float TOVp);
	void printStat();
	string getName();
	float getPER();
};

class HashTable {
public:
	HashTable(); // Default Constructor
	int hashFunc(string name); // String Hash Function
	void insertPlayer(string name, Player p); // Insertion into Hash Table
	void getPlayer(string name); // Return Player value of string key
	void rehash(); // rehash table if load_factor is exceeded
	void printTop(); // Prints top ten Player based  on PER value

private:
	int table_size; // Size of hash table
	static const int initial_size = 1000; // Initial size of hash table
	vector<list<pair<string, Player>>> table; // Hash Table
	int total; // tota; values
	const float max_load = 0.8;
	float load_factor; // Current load factor
};

void HeapifyDown(Player arr[], int sizeOfArr, int currentPosition);
void HeapifyUp(Player arr[], int size, int currentPosition);
void MaxHeapInsertion(Player arr[], int& sizeOfArr, Player playerToAdd);
void PrintTopTen(Player arr[], int& sizeOfArr, vector<Player>& topTenPlayers);
bool FindPlayerToCompare(Player arr[], stack<Player>& compareHeap, string nameToFind, int numOfLines);

int main() {
	cout << "-------- Welcome to our NBA statistics tool! --------\n";
	HashTable hash_table;
	ifstream file("playerList.txt");

	int sizeOfArr = 0;

	std::chrono::steady_clock::time_point hash_start, hash_end, heap_start, heap_end;
	double hash_time_taken = 0.0, heap_time_taken = 0.0;

	bool end = true;
	int age, games, minPlayed;
	float PER, DPM, TSp, threePAR, FTR, ORBp, DRBp, ASTp, STLp, BLKp, TOVp;
	string line, position, name;

	getline(file, line, ',');
	int num_of_lines = stoi(line);
	getline(file, line, '\n');
	int columns = stoi(line);

	Player* arr = new Player[num_of_lines];

	if (file.is_open()) {
		while (end) {
			if (getline(file, line, ',')) {
				name = line;
				for (int i = 0; i < columns - 2; i++) {
					getline(file, line, ',');
					if (i == 0) {
						position = line;
					}
					else if (i == 1) {
						age = stoi(line);
					}
					else if (i == 2) {
						games = stoi(line);
					}
					else if (i == 3) {
						minPlayed = stoi(line);
					}
					else if (i == 4) {
						PER = stof(line);
					}
					else if (i == 5) {
						DPM = stof(line);
					}
					else if (i == 6) {
						TSp = stof(line);
					}
					else if (i == 7) {
						threePAR = stof(line);
					}
					else if (i == 8) {
						FTR = stof(line);
					}
					else if (i == 9) {
						ORBp = stof(line);
					}
					else if (i == 10) {
						DRBp = stof(line);
					}
					else if (i == 11) {
						ASTp = stof(line);
					}
					else if (i == 12) {
						STLp = stof(line);
					}
					else if (i == 13) {
						BLKp = stof(line);
					}
				}
				getline(file, line, '\n');
				TOVp = stof(line);

				Player p(name, position, age, games, minPlayed, PER, DPM, TSp, threePAR, FTR, ORBp, DRBp, ASTp, STLp, BLKp, TOVp);
				hash_start = std::chrono::high_resolution_clock::now();
				ios_base::sync_with_stdio(false);
				hash_table.insertPlayer(name, p);
				hash_end = std::chrono::high_resolution_clock::now();
				hash_time_taken += chrono::duration_cast<chrono::nanoseconds>(hash_end - hash_start).count() * 1e-9;



				//MaxHeapInsertion(arr, num_of_lines, p); //example of adding playerObj to heap //change # to playerObj[i]
				heap_start = std::chrono::high_resolution_clock::now();
				ios_base::sync_with_stdio(false);
				MaxHeapInsertion(arr, sizeOfArr, p);
				heap_end = std::chrono::high_resolution_clock::now();
				heap_time_taken += chrono::duration_cast<chrono::nanoseconds>(heap_end - heap_start).count() * 1e-9;



			}
			else {
				end = false;
			}
		}
	}
	file.close();

	cout << "\nTime to create hash table: " << fixed << hash_time_taken << setprecision(6) << " sec" << endl;
	cout << "Time to create max heap: " << fixed << heap_time_taken << setprecision(6) << " sec" << endl;
	hash_time_taken = 0.0;
	heap_time_taken = 0.0;

	// Menu options
	int menu_option, option_1, option_1_2 = 0, which;
	bool found;
	string user_input1, user_input2;
	stack<string> compare;
	stack<Player> compareHeap;


	do {
		cout << "\nPlease choose an option below by typing in a number:\n";
		cout << "1: Search A Player's Stats/PER\n2: Display Top 10 Ranked Players\n3: EXIT";
		cout << endl;
		cin >> menu_option;

		if (menu_option == 1) {
			cout << "\nWould you like to gather data using a hash table (1) or a max heap (2)?\n";
			cin >> which;
			while (which < 1 || which > 2) {
				cout << "Invalid selection! Enter an number listed" << endl;
				cin >> which;
			}
			cout << endl;
			cout << "Enter a player name:\n";
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, user_input1);
			cout << endl;





			if (which == 1) {
				// display 1 player's stats/rank (in player class) - hash table
				hash_start = std::chrono::high_resolution_clock::now();
				ios_base::sync_with_stdio(false);
				hash_table.getPlayer(user_input1);
				hash_end = std::chrono::high_resolution_clock::now();
				hash_time_taken = chrono::duration_cast<chrono::nanoseconds>(hash_end - hash_start).count() * 1e-9;
				cout << "\nTime to retrieve: " << fixed << hash_time_taken << setprecision(6) << " sec" << endl;
				hash_time_taken = 0.0;
				do {
					cout << "\nWould you like to...\n1: Compare Stats/PER To Another Player\n2: Main Menu\n3: EXIT\n";
					cin >> option_1;
					cout << endl;

					if (option_1 == 1) {
						compare.push(user_input1);
						cout << "Enter another player's name:\n";
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						getline(cin, user_input1);
						cout << endl;
						compare.push(user_input1);

						// display 2 player's stats/rank (in player class)
						hash_start = std::chrono::high_resolution_clock::now();
						ios_base::sync_with_stdio(false);
						hash_table.getPlayer(compare.top());
						cout << endl << endl;
						compare.pop();
						hash_table.getPlayer(compare.top());
						hash_end = std::chrono::high_resolution_clock::now();
						hash_time_taken = chrono::duration_cast<chrono::nanoseconds>(hash_end - hash_start).count() * 1e-9;
						cout << "\nTime to retrieve: " << fixed << hash_time_taken << setprecision(6) << " sec" << endl;
						hash_time_taken = 0.0;

						do {
							cout << "\nWould you like to...\n1: Compare Stats/PER To Another Player\n2: Main Menu\n3: EXIT\n";
							cin >> option_1_2;
							cout << endl;

							if (option_1_2 == 1) {
								cout << "Enter another player's name:\n";
								cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
								getline(cin, user_input1);
								cout << endl;
								compare.push(user_input1);

								// display 2 player's stats/rank (in player class)
								hash_start = std::chrono::high_resolution_clock::now();
								ios_base::sync_with_stdio(false);
								hash_table.getPlayer(compare.top());
								cout << endl << endl;
								compare.pop();
								hash_table.getPlayer(compare.top());
								hash_end = std::chrono::high_resolution_clock::now();
								hash_time_taken = chrono::duration_cast<chrono::nanoseconds>(hash_end - hash_start).count() * 1e-9;
								cout << "\nTime to retrieve: " << fixed << hash_time_taken << setprecision(6) << " sec" << endl;
								hash_time_taken = 0.0;

								continue;

							}
							else if (option_1_2 == 2 || option_1_2 == 3) {
								compare.pop();
								break;
							}
							else {
								cout << "Invalid selection! Enter an number listed" << endl;
								option_1_2 = 1;
							}

						} while (option_1_2 >= 1 && option_1_2 <= 3);

					}
					else if (option_1 == 2 || option_1 == 3) {
						break;
					}
					else {
						cout << "Invalid selection! Enter an number listed" << endl;
						option_1 = 1;
						continue;
					}
					if (option_1_2 == 2) {
						option_1 = 2;
						break;
					}

					break;

				} while (option_1 >= 1 && option_1 <= 3);

				if (option_1 == 2) {
					continue;
				}

				break;
			}
			else {
				// display 1 player's stats/rank (in player class) - heap
				//cout << endl;
				heap_start = std::chrono::high_resolution_clock::now();
				ios_base::sync_with_stdio(false);
				found = FindPlayerToCompare(arr, compareHeap, user_input1, num_of_lines);
				if (found)
				{
					compareHeap.top().printStat();
					heap_end = std::chrono::high_resolution_clock::now();
					heap_time_taken = chrono::duration_cast<chrono::nanoseconds>(heap_end - heap_start).count() * 1e-9;
					cout << "\nTime to retrieve: " << fixed << heap_time_taken << setprecision(6) << " sec" << endl;
					heap_time_taken = 0.0;
				}
				do {
					cout << "\nWould you like to...\n1: Compare Stats/PER To Another Player\n2: Main Menu\n3: EXIT\n";
					cin >> option_1;
					cout << endl;

					if (option_1 == 1) {

						cout << "Enter another player's name:\n";
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						getline(cin, user_input1);
						//cout << endl;
						found = FindPlayerToCompare(arr, compareHeap, user_input1, num_of_lines);
						heap_start = std::chrono::high_resolution_clock::now();
						ios_base::sync_with_stdio(false);
						if (found) {
							compareHeap.top().printStat();
							compareHeap.pop();
						}
						cout << endl << endl;

						if (!compareHeap.empty())
						{
							compareHeap.top().printStat();
							heap_end = std::chrono::high_resolution_clock::now();
							heap_time_taken = chrono::duration_cast<chrono::nanoseconds>(heap_end - heap_start).count() * 1e-9;
							cout << "\nTime to retrieve: " << fixed << heap_time_taken << setprecision(6) << " sec" << endl;
							heap_time_taken = 0.0;

						}
						else
						{
							cout << "This person is not an NBA Player\n";
							heap_end = std::chrono::high_resolution_clock::now();
							heap_time_taken = chrono::duration_cast<chrono::nanoseconds>(heap_end - heap_start).count() * 1e-9;
							heap_time_taken = 0.0;
						}

						// display 2 player's stats/rank (in player class)

						do {
							cout << "\nWould you like to...\n1: Compare Stats/PER To Another Player\n2: Main Menu\n3: EXIT\n";
							cin >> option_1_2;
							cout << endl;

							if (option_1_2 == 1)
							{
								cout << "Enter another player's name:\n";
								cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
								getline(cin, user_input1);
								found = FindPlayerToCompare(arr, compareHeap, user_input1, num_of_lines);
								heap_start = std::chrono::high_resolution_clock::now();
								ios_base::sync_with_stdio(false);
								if (found)
								{
									cout << endl;
									compareHeap.top().printStat();
									compareHeap.pop();
								}

								cout << endl << endl;

								if (!compareHeap.empty())
								{
									//cout << endl;
									compareHeap.top().printStat();
									heap_end = std::chrono::high_resolution_clock::now();
									heap_time_taken = chrono::duration_cast<chrono::nanoseconds>(heap_end - heap_start).count() * 1e-9;
									cout << "\nTime to retrieve: " << fixed << heap_time_taken << setprecision(6) << " sec" << endl;
									heap_time_taken = 0.0;
								}
								else
								{
									cout << "This person is not an NBA Player\n";
									heap_end = std::chrono::high_resolution_clock::now();
									heap_time_taken = chrono::duration_cast<chrono::nanoseconds>(heap_end - heap_start).count() * 1e-9;
									heap_time_taken = 0.0;
								}

								continue;

							}
							else if (option_1_2 == 2 || option_1_2 == 3) {

								if (!compareHeap.empty())
								{
									compareHeap.pop();
								}

								break;
							}
							else {
								cout << "Invalid selection! Enter an number listed" << endl;
								option_1_2 = 1;
							}

						} while (option_1_2 >= 1 && option_1_2 <= 3);

					}
					else if (option_1 == 2 || option_1 == 3) {
						break;
					}
					else {
						cout << "Invalid selection! Enter an number listed" << endl;
						option_1 = 1;
						continue;
					}
					if (option_1_2 == 2) {
						option_1 = 2;
						break;
					}

					break;

				} while (option_1 >= 1 && option_1 <= 3);

				if (option_1 == 2) {
					continue;
				}

				break;
			}
		}
		else if (menu_option == 2) {
			// Uses Brian's Max Heap and sort of first 15 nodes
			cout << "\nWould you like to gather data using a hash table (1) or a max heap (2)?\n";
			cin >> which;
			cout << endl;
			while (which < 1 || which > 2) {
				cout << "Invalid selection! Enter an number listed" << endl;
				cin >> which;
			}
			if (which == 1) {
				hash_start = std::chrono::high_resolution_clock::now();
				ios_base::sync_with_stdio(false);
				hash_table.printTop();
				hash_end = std::chrono::high_resolution_clock::now();
				hash_time_taken = chrono::duration_cast<chrono::nanoseconds>(hash_end - hash_start).count() * 1e-9;
				cout << "\nTime to retrieve: " << fixed << hash_time_taken << setprecision(6) << " sec" << endl;
				hash_time_taken = 0.0;
			}
			else {
				heap_start = std::chrono::high_resolution_clock::now();
				ios_base::sync_with_stdio(false);
				vector<Player> topTenPlayers;
				PrintTopTen(arr, sizeOfArr, topTenPlayers);
				heap_end = std::chrono::high_resolution_clock::now();
				heap_time_taken = chrono::duration_cast<chrono::nanoseconds>(heap_end - heap_start).count() * 1e-9;
				cout << "\nTime to retrieve: " << fixed << heap_time_taken << setprecision(6) << " sec" << endl;
				heap_time_taken = 0.0;
			}

		}
		else if (menu_option == 3) {
			break;
		}
		else {
			cout << "Invalid selection! Enter an number listed" << endl;
			menu_option = 1;
		}

	} while (menu_option >= 1 && menu_option <= 3);

	cout << "\nThank You For Using Our Tool/PER!";

	return 0;
}

// Player constructor
Player::Player()
{
	this->name = "";
	this->position = "";
	this->age = 0;
	this->games = 0;
	this->minPlayed = 0;
	this->PER = 0;
	this->DPM = 0;
	this->TSp = 0;
	this->threePAR = 0;;
	this->FTR = 0;
	this->ORBp = 0;
	this->DRBp = 0;
	this->ASTp = 0;
	this->STLp = 0;
	this->BLKp = 0;
	this->TOVp = 0;
}

Player::Player(string name, string position, int age, int games, int minPlayed, float PER, float DPM, float TSp, float threePAR, float FTR, float ORBp, float DRBp, float ASTp, float STLp, float BLKp, float TOVp)
{
	this->name = name;
	this->position = position;
	this->age = age;
	this->games = games;
	this->minPlayed = minPlayed;
	this->PER = PER;
	this->DPM = DPM;
	this->TSp = TSp;
	this->threePAR = threePAR;
	this->FTR = FTR;
	this->ORBp = ORBp;
	this->DRBp = DRBp;
	this->ASTp = ASTp;
	this->STLp = STLp;
	this->BLKp = BLKp;
	this->TOVp = TOVp;
}

// Print out player's statistics
void Player::printStat()
{
	cout << "Name: " << name << endl;
	cout << "Position: " << position << endl;
	cout << "Age: " << age << endl;
	cout << "Games: " << games << endl;
	cout << "Minutes Played: " << minPlayed << endl;
	cout << "Player efficiency rating: " << PER << endl;
	cout << "Daily Plus-Minus: " << DPM << endl;
	cout << "True Shooting %: " << TSp << endl;
	cout << "3 Point attempt rate: " << threePAR << endl;
	cout << "Free throw attempt rate: " << FTR << endl;
	cout << "Offensive Rebound per game: " << ORBp << endl;
	cout << "Defensive Rebound per game " << DRBp << endl;
	cout << "Assist per game: " << ASTp << endl;
	cout << "Steal per game: " << STLp << endl;
	cout << "Block per game: " << BLKp << endl;
	cout << "Turnover per game: " << TOVp << endl;
}

// Return name of player
string Player::getName()
{
	return this->name;
}

// Return player efficiency rating
float Player::getPER()
{
	return this->PER;
}


HashTable::HashTable() // Default constructor
{
	total = 0;
	table_size = 1000;
	load_factor = (float)total / table_size;
	table.resize(initial_size);
}

int HashTable::hashFunc(string name) // Make string hashing function
{
	int sum = 0;
	for (int i = 0; i < name.size(); i++) {
		sum += (int)name[i] * i * 27;
	}

	return sum % table_size; // Returns index value
}

void HashTable::insertPlayer(string name, Player p) // Insert into Hash Table
{
	int index = hashFunc(name);
	table[index].emplace_back(name, p);
	total++;
	rehash();
	// no need to check for duplicate name keys
}

void HashTable::getPlayer(string name)
{
	int index = hashFunc(name), none = 0;
	if (table[index].size() == 0) {}
	else {
		for (auto itr = table[index].begin(); itr != table[index].end(); itr++) {
			if (itr->first == name) {
				itr->second.printStat();
				none = 1;
			}
		}
	}
	if (none == 0) {
		cout << "This person is not an NBA Player\n";
	}
}

void HashTable::rehash()
{

	load_factor = (float)total / table_size;

	if (load_factor >= max_load) {
		int temp = table_size, index;
		table_size *= 2;
		vector<list<pair<string, Player>>> new_table;
		new_table.resize(table_size);
		for (int i = 0; i < temp; i++) {
			if (table[i].size() == 0) {
				continue;
			}
			for (auto itr = table[i].begin(); itr != table[i].end(); itr++) {
				index = hashFunc(itr->first);
				new_table[index].emplace_back(itr->first, itr->second);

			}

		}
		table.resize(table_size);
		table = new_table;

	}
}


void HashTable::printTop()
{
	float lowest = 100, lowest_index = 0;
	string lowest_name;
	vector<Player> top;

	for (int i = 0; i < table_size; i++) {
		if (table[i].size() == 0) {
			continue;
		}
		else {
			for (auto itr = table[i].begin(); itr != table[i].end(); itr++) {

				if (top.size() < 10) {
					//top[itr->second.] = itr->first;
					top.push_back(itr->second);
					if (itr->second.getPER() < lowest) {
						lowest = itr->second.getPER();;
						lowest_index = top.size() - 1;
					}
				}
				else {
					if (itr->second.getPER() > lowest) {
						top[lowest_index] = itr->second;
						for (int j = 0; j < 10; j++) {
							if (j == 0) {
								lowest = top[j].getPER();
								lowest_index = j;
							}
							else {
								if (top[j].getPER() < lowest) {
									lowest = top[j].getPER();
									lowest_index = j;
								}
							}
						}
					}
				}
			}
		}
	}

	map<float, Player> order;
	for (int i = 0; i < 10; i++) {
		order[top[i].getPER()] = top[i];
	}
	int r = 0;
	for (auto it = order.rbegin(); it != order.rend(); it++) {
		cout << r + 1 << ":\n";
		r++;
		it->second.printStat();
		cout << endl;
	}
}

//got from prog quiz 7
//function to call heapify down on the array to organize it after each deletion/insertion
void HeapifyDown(Player arr[], int sizeOfArr, int currentPosition) //TA helped me create this function
{
	//need the values at the left and right child
	int leftNode = 2 * currentPosition + 1;
	int rightNode = 2 * currentPosition + 2;

	int maxValue = currentPosition;

	if (leftNode < sizeOfArr || rightNode < sizeOfArr)
	{
		if (leftNode < sizeOfArr && arr[leftNode].getPER() > arr[maxValue].getPER()) //left node exists && value at left node is greater than value at max val
		{
			maxValue = leftNode;
		}

		if (rightNode < sizeOfArr && arr[rightNode].getPER() > arr[maxValue].getPER()) //right node exists
		{
			maxValue = rightNode;
		}

		if (maxValue == currentPosition)
		{
			return;
		}
		else
		{
			Player tempPlayer = arr[currentPosition];
			arr[currentPosition] = arr[maxValue];
			arr[maxValue] = tempPlayer;
		}

		HeapifyDown(arr, sizeOfArr, maxValue);
	}
}


void HeapifyUp(Player arr[], int size, int currentPosition) //same as HeapifyDown but it does a bottom-up method to sort everything properly after each insertion
{
	int parentNode = (currentPosition - 1) / 2; //gets the index of the parent node

	if (arr[parentNode].getPER() >= 0 && arr[currentPosition].getPER() > arr[parentNode].getPER()) //only enter if the parentNode is greater than 0 and the current node val is > parent node value
	{
		swap(arr[currentPosition], arr[parentNode]); //swap child and parent nodes (at currentPosition and at parentNode index)

		HeapifyUp(arr, currentPosition, parentNode); //call this function where the currentPosition is updated as the "parent" node (recursion)
	}
}
//help with creating this function from "https://www.geeksforgeeks.org/insertion-and-deletion-in-heaps/" (Aman said using small snippets was ok as long as cited!!)
void MaxHeapInsertion(Player arr[], int& sizeOfArr, Player playerToAdd) //function that inserts a new value "playerObj" into the array (the heap)
{
	sizeOfArr++; //increases size of the heap (is a reference so the value changes everywhere!!)
	arr[sizeOfArr - 1] = playerToAdd; //insert the overallStat at end
	HeapifyUp(arr, sizeOfArr, sizeOfArr - 1);
}


Player* ExtractMax(Player arr[], int& sizeOfArr, vector<Player>& topTenPlayers) //used to get the max value from a max heap and reorganize it!!
{
	int currentPosition = 0;

	if (sizeOfArr == 1) //occurs when the size is at 1 and doesn't need to go through an organization algorithm (won't actually happen in our situation bc nodes are more than 100000)
	{
	}
	else //go through a heapify down function that organizes the unsorted heaps after each removal (first step is take the last value and put it as the first index then call heapify down)
	{
		sizeOfArr--;
		topTenPlayers.push_back(arr[0]);
		arr[0] = arr[sizeOfArr - 1];
		HeapifyDown(arr, sizeOfArr, currentPosition);
	}

	return arr;
}

void PrintTopTen(Player arr[], int& sizeOfArr, vector<Player>& topTenPlayers) //vector should only have at most 10 players at a time!!
{
	for (int i = 0; i < 10; i++)
	{
		ExtractMax(arr, sizeOfArr, topTenPlayers);
	}

	for (int i = 0; i < topTenPlayers.size(); i++)
	{
		cout << i + 1 << ":\n";
		topTenPlayers[i].printStat();
		MaxHeapInsertion(arr, sizeOfArr, topTenPlayers[i]);
		cout << endl;
	}
}


bool FindPlayerToCompare(Player arr[], stack<Player>& compareHeap, string nameToFind, int numOfLines)
{
	for (int i = 0; i < numOfLines; i++)
	{
		if (arr[i].getName() == nameToFind)
		{
			compareHeap.push(arr[i]);
			return true;
		}
	}
	cout << "\nThis person is not an NBA Player\n";
	return false;
}
