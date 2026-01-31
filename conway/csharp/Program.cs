using System.Collections;
using System.Collections.Concurrent;
using System.Data;
using System.Diagnostics;
using System.Reflection.Emit;
using System.Runtime.InteropServices.Marshalling;
using System.Transactions;

int numColumns = Console.WindowWidth-1;
int numRows = Console.WindowHeight-1;
BitArray grid1;
BitArray grid2;
int generation = 0;
var random = new Random();

BitArray GenerateGrid()
{
    var grid = new BitArray(numRows * numColumns);
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numColumns; column++)
        {
            grid[row * numColumns + column] = random.Next(2) == 1;
        }
    }
    return grid;
}

void PrintGrid(BitArray grid)
{
    Console.SetCursorPosition(0, 0);
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numColumns; column++)
        {
            Console.Write(grid[row * numColumns + column] ? "X" : " ");
        }
        Console.WriteLine();
    }
}

int GetNumberOfNeighbors(BitArray grid, int row, int column)
{
    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int newRow = row + i;
            int newCol = column + j;
            if (i == 0 && j == 0) continue;
            if (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numColumns)
            {
                if (grid[newRow * numColumns + newCol]) count++;
            }
        }
    }
    return count;
}

void UpdateGrid(BitArray current, BitArray next)
{
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numColumns; column++)
        {
            var count = GetNumberOfNeighbors(current, row, column);
            int idx = row * numColumns + column;
            if (current[idx])
            {
                next[idx] = count == 2 || count == 3;
            }
            else
            {
                next[idx] = count == 3;
            }
        }
    }
}

Console.Clear();
grid1 = GenerateGrid();
grid2 = new BitArray(numRows * numColumns);
var current = grid1;
var next = grid2;
while (true)
{
    PrintGrid(current);
    UpdateGrid(current, next);

    var temp = current;
    current = next;
    next = temp;
    generation++;

    Thread.Sleep(200);
}
