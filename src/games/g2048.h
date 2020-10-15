#pragma once
#include "../g.h"
#include <random>


namespace g2048
{
    struct Block
    {
        int value;
        
    };

    enum Cmd
    {
        Up,
        Down,
        Left,
        Right,
        Quit,
    };

    
    class Game : public g::Game
    {
    private:
        int row;
        int col;
        std::default_random_engine rand;

        Block* blocks; // 一维数组模拟二维数组

        Cmd cmd;
        
        bool manualQuit;
        int score;
        int steps;
        int first_blocks; // 刚开始的blocks数量
        int block_width; // 一格宽度

        // 获取Grid中Block引用
        Block& pos(int row, int col);

        void swap(Block& b1, Block&b2);
        void merge(Block& from, Block& to);

        void initGame();

        int nextValue();

    public:
        Game(int row, int col, int first_blocks, int block_width);
        ~Game();

        static Game* interactive_build();
        

        virtual void clear();
        virtual void render();
        virtual void recvInput();
        virtual void handleInput();
        virtual bool shouldQuit();
        
    };

    
} // namespace g2048
