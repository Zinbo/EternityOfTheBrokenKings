#include <string>
#include "State.h"


using namespace std;

string Option::getChoice(){
	return choice;
}

string Option::getConsequence(){
	return consequence;
}

State* Option::getNextState(){
	return nextState;
}

void Option::setChoice(string desc){
	choice = desc;
}

void Option::setConsequence(string conseq){
	consequence = conseq;
}

void Option::setNextState(State* state){
	nextState = state;
}