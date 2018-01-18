#include <math.h>
#include <algorithm>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <unordered_map>
using namespace std;
unordered_map<int, vector<int>> domain; //key is the slot's index in the board, value is this slot's domain, 

int constraint[256][18];
// vector<vector<int>> constraint;

// void initDomain(int(&board)[16][16], int x, int y);

int removeValue(int &value, vector<int> &vec)
{
	for (int i = 0; i<vec.size(); i++)
	{
		if (vec[i] == value)
		{
			vec.erase(vec.begin() + i, vec.begin() + i + 1);
			return 0;
		}

	}
	return -1;
}

void initDomain(int(&board)[16][16], int x, int y)
{
	int index = x * 16 + y;
	auto it = domain.find(index);
	for (int i = 0; i<16; i++)
	{
		// int index2 = i * 16 + y;
		if (!board[i][y] || i == x) continue;
		else
		{
			removeValue(board[i][y], it->second);
		}
	}

	for(int j = 0; j<16; j++)
	{
		// int index2 = x * 16 + j;
		if(!board[x][j] || j == y) continue;
		else
		{
			removeValue(board[x][j], it->second);
		}
	}

	int bigrow = x / 4;
	int bigcol = y / 4;
	for(int m = 4 * bigrow; m < 4 * (bigrow + 1); m++)
	{
		for (int n = 4 * bigcol; n < 4 * (bigcol + 1); n++)
		{
			int index2 = m * 16 + n;
			if (!board[m][n] || m == x || n == y) continue;
			removeValue(board[m][n], it->second);
		}
	}

}

int Count(int(&board)[16][16], int &x, int &y, int &value) {
	int count = 0;
	for (int i = 0; i < 16; i++) {
		if (board[x][i])
		{
			if (board[x][i] == value)
				count++;
		}
		if (board[i][y])
		{
			if (board[i][y] == value)
				count++;
		}
	}

	int bigrow = x / 4, bigcol = y / 4;
	for (int m = 4 * bigrow; m < 4 * (bigrow + 1); m++)
	{
		for (int n = 4 * bigcol; n < 4 * (bigcol + 1); n++)
		{
			if (board[m][n] && m != x && n != y) {
				if (board[m][n] == value)
					count++;
			}
		}
	}
	return count;
}
void initBoard(int(&board)[16][16])
{
	for (int i = 0; i<16; i++)
	{
		for (int j = 0; j<16; j++)
		{
			// int index = i * 16 + j;
			if (!board[i][j])
			{
				initDomain(board, i, j);
			}
		}
	}
}

bool forwardChecking(int &x, int &y)
{
	for (int q = 1; q <= 16; q++)
	{
		int index = x * 16 + y;
		if (!constraint[index][q])
			return false;
	}
	return true;
}

bool Assign(int(&board)[16][16], int &x, int &y, int &value) {
	board[x][y] = value;
	bool success = true;
	int index;
	// cout<<"[Assign]"<<"value:"<<value<<" x:"<<x<<" y:"<<endl;
	for(int i = 0; i < 16; i++) {
		if(!board[x][i]) {
			index = x * 16 + i;
			constraint[index][value]++;
			if(constraint[index][value] == 1)
			{
				constraint[index][17]++;
				if(constraint[index][17] >= 16)
				{
					if(forwardChecking(x, i))
						success = false;
				}
				// cout<<"index:"<<x*16+i<<" count"<<constraintCount[x][i]<<endl;
			}
		}

		if(!board[i][y]) {
			index = i * 16 + y;
			constraint[index][value]++;
			if(constraint[index][value] == 1)
			{
				constraint[index][17]++;
				// cout<<"index:"<<i*16+y<<" count"<<constraintCount[i][y]<<endl;
				if(constraint[index][17] >= 16) {
					if (forwardChecking(i, y))
						success = false;
				}
			}
		}

	}

	int bigrow = x / 4, bigcol = y / 4;
	for (int m = 4 * bigrow; m < 4 * (bigrow + 1); m++)
	{
		for (int n = 4 * bigcol; n < 4 * (bigcol + 1); n++)
		{
			if (!board[m][n]) {
				index = m * 16 + n;
				constraint[index][value]++;
				if (constraint[index][value] == 1)
				{
					constraint[index][17]++;
					if (constraint[index][17] >= 16)
					{
						if (forwardChecking(m, n))
							success = false;
					}
				}	
				// if (constraintCount[m][n] == 16) ans = -1;
			}
		}
	}
	return success;
}


void Recover(int(&board)[16][16], int &x, int &y, int &value) {
	// count[value]--;
	int index;
	for (int i = 0; i < 16; i++) {
		if (!board[x][i]) {
			index = x * 16 + i;
			constraint[index][value]--;
			if (!constraint[index][value])
				constraint[index][17]--;
		}
		if (!board[i][y]) {
			index = i * 16 + y;
			constraint[index][value]--;
			if (!constraint[index][value])
				constraint[index][17]--;
		}
	}
	int bigrow = x / 4, bigcol = y / 4;
	for (int m = 4 * bigrow; m < 4 * (bigrow + 1); m++)
	{
		for (int n = 4 * bigcol; n < 4 * (bigcol + 1); n++)
		{
			if (!board[m][n]) {
				index = m * 16 + n;
				constraint[index][value]--;
				if (!constraint[index][value])
					constraint[index][17]--;
			}
		}
	}
	board[x][y] = 0;
}

bool SortByValueConstraint(pair<int, int> p1, pair<int, int> p2)
{
	return p1.second > p2.second;
}

int getMostConstraintVariable(int(&board)[16][16])
{
	int max = -1;
	int index;
	int res;
	for (int i = 0; i<16; i++)
	{
		for (int j = 0; j<16; j++)
		{
			// cout<<constraintCount[i][j]<<" ";
			index = i * 16 + j;
			if (board[i][j]) continue;
			if (constraint[index][17] > max) {
				max = constraint[index][17];
				res = index;
			}
		}
	}
	if (max == -1) return -1;
	// cout<<"Max:"<<max<<endl;
	return res;
}

int valueCount(int(&board)[16][16], int &x, int &y, int &value)
{
	int count = 0;
	int index;
	for (int i = 0; i < 16; i++) {
		index = x * 16 + i;
		if (!board[x][i] && constraint[index][value]) {
			count++;
		}
		index = i * 16 + y;
		if (!board[i][y] && constraint[index][value]) {
			count++;
		}

	}

	int bigrow = x / 4, bigcol = y / 4;
	for (int m = 4 * bigrow; m < 4 * (bigrow + 1); m++)
	{
		for (int n = 4 * bigcol; n < 4 * (bigcol + 1); n++)
		{
			if (!board[m][n] && constraint[index][value]) {
				count++;
			}
		}
	}
	return count;
}

void getLeastRectrictValue(int(&board)[16][16], vector<pair<int, int>> &vec, vector<int> &domain, int &x, int &y)
{
	for (int c = 0;c < domain.size();c++)
	{
		int value = domain[c];
		int index = x * 16 + y;
		if (!constraint[index][value]) {
			// cout<< value << endl;
			int count = valueCount(board, x, y, value);
			// cout<<"count:"<<count<<endl;
			//Recover(nx,ny);
			vec.push_back(make_pair(value, count));
		}
	}
	sort(vec.begin(), vec.end(), SortByValueConstraint);
}

int backTrackCount = 0;
int nodes = 0;
bool dfs(int(&board)[16][16])
{
	nodes++;
	int index = getMostConstraintVariable(board);
	if(index == -1)
		return true;

	int x = index / 16;
	int y = index % 16;

	auto it = domain.find(index);
	vector<pair<int, int> > valueConstraint;
	getLeastRectrictValue(board,valueConstraint, it->second, x, y);

	for(int i = 0; i<valueConstraint.size(); i++)
	{
		bool res = Assign(board, x, y, valueConstraint[i].first);
		//  cout<<"[Assign]"<<"value:"<<valueConstraint[i].second<<" x:"<<x<<" y:"<<y<<endl;
		if(!res) {
			Recover(board,x, y, valueConstraint[i].first);
			continue;
		}
		if(dfs(board))
			return true;
		Recover(board, x, y, valueConstraint[i].first);
		// cout<<"[Recover]"<<"value:"<<valueConstraint[i].second<<" x:"<<x<<" y:"<<y<<endl;

	}
	backTrackCount++;
	return false;
}

int main() {
	int board[16][16] = {0};
	vector<int> originVec;
	vector<int> constraintVec;
	for (int i = 1; i <= 16; i++)
	{
		originVec.push_back(i);
		constraintVec.push_back(0);
	}
	constraintVec.push_back(0);
	constraintVec.push_back(0);
	// for(int i=0;i<256;i++)
	// 	constraint.push_back(constraintVec);

	ofstream sol;
	string fileName = "hexa-82.txt";
	string solution = "hexa-82_solution.txt";
	sol.open(solution, std::ofstream::out);

	ifstream infile;
	char arr[256];
	infile.open(fileName);
	assert(infile.is_open());
	char c;
	bool flag = false;

	int count = 0;
	while (!infile.eof())
	{
		infile >> c;
		if (c == '-') {
			flag = true;
			continue;
		}
		if (flag)
		{
			c = '*';
			flag = false;
		}
		arr[count] = c;
		count++;
	}

	infile.close();

	cout << "[initial]: -------------------------------------------------" << endl;
	for (int i = 0; i<16; i++) {
		for (int j = 0; j<16; j++) {
			int row = i;
			int col = j;
			int index = i * 16 + j;
			// domainSize.push_back(index);
			int temp;
			switch (arr[index]) {
			case 'A': {temp = 10; break; }
			case 'B': {temp = 11; break; }
			case 'C': {temp = 12; break; }
			case 'D': {temp = 13; break; }
			case 'E': {temp = 14; break; }
			case 'F': {temp = 15; break; }
			case 'G': {temp = 16; break; }
			case '*': {temp = -1; break; }
			default: {temp = arr[index] - '0'; break; }
			}
			cout << temp << '\t';
			temp++;
			board[i][j] = temp;
			for (int i = 0; i<16; i++)
			{
				for (int j = 0; j<16; j++)
				{
					if (board[i][j]) {
						//board[i][j] = true;
						Assign(board,i, j, board[i][j]);
						// cout << i <<" "<< j << endl;
						// cout <<constraint[1][2][1] <<endl;
					}
				}
			}
			if (temp == 0)
			{
				domain.insert(make_pair(index, originVec));
			}
			// else
			// {
			// 	auto ite = valueCount.find(temp);
			// 	if (ite != valueCount.end())
			// 	{
			// 		ite->second++;
			// 	}
			// }

		}
		cout << endl;
	}

	clock_t start;
	clock_t end;
	start = clock();

	// initBoard();
	dfs(board);

	end = clock();
	double duration;
	duration = (end - start) / (double)CLOCKS_PER_SEC;
	cout << fileName << " Time: " << duration * 1000 << "ms" << endl;

	// cout << "[result]: ----------------------------------------------------" << endl;
	// for (int i = 0; i<16; i++) {
	// 	for (int j = 0; j<16; j++) {
	// 		cout << board[i][j] << '\t';
	// 	}
	// 	cout << endl;
	// }

	cout << "[standard result]: ----------------------------------------------------" << endl;
	for (int i = 0; i<16; i++) {
		for (int j = 0; j<16; j++) {
			board[i][j]--;
			switch (board[i][j]) {
			case 10: {cout << 'A' << '\t'; sol<<'A';break; }
			case 11: {cout << 'B' << '\t'; sol<<'B';break; }
			case 12: {cout << 'C' << '\t'; sol<<'C';break; }
			case 13: {cout << 'D' << '\t'; sol<<'D'; break; }
			case 14: {cout << 'E' << '\t'; sol<<'E'; break; }
			case 15: {cout << 'F' << '\t'; sol<<'F'; break; }
			case 16: {cout << 'G' << '\t'; sol<<'G'; break; }
			default: {cout << board[i][j] << '\t'; sol<<board[i][j];break; }
			}
		}
		cout << endl;
	}
	cout<<"BackTrack Count:"<<backTrackCount<<"	 Search space nodes generated:"<<nodes<<endl;
	return 0;
}
