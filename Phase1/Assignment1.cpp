/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

// Functions are explained at the definition area

//Const global variables to hold file names
const string keywordsFile = "keywords.txt", spamFile = "spam_filenames.txt",
lingFile = "ling_filenames.txt";

int loadFiles(vector<string> &a , vector<string> &b);
void processMails(const vector<string> &a ,map<string,unsigned> &b ,const vector<string> &c);
int countFeatures(const string str,const vector<string> &a , map<string,unsigned> &b) ;
string checkCat(string str);
string generateTokens(const vector<string> &a ,const map<string,unsigned> &b);

/*
* In the main function only two methods shall be called. Everything is done by these and their sub-functions
*/
int main(){
/*
* "keywords" is the vector where the keywords.txt words are saved
* "mailData" is the vector where the spam_filenames.txt and ling_filenames.txt words are saved
* "keywordCount" is the map where the keywords and their corresponding count number are saved
*/
vector<string> keywords,mailData;
map<string, unsigned> keywordCount;
if(loadFiles(keywords,mailData)==3) //if loadFiles returns 3 , then everything is fine and files were loaded properly
processMails(mailData,keywordCount,keywords);
else
cerr<<"Files' loading was unsuccessful. Program will exit."<<endl;
system("pause");
return 0;
}//end of main


/* loadFiles function
* Opens the keywordsFile through an ifstream, reads the words and saves them in the keywords vector
* Opens the spamFile and lingFile the ifstream, reads the words and saves them in the mailNames vector
* (Checks for valid files, closes and clears stream)
* Arguments are passed by reference and can be changed.
*/
int loadFiles(vector<string> &a , vector<string> &b) {
string word;
unsigned success=0;
ifstream rf;
//Loading keywordsFile
rf.open(keywordsFile.c_str());
if (!rf.fail()) {
success++;
while (rf >> word) {
a.push_back(word);
}
} else {
cerr << keywordsFile.c_str() << " file failed loading." << endl;
}
rf.clear();
rf.close();
//Loading spamFile
rf.open(spamFile.c_str());
if (!rf.fail()) {
success++;
while (rf >> word) {
b.push_back(word);
}
} else {
cerr << spamFile.c_str() << " file failed loading." << endl;
}
rf.clear();
rf.close();
//Loading lingFile
rf.open(lingFile.c_str());
if (!rf.fail()) {
success++;
while (rf >> word) {
b.push_back(word);
}
} else {
cerr << lingFile.c_str() << " file failed loading.";
}
rf.clear();
rf.close();
return success;
} //end of loadFiles


/*processMails function
* Sends to the cout (screen), the desired XML format output. Also, for each message:
* file name, category, number of features(keywords) and a list of the tokens(keywords) there with their name, number and count
* Calls the "countFeatures" function to count the features and their frequencies
* Calls the "generateTokens" function to print the token lists
* Arguments are passed by reference. Only map b can be modified inside this function.
*/
void processMails(const vector<string> &a,map<string,unsigned> &b ,const vector <string> &c) {
string mail;
cout << "< messagecollection messages = \"" << a.size() << "\" >"
<< endl;
for (unsigned i = 0; i < a.size(); i++) {
mail = a[i];
cout << "< message file = \"" << mail <<"\" category = \""
<< checkCat(mail) << "\" features = \"" << countFeatures(mail,c,b)
<< "\" >" << endl;
cout << generateTokens(c,b);
b.clear(); //clears the map to insert next mail's keywords' information
}
cout << "\n</ messagecollection >"<<endl;
} //end of processMails


/*checkCat function
* Checks the given string (mail filename) so as to define the mail file's category (spam or ham)
*/
string checkCat(string str) {
string mcat = str.substr(0, str.find("/"));
if (mcat.compare("spam") == 0) {
return mcat;
} else {
return "ham";
}
} //end of checkCat


/*countFeatures
* Counts the number of the features(keywords) in a mail file
* Loads through an ifstream, checks if a keyword already exists
* If it doesn't already exists, it adds it to the keywordCount map(map columns: keyword id,count in mail)
* If it does, it just adds to the counter of the keyword
* The keywordCount map which is "filled" by this function is going to be used for the whole process of enlisting and sending to the cout the desired numbers
* (feature number, token(keyword) name, token count and matching the token to its id)
* Argument vector a cannot be modified, but map b can be modified. Both arguments are passed by reference.Argument str is passed by value.
*/
int countFeatures(string str,const vector<string> &a, map<string,unsigned> &b) {
ifstream rf;
string word;
rf.open(str.c_str());
if (!rf.fail()) {
while (rf >> word) {
for (unsigned i = 0; i < a.size(); i++) {
if (word.compare(a[i]) == 0) {
b[word]++;
break;
}
}
}
} else {
cerr << str.c_str() << " file failed loading.";
}
rf.close();
rf.clear();
return b.size();
} //end of countFeatures


/*generateTokens function
* Creates a stringstream which will be the output to be printed.
* Declares a const_iterator for the keywordsCount map parsing
* For each mail file:
* Reads one keyword at a time, comparing the strings at the keywordsCount map with it
* If it's found, the desired message (containing the keyword/token name, ID and frequency in the file) is sent to the stringstream, then appended to the string "content"
* (keyword frequency is computed by the countFeatures method)
* stringstream is cleared and emptied before proceeding to the next mail file
* Finally, the whole "content" string, containing almost the whole message block, is returned
* Arguments are passed by reference and cannot be modified inside this function.
*/
string generateTokens(const vector<string> &a ,const map<string,unsigned> &b) {
string content = "";
stringstream os;
map<string, unsigned>::const_iterator itr;
for(unsigned i=0; i<a.size() ; i++)
{
if( (itr=b.find(a[i]))!=b.end())
{
os << "< feature token = \"" << (*itr).first << "\" id = \""
<< i << "\" freq = \"" << (*itr).second << "\" />\n";
content = content + os.str();
os.clear();
os.str("");
}
}
return content + "</message>\n\n";
} // end of generateTokens