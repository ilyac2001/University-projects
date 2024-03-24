#pragma once
const int QUANTITY_PLATFORM_ = 4;
const int QUANTITY_COUNTER_FOR_WIN = 5;
const int SIZE_PLATFORM_IN_COUNTER = 3;

const int COLOR_COUNTER_PLAYER1[3] = { 255, 255, 255 }; //ходят первыми

const int COLOR_COUNTER_PLAYER2[3] = { 56, 56, 56 };	//ходят вторыми
struct Matrix {
	bool items[QUANTITY_PLATFORM_][SIZE_PLATFORM_IN_COUNTER][SIZE_PLATFORM_IN_COUNTER];
};
class Player
{
public:
	Player();
	Player(const int color[3]);
	~Player();


	bool SetMatrix(int number_platform, int number_counter, Matrix opponent_matrix); //врзвращает true, если удалось присвоить значение

	Matrix GetMatrix() { return _matrix; };

	void RotateMatrix(int number_platform, bool direction); // direction = true - движение по часовой

	bool CheckFiveInRow(); //возвращает true в слуачее победы

	int* GetColor() { return _color; };

	int _color[3];

	void NullifyMatrix();

private:

	Matrix _matrix;
	bool _rewritten_matrix[2 * SIZE_PLATFORM_IN_COUNTER][2 * SIZE_PLATFORM_IN_COUNTER];
	/*
	0,0	 0,1  0,2
	1,0	 1,1  2,2
	2,0	 2,1  2,2
	*/
	void RewriteMatrix(); //переписываем 4 матрицы 3 на 3 в одну 6 на 6, с ней проще работать при поиске 5 в ряд

	bool СheckDiagonal(int offsetX, int offsetY) {
		int size = 2 * SIZE_PLATFORM_IN_COUNTER; // размер игрового поля
		int block = QUANTITY_COUNTER_FOR_WIN; // размер блока = кол-ву фишек в ряд для победы
		bool toright, toleft;
		toright = true;
		toleft = true;
		for (int i = 0; i < block; i++) {
			toright &= _rewritten_matrix[i + offsetX][i + offsetY];
			toleft &= _rewritten_matrix[QUANTITY_COUNTER_FOR_WIN - i - 1 + offsetX][i + offsetY];
		}

		if (toright || toleft) return true;

		return false;
	}

	bool СheckLanes(int offsetX, int offsetY) {
		int size = 2 * SIZE_PLATFORM_IN_COUNTER; // размер игрового поля
		int block = QUANTITY_COUNTER_FOR_WIN; // размер блока = кол-ву фишек в ряд для победы
		bool cols, rows;
		for (int col = offsetX; col < block + offsetX; col++) {
			cols = true;
			rows = true;
			for (int row = offsetY; row < block + offsetY; row++) {
				cols &= _rewritten_matrix[col][row];
				rows &= _rewritten_matrix[row][col];
			}

			if (cols || rows) return true;
		}

		return false;
	}
};

