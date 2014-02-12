//============================================================================
// Name        : TextAdventure.cpp
// Author      : Shane Jennings
// Version     : 1
// Description : Eternity of Broken Kings
//============================================================================

#include <iostream>
#include<fstream>
#include "State.h"
#include <map>
#include <boost/regex.hpp>
#include <iterator>
#include <algorithm>
#include <cstdio>
#include <limits>

using namespace std;
using namespace boost;


string show_match(std::string const &s, regex const &r, smatch match) { 
	if (regex_search(s, match, r)){
		return match[1];
	}
}

bool isMatch(std::string const &s, regex const &r){
	if (regex_search(s, r)){
		return true;
	}else{
		return false;
	}
}

std::map<int, State*> readInStory(){
	std::map<int, State*> statesMap;

	ifstream myReadFile;
	string STRING;
	smatch match;
	regex numberPattern("(\\d+)");
	regex statePattern("^STATE");
	regex descPattern("^DESCRIPTION");
	regex noOfChoicesPattern("^NO_OF_CHOICES");
	regex stringPattern("\"([^\"]*)\"");

	myReadFile.open("story.txt");
	State* state;
	Option* option;
	int noOfChoices;

	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {

			getline(myReadFile,STRING); // Saves the line in STRING.
			
			if(isMatch(STRING, statePattern)){
				int value = atoi(show_match(STRING, numberPattern, match).c_str());
				//cout << "Creating new state with number: " << value << "\n"; 
				state = new State();
				statesMap[value] = state;


			}else if(isMatch(STRING, descPattern)){
				string description = show_match(STRING, stringPattern, match);
				//cout << "Has description: " << description << "\n";
				state->setDescript(description);
				//cout << "description from state: " << state->getDescript();
			}
			else if(isMatch(STRING, noOfChoicesPattern)){
				noOfChoices = atoi(show_match(STRING, numberPattern, match).c_str());
				//cout << "Number of choices found: " << noOfChoices << "\n";
				for(int i = 0; i < noOfChoices; i++){
					option = new Option();
						
					getline(myReadFile,STRING);
					string choice = show_match(STRING, stringPattern, match);
					//cout << "Choice: " << choice << "\n";
					option->setChoice(choice);

					getline(myReadFile,STRING); // Saves the line in STRING.
					string consequence = show_match(STRING, stringPattern, match);
					option->setConsequence(consequence);
					//cout << "Consequence: " << consequence << "\n";

					getline(myReadFile,STRING); // Saves the line in STRING.
					int nextStateNumber = atoi(show_match(STRING, numberPattern, match).c_str());
					//cout << "Next State: " << nextStateNumber << "\n";
					option->setNextState(statesMap.at(nextStateNumber));
					state->addOption(option);

				}
			}

		}
	}
	myReadFile.close();
	
	return statesMap;
}

map<int, State*> addAnomalyState(map<int, State*> states){
	State* state = states.at(8);
	Option* option = new Option();
	option->setChoice("Watch her to see what she will do");
	option->setNextState(states.at(7));
	state->addOption(option);
	return states;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void playGame(map<int, State*> states){
	State* nextState = states.at(18);
	int numberOfChoices = nextState->getOptions().size();
	int choiceNumber;
	string consequence;
	Option* option;
	vector<Option*> options;
	string usersInput;
	string desc;
	bool rightInput = false;
	bool newLine = false;

	while(numberOfChoices != 0){
		rightInput = false;
		desc = nextState->getDescript();
		replace(desc, "\\n", "\n");
		cout << desc << "\n";
		cout << "Make your choice: " << "\n";
		options = nextState->getOptions();
		for(int i = 1; i <= numberOfChoices; i++){
			cout << i << ": " << options.at(i-1)->getChoice() << "\n";
		}
		while(!rightInput){
			cin >> usersInput;
			choiceNumber = atoi(usersInput.c_str());
			if(choiceNumber > 0 && choiceNumber <= numberOfChoices){
				rightInput = true;
				option = options.at(choiceNumber-1);
				nextState = option->getNextState();
				consequence = option->getConsequence();
				do{
					newLine = replace(consequence, "\\n", "\n");
				}
				while(newLine);
				cout << "\n";
				if(!consequence.empty()){
					cout << consequence << "\n";
					cout << endl;
				}
			}else{
				cout << "Input a choice number." << "\n";
			}
		}
		numberOfChoices = nextState->getOptions().size();
	}

}

int main(){ 
    map<int, State*> states = readInStory();
	addAnomalyState(states);
	
	playGame(states);

	std::cout << "Press ENTER to continue...";
	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

	return 0;
}