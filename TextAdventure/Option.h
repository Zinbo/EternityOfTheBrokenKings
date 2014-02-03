#include <string>
class State;
#ifndef OPTION_H
#define OPTION_H

using namespace std;

class Option
{
    public:
		string getChoice();
		string getConsequence();
		State* getNextState();
		void setChoice(string desc);
		void setConsequence(string consequence);
		void setNextState(State* state);

    protected:
    private:
		string choice;
		string consequence;
		State* nextState;
};

#endif // OPTION_H