#include "./g.h"
#include <iostream>
#include <iomanip>

using namespace g;
using namespace std;
Runner::Runner() {}
Runner::~Runner() {
    for (auto it=this->games.begin(); it!=this->games.end(); it++) {
         delete it->second;
    }
}
void Runner::registerGame(const string& title,const string &id, BuildGameMethod buildMethod)
{
    GameInfo* inf = new GameInfo{
        id,
        title,
        buildMethod,
    };

    this->games.insert(pair<string, GameInfo*>(id, inf));
}
void Runner::printGames() {
    cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left);
    for (auto it=this->games.begin(); it!=this->games.end(); it++) {
         cout << setw(8) << it->second->id << " : "<<it->second->title << endl;
    }
}
void Runner::menu() {
    this->printGames();
    string gid;
    cout << "choose: ";
    cin >> gid;
    
    auto iter = this->games.find(gid);
    if (iter == this->games.end()) {
        cout << "Invalid Game Id" << endl;
        return;
    }
    cout << "Chose Game:" << iter->second->title << endl;

    Game* g = iter->second->method();

    this->run(*g);

    delete g;
}
void Runner::run(Game &g)
{

    g.clear();
    g.render();
    while (true)
    {

        g.recvInput();
        g.handleInput();

        g.clear();
        g.render();

        if (g.shouldQuit())
        {
            break;
        }
    }
}