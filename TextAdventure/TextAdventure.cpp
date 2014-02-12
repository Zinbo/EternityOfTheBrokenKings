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


string showMatch(std::string const &s, regex const &r, smatch match) { 
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

void readInStory(map<int, State*> &statesMap){
	ifstream myReadFile;
	string storyLine;
	smatch match;
	regex numberPattern("(\\d+)");
	regex statePattern("^STATE");
	regex descPattern("^DESCRIPTION");
	regex noOfOptionsPattern("^NO_OF_CHOICES");
	regex stringPattern("\"([^\"]*)\"");

	myReadFile.open("story.txt");
	State* state;
	Option* option;
	int noOfChoices;

	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {

			//retrieve the next line in the text file.
			getline(myReadFile,storyLine);
			
			//If the line is a state, then create a new state.
			if(isMatch(storyLine, statePattern)){
				int value = atoi(showMatch(storyLine, numberPattern, match).c_str());
				state = new State();
				statesMap[value] = state;
			//Else if the line is a description, add it to the current state.
			}else if(isMatch(storyLine, descPattern)){
				string description = showMatch(storyLine, stringPattern, match);
				state->setDescript(description);
			}
			//Else if the line gives the number of options for the state, then create that many choices.
			else if(isMatch(storyLine, noOfOptionsPattern)){
				noOfChoices = atoi(showMatch(storyLine, numberPattern, match).c_str());
				for(int i = 0; i < noOfChoices; i++){
					option = new Option();
					//Each option has a choice, consequence, and nextStateNumber, so we read all 3 of these in for each option.	
					getline(myReadFile,storyLine);
					string choice = showMatch(storyLine, stringPattern, match);
					option->setChoice(choice);

					getline(myReadFile,storyLine);
					string consequence = showMatch(storyLine, stringPattern, match);
					option->setConsequence(consequence);

					getline(myReadFile,storyLine);
					int nextStateNumber = atoi(showMatch(storyLine, numberPattern, match).c_str());
					option->setNextState(statesMap.at(nextStateNumber));
					state->addOption(option);
				}
			}

		}
	}
	myReadFile.close();

}

void addAnomalyState(map<int, State*> & states){
	State* state = states.at(8);
	Option* option = new Option();
	option->setChoice("Watch her to see what she will do");
	option->setNextState(states.at(7));
	state->addOption(option);
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void playGame(map<int, State*> states){
	//This starting state is hardcoded to start at state 18.
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

	//While we still have choices print them out. If we run out of choices then the game is over.
	while(numberOfChoices != 0){
		rightInput = false;
		desc = nextState->getDescript();
		//I'm not too sure why, but when the file is read in it replaces all '\' with '\\', so they need to be brought back to just one slash.
		replace(desc, "\\n", "\n");
		cout << desc << endl;
		cout << "Make your choice: " << endl;
		options = nextState->getOptions();
		//Print out all the choices the player can make
		for(int i = 1; i <= numberOfChoices; i++){
			cout << i << ": " << options.at(i-1)->getChoice() << "\n";
		}
		//We loop until the user selects a number which corresponds to an actual choice.
		while(!rightInput){
			cin >> usersInput;
			choiceNumber = atoi(usersInput.c_str());
			if(choiceNumber > 0 && choiceNumber <= numberOfChoices){
				rightInput = true;
				option = options.at(choiceNumber-1);
				nextState = option->getNextState();
				consequence = option->getConsequence();
				//Again, not sure why when the file is read it replaces all '\' with '\\', but this sets them all back for each line.
				do{
					newLine = replace(consequence, "\\n", "\n");
				}
				while(newLine);
				cout << endl;
				if(!consequence.empty()){
					cout << consequence << endl;
					cout << endl;
				}
			}else{
				cout << "Input a choice number." << endl;
			}
		}
		//Get the next state.
		numberOfChoices = nextState->getOptions().size();
	}

}

void destructStory(map<int, State*> &states){
	for(int i = 1; i <= states.size(); i++){
		for(int j = 0; j < states.at(i)->getOptions().size(); j++){
			delete states.at(i)->getOptions().at(j);
		}
		delete states.at(i);
	}
}

void waitForUser(){
	std::cout << "Press ENTER to continue...";
	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
}

int main(){ 
	//Read in the file and populate the map. We add what's deemed an "anomaly state" as it needs to be added when all over states have been added due to its cyclic nature.  
    map<int, State*> states;
	readInStory(states);
	addAnomalyState(states);

	playGame(states);

	//Destroy all objects in the map and wait for the user to press enter.
	destructStory(states);
	waitForUser();

	return 0;
}