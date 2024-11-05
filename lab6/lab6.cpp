#include <iostream>
#include <locale.h>

int** createG(int size) {
	int** G;
	G = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		G[i] = (int*)malloc(size * sizeof(int));
	}

	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			if (i == j)
			{
				G[i][j] = 0;
			}
			else
			{
				G[i][j] = rand() % 2;
				G[j][i] = G[i][j];
			}
		}
	}
	return G;
}

void printG(int** G, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
}

//удаление вершин
int** delV(int** G, int size,int v) {
	int** Gnew = createG(size - 1);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i < v && j < v) {
				Gnew[i][j] = G[i][j];
			}
			if (i > v && j > v) {
				Gnew[i-1][j-1] = G[i][j];
			}
			if (i > v && j < v) {
				Gnew[i-1][j] = G[i][j];
			}
			if (i < v && j > v) {
				Gnew[i][j-1] = G[i][j];
			}
		}
	}
	for (int i = 0; i < size; i++) {
		free(G[i]);
	}
	free(G);
	G = NULL;
	return Gnew;
}

//отожествление вершин
int** unionV(int** G, int size, int v1, int v2) {
	for (int i = 0; i < size; i++) {
		if (G[v2][i] == 1) {
			G[v1][i] = G[v2][i];
			G[i][v1] = G[i][v2];
		}
	}
	G = delV(G, size, v2);
	return G;
}

//стягивание ребра
int** contrE(int** G, int size, int v1, int v2) {
	G[v1][v2] = 0;
	G[v2][v1] = 0;
	for (int i = 0; i < size; i++) {
		if (G[v2][i] == 1) {
			G[v1][i] = G[v2][i];
			G[i][v1] = G[i][v2];
		}
	}
	G = delV(G, size, v2);
	return G;
}

//расщипление
int** splitV(int** G, int size, int v) {
	int** Gnew = createG(size + 1);

	for (int i = 0; i < size; i++) {
		Gnew[i][size] = G[i][v];
		Gnew[size][i] = G[v][i];
	}
	printG(Gnew, size + 1);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Gnew[i][j] = G[i][j];
		}
	}
	Gnew[v][v] = 0;
	Gnew[size + 1][size + 1] = 0;
	for (int i = 0; i < size; i++) {
		free(G[i]);
	}
	free(G);
	G = NULL;
	return Gnew;
}

//объединение графов
int** unionG(int** G1, int** G2, int size1, int size2) {
	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;

	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;

	int** Gnew = createG(sizemax);
	for (int i = 0; i < sizemax; i++) {
		for (int j = 0; j < sizemax; j++) {
			Gnew[i][j] = Gmax[i][j];
		}
	}

	for (int i = 0; i < sizemin; i++) {
		for (int j = 0; j < sizemin; j++) {
			if (Gmin[i][j] == 1) {
				Gnew[i][j] = Gmin[i][j];
			}
		}
	}
	return Gnew;
}

//пересечение 
int** intersectG(int** G1, int** G2, int size1, int size2) {
	int sizemin = (size1 < size2) ? size1 : size2;
	int** Gnew = createG(sizemin);
	for (int i = 0; i < sizemin; i++) {
		for (int j = 0; j < sizemin; j++) {
			Gnew[i][j] = G1[i][j] & G2[i][j];
		}
	}
	return Gnew;
}

//кольцевая сумма
int** xorG(int** G1, int** G2, int size1, int size2, int *nG5) {
	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;
	int step = 0;

	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;

	int** Gnew = createG(sizemax);
	for (int i = 0; i < sizemin; i++) {
		for (int j = 0; j < sizemin; j++) {
			Gnew[i][j] = G1[i][j] ^ G2[i][j];
		}
	}

	for (int i = 0; i < sizemin; i++) {
		for (int j = sizemin; j < sizemax; j++) {
			Gnew[i][j] = Gmax[i][j];
		}
	}

	for (int i = sizemin; i < sizemax; i++) {
		for (int j = 0; j < sizemax; j++) {
			Gnew[i][j] = Gmax[i][j];
		}
	}

	for (int i = 0; i < sizemax; i++) {
		step = 0;
		for (int j = 0; j < sizemax; j++) {
			if (Gnew[i][j] == 1) {
				step++;
			}
		}
		if (step == 0) {
			Gnew = delV(Gnew, sizemax, i);
			sizemax--;
			i--;
		}
	}
	*nG5 = sizemax;
	return Gnew;
}

int main(void)
{
	srand(time(0));
	setlocale(LC_ALL, "");
	int nG1, nG2,nGG, v1, v2, zapros, numb;
	printf("Введите количество вершин первого графа:");
	scanf("%d", &nG1);
	printf("Введите количество вершин второго графа:");
	scanf("%d", &nG2);

	printf("Выберите действие над графами:");
	printf("\n1. Отожествление вершин\n2. Стягивание ребра\n3. Объединение\n4. Пересечение\n5. Кольцевая сумма");
	printf("\nВведите номер выбранного действия:");
	scanf("%d", &zapros);

	int** G1 = createG(nG1);
	printf("\nГраф 1:\n");
	printG(G1, nG1);

	int** G2 = createG(nG2);
	printf("\nГраф 2:\n");
	printG(G2, nG2);
	
	int** GG;
	int nG5;
	switch (zapros)
	{
	case 1:
		printf("С каким графом будем работать?(введите номер): ");
		scanf("%d", &numb);
		printf("Введите номера вершин для отожествления: ");
		scanf("%d %d", &v1, &v2);
		if (numb == 1)
		{
			GG = unionV(G1, nG1, v1, v2);
			printf("\nИзмененный граф\n");
			printG(GG, nG1 - 1);
		}
		else {
			GG = unionV(G2, nG2, v1, v2);
			printf("\nИзмененный граф\n");
			printG(GG, nG2 - 1);
		}
		break;
	case 2:
		printf("С каким графом будем работать?(введите номер): ");
		scanf("%d", &numb);
		printf("Введите номера вершин для отожествления: ");
		scanf("%d %d", &v1, &v2);
		if (numb == 1)
		{
			GG = contrE(G1, nG1, v1, v2);
			printf("\nИзмененный граф\n");
			printG(GG, nG1 - 1);
		}
		else {
			 GG = contrE(G2, nG2, v1, v2);
			printf("\nИзмененный граф\n");
			printG(GG, nG2 - 1);
		}
		break;
	case 3:
		GG = unionG(G1, G2, nG1, nG2);
		nGG = (nG1 > nG2) ? nG1 : nG2;
		printf("\nОбъединение графов\n");
		printG(GG, nGG);
		break;
	case 4:
		GG = intersectG(G1, G2, nG1, nG2);
		 nGG = (nG1 < nG2) ? nG1 : nG2;
		printf("\nПересечение графов\n");
		printG(GG, nGG);
		break;
	case 5:
		GG = xorG(G1, G2, nG1, nG2, &nG5);
		printf("\nКольцевая сумма графов\n");
		printG(GG, nG5);
		break;
	}
	
	return 0;
}