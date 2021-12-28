// WordChecker.cpp
//
// ICS 46 Spring 2021
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector<std::string> v;
	int stringSize = word.size();
	std::string tempString;

	// if word is empty, return empty vector
	if (word == "" && stringSize == 0)
	{
		return v;
	}

    // SWAP
	// Swapping each adjacent pair of characters in the word.
	for (int i = 0; i < stringSize - 1; i++)
	{
		std::string copyString = word;
		std::string tempString = word;
		copyString[i+1] = tempString[i];
		copyString[i] = tempString[i+1];

		if (wordExists(copyString) == true)
		{
			v.push_back(copyString);
		}
	}

    // INSERT
	// In between each adjacent pair of characters in the word (also before the first character 
	// and after the last character), each letter from 'A' through 'Z' is inserted.
	std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string copyString2 = word;

	// before word
	for (int k = 0; k < 26; k++)
	{
		tempString = letters.substr(k, 1);
		tempString = tempString + copyString2;

		if (wordExists(tempString) == true)
		{
			v.push_back(tempString);
		}
	}

	// in between each letter
	for (int g = 0; g < stringSize; g++)
	{
		for (int h = 0; h < 26; h++)
		{
			std::string insertWord = word;
			std::string letter = letters.substr(h, 1);

			std::string newWord = insertWord.insert(g, letter);

			if (wordExists(newWord) == true)
			{
				v.push_back(newWord);
			}
		}
	}

	// after word
	for (int y = 0; y < 26; y++)
	{
		tempString = letters[y];
		tempString = word + tempString;

		if (wordExists(tempString) == true)
		{
			v.push_back(tempString);
		}
	}


    // DELETE
	// Deleting each character from the word.
	for (int z = 0; z < stringSize; z++)
	{
		std::string copyString3 = word;
		copyString3 = copyString3.erase(z,1);

		if (wordExists(copyString3) == true)
		{
			v.push_back(copyString3);
		}
	}

    // REPLACE
	// Replacing each character in the word with each letter from 'A' through 'Z'.
	for (int x = 0; x < stringSize; x++)
	{
		for (int f = 0; f < 26; f++)
		{
			std::string copyString4 = word;
			std::string letter = letters.substr(f,1);
			copyString4.erase(x,1);
			copyString4 = copyString4.insert(x, letter);

			if (wordExists(copyString4) == true)
			{
				v.push_back(copyString4);
			}
		}
	}

    // SPLIT
	// Splitting the word into a pair of words by adding a space in between each adjacent pair of characters in the word. 
	// It should be noted that this will only generate a suggestion if both words in the pair are found in the word set.
	std::string copyString5 = word;
	for (int a = 0; a < stringSize; a++)
	{
		std::pair<std::string, std::string> tempPair;
		tempPair.first = copyString5.substr(0, a);
		tempPair.second = copyString5.substr(a);

		std::string twoWords = tempPair.first + " " + tempPair.second;

		if (wordExists(tempPair.first) == true && wordExists(tempPair.second) == true)
		{
			v.push_back(twoWords);
		}

	}


	// Delete duplicate words within the vector
	for (int b = 0; b < v.size(); b++)
	{
		std::string tempStringX = v.at(b);
		for (int c = 0; c < v.size(); c++)
		{
			// checks if strings are same, checks if positions are different (duplicate in different position)
			if (tempStringX == v.at(c) && b != c)
			{
				// delete the dupe at position c
				v.erase(v.begin() + c);
			}
		}
	}

    return v;
}

