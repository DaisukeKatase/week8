#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include <time.h>

void my_init_cells(const int height, const int width, int cell[height][width]) {
  double rate = 0.1;
  double i = 0;
  while(i < height*width*rate) {
      int random_x, random_y;
      random_x = rand()%width;
      random_y = rand()%height;
      if(cell[random_y][random_x] == 0) {
          cell[random_y][random_x] = 1;
          i += 1.0;
      }
      else {
          continue;
      }
  }
}

void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]) {
  int x,y;
  fprintf(fp, "generation = %d\n", gen);
  fprintf(fp, "+");
  for(x = 0; x < width; ++x) {
    fprintf(fp, "-");
  }
  fprintf(fp, "+\n");
  for(y = 0; y < height; ++y) {
    fprintf(fp, "|");
    for(x = 0; x < width; ++x) {
      if(cell[y][x] == 1) {
        fprintf(fp, "\e[31m#\e[0m");
      }
      else {
        fprintf(fp, " ");
      }
    }
    fprintf(fp, "|\n");
  }
  fprintf(fp, "+");
  for(x = 0; x < width; ++x) {
    fprintf(fp, "-");
  }
  fprintf(fp, "+\n");
}

void my_update_cells(const int height, const int width, int cell[height][width]) {
  int nextcell[height][width];
  int x,y;
  for(y = 0; y < height; ++y) {
    for(x = 0; x < width; ++x) {
      nextcell[y][x] = 0;
    }
  }
  int counter = 0;
  int i,j;
  for(y = 0; y < height; ++y) {
    for(x = 0; x < width; ++x) {
      if(y == 0) {
        if(x == 0) {
          for(i = 0; i <= 1; ++i) {
            for(j = 0; j <= 1; ++j) {
              if(i == 0 && j == 0) {
                continue;
              }
              else {
                if(cell[y+i][x+j] == 1) {
                  counter += 1;
                }
              }
            }
          }          
        }  //左上
        else if(x == width-1) {
          for(i = 0; i <= 1; ++i) {
            for(j = -1; j <= 0; ++j) {
              if(i == 0 && j == 0) {
                continue;
              }
              else {
                if(cell[y+i][x+j] == 1) {
                  counter += 1;
                }
              }
            }
          }
        }  //右上
        else {
          for(i = 0; i <= 1; ++i) {
            for(j = -1; j <= 1; ++j) {
              if(i == 0 && j == 0) {
                continue;
              }
              else {
                if(cell[y+i][x+j] == 1) {
                  counter += 1;
                }
              }
            }
          }
        }
      }  //上一列(角を除く)

      else if(y == height-1) {
        if(x == 0) {
          for(i = -1; i <= 0; ++i) {
            for(j = 0; j <= 1; ++j) {
              if(i == 0 && j == 0) {
                continue;
              }
              else {
                if(cell[y+i][x+j] == 1) {
                  counter += 1;
                }
              }
            }
          }          
        }  //左下
        else if(x == width-1) {
          for(i = -1; i <= 0; ++i) {
            for(j = -1; j <= 0; ++j) {
              if(i == 0 && j == 0) {
                continue;
              }
              else {
                if(cell[y+i][x+j] == 1) {
                  counter += 1;
                }
              }
            }
          }
        }  //右下
        else {
          for(i = -1; i <= 0; ++i) {
            for(j = -1; j <= 1; ++j) {
              if(i == 0 && j == 0) {
                continue;
              }
              else {
                if(cell[y+i][x+j] == 1) {
                  counter += 1;
                }
              }
            }
          }
        } //下一列(角を除く)
      }

      else if(x == 0) {
        for(i = -1; i <= 1; ++i) {
          for(j = 0; j <= 1; ++j) {
            if(i == 0 && j == 0) {
              continue;
            }
            else {
              if(cell[y+i][x+j] == 1) {
                counter += 1;
              }
            }
          }
        }
      } //左一列(角は除く)

      else if(x == width-1) {
        for(i = -1; i <= 1; ++i) {
          for(j = -1; j <= 0; ++j) {
            if(i == 0 && j == 0) {
              continue;
            }
            else {
              if(cell[y+i][x+j] == 1) {
                counter += 1;
              }
            }
          }
        }
      } //右一列(角を除く)

      else {
        for(i = -1; i <= 1; ++i) {
          for(j = -1; j <= 1; ++j) {
            if(i == 0 && j == 0) {
              continue;
            }
            else {
              if(cell[y+i][x+j] == 1) {
                counter += 1;
              }
            }
          }
        }
      }  //内部の点

      if(counter == 3) {
        nextcell[y][x] = 1;
      }
      else if(cell[y][x] == 1 && counter == 2) {
        nextcell[y][x] = 1;
      }
      counter = 0;  //周囲のセルの数の条件によって、次世代のセルの有無を決定する
    }
  }
  
  for(y = 0; y < height; ++y) {
    for(x = 0; x < width; ++x) {
      cell[y][x] = nextcell[y][x];
    }
  } //cellを書き換える
}

double cell_rate(const int height, const int width, int cell[height][width]) {
    double r;
    double sum = 0.0;
    int x,y;
    for(y = 0; y < height; ++y) {
        for(x = 0; x < width; ++x) {
            if(cell[y][x] == 1) {
                sum += 1.0;
            }
        }
    }
    r = sum/((double)height*(double)width) * 100;
    return r;
}

int main(int argc, char **argv)
{
  FILE *fp = stdout;
  const int height = 40;
  const int width = 70;
  srand((unsigned)time(NULL));

  int cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = 0;
    }
  }
  
  my_init_cells(height, width, cell);
  
  my_print_cells(fp, 0, height, width, cell); // 表示する
  double r = cell_rate(height, width, cell);
  fprintf(fp, "Rate of living cells: %lf%\n", r);
  sleep(1); // 1秒休止
  fprintf(fp,"\e[%dA",height+4);//height+4 の分、カーソルを上に戻す(壁2、表示部2)

  /* 世代を進める*/
  for (int gen = 1 ;; gen++) {
    my_update_cells(height, width, cell); // セルを更新
    my_print_cells(fp, gen, height, width, cell);  // 表示する
    double r = cell_rate(height, width, cell);
    fprintf(fp, "Rate of living cells: %lf%\n", r);
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+4);//height+4 の分、カーソルを上に戻す(壁2、表示部2)
  }

  return EXIT_SUCCESS;
}