var numColumns = 40;
var numRows = 20;
int[,] grid;
var random = new Random();

int[,] GenerateGrid()
{
    var grid = new int[numRows, numColumns];
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numColumns; column++)
        {
            grid[row, column] = random.Next(2);
        }
    }
    return grid;
}

void PrintGrid(int[,] grid)
{
    Console.SetCursorPosition(0, 0);
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numColumns; column++)
        {
            Console.Write(grid[row, column] == 1 ? "X" : " ");
        }
        Console.WriteLine();
    }
}

int GetNumberOfNeighbors(int[,] grid, int row, int column)
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
                if (grid[newRow, newCol] == 1) count++;
            }
        }
    }
    return count;
}

void UpdateGrid(ref int[,] grid)
{
    int[,] newGrid = new int[numRows, numColumns];
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numColumns; column++)
        {
            var count = GetNumberOfNeighbors(grid, row, column);
            if (grid[row, column] == 1)
            {
                newGrid[row, column] = (count == 2 || count == 3) ? 1 : 0;
            }
            else
            {
                newGrid[row, column] = (count == 3) ? 1 : 0;
            }
        }
    }
    grid = newGrid;
}


Console.Clear();
grid = GenerateGrid();
while (true)
{
    PrintGrid(grid);
    UpdateGrid(ref grid);
    Thread.Sleep(100);
}
