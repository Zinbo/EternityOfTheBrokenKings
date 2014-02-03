#include "State.h"
#include "Option.h"
#include <vector>
#include <iostream>

using namespace std;

std::vector<Option*> State::getOptions(){
	return optionVector;
}

string State::getDescript(){
	return descript;
}

void State::setDescript(string desc){
	descript = desc;
}

void State::addOption(Option* option){
	optionVector.push_back(option);
}
