#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Casilla{
	char tipo;
	int estado;
	int minas;
	char etiqueta;
};

typedef struct Casilla tipoCasilla;

tipoCasilla** crearTablero(int filas,int columnas);

void inicializarJuego(int filas,int columnas,tipoCasilla** nmatrix);

char asignarEtiqueta(tipoCasilla casilla);

void dibujarJuego(int filas,int columnas,tipoCasilla** matrix);

void asignarMinas(int filas,int columnas,int mines,tipoCasilla** matrix);

void asignarNumeros(int filas,int columnas,int mines,tipoCasilla** matrix);

int movimiento(int f,int c,tipoCasilla** matrix,char orden,int filas,int columnas);

int verificarGanador(int filas,int columnas,tipoCasilla** matrix,int state);

void destapaMinas(int filas,int columnas,tipoCasilla** matrix);

void abrir(int nf,int nc,int nfilas,int ncolumnas,tipoCasilla** nmatrix);

int calcMines(int mines,char orden,int f,int c,tipoCasilla** nmatrix);


int main(int argc,char* argv[]){
	int Filas,Columnas,Mines;
	if(argc==1){
		Filas=10;
		Columnas=10;
		Mines=10;
	}
	else if(argc==2){
		Filas=atoi(argv[1]);
		Columnas=atoi(argv[1]);
		Mines=2;
	}
	else if(argc==3){
		Filas=atoi(argv[1]);
		Columnas=atoi(argv[2]);
		Mines=2;
	}
	else if(argc==4){
		Filas=atoi(argv[1]);
		Columnas=atoi(argv[2]);
		Mines=atoi(argv[3]);
	}

	int State=1,F,C;
	char Orden;
	srand(time(NULL));

	tipoCasilla** Matrix;
	Matrix=crearTablero(Filas,Columnas);
	inicializarJuego(Filas,Columnas,Matrix);
	asignarMinas(Filas,Columnas,Mines,Matrix);
	asignarNumeros(Filas,Columnas,Mines,Matrix);
	printf("Has iniciado un nuevo juego. Disfrutalo!!\n\n");
	printf("Ingresa <fila> <columna> <orden>\n\n");
	while(State==1){
		dibujarJuego(Filas,Columnas,Matrix);
		printf("\nQuedan %d minas.\n",Mines);
		printf("'a' : abrir casilla | 'm' : marcar casilla | 'd' : desmarcar casilla |\n");
		printf("Ingresa tu movimiento (ej. '2 2 a'): ");
		scanf("%d %d %c",&F,&C,&Orden);
		Mines=calcMines(Mines,Orden,F,C,Matrix);
		State=movimiento(F,C,Matrix,Orden,Filas,Columnas);
		State=verificarGanador(Filas,Columnas,Matrix,State);
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	}
	if(State==0){
		destapaMinas(Filas,Columnas,Matrix);
		printf("Perdiste :(\n\n");
	}
	else{
		printf("Ganaste :D\n\n");
	}
	dibujarJuego(Filas,Columnas,Matrix);

	return 0;
}

tipoCasilla** crearTablero(int filas,int columnas){
	int i;
	tipoCasilla** matrix;
	matrix=(tipoCasilla**)malloc(filas*sizeof(tipoCasilla*));
	for(i=0;i<filas;i++){
		matrix[i]=(tipoCasilla*)malloc(columnas*sizeof(tipoCasilla));
	}
	return matrix;
}

void inicializarJuego(int filas,int columnas,tipoCasilla** matrix){
	int i,j;
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			matrix[i][j].tipo='n'; //sin 'c'
			matrix[i][j].estado=0; //prueba =1
			matrix[i][j].minas=0;
			matrix[i][j].etiqueta=asignarEtiqueta(matrix[i][j]);
		}
	}
}

char asignarEtiqueta(tipoCasilla casilla){
	if(casilla.estado==0){
		return '-';
	}
	else if (casilla.estado==1){
		if(casilla.tipo=='m'){
			return '*';
		}
		else{
			return (casilla.minas+'0');
		}
	}
	else if (casilla.estado==2){
		return 'x';
	}
}

void dibujarJuego(int filas,int columnas,tipoCasilla** matrix){
	int i,j;
	printf("     ");
	for(j=0;j<columnas;j++){
		if(j<10){
			printf("%d   ",j);
		}
		else{
			printf("%d  ",j);
		}
	}
	printf("\n\n");
	for(i=0;i<filas;i++){
		if(i<10){
			printf("%d  |",i);
		}
		else{
			printf("%d |",i);
		}
		for(j=0;j<columnas;j++){
			printf(" %c |",matrix[i][j].etiqueta);
		}
		printf("\n\n");
	}
}

void asignarMinas(int filas,int columnas,int mines,tipoCasilla** matrix){
	int i,j;
	while(mines>0){
		i=rand()%filas;
		j=rand()%columnas;
		if(matrix[i][j].tipo=='n'){
			matrix[i][j].tipo='m';
			matrix[i][j].etiqueta=asignarEtiqueta(matrix[i][j]);
			mines--;
		}
	}
}

void asignarNumeros(int filas,int columnas,int mines,tipoCasilla** matrix){
	int i,j;
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			if(matrix[i][j].tipo=='n'){

				if(i<(filas-1) && j>0){
					if(matrix[i+1][j-1].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				if(i>0 && j<(columnas-1)){
					if(matrix[i-1][j+1].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				if(i>0 && j>0){
					if(matrix[i-1][j-1].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				if(i>0){
					if(matrix[i-1][j].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				if(j>0){
					if(matrix[i][j-1].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				if(i<(filas-1) && j<(columnas-1)){
					if(matrix[i+1][j+1].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				if(i<(filas-1)){
					if(matrix[i+1][j].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				if(j<(columnas-1)){
					if(matrix[i][j+1].tipo=='m'){
						matrix[i][j].minas++;
					}
				}

				matrix[i][j].etiqueta=asignarEtiqueta(matrix[i][j]);
			}
		}
	}
}

int movimiento(int f,int c,tipoCasilla** matrix,char orden,int filas,int columnas){
	if(orden=='a'){
		if (matrix[f][c].tipo=='m'){
			return 0;
		}
		abrir(f,c,filas,columnas,matrix);
		return 1;
	}
	else if(orden=='m' && matrix[f][c].estado!=1){
		matrix[f][c].estado=2;
		matrix[f][c].etiqueta=asignarEtiqueta(matrix[f][c]);
		return 1;
	}
	else if(orden=='d' && matrix[f][c].estado!=1){
		matrix[f][c].estado=0;
		matrix[f][c].etiqueta=asignarEtiqueta(matrix[f][c]);
		return 1;
	}
}

int verificarGanador(int filas,int columnas,tipoCasilla** matrix,int state){
	int i,j;
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			if((matrix[i][j].tipo=='m' && matrix[i][j].estado<2) || (matrix[i][j].tipo=='n' && matrix[i][j].estado!=1)){
				return state;
			}
		}
	}
	return 2;
}

void destapaMinas(int filas,int columnas,tipoCasilla** matrix){
	int i,j;
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			if(matrix[i][j].tipo=='m'){
				matrix[i][j].estado=1;
				matrix[i][j].etiqueta=asignarEtiqueta(matrix[i][j]);
			}
		}
	}
}

void abrir(int nf, int nc,int nfilas,int ncolumnas,tipoCasilla** nmatrix){
	nmatrix[nf][nc].estado=1;
	nmatrix[nf][nc].etiqueta=asignarEtiqueta(nmatrix[nf][nc]);
	if(nmatrix[nf][nc].minas==0){
		if(nf<(nfilas-1) && nc>0){
			if(nmatrix[nf+1][nc-1].estado==0){
				abrir(nf+1,nc-1,nfilas,ncolumnas,nmatrix);
			}
		}

		if(nf>0 && nc<(ncolumnas-1)){
			if(nmatrix[nf-1][nc+1].estado==0){
				abrir(nf-1,nc+1,nfilas,ncolumnas,nmatrix);
			}
		}

		if(nf>0 && nc>0){
			if(nmatrix[nf-1][nc-1].estado==0){
				abrir(nf-1,nc-1,nfilas,ncolumnas,nmatrix);
			}
		}

		if(nf>0){
			if(nmatrix[nf-1][nc].estado==0){
				abrir(nf-1,nc,nfilas,ncolumnas,nmatrix);
			}
		}

		if(nc>0){
			if(nmatrix[nf][nc-1].estado==0){
				abrir(nf,nc-1,nfilas,ncolumnas,nmatrix);
			}
		}

		if(nf<(nfilas-1) && nc<(ncolumnas-1)){
			if(nmatrix[nf+1][nc+1].estado==0){
				abrir(nf+1,nc+1,nfilas,ncolumnas,nmatrix);
			}
		}

		if(nf<(nfilas-1)){
			if(nmatrix[nf+1][nc].estado==0){
				abrir(nf+1,nc,nfilas,ncolumnas,nmatrix);
			}
		}

		if(nc<(ncolumnas-1)){
			if(nmatrix[nf][nc+1].estado==0){
				abrir(nf,nc+1,nfilas,ncolumnas,nmatrix);
			}
		}
	}
}

int calcMines(int mines,char orden,int f,int c,tipoCasilla** matrix){
	if(orden=='m'){
		if(matrix[f][c].estado==0){
			mines--;
			return mines;
		}
	}
	else if(orden=='d'){
		if(matrix[f][c].estado==2){
			mines++;
			return mines;
		}
	}
	else if(orden=='a'){
		if(matrix[f][c].estado==2){
			mines++;
			return mines;
		}
	}
	return mines;
}
