/*
    Tanner Fry
    CS 594 - lab 3
    This project can generate and solve sudoku puzzles that are represented by jgraph files. 
    The solver is only guranteed to solve jgraph sudoku puzzles that are created by this program
    because the jgraph format used has a specific format expected.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <random>

using namespace std;

//create a 9x9 grid of 0s
vector<vector<int> > create_grid()
{
    vector<vector<int> > grid;
    for(int i = 0; i < 9; i++)
    {
        vector<int> row;
        for(int j = 0; j < 9; j++)
        {
            row.push_back(0);
        }
        grid.push_back(row);
    }
    return grid;
}

//helper function for printing grid vector
void print_grid(vector<vector<int> > &grid)
{
    for(int i = 0; i < grid.size(); i++)
    {
        for(int j = 0; j < grid[i].size(); j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//write out grid to file
void write_sudoku_table(vector<vector<int> > &grid, ofstream &f)
{
    //static table formatting
    f << "newgraph" << endl;
    f << "(* axis *)" << endl;
    f << "xaxis min -1 max 11 no_draw_axis" << endl;
    f << "no_grid_lines no_draw_hash_marks no_auto_hash_labels grid_gray" << endl;
    f << "yaxis min -1 max 11 no_draw_axis" << endl;
    f << "no_grid_lines no_draw_hash_marks no_auto_hash_labels grid_gray" << endl;
    f << "(* grey lines *)" << endl;
    f << "(* horizontal *)" << endl;
    f << "newline pts 0 1 9 1 color 0.55 0.55 0.55" << endl;
    f << "newline pts 0 2 9 2 color 0.55 0.55 0.55" << endl;
    f << "newline pts 0 4 9 4 color 0.55 0.55 0.55" << endl;
    f << "newline pts 0 5 9 5 color 0.55 0.55 0.55" << endl;
    f << "newline pts 0 7 9 7 color 0.55 0.55 0.55" << endl;
    f << "newline pts 0 8 9 8 color 0.55 0.55 0.55" << endl;
    f << "(* vertical *)" << endl;
    f << "newline pts 1 0 1 9 color 0.55 0.55 0.55" << endl;
    f << "newline pts 2 0 2 9 color 0.55 0.55 0.55" << endl;
    f << "newline pts 4 0 4 9 color 0.55 0.55 0.55" << endl;
    f << "newline pts 5 0 5 9 color 0.55 0.55 0.55" << endl;
    f << "newline pts 7 0 7 9 color 0.55 0.55 0.55" << endl;
    f << "newline pts 8 0 8 9 color 0.55 0.55 0.55" << endl;
    f << "(* dark lines *)" << endl;
    f << "newline pts 0 3 9 3 color 0 0 0" << endl;
    f << "newline pts 0 6 9 6 color 0 0 0" << endl;
    f << "newline pts 3 0 3 9 color 0 0 0" << endl;
    f << "newline pts 6 0 6 9 color 0 0 0" << endl;
    f << "(* outer square *)" << endl;
    f << "newline pts 0 0 0 9" << endl;
    f << "newline pts 0 0 9 0" << endl;
    f << "newline pts 9 9 0 9" << endl;
    f << "newline pts 9 9 9 0" << endl;

    //write out grid values
    string start = "newstring hjc vjc x ";
    float row, col;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(grid[i][j] != 0)
            {
                col = abs(i + 0.5 - 9);
                row = j + 0.5;
                start += to_string(row);
                start += " y ";
                start += to_string(col);
                start += " : ";
                start += to_string(grid[i][j]);
                f << start << endl;
                start = "newstring hjc vjc x ";
            }
        }
    }
    f.close();
}

//read grid from file
vector<vector<int> > read_sudoku_table(ifstream &f)
{
    vector<vector<int> > grid = create_grid();
    vector<string> lines;
    string line;
    string str;
    int x;
    int y;
    int val;

    //read in newstring lines from jgraph file
    while ( getline (f,line) )
    {
        if(line.find("newstring") != string::npos)
        {
            lines.push_back(line);
        }
    }

    //extract numbers and locations
    for(int i = 0; i < lines.size(); i++)
    {
        istringstream iss(lines[i]);
        while (iss >> str)
        {
            if(str == "x")
            {
                iss >> str;
                x = atoi(str.c_str());
            }
            else if(str == "y")
            {
                iss >> str;
                y = atoi(str.c_str());
            }
            else if(str == ":")
            {
                iss >> str;
                val = atoi(str.c_str());
            }
        }
        y = abs(y - 8);
        grid[y][x] = val;
    }
    return grid;
}

//check if value is valid in position given for grid given
int check_position(vector<vector<int> > &grid, int &row, int &col, int &val)
{
    //check current row
    for(int i = 0; i < 9; i++)
    {
        if(grid[row][i] == val)
        {
            return -1;
        }
    }
    
    //check current col
    for(int i = 0; i < 9; i++)
    {
        if(grid[i][col] == val)
        {
            return -1;
        }
    }
    
    //check local 3x3
    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;
    for(int i = start_row; i < start_row + 3; i++)
    {
        for(int j = start_col; j < start_col + 3; j++)
        {
            if(grid[i][j] == val)
            {
                return -1;
            }
        }
    }
    grid[row][col] = val;
    return 0;
}

//solve grid - by reference so grid passed is edited. uses recursive backtracking
bool solve_grid(vector<vector<int> > &grid)
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(grid[i][j] == 0)
            {
                for(int val = 1; val <= 9; val++)
                {
                    if(check_position(grid, i, j, val) == 0)
                    {
                        if(solve_grid(grid))
                        {
                            return true;
                        }
                        grid[i][j] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

//check if grid is solvable. same as solve_grid() but grid is not passed by reference
bool grid_solvable(vector<vector<int> > grid)
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(grid[i][j] == 0)
            {
                for(int val = 1; val <= 9; val++)
                {
                    if(check_position(grid, i, j, val) == 0)
                    {
                        if(solve_grid(grid))
                        {
                            return true;
                        }
                        grid[i][j] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

//create randome sudoku grid
vector<vector<int> > create_random_puzzle()
{
    vector<vector<int> > grid = create_grid();
    vector<int> vals;
    vector<int> cols;
    int random_val, random_val_index = 0;
    int random_col, random_col_index = 0;
    int nums = 0;
    int min = 1;
    int max = 9;
    int max_iterations = 1000;
    //put a random number of numbers in each row at random columns
    for(int i = 0; i < grid.size(); i++)
    {
        nums = 1 + rand() % (( 6 + 1 ) - 1);
        vals = {1,2,3,4,5,6,7,8,9};
        cols = {0,1,2,3,4,5,6,7,8};
        for(int j = 0; j < nums; j++)
        {
            random_val_index = 0 + rand() % (( vals.size() + 1 ) - 1);
            random_col_index = 0 + rand() % (( cols.size() + 1 ) - 1);
            random_val = vals[random_val_index];
            random_col = cols[random_col_index];
            if(check_position(grid, i, random_col, random_val) == 0)
            {
                grid[i][random_col] = random_val;
                if(grid_solvable(grid) == false)
                {
                    grid[i][random_col] = 0;
                    j--;
                }
                else
                {
                    vals.erase(vals.begin() + random_val_index);
                    cols.erase(cols.begin() + random_col_index);
                }
            }
            else
            {
                j--;
            }
        }

        //safety net
        max_iterations--;
        if(max_iterations == 0)
        {
            return grid;
        }
    }
    return grid;   
}

//print usage
void print_usage()
{
    cerr << "Usage: \n\tCREATE filename.jgraph" << endl;
    cout << "\tSOLVE filename.jgr" << endl;
}


int main(int argc, char *argv[])
{   
    //vars
    random_device rd;
    srand( rd() );
    vector<vector<int> > grid;
    int create;
    string filename;
    string output_file;
    //check args
    if(argc == 3)
    {
        if (strcmp(argv[1], "SOLVE") == 0)
        {
            create = 0;
        } 
        else if(strcmp(argv[1], "CREATE") == 0)
        {
            create = 1;
        }
        else
        {
            print_usage();
            exit(-1);
        }
        
        if(string(argv[2]).find(".jgr") == string::npos)
        {
            print_usage();
            exit(-1);
        }
    
        filename = string(argv[2]);

    }
    else
    {
        print_usage();
        exit(-1);
    }
    
    //create or solve puzzle
    if(create == 1)
    {
        //create random sudoku puzzle
        ofstream ofile(filename.c_str());
        grid = create_random_puzzle();
        if(ofile.is_open())
        {
            write_sudoku_table(grid, ofile);
        }
    }
    else
    {
        //solve sudoku puzzle
        ifstream ifile(filename.c_str());
        output_file = filename.erase(filename.find("."), 4);
        output_file = output_file + "_solution.jgr";
        ofstream ofile(output_file.c_str());
        if(ifile.is_open())
        {
            //read in grid
            grid = read_sudoku_table(ifile);
            //solve grid
            solve_grid(grid);
            //write grid
            if(ofile.is_open())
            {
                write_sudoku_table(grid, ofile);
            }
            else
            {
                cerr << "Could not create solution file." << endl;
                exit(-1);
            }
        }   
        else
        {
            cerr << "Could not open file." << endl;
            exit(-1);
        }   
    }
    
    return 0;
}