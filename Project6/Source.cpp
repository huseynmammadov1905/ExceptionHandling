#include <iostream>
#include<ctime>
using namespace std;





// Database elave olunsun
void deleteUserById(const int& id);//eger bele bir id istifadechi yoxdursa exception atilsin
void hashUserDataById(const int& id);//gonderilen id deki istifadechinin butun melumatlari hash olunsun(string olanlar)digerleri qalsin


class Exception : public exception {

	string text;
	int line;
	string source;
	time_t now = time(0);

#pragma warning (disable : 4996)
	char* d = asctime(localtime(&now));
public:

	Exception(string text, int line, string source) {
		this->text = text;
		this->line = line;
		this->source = source;

	}

	void Message() const {
		cout << text << endl << line << endl << source << endl;
	}

}; // text, line, source, time. //cari vaxti alinmasini internetden arashdirin
class DatabaseException :public Exception {
public:
	DatabaseException(string text, int line, string source) : Exception(text, line, source) {

	}
};
class InvalidArgumentException :public Exception {
public:
	InvalidArgumentException(string text, int line, string source) : Exception(text, line, source) {

	}
};


class User {
	string username;
	string password;
	string name;
	string surname;
	static int id;
public:

	User(string username, string password, string name, string surname) {
		this->id++;
		this->username = username;
		this->password = password;
		this->name = name;
		this->surname = surname;
	}
	string GetUsername() const { return username; }
	string GetPassword() const { return password; }
	string GetName() const { return name; }
	string GetSurname() const { return surname; }

	static int GetId() { return id; }

	void show() const {
		cout << "username : " << username << endl << "password : " << password << endl << "name : " << name << "surname : " << surname << endl;
	}

	bool operator == (User& obj) {
		return (this->username == obj.GetUsername() && this->password == obj.GetPassword() && this->name == obj.GetName() && this->surname == obj.GetSurname());
	}


};

int User::id = 1;

class Database {
	User** users;
	int user_count = 0;
public:
	void addUser(const User& user) {
		User** temp = new User * [user_count + 1];
		for (int i = 0; i < user_count; i++)
		{
			temp[i] = users[i];
		}
		temp[user_count] = new User(user.GetUsername(), user.GetPassword(), user.GetName(), user.GetSurname());
		user_count++;
		delete users;
		users = temp;
		temp = nullptr;
	}

	User& getUserByUsername(string username) {

		if (user_count <= 0) {
			throw DatabaseException("empty", __LINE__, " ");
		}
		else {
			for (int i = 0; i < user_count; i++)
			{
				username == users[i]->GetUsername();
				return *users[i];
			}
			throw DatabaseException("User not found", __LINE__, " ");
		}

	}

	bool getUserByUserNameBool(string username) {

		if (user_count <= 0) {
			return false;
		}
		else {
			for (int i = 0; i < user_count; i++)
			{
				if (username == users[i]->GetUsername());

				return true;
			}
			return false;
		}

	}
	void updateUser(User& olduser, const User& newuser) {
		if (user_count <= 0) {
			throw DatabaseException("empty", __LINE__, " ");
		}
		else {
			for (int i = 0; i < user_count; i++)
			{
				if (*users[i] == olduser) {
					delete users[i];
					User* t = new User(newuser.GetUsername(), newuser.GetPassword(), newuser.GetName(), newuser.GetSurname());
					users[i] = t;
				}
				else DatabaseException("User not found", __LINE__, " ");
			}
		}


	}

	void deleteUserById(const int& id) {

		User** temp = new User * [user_count - 1];

		for (int i = 0; i < id - 1; i++)
		{
			temp[i] = users[i];

		}

		for (int i = id; i < user_count; i++)
		{
			temp[i] = users[i];
		}
		user_count--;
		users = temp;
		temp = nullptr;
	}
	void hashUserDataById(const int& id);
};


class Registration {
	Database _database;
public:
	Registration(const Database& database) {
		_database = database;
	}


	Database& getDatabase() {
		return _database;
	}

	//eger bu username istifadechi yoxdursa error atsin
	//eger username varsa amma password yanlishdirsa error atsin
	void signIn(string username, string password) {
		User t = _database.getUserByUsername(username);
		if (t.GetUsername() == username) {



			if (t.GetPassword() == password) {
				cout << "Succesfull" << endl;
			}
			else throw DatabaseException("wrong password", __LINE__, " ");

		}
		else throw DatabaseException("wrong username", __LINE__, " ");
	}


	//Eger istifadechi varsa hemen username de throw DatabaseException
	//Eger username xarakter sayi 6 dan kichikdirse InvalidArgumentException
	//Eger username ilk herf kichikdirse InvalidArgumentException
	//Eger password xarakter sayi 6 dan kichikdirse InvalidArgumentException
	//Eger name xarakter sayi 4 dan kichikdirse InvalidArgumentException
	//Eger surname xarakter sayi 4 dan kichikdirse InvalidArgumentException
	void signUp(string username, string password, string name, string surname) {
		if (_database.getUserByUserNameBool(username)) {
			throw DatabaseException("wrong", __LINE__, " ");
		}
		if (username.size() < 6)
			throw InvalidArgumentException("username_size >= 6 ", __LINE__, " ");
		if (username[0] >= char(97) && username[0] <= char(122))
			throw InvalidArgumentException("username first char must large ", __LINE__, " ");
		if (password.size() < 6)
			throw InvalidArgumentException("password_size >= 6 ", __LINE__, " ");
		if (name.size() < 4)
			throw InvalidArgumentException("name_size >= 4 ", __LINE__, " ");
		if (surname.size() < 4)
			throw InvalidArgumentException("surname_size >= 4 ", __LINE__, " ");

	}


};


class System {
public:
	static void Control() {

		try
		{
			Database db;
			Registration twitter(db);
			User user("huseyn2000", "12345", "huseyn", "mammadov");

			twitter.getDatabase().addUser(user);
			twitter.signIn("huseyn2000", "12345");

		}
		catch (Exception ex)
		{
			ex.Message();
		}




		//mainde yazilacaq her shey burda yazilsin.
	}

};

void main() {
	System::Control();


	system("pause");
}