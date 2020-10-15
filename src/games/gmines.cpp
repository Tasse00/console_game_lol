#include "./gmines.h"
#include <random>
#include <iostream>
#include <iomanip>

using namespace gmines;

Game::Game(int rows, int cols, int mines)
    : rows(rows), cols(cols), mines(mines), 
    rand(std::default_random_engine()), 
    manual_quit(false)
    
{
    this->rand.seed(time(0));
    this->cur_col = 0;
    this->cur_row = 0;
    this->start_at = time(0);

    int blocks_len = rows * cols;
    this->blocks = new Block[blocks_len];

    // init games
    for (int l = 0; l < blocks_len; l++)
    {
        this->blocks[l].isFlag = false;
        this->blocks[l].isMine = false;
        this->blocks[l].isOpen = false;
    }

    if (this->mines > blocks_len)
    {
        this->mines = blocks_len;
    }
    int rest_mines = this->mines;
    while (rest_mines)
    {
        int mine_col = this->rand() % this->cols;
        int mine_row = this->rand() % this->rows;

        Block &mine_blk = this->block(mine_row, mine_col);
        if (mine_blk.isMine)
            continue;

        mine_blk.isMine = true;
        rest_mines--;
    }
}

Game::~Game()
{
    delete[] this->blocks;
}

Block &Game::block(int row, int col)
{
    return this->blocks[row * this->cols + col];
}

void Game::open(int r, int c)
{
    // 执行打开操作

    Block &blk = this->block(r, c);
    blk.isOpen = true;

    if (blk.isMine)
        return;

    // 传递开启操作
    if (this->countMinesAround(r, c) == 0)
    {

        for (int dr = -1; dr < 2; dr++)
        {
            for (int dc = -1; dc < 2; dc++)
            {
                if (dr + r < 0 || dr + r >= this->rows || dc + c < 0 || dc + c >= this->cols || (dc == 0 && dr == 0))
                    continue;
                Block &this_blk = this->block(r + dr, c + dc);
                if (this_blk.isOpen)
                    continue;
                this->open(dr + r, dc + c);
            }
        }
    }
}

int Game::countMinesAround(int r, int c)
{
    int mines_around = 0;
    // count mines in neighbor;
    for (int dr = -1; dr < 2; dr++)
    {
        for (int dc = -1; dc < 2; dc++)
        {
            if (dr + r < 0 || dr + r >= this->rows || dc + c < 0 || dc + c >= this->cols || (dc == 0 && dr == 0))
                continue;

            if (this->block(r + dr, c + dc).isMine)
            {
                mines_around++;
            }
        }
    }
    return mines_around;
}

void Game::clear()
{
    system("clear");
}

void Game::render()
{
    std::string unknown = "██";
    std::string flagged = " ★";
    std::string errored = " ◎";
    std::string cursor = " □";
    std::string safe = "  ";

    for (int r = 0; r < this->rows; r++)
    {
        for (int c = 0; c < this->cols; c++)
        {

            if (r == this->cur_row && c == this->cur_col)
            {
                std::cout << cursor;
                continue;
            }

            Block &blk = this->block(r, c);
            if (blk.isOpen)
            {
                if (blk.isMine)
                {
                    std::cout << errored;
                }
                else
                {
                    int minesAround = this->countMinesAround(r, c);
                    if (minesAround == 0)
                    {
                        std::cout << safe;
                    }
                    else
                    {
                        std::cout << " " << minesAround;
                    }
                }
            }
            else
            {
                if (blk.isFlag)
                {
                    std::cout << flagged;
                }
                else
                {
                    std::cout << unknown;
                }
            }
        }
        std::cout << std::endl;
    }


    int duration = time(0) - this->start_at;
    int flagged_blocks = 0;
    for (int l=0; l<this->cols * this->rows; l++) {
        if (this->blocks[l].isFlag) flagged_blocks ++;
    }
    std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left) << std::setw(14) << "Total Mines:";
    std::cout << std::resetiosflags(std::ios::left) << std::setiosflags(std::ios::right) << std::setw(4) << this->mines << std::endl;
    std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left) << std::setw(14) << "Flagged Block:";
    std::cout << std::resetiosflags(std::ios::left) << std::setiosflags(std::ios::right) << std::setw(4) << flagged_blocks<< std::endl;
    std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left) << std::setw(14) << "Durations(s):";
    std::cout << std::resetiosflags(std::ios::left) << std::setiosflags(std::ios::right) << std::setw(4) << duration<< std::endl;
    
}
void Game::recvInput()
{
    char cmd;
    bool valid_cmd = false;
    while (!valid_cmd)
    {
        system("stty raw");
        cmd = getchar();
        system("stty cooked");
        std::cout << std::endl;

        switch (cmd)
        {
        case 'w':
            valid_cmd = true;
            this->latest_cmd = Cmd::MoveUp;
            break;
        case 'a':
            valid_cmd = true;
            this->latest_cmd = Cmd::MoveLeft;
            break;
        case 's':
            valid_cmd = true;
            this->latest_cmd = Cmd::MoveDown;
            break;
        case 'd':
            valid_cmd = true;
            this->latest_cmd = Cmd::MoveRight;
            break;
        case 'j':
            valid_cmd = true;
            this->latest_cmd = Cmd::ActOpen;
            break;
        case 'k':
            valid_cmd = true;
            this->latest_cmd = Cmd::ActFlag;
            break;
        case 'q':
            valid_cmd = true;
            this->latest_cmd = Cmd::Quit;
            break;

        default:
            break;
        }
    }
}
void Game::handleInput()
{
    Block &blk = this->block(this->cur_row, this->cur_col);
    switch (this->latest_cmd)
    {
    case Cmd::MoveLeft:
        if (this->cur_col > 0)
            this->cur_col--;
        break;
    case Cmd::MoveUp:
        if (this->cur_row > 0)
            this->cur_row--;
        break;
    case Cmd::MoveRight:
        if (this->cur_col + 1 < this->cols)
            this->cur_col++;
        break;
    case Cmd::MoveDown:
        if (this->cur_row + 1 < this->rows)
            this->cur_row++;
        break;
    case Cmd::ActOpen:
        if (!blk.isOpen && !blk.isFlag)
            this->open(this->cur_row, this->cur_col);
        break;
    case Cmd::ActFlag:
        if (!blk.isOpen)
            blk.isFlag = !blk.isFlag;
        break;
    case Cmd::Quit:
        this->manual_quit = true;
        break;
    default:
        break;
    }
}

bool Game::shouldQuit()
{
    if (this->manual_quit)
    {
        std::cout << "Quit" << std::endl;
        return true;
    }
    // 1. win 非雷全部打开
    // 2. los 打开了雷

    int flag_correctly_blocks = 0;

    for (int r = 0; r < this->rows; r++)
    {
        for (int c = 0; c < this->cols; c++)
        {
            Block &blk = this->block(r, c);
            // std::cout << r << "," << c << ","
            //           << "|" << blk.isMine << "," << blk.isOpen << std::endl;
            if (blk.isMine && blk.isOpen)
            {
                std::cout << "LOST" << std::endl;
                return true;
            }

            if (blk.isMine && blk.isFlag)
            {
                flag_correctly_blocks++;
            }
        }
    }

    if (this->mines == flag_correctly_blocks)
    {
        // win
        std::cout << "WIN" << std::endl;
        return true;
    }
    return false;
}


Game* Game::buildGame() {
    int rows;
    int cols;
    int mines;

    char use_default = 'y';
    std::cout << "Use default settings? (y/n) ";
    std::cin >> use_default;

    if (use_default == 'y')
    {
        rows = 20;
        cols = 20;
        mines = 40;
    }
    else
    {
        std::cout << "Setting the MineSweeper Game" << std::endl;
        std::cout << "rows(eg. 20): ";
        std::cin >> rows;
        std::cout << "cols(eg. 20): ";
        std::cin >> cols;
        std::cout << "mines(eg. 40): ";
        std::cin >> mines;
    }

    return new Game(rows, cols, mines);
}