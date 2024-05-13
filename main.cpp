#include <iostream>
#include <array>
#include <vector>
#include <unordered_set>

bool win = false;

typedef std::array<std::array<int, 9>, 9> Grid;
Grid board;
struct RowCol
{
    int row;
    int col;
};
std::vector<RowCol> toFill = {};
typedef std::array<std::unordered_set<int>, 9> SetArray;
struct
{
    SetArray rowSet;
    SetArray colSet;
    SetArray clusterSet;
} checkSets;

void initializeBoard()
{
    for (auto &row : board)
    {
        for (auto &col : row)
        {
            col = 0;
        }
    }
    return;
}

void tempInitializeBoard()
{
    board = {{{{4, 5, 0, 0, 0, 0, 0, 0, 0}},
              {{0, 0, 2, 0, 7, 0, 6, 3, 0}},
              {{0, 0, 0, 0, 0, 0, 0, 2, 8}},
              {{0, 0, 0, 9, 5, 0, 0, 0, 0}},
              {{0, 8, 6, 0, 0, 0, 2, 0, 0}},
              {{0, 2, 0, 6, 0, 0, 7, 5, 0}},
              {{0, 0, 0, 0, 0, 0, 4, 7, 6}},
              {{0, 7, 0, 0, 4, 5, 0, 0, 0}},
              {{0, 0, 8, 0, 0, 9, 0, 0, 0}}}};
    return;
}

int getCluster(RowCol loc)
{
    return (loc.row/3*3) + (loc.col/3);
}

int numSquaresToFill = 0;

void getSquaresToFill()
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (board[row][col] == 0)
            {
                toFill.push_back({row, col});
                numSquaresToFill++;
            }
            else
            {
                checkSets.rowSet[row].insert(board[row][col]);
                checkSets.colSet[col].insert(board[row][col]);
                checkSets.clusterSet[getCluster({row, col})].insert(board[row][col]);
            }
        }
    }
    return;
}

void printBoard()
{
    std::cout << "    0 1 2 3 4 5 6 7 8\n";
    std::cout << "    -----------------\n";
    int rowNum = 0;
    for (auto &row : board)
    {
        std::cout << rowNum << " | ";
        for (auto &col : row)
        {
            std::cout << col << " ";
        }
        std::cout << "\n";
        rowNum++;
    }
    std::cout << "\n";
    return;
}

bool checkRowColCluster(const unsigned int layer, const unsigned int num)
{
    if (checkSets.colSet[toFill[layer].col].count(num))
    {
        return false;
    }
    if (checkSets.rowSet[toFill[layer].row].count(num))
    {
        return false;
    }
    if (checkSets.clusterSet[getCluster(toFill[layer])].count(num))
    {
        return false;
    }
    return true;
}

void insertIntoSets(const unsigned int layer, const unsigned int num)
{
    checkSets.colSet[toFill[layer].col].insert(num);
    checkSets.rowSet[toFill[layer].row].insert(num);
    checkSets.clusterSet[getCluster(toFill[layer])].insert(num);
    return;
}

void removeFromSets(const unsigned int layer, const unsigned int num)
{
    checkSets.colSet[toFill[layer].col].erase(num);
    checkSets.rowSet[toFill[layer].row].erase(num);
    checkSets.clusterSet[getCluster(toFill[layer])].erase(num);
    return;
}

void solveBoard(unsigned int layer)
{
    for (unsigned int num = 1; num < 10; num++)
    {
        if(checkRowColCluster(layer, num))
        {
            board[toFill[layer].row][toFill[layer].col] = num;
            if (layer < toFill.size()-1)
            {
                insertIntoSets(layer, num);
                solveBoard(layer+1);
            }
            else
            {
                win = true;
            }
            if (win)
            {
                return;
            }
            removeFromSets(layer, num);
            board[toFill[layer].row][toFill[layer].col] = 0;
        }
    }
    return;
}

int main()
{
    tempInitializeBoard();
    getSquaresToFill();
    printBoard();
    solveBoard(0);
    printBoard();
    return 0;
}