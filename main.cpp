#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

const int Grid_rows = 20;
const int Grid_columns = 20;
const int secArray_size = 100;
const int neiArray_size = 300;

void start();
void readDataFromFile(string name_of_file, int *num_of_gen, int *num_of_life_cells, int *sec_Array);
void gridPopulate(int main_Grid[][20], int num_of_life_cell,int sec_array[]);
void displayGrid(int main_Grid[][20]);
void populateNeiArray(int num_of_life_cell, int sec_array[], int formula[], int main_grid[][Grid_columns], int nei_array[], int &number_of_nei);
void secArrayLiveCount(int num_of_life_cell,int sec_array[], int formula[],int main_grid[][Grid_columns], int live_nei_live_count[]);
void neiArrayLiveCount(int nei_array[],int number_of_nei, int formula[], int main_grid[][Grid_columns], int nei_nei_live_count[], int num_of_life_cell,int sec_array[]);
void liveDeadOrSurvive(int &num_of_life_cell, int sec_array[], int live_nei_live_count[]);
void writeDataInFile(string nameoffile, int num_of_gen,int num_of_life_cells,int sec_Array[]);
void reInitializeArrays(int sec_Array[],int nei_array[],int nei_nei_live_count[],int live_nei_live_count[],int main_grid[][Grid_columns]);
void neiLiveOrDead(int number_of_nei, int nei_array[],int nei_nei_live_count[], int sec_array[], int &num_of_life_cell);
void displayFileData(int sec_array[], int num_of_gen, int num_of_life_cell);

int main()
{
    start();
    return 0;
}

void start()
{


    string name_of_file;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  GAME OF LIFE  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nMade by : Bilal ali\nrollno : 19i-2155"<<endl<<endl;
    cout<<"Enter the file name : ";
    getline(cin, name_of_file);
    cout<<name_of_file<<endl;
    int formula[] = {-1,-1,   -1,0,   -1,1,   0,-1,   0,1,   1,-1,   1,0,   1,1};
    int num_of_gen;
    int num_of_life_cells;
    int number_of_nei = 0;
    int gen_times = 0;
    int sec_Array[secArray_size];
    int nei_array[neiArray_size] = {0};
    int nei_nei_live_count[secArray_size] = {0};
    int live_nei_live_count[neiArray_size] =  {0};
    int main_grid[Grid_rows][Grid_columns] = {0};


    do{
        reInitializeArrays(sec_Array,nei_array,nei_nei_live_count,live_nei_live_count,main_grid);
        readDataFromFile(name_of_file, &num_of_gen, &num_of_life_cells, sec_Array);
        gridPopulate(main_grid, num_of_life_cells, sec_Array);
        displayGrid(main_grid);
        populateNeiArray(num_of_life_cells,sec_Array,formula,main_grid,nei_array,number_of_nei);
        secArrayLiveCount(num_of_life_cells,sec_Array,formula,main_grid,live_nei_live_count);
        neiArrayLiveCount(nei_array,number_of_nei,formula,main_grid,nei_nei_live_count,num_of_life_cells,sec_Array);
        liveDeadOrSurvive(num_of_life_cells,sec_Array,live_nei_live_count);
        neiLiveOrDead(number_of_nei,nei_array,nei_nei_live_count,sec_Array,num_of_life_cells);
        writeDataInFile(name_of_file,num_of_gen,num_of_life_cells,sec_Array);
        gridPopulate(main_grid, num_of_life_cells, sec_Array);
        displayGrid(main_grid);
        gen_times++;

//    cout<<endl<<endl<<endl<<"***************************    Gen 1   *************************************"<<endl<<endl<<endl;
//
//
//    cout<<endl<<"***** This is Sec Array *****************"<<endl;
//    for(int i = 0; i < num_of_life_cells*2; i+=2)
//    {
//        cout<<sec_Array[i]<<" : "<<sec_Array[i+1]<<endl;
//    }
//    cout<<"**********************************************************************************************************"<<endl<<endl;

    }while(gen_times<num_of_gen);

    //displayFileData(sec_Array,num_of_gen,num_of_life_cells);
}


/*
    This function is used for taking input from file
    Input:
        name of file
    output:
        number of generation
        number of live cells
        life cells coordinates in sec-array
*/
void readDataFromFile(string name_of_file, int *num_of_gen, int *num_of_life_cells, int *sec_Array)
{
    string line;
    int count = 0;
    int sec_index_x = 0;
    int sec_index_y = 1;

    ifstream myfile (name_of_file.c_str());

  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
        switch(count){
        case 0:
            {
                // Store the number of generation in variable
                stringstream string_to_int(line);
                string_to_int >> *num_of_gen;
                count++;
                break;
            }
        case 1:
            {
                // Store the number of Life cell in variable
                stringstream string_to_int(line);
                string_to_int >> *num_of_life_cells;
                count++;
                break;
            }
        case 2:
            {
                sec_Array[sec_index_x] = line[0] - '0';
                sec_Array[sec_index_y] = line[2] - '0';
                sec_index_x += 2;
                sec_index_y += 2;
                break;
            }

        }
    }
    myfile.close();
  }

  else cout << "Unable to open file";
}

/*
    This function is used for setting up main grid using sec-array
    Input:
        number of life cells
        sec-array
    output:
        populate main grid with 0 and 1: 0 for dead cells and 1 for alive cell
*/
void gridPopulate(int main_Grid[][Grid_columns], int num_of_life_cell,int sec_array[])
{
   for(int i = 0; i < Grid_columns; i++)
   {
       for(int j = 0; j<Grid_columns; j++)
       {
           main_Grid[i][j] = 0;
       }
   }

  for(int i = 0; i < num_of_life_cell*2; i+=2)
  {
      main_Grid[sec_array[i]][sec_array[i+1]] = 1;
  }
}

/*
    This function is used to display main grid
    Input:
        main grid
    output:
        print main grid on screen
*/
void displayGrid(int main_Grid[][Grid_columns])
{
    //Printing The Main Grid
  for(int i = 0; i < Grid_columns ; i++)
  {
      for(int j = 0; j<Grid_columns; j++)
      {
          cout<<main_Grid[i][j]<<"   ";
      }
      cout<<endl;
  }
}

/*
    This function is used for finding the neighbor of live cells and store in neighbor array
    Input:
        number of life cells
        sec-array
        formula for finding neighbor
        main grid
    output:
        neighbor array containing neighbor coordinates
        number of neighbors
*/
void populateNeiArray(int num_of_life_cell, int sec_array[], int formula[], int main_grid[][Grid_columns], int nei_array[], int &number_of_nei)
{
     for(int i = 0; i < num_of_life_cell*2; i+=2)
  {
      // Extracting index of live cell
      int cell_x = sec_array[i];
      int cell_y = sec_array[i+1];
      for(int j = 0; j < 8*2; j+=2)
      {
          // Extracting value of neighbor cell
          int nei_x = cell_x + formula[j];
          int nei_y = cell_y + formula[j+1];
          if(main_grid[nei_x][nei_y] == 0)
          {
              //this condition will be true if cell is dead

                bool flag = true; // assuming if cell is not present in neighbor array

                // Initial index value of first neighbor in neighbor array
                int temp_i_x = 0;
                int temp_i_y = 1;

                // for checking if neighbor is not already in neighbor array
                for(int k = 0; k < number_of_nei; k++)
                    {
                            if(nei_x == nei_array[temp_i_x] && nei_y == nei_array[temp_i_y])
                            {
                                // if this condition is true this means cell is already in neighbor
                                flag = false;
                                break;
                            }
                            // shifting to next cell index
                            temp_i_x += 2;
                            temp_i_y += 2;
                    }

                if(flag)
                {
                    // If this condition is true then it means neighbor is not already present and we are inserting that neighbor
                    nei_array[number_of_nei * 2]    = nei_x;
                    nei_array[number_of_nei *2 + 1] = nei_y;
                    number_of_nei++;
                }

          }

      }

  }
}

/*
    This function is used for counting live Neighbors Of Live Cells
    Input:
        sec-array
        formula
        Empty Array for count of live neighbors of alive cells
    output:
        Filled Array for count of live neighbors of alive cells
*/
void secArrayLiveCount(int num_of_life_cell,int sec_array[], int formula[],int main_grid[][Grid_columns], int live_nei_live_count[])
{
    int index_of_live_count = 0;
    for(int i = 0; i < num_of_life_cell*2; i+=2)
  {
      // Extracting index of live cell
      int cell_x = sec_array[i];
      int cell_y = sec_array[i+1];
      for(int j = 0; j < 8*2; j+=2)
      {
          // Extracting value of neighbor cell
          int nei_x = cell_x + formula[j];
          int nei_y = cell_y + formula[j+1];

          for(int x = 0; x < num_of_life_cell*2; x+=2)
          {
              if(nei_x == sec_array[x] && nei_y == sec_array[x+1])
              {
                  live_nei_live_count[index_of_live_count] = ++live_nei_live_count[index_of_live_count];
              }
          }
      index_of_live_count++;
  }
}
}
/*
    This function is used for counting live neighbors Of neighbor Cells
    Input:
        nei-array
        formula
        Empty Array for count of live neighbors of alive cells
    output:
        Filled Array for count of live neighbors of alive cells
*/

void neiArrayLiveCount(int nei_array[],int number_of_nei, int formula[], int main_grid[][Grid_columns], int nei_nei_live_count[], int num_of_life_cell, int sec_array[])
{
    int index_of_live_count = 0;
    for(int i = 0; i < number_of_nei*2; i+=2)
  {
      // Extracting index of live cell
      int cell_x = nei_array[i];
      int cell_y = nei_array[i+1];
      for(int j = 0; j < 8*2; j+=2)
      {
          // Extracting value of neighbor cell
          int nei_x = cell_x + formula[j];
          int nei_y = cell_y + formula[j+1];

          for(int x = 0; x < num_of_life_cell*2; x+=2)
          {
              if(nei_x == sec_array[x] && nei_y == sec_array[x+1])
              {
                  nei_nei_live_count[index_of_live_count] = ++nei_nei_live_count[index_of_live_count];
              }
          }
      index_of_live_count++;
  }
}
}
void liveDeadOrSurvive(int &num_of_life_cell, int sec_array[], int live_nei_live_count[])
{
    int cel_index_for_del = 0;
    int count_index = 0;
    for(int i = 0; i < num_of_life_cell; i++)
        {
            int cell_x = sec_array[i];
            int cell_y = sec_array[i+1];

            if(( live_nei_live_count[count_index] < 2 )  || ( live_nei_live_count[count_index] > 3 ))
            {
                int update_count_index = count_index;
                for(int k = cel_index_for_del; k < num_of_life_cell*2; k+=2)
                {
                    sec_array[k] = sec_array[k+2];
                    sec_array[k+1] = sec_array[k+3];
                    live_nei_live_count[update_count_index] = live_nei_live_count[update_count_index+1];
                    update_count_index++;
                }
                num_of_life_cell--;
                --i;
            }
            else
            {
               count_index++;
               cel_index_for_del += 2;
            }


    }
}
void neiLiveOrDead(int number_of_nei, int nei_array[],int nei_nei_live_count[], int sec_array[], int &num_of_life_cell)
{
    //int cel_index_for_del = 0;
int count_index = 0;
for(int i = 0; i < number_of_nei*2 ; i+=2)
    {
        int nei_x = nei_array[i];
        int nei_y = nei_array[i+1];

        if(( nei_nei_live_count[count_index] == 3 ))
            {
                sec_array[num_of_life_cell*2] = nei_x ;//x
                sec_array[num_of_life_cell*2+1] = nei_y ;//y
                num_of_life_cell++;
            }

        count_index++;
    }
}

void writeDataInFile(string nameoffile, int num_of_gen,int num_of_life_cells,int sec_Array[])
{
    ofstream outputFile;
    outputFile.open(nameoffile.c_str(), ios::trunc);
    if(outputFile.is_open())
    {
        outputFile << num_of_gen;
        outputFile << "\n";
        outputFile << num_of_life_cells;
        outputFile << "\n";
        int index_of_live_count = 0;
        for(int i = 0; i < num_of_life_cells*2; i+=2)
        {
            int cell_x = sec_Array[i];
            int cell_y = sec_Array[i+1];
            outputFile << cell_x;
            outputFile << " ";
            outputFile << cell_y;
            outputFile << "\n";
        }

    }
    outputFile.close();
}

void reInitializeArrays(int sec_Array[],int nei_array[],int nei_nei_live_count[],int live_nei_live_count[],int main_grid[][Grid_columns])
{
    // for 100 elements arrays
    for(int i=0;i<99;i++)
    {
        nei_nei_live_count[i]=0;
        sec_Array[i]=0;

    }
    // for 200 elements arrays
    for(int i=0;i<299;i++)
    {
        nei_array[i] = 0;
        live_nei_live_count[i] = 0;
    }
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            main_grid[i][j] = 0;
        }
    }

}
void displayFileData(int sec_array[], int num_of_gen, int num_of_life_cell)
{
    cout<<"Output File"<<endl;
    cout<<endl<<num_of_gen<<endl;
    cout<<endl<<num_of_life_cell<<endl;
    for(int i=0;i<num_of_life_cell*2; i+=2)
    {
        cout<<sec_array[i]<<" "<<sec_array[i+1]<<endl;
    }
}
