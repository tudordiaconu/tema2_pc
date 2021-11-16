//DIACONU Tudor-Gabriel
//grupa 312CA
#include <stdio.h>
#include <stdlib.h>

//functie de verificare daca se iese din matrice in algoritmul fill
int verificare(int n, int *m, int **x, char *point, int i, unsigned int j)
{
	if (i < 0)
		return 0;
	if (i > n - 1)
		return 0;
	if (j > m[i] * sizeof(int) - 1)
		return 0;
	point = (char *)x[i];
	//daca elementul este 0, verificarea continua pe urmatoarele pozitii
	if (point[j] != 0)
		return 0;
	return 1;
}

//functia algoritmului fill
int algoritmfill(int n, int *m, int **x, char *point, int i, unsigned int j)
{
	int size = 0;
	point = (char *)x[i];
	point[j] = 1;
	//verificam daca elementul din dreapta elementului curent este 0
	//si daca da, apelam recursiv functia si ne mutam in respectivul element
	if (verificare(n, m, x, point, i, j + 1) == 1)
		size = size + 1 + algoritmfill(n, m, x, point, i, j + 1);
	//verificam daca elementul de deasupra elementului curent este 0
	//si daca da, apelam recursiv functia si ne mutam in respectivul element
	if (verificare(n, m, x, point, i + 1, j) == 1)
		size = size + 1 + algoritmfill(n, m, x, point, i + 1, j);
	//verificam daca elementul din stanga elementului curent este 0
	//si daca da, apelam recursiv functia si ne mutam in respectivul element
	if (verificare(n, m, x, point, i, j - 1) == 1)
		size = size + 1 + algoritmfill(n, m, x, point, i, j - 1);
	//verificam daca elementul din dedesubtul elementului curent este 0
	//si daca da, apelam recursiv functia si ne mutam in respectivul element
	if (verificare(n, m, x, point, i - 1, j) == 1)
		size = size + 1 + algoritmfill(n, m, x, point, i - 1, j);
	return size;
}

//calculul supermasivitatii maxime cu ajutorul algoritmului fill
void maxim(int *a, int *b, int *big, int **x, int n, char *p, int *m, int size)
{
	for (int i = 0; i < n; i++) {
		p = (char *)x[i];
		for (unsigned int j = 0; j < m[i] * sizeof(int); j++) {
			if (p[j] == 0) {
				size = 1 + algoritmfill(n, m, x, p, i, j);
				if (size > *big) {
					*big = size;
					*a = i;
					*b = j;
				}
			}
		}
	}
	printf("%d %d %d\n", *a, *b, *big);
}

//calculul mediei aritmetice a "scutului"
double shield(int n, int *m, char *point, int **x)
{
	int s = 0, count = 0;
	double m_a;
	for (int i = 0; i < 4 * m[0]; i++)
		s += point[i];
	point = (char *)x[n - 1];
	for (int i = 0; i < 4 * m[n - 1]; i++)
		s += point[i];
	count = 4 * m[0] + 4 * m[n - 1];
	//adunarea octetilor de pe marginea fiecarei linii din mijloc
	for (int i = 1; i < n - 1; i++) {
		count += 2;
		point = (char *)x[i];
		s += point[0] + point[4 * m[i] - 1];
	}
	//calculul mediei cerute
	m_a = (double)s / count;
	return m_a;
}

//functie de swap de pointeri
void swap_ptrs(char *p, char *q)
{
	char tmp;
	tmp = *p;
	*p = *q;
	*q = tmp;
}

//functie pentru operatia swap
void swap(char dimensiune, char *point, int **x, int linie, int l)
{
	if (dimensiune == 'I') {
		point = (char *)x[linie];
		swap_ptrs(&point[4 * l], &point[4 * l + 3]);
		swap_ptrs(&point[4 * l + 1], &point[4 * l + 2]);
	}
	if (dimensiune == 'S') {
		point = (char *)x[linie];
		swap_ptrs(&point[2 * l], &point[2 * l + 1]);
	}
}

//functie pentru operatia delete
void delete(char dim, char *point, short int *shp, int **x, int linie, int l)
{
	//variabila l reprezinta indexul citit pe care il scadem pentru ca
	//in functia delete indexarea incepe de la 1
	l--;
	if (dim == 'C') {
		point = (char *)x[linie];
		point[l] = 0;
	}
	if (dim == 'S') {
		shp = (short int *)x[linie];
		shp[l] = 0;
	}
	if (dim == 'I')
		x[linie][l] = 0;
}

//functie pentru operatia modify, in cazul in care dimensiunea este char
void mod_dim_c(char *point, int **x, int linie, int l, int *m, int new_nr)
{
	point = (char *)x[linie];
	if (l < 4 * m[linie]) {
		point[l] = new_nr;
	} else {
		x[linie] = realloc(x[linie], (l / 4 + 1) * sizeof(int));
		point = (char *)x[linie];
		int cop = l;
		while (cop % 4 != 0)
			cop++;
		//in cazul in care indexul > dimensiunea liniei
		//facem octetii de la finalul liniei citite pana la index 0
		for (int j = 4 * m[linie]; j < l; j++)
			point[j] = 0;
		//octetul cu indexul cautat devine noul numar
		point[l] = new_nr;
		//octetii de dupa index, daca raman, devin 0
		for (int j = l + 1; j < cop; j++)
			point[j] = 0;
		//actualizam noua dimensiune a liniei
		m[linie] = l / 4 + 1;
	}
}

//functie pentru operatia modify, in cazul in care dimensiunea este int
void mod_dim_i(int **x, int linie, int l, int *m, int new_nr)
{
	if (l < m[linie]) {
		x[linie][l] = new_nr;
	} else {
		x[linie] = realloc(x[linie], (l + 1) * sizeof(int));
		//in cazul in care indexul > dimensiunea liniei
		//facem int-urile de la finalul liniei citite pana la index 0
		for (int j = m[linie]; j < l; j++)
			x[linie][j] = 0;
		//intul cu indexul cautat devine noul numar
		x[linie][l] = new_nr;
		//actualizam noua dimensiune a liniei
		m[linie] = l + 1;
	}
}

//functie pentru operatia modify, in cazul in care dimensiunea este short int
void mod_dim_s(short int *shp, int **x, int linie, int l, int new_nr, int *m)
{
	x[linie] = realloc(x[linie], (l / 2 + 1) * sizeof(int));
	shp = (short int *)x[linie];
	int copie = l;
	if (copie % 2 != 0)
		copie++;
	//in cazul in care indexul > dimensiunea liniei
	//facem short-urile de la finalul liniei citite pana la index 0
	for (int j = 2 * m[linie]; j < l; j++)
		shp[j] = 0;
	//short-ul cu indexul cautat devine noul numar
	shp[l] = new_nr;
	//short-urile de dupa index, daca raman, devin 0
	for (int j = l + 1; j < copie; j++)
		shp[j] = 0;
	//actualizam noua dimensiune a liniei
	m[linie] = l / 2 + 1;
}

int main(void)
{
	int n, *m, **x, line, l, new_nr, nr, a, b, size = 0, maxx = -1;
	char op, dimensiune, *point;
	short int *shop;
	double m_a;
	scanf("%d", &n);
	//alocarea vectorului ce reprezinta numarul de coloane de pe fiecare linie
	m = malloc(n * sizeof(int));
	if (!m) {
		printf("Eroare aparuta la alocarea dinamica.");
		return -1;
	}
	//alocarea  matricii
	x = malloc(n * sizeof(int *));
	if (!x) {
		printf("Eroare aparuta la alocarea dinamica.");
		return -1;
	}
	for (int i = 0; i < n; i++) {
		scanf("%d", &m[i]);
		x[i] = malloc(m[i] * sizeof(int));
		if (!x[i]) {
			printf("Eroare aparuta la alocarea dinamica.");
			return -1;
		}
		for (int j = 0; j < m[i]; j++)
			scanf("%x", &x[i][j]);
	}
	//cast la char pentru a putea prelucra octetii de pe prima
	//si ultima linie din matrice si adunarea lor la suma, respectiv
	//numararea lor
	point = (char *)*x;
	m_a = shield(n, m, point, x);
	scanf("%d", &nr);
	for (int i = 0; i < nr; i++) {
		scanf(" %c %c %d %d", &op, &dimensiune, &line, &l);
		if (op == 'M') {
			scanf("%x", &new_nr);
			//variabila l reprezinta indexul pe care il scadem pentru ca
			//in modify incepe de la 1
			l--;
			if (dimensiune == 'C')
				mod_dim_c(point, x, line, l, m, new_nr);
			if (dimensiune == 'S') {
				//cast la short
				shop = (short int *)x[line];
				if (l < 2 * m[line])
					shop[l] = new_nr;
					else
						mod_dim_s(shop, x, line, l, new_nr, m);
			}
			if (dimensiune == 'I')
				mod_dim_i(x, line, l, m, new_nr);
		}
		if (op == 'D')
			delete(dimensiune, point, shop, x, line, l);
		if (op == 'S')
			swap(dimensiune, point, x, line, l);
	}
	printf("task 1\n");
	printf("%.8lf\n", m_a);
	printf("task 2\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m[i]; j++)
			printf("%08X ", x[i][j]);
		printf("\n");
	}
	printf("task 3\n");
	maxim(&a, &b, &maxx, x, n, point, m, size);
	free(m);
	for (int i = 0; i < n; i++)
		free(x[i]);
	free(x);
}
