#pragma once
#include <map>
#include <string>

namespace g
{
    using namespace std;
    
   

    class Renderer
    {
    public:
        virtual void render() = 0;
        virtual void clear() = 0;
    };

    class Loop
    {
    public:
        virtual void recvInput() = 0;
        virtual void handleInput() = 0;
        virtual bool shouldQuit() = 0;
    };

    class Game : public Renderer, public Loop
    {
    public:
        virtual ~Game() {};
    };

    typedef Game* (*BuildGameMethod)(void); 

    struct GameInfo {
        string id;
        string title;
        BuildGameMethod method;
    };

    class Runner
    {
    private:
        map<string, GameInfo*> games;
        void printGames();
    public:
        Runner();
        ~Runner();
        void registerGame(const string& title, const string &id, BuildGameMethod buildMethod);
        
        void menu();
        
        void run(Game &g);
    };

}; // namespace g
