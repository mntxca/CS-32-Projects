#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


class trieNode;

class StudentSpellCheck : public SpellCheck {
public:
	StudentSpellCheck();
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions); //Complete at O(max_suggestions + L^2) complexity, like spec
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	class trieNode
	{
	public:
		trieNode(char ch = '\0')
			:end(false)
		{
			for (int i = 0; i < 27; i++)
				array[i] = nullptr;
			letter = ch; //the character here is nullbyte because this is a root if there is no ch input
		}
		~trieNode();
		bool addWord(string word, trieNode* root);
		//trieNode* getParent() { return parent; }
		//bool isLeaf() { return leafNode; }
		bool isEnd() { return end; }
		void markAsEnd() { end = true; }
		//string getWord() { return word; }
		char getLetter() { return letter; }
		bool foundWord(string word);
		int convertCharToIndex(char ch);
	private:
		trieNode* array[27];
		//bool leafNode;
		//string word;
		char letter;
		bool end;
		//trieNode* parent;
	};
	trieNode* root;
	string findWord(const std::string& line, int &positionStart, int& positionEnd);
};

#endif  // STUDENTSPELLCHECK_H_
