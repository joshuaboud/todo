#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<ctime>
using namespace std;

class item{
	friend class list;
private:
	string title;
	string date;
public:
	item(string title_, string date_);
	string daysLeft(void) const;			// returns days between today and set date
};

class list{
private:
	vector<item> items;
public:
	bool empty(void) const;
	void load(istream & in);				// load vector of items from file
	void addItem(void);
	void removeItem(unsigned int num_);
	void save(ostream & out) const;			// save vector to file
	void print(ostream & out) const;
	int size(void) const;
};

int main(int argc, char *argv[]){
	const char * path = "/home/joshuaboud/todoList.dat";
	ifstream fin(path);			// load
	list myList;
	myList.load(fin);
	fin.close();
	
	ofstream f(path);
	if(argc == 1){							// if no arguments, just display list
		if(myList.empty()){
			cout << "List is all done. :)\n";
		}else{
			myList.print(cout);
		}
	}else{		
		if(string(argv[1]) == "-a"){		// "todo -a" -> submenu for adding item
			myList.addItem();
		}else if(string(argv[1]) == "-r"){	// "todo -r #" -> removes item number #
			istringstream iss(argv[2]);
			int val;
			if(iss >> val){
				if(val > myList.size()){
					cout << "Not a valid list number.\n";
				}else{
					myList.removeItem(val);
				}
			}
		}
	}
	myList.save(f);
	f.close();
	return 0;
}

item::item(string title_, string date_){
	title = title_;
	date = date_;
}

void list::load(istream & in){
	string title, date;
	while(getline(in, title, ',') && getline(in, date, '\n')){
		item temp(title, date);
		items.push_back(temp);
	}
	return;
}

void list::addItem(void){
	string title_;
	string date_;
	
	cout << "\nItem to add: ";
	getline(cin, title_, '\n');
	
	cout << "\nDate YY/MM/DD (optional): ";
	getline(cin, date_, '\n');				// get date as string
	
	if(date_ == "")							// if no date, 'N/A'
		date_ = "N/A";
	
	item x(title_, date_);
	items.push_back(x);
	return;
}

void list::removeItem(unsigned int num_){
	if(num_ <= items.size()){
		cout << "Removed item: " << items[num_-1].title << endl;
		for(unsigned int i = num_-1; i<items.size()-1; i++){	// shift all items after num back
			items[i] = items[i+1];
		}
		items.pop_back();										// delete last
	}
	return;
}

void list::save(ostream & out) const{
	for(unsigned int i = 0; i < items.size(); i++){
		item temp = items[i];
		out << temp.title << "," << temp.date << endl;
	}
	return;
}

void list::print(ostream & out) const{
	out << "TODO:\n";
	for(unsigned int i = 0; i<items.size(); i++){
		out << i+1 << ". " << items[i].title << ". " << items[i].daysLeft() << " days left. (due " << items[i].date << ")\n";
	}
	return;
}

string item::daysLeft(void) const{
	if(date == "N/A"){
		return "???";
	}
	string  yy_, mm_, dd_;
	int leap = 0, daysThen, daysNow, diff;
	
	istringstream iss(date);			// set item's date as instream iss
	
	getline(iss, yy_, '/');
	getline(iss, mm_, '/');
	getline(iss, dd_);
	
	int yy = stoi(yy_);
	int mm = stoi(mm_);
	int dd = stoi(dd_);
	
	if(yy%4 == 0){
		leap = 1;
	}
	int mdays[12] = {31, 28 + leap, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	daysThen = (yy+100)*365.25;			// calculate days from 1900
	for(int i = 0; i < mm - 1; i++){
		daysThen += mdays[i];
	}
	daysThen += dd;
	
	time_t now = time(0);				// calculate days from 1900
	tm *ltm = localtime(&now);
	
	daysNow = ltm->tm_year * 365.25;	// ltm->tm_year is years from 1900
	for(int i = 0; i < ltm->tm_mon; i++){
		daysNow += mdays[i];
	}
	daysNow += ltm->tm_mday;
	
	diff = daysThen - daysNow;
	
	return to_string(diff);
}

bool list::empty(void) const{
	return items.empty();
}

int list::size(void) const{
	return items.size();
}
