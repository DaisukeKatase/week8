#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う

void my_init_cells(const int height, const int width, int cell[height][width], FILE *fp) {
  if(fp == NULL) {
    cell[20][30] = 1;
    cell[22][30] = 1;
    cell[22][31] = 1;
    cell[23][31] = 1;
    cell[20][32] = 1;
  }
  else {
    int x,y;
    char buf[100];
    fscanf(fp, "%s\n", buf);
    fscanf(fp, "%s\n", buf);
    while(fscanf(fp, "%d %d\n", &x, &y) != EOF) {
      cell[y][x] = 1;    
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

  int cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = 0;
    }
  }

  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if ( argc > 2 ) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  }
  else if (argc == 2) {
    FILE *lgfile;
    if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
      my_init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile);
  }
  else{
    my_init_cells(height, width, cell, NULL); // デフォルトの初期値を使う
  }

  my_print_cells(fp, 0, height, width, cell); // 表示する
  double r = cell_rate(height, width, cell);
  fprintf(fp, "Rate of living cells: %lf%\n", r);
  sleep(1); // 1秒休止
  fprintf(fp,"\e[%dA",height+4);//height+3 の分、カーソルを上に戻す(壁2、表示部1)

  /* 世代を進める*/
  for (int gen = 1 ;; gen++) {
    my_update_cells(height, width, cell); // セルを更新
    my_print_cells(fp, gen, height, width, cell);  // 表示する
    double r = cell_rate(height, width, cell);
    fprintf(fp, "Rate of living cells: %lf%\n", r);
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+4);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}
