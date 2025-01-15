/**------------------------------------------
    Programming Project: Password Check
    Author: Zainab Ahmed
    Date: 3/29/24
-------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>
#include <algorithm>

using namespace std;

const string critrion_msg_length = "The selected password must be at least eight characters long.";
const string critrion_msg_upper = "The selected password must have at least one uppercase letter.";
const string critrion_msg_lower = "The selected password must have at least one lowercase letter.";
const string critrion_msg_digit = "The selected password must have at least one digit.";
const string critrion_msg_special_chars = "The selected password must have at least one of special characters: [@,!,#,^,&,*,$]";
const string critrion_msg_dictionary = "The selected password cannot contain a dictionary word.";
const string critrion_msg_leaked = "The selected password has been leaked.";

//Use this enum for check the characters of the password
const int CHECKS_NUM = 5;
enum check_names {CHECK_LENGTH, CHECK_UPPER, CHECK_LOWER, CHECK_DIGIT, CHECK_SPECIAL_CHAR};

vector<string> leaked_password_dic;
vector<string> english_dic;

//This functions reads in string in the file specified by the filename and fill word_dic dictionary. It going to be used to read both leaked passwords and word dictionary.
int readWordList(string filename, vector<string>& word_dic) {
    //Open and read file:
    fstream word_file;
    word_file.open(filename, ios::in);
    
    if (word_file.is_open()) {
        string line;
        while (getline(word_file, line)) {
            //Add each line into the vector:
            word_dic.push_back(line);
        }
        word_file.close(); //close file
        return 0;
    }
    else {
        cout << "Error opening file" << endl;
        return -1;
    }	
}


int compare(string word, string query) { 
    string lowercase_word;
    
    for (int i = 0; i < word.length(); i++) {
        if (isalpha(word[i]) || isdigit(word[i])) { 
            lowercase_word.push_back(tolower(word[i]));
        }
    }

    if (word == query) {
        return 0;
    }
    else if (lowercase_word < query) {
        return -1;
    }
    else {
        return 1;
    }
}


//for each rule if the passwords satisfy the rule, set the corresponding entry in the checks array to true
void checkPasswordChars(bool checks[CHECKS_NUM], const string& password) { //Was originally "string password"
    //Initialize checks to false:
    for (int i = 0; i< CHECKS_NUM; i++) {
        checks[i] = false;
    }
    
    //Check for at least 8 characters:
    if (password.size() >= 8) {
        checks[CHECK_LENGTH] = true;
    }

    //Check for at least one upper case letter:
    int upper_letters = 0;
    for (int i = 0; i < password.size(); i++) {
        if (isupper(password[i])) {
            upper_letters += 1;
        }
    }
    if (upper_letters >= 1) {
        checks[CHECK_UPPER] = true;
    }

    //Check for at least one lower case letter:
    int lower_letters = 0;
    for (int i = 0; i < password.size(); i++) {
        if (islower(password[i])) {
            lower_letters += 1;
        }
    }
    if (lower_letters >= 1) {
        checks[CHECK_LOWER] = true;
    }

    //Check for at least one digit:
    int digits = 0;
    for (int i = 0; i < password.size(); i++) {
        if (isdigit(password[i])) {
            digits += 1;
        }
    }
    if (digits >= 1) {
       checks[CHECK_DIGIT] = true;
    }


    //Check for at least one special character:
    int special_chars = 0;
    for (int i = 0; i < password.size(); i++) {
        if (password[i] == '@' || password[i] == '!' || password[i] == '#' || password[i] == '^' || password[i] == '&' || password[i] == '*' || password[i] == '$') {
            special_chars += 1;
        }
    }
    if (special_chars >= 1) {
        checks[CHECK_SPECIAL_CHAR] = true;
    }
}


//This function checks if the passowrd is among the leaked password provided by password dic
bool isInLeakedDic(vector<string> password_dic, string password){
    //Convert password to lower case:
    string lowercase_password;
    for (int i = 0; i < password.length(); i++) {
        if (isalpha(password[i]) || isdigit(password[i])) { 
            lowercase_password.push_back(tolower(password[i]));
        }
    }
    
    int min = 0;
    int max = password_dic.size() - 1;

    //Binary search:
    while (min <= max) {
        int middle = min + (max - min) / 2;
        string word = password_dic[middle];
       
        int compareResult = compare(word, lowercase_password);
        if (compareResult == 0) {
            return true; //password was found in password dictionary
        } 
        else if (compareResult < 0) {
            min = middle + 1;
        } 
        else {
            max = middle - 1;
        }
    }
	return false; //password not found in password dictionary
}


//This function checks if the password contains a dictionary word provided by english_dic
bool isInEnglishDic(vector<string> english_dic, string password) {
    //Convert password to lowercase
    string lowercase_password = password;
    transform(lowercase_password.begin(), lowercase_password.end(), lowercase_password.begin(), ::tolower);

    //Do npos to loop over everything and see if the password has a word/substring from english_dic:
    int i = 0;
    bool found = false;
    while (!found && i < english_dic.size()) {
        const string& word = english_dic[i];
        size_t loc = lowercase_password.find(word);
        if (loc != string::npos) {
            found = true; //Word is in dictionary
        }
        i++; //Increments to next word
    }

    return found; //Return true if a word was found, false otherwise
	
}


int main(int argc, char* argv[]) {

	if (argc < 3){
		cout << "Usage: program-name path-to-leaked-password-file path-to-english-word-file" << endl;
		return 1;
	}
	string leaked_password_file = argv[1]; 
	string english_word_file = argv[2]; 
	
	string password;


	vector<string> leaked_password_dic;
	vector<string> english_dic;

	clock_t  start = clock();
	readWordList(leaked_password_file, leaked_password_dic);
	//cout << "leaked password size: " << leaked_password_dic.size() << endl;
	
	readWordList(english_word_file, english_dic);
	//cout << "english word size: " << english_dic.size() << endl;
	
	clock_t stop = clock();

	cout << "Loading data files in " << (double)(stop - start) / CLOCKS_PER_SEC * 1000 << " milliseconds" << endl;

	cout << "Enter password: ";
	cin >> password;

	bool checks[CHECKS_NUM];

	checkPasswordChars(checks, password);


	bool failed = false;
	if (!checks[CHECK_LENGTH]) {
		failed = true;
		cout << critrion_msg_length << endl;
	}
	if (!checks[CHECK_UPPER]) {
		failed = true;
		cout << critrion_msg_upper << endl;
	}

	if (!checks[CHECK_LOWER]) {
		failed = true;
		cout << critrion_msg_lower << endl;
	}

	if (!checks[CHECK_DIGIT]) {
		failed = true;
		cout << critrion_msg_digit << endl;
	}

	if (!checks[CHECK_SPECIAL_CHAR]) {
		failed = true;
		cout << critrion_msg_special_chars << endl;
	}

	bool is_leaked = false;
	bool is_english = true;
	if (!failed) {
		start = clock();
		is_english = isInEnglishDic(english_dic, password);
		stop = clock();

		cout << "Checking against dictionary words took " <<  (double)(stop - start) / CLOCKS_PER_SEC * 1000 << " milliseconds" << endl;
		if (is_english) {
			cout << critrion_msg_dictionary << endl;
			is_english = true;
		}
		
		start = clock();
		is_leaked = isInLeakedDic(leaked_password_dic, password);
	    	stop = clock();
	    	
		cout << "Checking against leaked passwords took " <<  (double)(stop - start) / CLOCKS_PER_SEC * 1000 << " milliseconds" << endl;		
		if (is_leaked) {
			cout << critrion_msg_leaked << endl;
		}

	}
	if (!failed && !is_leaked && !is_english) {
		cout << "Password accepted!" << endl;
	}
	else {
		cout << "Password rejected!" << endl;
	}

	
	return 0;	
}
