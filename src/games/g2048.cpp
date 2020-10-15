#include <iostream>
#include <string>
#include <iomanip>

// #include <stdlib.h>

#include "./g2048.h"

using namespace g2048;

Game::Game(int row, int col, int first_blocks, int block_width)
    : row(row), col(col), rand(std::default_random_engine()),
      score(0), steps(0), first_blocks(first_blocks), block_width(block_width),
      manualQuit(false)
{
    this->rand.seed(time(0));
    this->blocks = new Block[row * col];
    for (int i = 0; i < row * col; i++)
    {
        this->blocks[i].value = 0;
    }

    this->initGame();
}

Game::~Game()
{
    delete[] this->blocks;
}

int Game::nextValue()
{
    int candidates[] = {2, 4};

    int size = static_cast<int>(sizeof(candidates) / sizeof(candidates[0]));

    return candidates[this->rand() % size];
}

void Game::initGame()
{

    int rest = this->first_blocks > (this->row * this->col)
                   ? (this->row * this->col)
                   : this->first_blocks;
    while (rest)
    {
        int col = this->rand() % this->col;
        int row = this->rand() % this->row;
        if (this->pos(row, col).value != 0)
        {
            continue;
        }

        this->pos(row, col).value = this->nextValue();

        rest--;
    }
}

Block &Game::pos(int row, int col)
{
    return this->blocks[row * this->col + col];
}

void Game::clear()
{
    system("clear");
}

void Game::render()
{

    const int width = this->block_width;
    std::string border = std::string((width + 1) * this->col + 1, '-');
    std::cout << border << std::endl;
    for (int r = 0; r < this->row; r++)
    {
        std::cout << "|";
        for (int c = 0; c < this->col; c++)
        {
            std::cout << std::setiosflags(std::ios::right) << std::setw(width);
            if (this->pos(r, c).value == 0)
            {
                std::cout << ""
                          << "|";
            }
            else
            {
                std::cout << this->pos(r, c).value << "|";
            }
        }
        std::cout << std::endl;
        std::cout << border << std::endl;
    }

    std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left) << std::setw(5) << "Score";
    std::cout << std::resetiosflags(std::ios::left) << std::setiosflags(std::ios::right) << std::setw(16) << this->score << std::endl;
    std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left) << std::setw(5) << "Steps";
    std::cout << std::resetiosflags(std::ios::left) << std::setiosflags(std::ios::right) << std::setw(16) << this->steps << std::endl;
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
            this->cmd = Cmd::Up;
            break;
        case 'a':
            valid_cmd = true;
            this->cmd = Cmd::Left;
            break;
        case 's':
            valid_cmd = true;
            this->cmd = Cmd::Down;
            break;
        case 'd':
            valid_cmd = true;
            this->cmd = Cmd::Right;
            break;
        case 'q':
            valid_cmd = true;
            this->cmd = Cmd::Quit;
            break;
        default:
            break;
        }
    }
}

void Game::swap(Block &b1, Block &b2)
{
    int v = b1.value;
    b1.value = b2.value;
    b2.value = v;
}

void Game::merge(Block &from, Block &to)
{
    to.value += from.value;
    from.value = 0;
    this->score += to.value;
}

void Game::handleInput()
{
    // 1. 执行合并
    // 2. 生成新值

    // 合并
    if (this->cmd == Cmd::Left)
    {
        for (int c_row = 0; c_row < this->row; c_row++)
        {
            for (int c_col = 0; c_col < this->col; c_col++)
            {
                Block &blk = this->pos(c_row, c_col);
                if (blk.value == 0)
                {
                    continue;
                }

                // right equal
                for (int after_col = c_col + 1; after_col < this->col; after_col++)
                {
                    Block &after_blk = this->pos(c_row, after_col);
                    if (after_blk.value != 0)
                    {
                        if (after_blk.value == blk.value)
                        {
                            this->merge(after_blk, blk);
                        }
                        break;
                    }
                }

                // left 0
                for (int prev_col = 0; prev_col < c_col; prev_col++)
                {
                    Block &prev_blk = this->pos(c_row, prev_col);
                    if (prev_blk.value == 0)
                    {
                        this->swap(prev_blk, blk);
                        break;
                    }
                }
            }
        }
    }
    else if (this->cmd == Cmd::Right)
    {
        for (int c_row = 0; c_row < this->row; c_row++)
        {
            for (int c_col = this->col - 1; c_col >= 0; c_col--)
            {
                Block &blk = this->pos(c_row, c_col);
                if (blk.value == 0)
                {
                    continue;
                }

                // left equal
                for (int after_col = c_col - 1; after_col >= 0; after_col--)
                {
                    Block &after_blk = this->pos(c_row, after_col);
                    if (after_blk.value != 0)
                    {
                        if (after_blk.value == blk.value)
                        {
                            this->merge(after_blk, blk);
                        }
                        break;
                    }
                }

                // right 0
                for (int prev_col = this->col - 1; prev_col > c_col; prev_col--)
                {
                    Block &prev_blk = this->pos(c_row, prev_col);
                    if (prev_blk.value == 0)
                    {
                        this->swap(prev_blk, blk);
                        break;
                    }
                }
            }
        }
    }
    else if (this->cmd == Cmd::Up)
    {
        for (int c_col = 0; c_col < this->col; c_col++)
        {
            for (int c_row = 0; c_row < this->row; c_row++)
            {
                Block &blk = this->pos(c_row, c_col);
                if (blk.value == 0)
                {
                    continue;
                }

                // down equal
                for (int after_row = c_row + 1; after_row < this->row; after_row++)
                {
                    Block &after_blk = this->pos(after_row, c_col);
                    if (after_blk.value != 0)
                    {
                        if (after_blk.value == blk.value)
                        {
                            this->merge(after_blk, blk);
                        }
                        break;
                    }
                }

                // left 0
                for (int prev_row = 0; prev_row < c_row; prev_row++)
                {
                    Block &prev_blk = this->pos(prev_row, c_col);
                    if (prev_blk.value == 0)
                    {
                        this->swap(prev_blk, blk);
                        break;
                    }
                }
            }
        }
    }
    else if (this->cmd == Cmd::Down)
    {
        for (int c_col = 0; c_col < this->col; c_col++)
        {
            for (int c_row = this->row - 1; c_row >= 0; c_row--)
            {

                // TODO!!!!!
                Block &blk = this->pos(c_row, c_col);
                if (blk.value == 0)
                {
                    continue;
                }

                // down equal
                for (int after_row = c_row - 1; after_row >= 0; after_row--)
                {
                    Block &after_blk = this->pos(after_row, c_col);
                    if (after_blk.value != 0)
                    {
                        if (after_blk.value == blk.value)
                        {
                            this->merge(after_blk, blk);
                        }
                        break;
                    }
                }

                // left 0
                for (int prev_row = this->row - 1; prev_row > c_row; prev_row--)
                {
                    Block &prev_blk = this->pos(prev_row, c_col);
                    if (prev_blk.value == 0)
                    {
                        this->swap(prev_blk, blk);
                        break;
                    }
                }
            }
        }
    }
    else if (this->cmd == Cmd::Quit)
    {
        this->manualQuit = true;
        return;
    }
    // 新值
    bool has_empty = false;

    for (int l = 0; l < this->row * this->col; l++)
    {
        if (this->blocks[l].value == 0)
        {
            has_empty = true;
            break;
        }
    }

    while (has_empty)
    {
        int next_col = this->rand() % this->col;
        int next_row = this->rand() % this->row;

        Block &blk = this->pos(next_row, next_col);
        if (blk.value == 0)
        {
            blk.value = this->nextValue();
            break;
        }
    }

    this->steps += 1;
}

bool Game::shouldQuit()
{
    if (this->manualQuit)
    {
        std::cout << "Quit" << std::endl;
        return true;
    }
    for (int r = 0; r < this->row; r++)
    {
        for (int c = 0; c < this->col; c++)
        {
            Block &blk = this->pos(r, c);

            if (
                blk.value == 0 ||
                (r - 1 > 0 && (this->pos(r - 1, c).value == blk.value)) ||
                (r + 1 < this->row && (this->pos(r + 1, c).value == blk.value)) ||
                (c - 1 > 0 && (this->pos(r, c - 1).value == blk.value)) ||
                (c + 1 < this->col && (this->pos(r, c + 1).value == blk.value)))
                return false;
        }
    }
    std::cout << "LOST" << std::endl;
    return true;
}

Game* Game::interactive_build()
{
    int rows;
    int cols;
    int block_width;
    int first_blocks;

    char use_default = 'y';
    std::cout << "Use default settings? (y/n) ";
    std::cin >> use_default;

    if (use_default == 'y')
    {
        rows = 4;
        cols = 4;
        first_blocks = 3;
        block_width = 4;
    }
    else
    {
        std::cout << "Setting the 2048 Game" << std::endl;
        std::cout << "rows(eg. 4): ";
        std::cin >> rows;
        std::cout << "cols(eg. 4): ";
        std::cin >> cols;
        std::cout << "first blocks(eg. 3): ";
        std::cin >> first_blocks;
        std::cout << "block width(eg. 4): ";
        std::cin >> block_width;
    }

    return new Game(rows, cols, first_blocks, block_width);
}
