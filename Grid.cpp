#include "Grid.h"

Grid::Grid(int nRows, int nCols, int cSize)
{
	// rand
	std::srand(std::time(nullptr));

	numRows = nRows;
	numCols = nCols;
	cellSize = cSize; // pixels

	//draw grid margins
	leftMargin = 0;
	topMargin = 0;

	Initialize(); // set grid to 0
}

void Grid::Initialize()
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			grid[i][j] = 0; //rand() % 9;

		}
	}

}

void Grid::PrintValues()
{
	std::cout <<"=======================" << std::endl;

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			std::cout << grid[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::Draw()
{
	Color cellColor ;

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			// set cell color 
			cellColor = ApplyCellColors(row, col);

			DrawOneCell( col*cellSize , row * cellSize, cellSize , cellSize, cellColor);
		}
	}
}

void Grid::DrawOneCell(int x , int y, int w , int h, Color color)
{
	DrawRectangle(leftMargin + x, topMargin + y, w, h, color);
}

int Grid::getNumRows()
{
	return numRows;
}

int Grid::getNumCols()
{
	return numCols;
}


void Grid::SetMargins(int left, int top)
{
	leftMargin = left;
	topMargin = top;
}

Color Grid::ApplyCellColors(int row, int col)
{
	Color cellColor;

	if ((row % 2 == 0 && col % 2 != 0) ||
		(row % 2 != 0 && col % 2 == 0))
		cellColor = BLACK;
	else
		cellColor = WHITE;

	return cellColor;
}

