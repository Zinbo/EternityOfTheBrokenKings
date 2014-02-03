#include <vector>
#include <string>
#include "Option.h"
#ifndef STATE_H
#define STATE_H

using namespace std;

class State
{
    public:
        vector<Option*> getOptions();
        string getDescript();
		void setDescript(string desc);
		void addOption(Option* option);
    private:
		vector<Option*> optionVector;
        string descript;
        
};

#endif // STATE_H