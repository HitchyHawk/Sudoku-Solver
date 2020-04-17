#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*block
{0,2} {1,2} {2,2}
{0,1} {1,1} {2,1}
{0,0} {1,0} {2,0}

{ 1, 4, 7}
  2  5  8
  3  6  9
*/

struct block {
	bool cell[3][3];
};
struct guessBlock {
	bool cell[3][3][9];
};

class board {
private:
	block chunk[3][3];
	guessBlock notes[3][3];

	//tells me if a board is solved
	bool solved[3][3];
public:
	board() {
		clean();
	}

	void clean() {
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				solved[row][col] = false;

				for (int cRow = 0; cRow < 3; cRow++) {
					for (int cCol = 0; cCol < 3; cCol++) {
						chunk[row][col].cell[cRow][cCol] = false;

						for (int i = 0; i < 9; i++) {
							notes[row][col].cell[cRow][cCol][i] = true;
						}
					}
				}
			}
		}
	}

	void insert(int row, int col, int cRow, int cCol, int value) {
		if (value != 0) {
			chunk[row][col].cell[cRow][cCol] = true;
			clearChunk(row, col, value);
			clearRow(col, cCol, value);
			clearCol(row, cRow, value);
			clearCell(row, col, cRow, cCol);

			notes[row][col].cell[cRow][cCol][value - 1] = true;
		}
		
	}
	void insert(int x, int y, int value) {
		int bx = 0;
		int by = 0;
		while (x >= 3) {
			bx++;
			x -= 3;
		}
		while (y >= 3) {
			by++;
			y -= 3;
		}
		insert(bx,by , x, y, value);
	}

	void clearChunk(int row, int col, int value) {
		for (int cRow = 0; cRow < 3; cRow++) {
			for (int cCol = 0; cCol < 3; cCol++) {
				notes[row][col].cell[cRow][cCol][value - 1] = false;
			}
		}
	}
	void clearRow(int col, int cCol, int value) {
		for (int row = 0; row < 3; row++) {
			for (int cRow = 0; cRow < 3; cRow++) {
				notes[row][col].cell[cRow][cCol][value - 1] = false;
			}
		}
	}
	void clearCol(int row, int cRow, int value) {
		for (int col = 0; col< 3; col++) {
			for (int cCol = 0; cCol < 3; cCol++) {
				notes[row][col].cell[cRow][cCol][value - 1] = false;
			}
		}
	}
	void clearCell(int row, int col, int cRow, int cCol) {
		for (int i = 0; i < 9; i++) notes[row][col].cell[cRow][cCol][i] = false;
	}
	
	void clearExcRow(int xRow, int col, int cCol, int value) {
		for (int row = 0; row < 3; row++) {
			if (row != xRow) {
				for (int cRow = 0; cRow < 3; cRow++) {
					notes[row][col].cell[cRow][cCol][value - 1] = false;
				}
			}
		}
	}
	void clearExcCol(int row, int xCol, int cRow, int value) {
		for (int col = 0; col < 3; col++) {
			if (col != xCol) {
				for (int cCol = 0; cCol < 3; cCol++) {
					notes[row][col].cell[cRow][cCol][value - 1] = false;
				}
			}
		}
	}

	int chunkTruth( int row, int col, int value) {
		int count = 0;
		for (int cRow = 0; cRow < 3; cRow++) {
			for (int cCol = 0; cCol < 3; cCol++) {
				if (notes[row][col].cell[cRow][cCol][value])count++;
			}
		}
		return count;

	}
	int excRowTruth(int row, int col, int cCol, int value){
		int count = 0;
		for (int cRow = 0; cRow < 3; cRow++) {
			if (notes[row][col].cell[cRow][cCol][value]) count++;
		}
		return count;
	}
	int excColTruth(int row, int col, int cRow, int value) {
		int count = 0;
		for (int cCol = 0; cCol < 3; cCol++) {
			if (notes[row][col].cell[cRow][cCol][value]) count++;
		}
		return count;
	}

	void chunkSolve(int row, int col) {
		int tracker[9] = { 0,0,0, 0,0,0, 0,0,0 };
		for (int cRow = 0; cRow < 3; cRow++) {
			for (int cCol = 0; cCol < 3; cCol++) {

				//if cell isnt solved, try solving it
				if (!chunk[row][col].cell[cRow][cCol]) {


					for (int value = 0; value < 9; value++) {
						//checks all nine value possibilities
						if (notes[row][col].cell[cRow][cCol][value]) {
							//if a value is possible check to see if its the only one

							//there is only one value here, insert that value
							for (int i = 0; i < 9; i++) {
								if (i != value && notes[row][col].cell[cRow][cCol][i]) break;
								if (i == 8) {
									insert(row, col, cRow, cCol, value + 1);
								}
							}

						}

						if (notes[row][col].cell[cRow][cCol][value]) tracker[value]++;
					}
				}
			}
		}

		for (int i = 0; i < 9; i++) {
			if (tracker[i] == 1) {
			
				for (int cRow = 0; cRow < 3; cRow++) {
					for (int cCol = 0; cCol < 3; cCol++) {
						if (notes[row][col].cell[cRow][cCol][i]) insert(row, col, cRow, cCol, i + 1);
					}
				}
			}
		}
	}
	void update(int row, int col) {
		int pureCount;
		int tracker;
		for (int value = 0; value < 9; value++) {
			tracker = 0;
			pureCount = pureCount = chunkTruth(row, col, value);
			for (int i = 0; i < 3; i++) {
				if (pureCount == excColTruth(row, col, i, value)) clearExcCol(row, col, i, value + 1);
				
				if (pureCount == excRowTruth(row, col, i, value)) clearExcRow(row, col, i, value + 1);

			}
		}	
	}

	void display() {
		bool flip = false;
		for (int row = 0; row < 3; row++) {
			for (int cRow = 0; cRow < 3; cRow++) {
				for (int col = 0; col < 3; col++) {
					for (int cCol = 0; cCol < 3; cCol++) {
						//if cell is solved
						if (chunk[row][col].cell[cRow][cCol]) {
							for (int i = 0; i < 9; i++) {
								if (notes[row][col].cell[cRow][cCol][i]) {
									cout << i + 1 << " ";
									break;
								}
							}
						}
						else cout << "0 ";
						
					}
					cout << "  ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}

	int getSpecial() {
		int answer = 0;
		for (int h = 0; h < 9; h++) {
			if (notes[0][0].cell[0][0][h]) answer += (h + 1) * 100;
		}

		for (int t = 0; t < 9; t++) {
			if (notes[0][0].cell[1][0][t]) answer += (t + 1) * 10;
		}

		for (int o = 0; o < 9; o++) {
			if (notes[0][0].cell[2][0][o]) answer += (o + 1);
		}
		return answer;
	}
};


int main() {
	board A;
	ifstream file("sudoku.txt");
	string output;
	int storage=0;
	int a = 0;
	if (file.is_open())
	{
			
		for (int i = 0; i < 10; i++) {
			//skips sudoku n
			file >> output;
			file >> output;
			
			
			//===================
			//===P A R S E R=====
			//===================

			cout << "Parsing board: " << i+1 << "/" << 50 << endl;
			for (int y = 0; y < 9; y++) {
				file.get();
				for (int x = 0; x < 9; x++) {
					a = file.get() - '0';
					//cout << a << " ";
					A.insert(x, y, a);
				}
				//cout << endl;
			}
			
			cout << endl;
			//===================
			//== S O L V I N G ==
			//===================
			
			//A.display();
			cout << endl;
			for (int trail = 0; trail < 100; trail++) {
				for (int row = 0; row < 3; row++) {
					for (int col = 0; col < 3; col++) {
						A.chunkSolve(row, col);
						A.update(row, col);
					}
				}
			}
			storage += A.getSpecial();
			//===================
			//== S T O R A G E ==
			//===================
			A.display();
			A.clean();
			
		}

		cout << "Final score: " << storage << endl;
		file.close();
	}
	else cout << "Unable to open file\n";

	system("PAUSE");
	return 0; 
}