#pragma once

#include "../g.h"
#include <random>

namespace gmines
{

    enum Cmd
    {
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight,

        ActOpen,
        ActFlag,

        Quit,
    };

    struct Block
    {
        bool isMine;
        bool isOpen;
        bool isFlag;
    };

    class Game : public g::Game
    {
    private:
        // Game Map Size
        int rows;
        int cols;

        // Mines in Map
        int mines;

        // Cursor Position
        int cur_row;
        int cur_col;

        // Blocks in map.
        Block *blocks;

        // rand utils
        std::default_random_engine rand;

        // runtime vars
        Cmd latest_cmd;
        bool manual_quit;
        int start_at;

        Block& block(int row, int col);
        void open(int row, int col);
        int countMinesAround(int row, int col);

    public:
        Game(int rows, int cols, int mines);
        ~Game();
        static Game* buildGame();

        virtual void clear();

        /**
         * '#' Unknown
         * ' ' / 'N' Opened
         * '!' Flagged
         * 'X' Errored
         **/

        virtual void render();
        virtual void recvInput();
        virtual void handleInput();
        virtual bool shouldQuit();
        
    };

}; // namespace gmainesweeper

/*
##2 #
# 4 #
# 2
*/