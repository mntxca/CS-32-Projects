#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <ctype.h>
using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck()
{
	root = new trieNode();
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
	delete root;
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	
	ifstream infile(dictionaryFile);
	if (!infile)
	{
		cerr << "Error: Cannot open the file!" << endl;
		return false;
	}

	string word;
	while (getline(infile, word))
	{
		root->addWord(word, root);
	}
	return true;
}

//Complete at O(max_suggestions + L^2) complexity, like spec
bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	// TODO
	if(root->foundWord(word)) //O(L)
		return true;
	
	//now, find suggestions up to max_suggestions
	suggestions.clear(); //O(max_suggestions)
	int length = word.size(); //O(1) constant time
	
	char ch = '\0';
	string changingWord = word;
	int suggestionCount = 0;
	for (int changedLetterIndex = 0; changedLetterIndex < length; changedLetterIndex++) //O(L^2) because runs L times O(L)
	{
		changingWord = word;
		for (int j = 0; j < 27; j++) //O(1), because runs fixed number of times (27 times)
		{
			if(j == 26)
				ch = '\'';
			else
				ch = 'a' + j;
			//now, the character has been picked
			changingWord[changedLetterIndex] = ch; //replace the correct index with the character
			if (root->foundWord(changingWord)) //O(L)
			{
				suggestions.push_back(changingWord);
				suggestionCount++;
				if (suggestionCount == max_suggestions)
					return false; //basically, in this case, you are done, so just return the false here
			}
		}
		
		changingWord[changedLetterIndex] = ch;
	}
	
	return false; 
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
	int length = line.size();
	int positionStart = 0;
	int positionEnd = 0;
	bool wordFound = false;
	string word;
	
	while (true) //can just make this true because there is a break that will end this loop when necessary
	{
		word = findWord(line, positionStart, positionEnd);
		if (word == "")
			break; //then you found no more words in the rest of the line
		if(root->foundWord(word)) //see if the trie has the word inside
			; //do nothing, because this isn't a problem
		else {
			Position pos;
			pos.start = positionStart;
			pos.end = positionEnd - 1;
			problems.push_back(pos);
		}
		positionStart = positionEnd; //basically moving forward to next part of line
	}
}

//Complete with O(
string StudentSpellCheck::findWord(const std::string& line, int &positionStart, int &positionEnd)
{
	int length = line.size();
	int posOfWordStart = 0;
	int lengthOfWord = 0;
	bool wordFound = false;
	if (positionStart < 0 || positionStart >= length) //this will prevent function from progressing for invalid position input
		return "";
	for (int i = positionStart; i < length; i++)
	{
		if (isalpha(line[i]) || line[i] == '\'') //found proper character
		{
			if (wordFound == false) //if you haven't found a word yet, and find a proper letter or apostrophe
			{
				posOfWordStart = i;
				wordFound = true;
			}
			lengthOfWord++;
			if (i == length - 1) //you are at the end of the line, so end the word
			{
				positionStart = posOfWordStart; //first position of the word
				positionEnd = i + 1; //first position right after the word ended
				return line.substr(posOfWordStart, lengthOfWord);
			}

		}
		else
		{
			if (wordFound)
			{
				positionStart = posOfWordStart; //first position of the word
				positionEnd = i; //first position right after the word ended
				return line.substr(posOfWordStart, lengthOfWord);
			}
		}

	}
	return ""; //if nothing is found in the entire string, then return empty string
}

StudentSpellCheck::trieNode::~trieNode()
{
	for (int i = 0; i < 27; i++)
		delete array[i];
}

bool StudentSpellCheck::trieNode::addWord(string word, trieNode* root)
{
	int length = word.size();
	trieNode* currentPosition = root;
	int nextPos = -1;
	char ch;
	for (int i = 0; i < length; i++)
	{
		ch = word[i];
		//this directly strips out all non letter and apostrophe characters by not even dealing with them
		nextPos = convertCharToIndex(ch);

		if (nextPos != -1)
		{
			//if there is no continuation of the trie at the appropriate position
			if (currentPosition->array[nextPos] == nullptr)
				currentPosition->array[nextPos] = new trieNode(ch);
			if (i == length - 1) //if you are at the last letter
				(currentPosition->array[nextPos])->markAsEnd(); //mark this as the last letter
			currentPosition = currentPosition->array[nextPos]; //this essentially moves you down the tree
		}
	}
	return !(nextPos == -1); //if nextPos is still -1, nothing was attempted to be added
}

bool StudentSpellCheck::trieNode::foundWord(string word)
{
	int length = word.size();
	char ch;
	int nextPos = -1;
	trieNode* currentPosition = this;
	for (int i = 0; i < length; i++)
	{
		ch = word[i];
		nextPos = convertCharToIndex(ch);
		if (currentPosition->array[nextPos] == nullptr)
			return false;
		else
		{
			currentPosition = currentPosition->array[nextPos]; //this essentially moves you forward a letter
			if (i == length - 1)
				return currentPosition->isEnd();
		}
	}
	
	return false;
}

int StudentSpellCheck::trieNode::convertCharToIndex(char ch)
{
	int nextPos = -1;
	if (ch == '\'')
		nextPos = 26;
	else if (isalpha(ch))
	{
		ch = tolower(ch); //makes all letters lowercase
		nextPos = ch - 'a';
	}
	return nextPos;
}




