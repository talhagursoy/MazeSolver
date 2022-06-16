#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Windows.h>

enum parcalar{
	empty,
	wall,
	cikis,
	edge,
	elma,
	trail
};

char** maze;
int** visited;
int satir;
int sutun;
int score=0;

void allocMaze(){
	maze = malloc(satir * sizeof(char*));
	int i;
	for (i = 0; i < satir; ++i){
		maze[i] = malloc(sutun * sizeof(char*));
	}
}

void readMaze(char* fileName,int* start_satir,int* start_sutun){
	char c;
	char rows[2]={'\0'};
	char cols[2]={'\0'};
	int rows_i=0,cols_i=0;
	int swap = 0;
	FILE* mazeFile = fopen(fileName, "r");
	if (mazeFile){
		while ((c = getc(mazeFile)) != '\n') {
			if (c == ','){
				swap = 1;
			} 
			else if (swap==0) {
				rows[rows_i] = c;
				rows_i++;
			} 
			else {
				cols[cols_i] = c;
				cols_i++;
			}
		}
	} 
	else {
		printf("File could not be opened");
		return;
	}
	satir=atoi(rows);
	sutun=atoi(cols);
	allocMaze();
	int i,j;
	if(mazeFile){
		for (i=0;i<satir;++i) {
			for (j=0;j<sutun;++j) {
				if ((c = getc(mazeFile)) == '\n'){
					c = getc(mazeFile);
				}
				maze[i][j] = c;
				if (c=='b'){
					*start_satir=i;
					*start_sutun=j;
				}	
			}
		}
	}
	else{
		printf("Problem with file reading");
	}
	fclose(mazeFile);
}

void allocVisited(){
	visited = malloc(satir * sizeof(int*));
	int i;
	for (i = 0; i < satir; ++i){
		visited[i] = malloc(sutun * sizeof(int*));
	}
}

void initVisited(){
	allocVisited();
	int i, j;
	for (i = 0; i < satir; ++i) {
		for (j = 0; j < sutun; ++j) {
			if (maze[i][j] == '-') {
				visited[i][j] = wall;
			} 
			else if (maze[i][j] == 'c') {
				visited[i][j] = cikis;
			} 
			else if (maze[i][j] == '|') {
				visited[i][j] = wall;
			} 
			else if (maze[i][j] == '+') {
				visited[i][j] = edge;
			}
			else if (maze[i][j] == 'O') {
				visited[i][j] = elma;
			}
			else {
				visited[i][j] = empty;
			}
		}
	}
}

void print_maze(){
	int i, j;
	for (i = 0; i < satir; ++i) {
		for (j = 0; j < sutun; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}	
	printf("\n");
}
	

int dfs(int row, int col){
	if (visited[row][col] == cikis) {
		return 1;
	}
	if (visited[row][col] == empty || visited[row][col] == elma) {
		if(visited[row][col] == elma){
			score+=10;
		}	
		visited[row][col] = trail;
		if(maze[row][col]!= 'b'){
			usleep(600000);
			maze[row][col]= '.';
			print_maze();
			printf("Score : %d \n",score);
		}
		if (dfs(row + 1, col)){
			visited[row][col] = trail;
			return 1;
		} 
		if (dfs(row, col + 1)){
			visited[row][col] = trail;
			return 1;		
		} 
		if (dfs(row, col - 1)){
			visited[row][col] = trail;
			return 1;
		} 
		if (dfs(row - 1, col)){
			visited[row][col] = trail;
			return 1;
		}
		int count=0;
		if(maze[row][col-1]== '|' || maze[row][col-1]== '-'){
			count++;
		}
		if(maze[row-1][col]== '|' || maze[row-1][col]== '-'){
			count++;
		}
		if(maze[row][col+1]== '|' || maze[row][col+1]== '-'){
			count++;
		}
	    if(maze[row+1][col]== '|' || maze[row+1][col]== '-'){
			count++;
		}
		if(count==3){
			printf("duvara carpildi \n");
			score-=5;
		}
	}
	
	return 0;
}

int main() {
	int start_satir,start_sutun;
	readMaze("maze.txt",&start_satir,&start_sutun);
	initVisited();
	print_maze();
	if(dfs(start_satir,start_sutun)==0) {
		printf("Cikisa ulasan yol yok\n");
	} 
	else{
		print_maze();
		printf("Score : %d \n",score);
	}
	return 0;
}
