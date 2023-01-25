// Copyright Tudor Cristian-Andrei 311CAa 2022-2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OVERFLOW 10007

// ZONA PENTRU FUNCTII UTILE / AJUTATOARE
// functie pentru interschimbare (utila la sortare)
void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// functie care copiaza elemntele dintr-o matrice in alta
void copy_elements(int **a, int **b, int m, int n)
{
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i][j] = b[i][j];
	}
}

// ZONA PENTRU CITIRE
// functie simpla de citire a unei matrici
void read_matrix(int **a, int m, int n)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			scanf("%d", &a[i][j]);
}

// functie simpla de citire a unui vector
void read_vector(int *v, int n)
{
	for (int i = 0; i < n; i++)
		scanf("%d", v + i);
}

// ZONA PENTRU AFISARE
// functie simpla de afisare a unei matrici
void print_matrix(int **a, int m, int n)
{
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}

// functie care afiseaza matricea cu index-ul dat
// se foloseste de functia print_matrix
void print_given(int ***array, int last, int *row, int *coll)
{
	int index;
	scanf("%d", &index);
	if (index >= last || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	int m = row[index];
	int n = coll[index];
	print_matrix(*(array + index), m, n);
}

// functie pentru afisarea dimensiunilor unei matrici
void show_dimm(int *row, int *coll, int last)
{
	int index;
	scanf("%d", &index);
	if (index >= last || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	printf("%d %d\n", row[index], coll[index]);
}

// ZONA PENTRU SORTARE

// disclaimer : am pus mai intai sortarea pentru ca functia add este apelata
// de multe functii in continuare
// functie care aduna toate elemntele dintr-o matrice
int add(int **a, int m, int n)
{
	int sum = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			sum = (sum + a[i][j]) % OVERFLOW;
			if (sum < 0)
				sum += OVERFLOW;
		}
	}

	return sum;
}

// sortez prin metoda paharelor toti vectorii deodata, in functie de vectorul
// sum
void do_sort_array(int *sum, int ***a, int *row, int *coll, int idx)
{
	for (int i = 0; i < idx - 1; i++) {
		for (int j = i + 1; j < idx; j++) {
			if (sum[i] > sum[j]) {
				int **auxm = *(a + i);
				*(a + i) = *(a + j);
				*(a + j) = auxm;
				swap(row + i, row + j);
				swap(coll + i, coll + j);
				swap(sum + i, sum + j);
			}
		}
	}
}

// ZONA PENTRU ALOCARI SI ELIBERARI
// ELIBERARAI
// eliberarea unei matrice
void free_matrix(int **a, int m)
{
	for (int i = 0; i < m; i++)
		free(a[i]);
	free(a);
}

//eliberarea tuturor resurselor, facuta dupa litera Q
void free_all(int ***a, int *row, int *coll, int *sum, int index)
{
	if (index == 0) {
		free(a);
	} else {
		for (int i = 0; i < index; i++)
			free_matrix(*(a + i), row[i]);
		free(a);
		free(row);
		free(coll);
		free(sum);
	}
}

//ALOCARI
// realocarea vectorului de matrici
int ***resize_array(int ***arr, int index)
{
	if (index > 0)
		arr = (int ***)realloc(arr, (index + 1) * sizeof(int **));
	if (!arr) {
		printf("Ups... Something went wrong\n");
		exit(1);
	}

	return arr;
}

// alocarea unei matrici
int **alloc_matrix(int M, int N)
{
	int **a = (int **)malloc(M * sizeof(int *));
	if (!a) {
		printf("Ups... Something went wrong\n");
		exit(1);
	}
	for (int i = 0; i < M; i++) {
		a[i] = (int *)malloc(N * sizeof(int));
		if (!a[i]) {
			printf("Ups... Something went wrong\n");
			for (int j = i; j >= 0; j--)
				free(a[j]);
			free(a);
			exit(1);
		}
	}
	return a;
}

// functie care realoca un vector de n elemtele la n + 1 elementee si insereaza
// pe pozitia n + 1 un element dat ca parametru
int *add_value(int *v, int index, int place)
{
	int *array;

	if (index == 0) {
		array = (int *)malloc(1 * sizeof(int));
		if (!array) {
			printf("Ups... Something went wrong\n");
			exit(1);
		}
	} else {
		array = (int *)realloc(v, (index + 1) * sizeof(int));
		if (!array) {
			printf("Ups... Something went wrong\n");
			exit(1);
		}
	}

	array[index] = place;
	return array;
}

// functia apelata in main la citirea unei noi matrici
void do_add_matrix(int ****a, int **row, int **coll, int **sum, int *idx)
{
	int m, n;
	scanf("%d%d", &m, &n);
	*a = resize_array(*a, *idx);
	*(*a + *idx) = alloc_matrix(m, n);
	read_matrix(*(*a + *idx), m, n);
	*row = add_value(*row, *idx, m);
	*coll = add_value(*coll, *idx, n);
	*sum = add_value(*sum, *idx, add(*(*a + *idx), m, n));
	*idx = *idx + 1;
}

// ZONA PENTRU TRANSPUSA
// functie care returneaza transpusa unei matrici
int **transpose(int **a, int m, int n)
{
	int **tr_matrix = alloc_matrix(n, m);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			tr_matrix[i][j] = a[j][i];
	}

	return tr_matrix;
}

// functie care pune transpusa in locul matricei originale in vectorul
// de matrici (apelata in main)
void do_transpose(int ***a, int *row, int *coll, int last)
{
	int index;
	scanf("%d", &index);
	if (index >= last || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	int **tr_mat = transpose(*(a + index), row[index], coll[index]);
	free_matrix(*(a + index), row[index]);
	*(a + index) = tr_mat;

	swap(row + index, coll + index);
}

// ZONA PENTRU INMULTIRE
// functie care verifica daca inmultirea poate fi efectuata
int mult_ok(int idx1, int idx2, int *row, int *coll, int last)
{
	if (idx1 >= last || idx2 >= last || idx1 < 0 || idx2 < 0) {
		printf("No matrix with the given index\n");
		return 0;
	}
	if (coll[idx1] != row[idx2]) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	}
	return 1;
}

// functia alg_multiply (alg = algebric) inmulteste matricile fara a le
// modifica si aloca o zona noua de memorie pentru rezultat
// salveaza rezulatul intr-o alta matrice returnata de functie
int **alg_multiply(int ***a, int idx1, int idx2, int *row, int *coll)
{
	// aloc memorie pentru matricea rezultata
	int **m_matrix = alloc_matrix(row[idx1], coll[idx2]);
	// initializez toate elementele cu 0
	// si fac inmultirea
	int **m1 = *(a + idx1);
	int **m2 = *(a + idx2);
	for (int i = 0; i < row[idx1]; i++) {
		for (int j = 0; j < coll[idx2]; j++) {
			m_matrix[i][j] = 0;
			for (int k = 0; k < coll[idx1]; k++) {
				m_matrix[i][j] += ((m1[i][k] * m2[k][j]) % OVERFLOW);
				m_matrix[i][j] = (m_matrix[i][j]) % OVERFLOW;
				if (m_matrix[i][j] < 0)
					m_matrix[i][j] += OVERFLOW;
			}
		}
	}

	return m_matrix;
}

// ZONA PENTRU REDIMENSIONARE
// functie care realizeaza redimensionarea
int **cut_matrix(int **a, int *l, int *c)
{
	scanf("%d", l);
	int L = *l;
	int *cut_l = (int *)malloc(L * sizeof(int));
	if (!cut_l) {
		free(cut_l);
		printf("Ups... Something went wrong\n");
		exit(1);
	}
	read_vector(cut_l, L);

	scanf("%d", c);
	int C = *c;
	int *cut_c = (int *)malloc(C * sizeof(int));
	if (!cut_c) {
		free(cut_c);
		printf("Ups... Something went wrong\n");
		exit(1);
	}
	read_vector(cut_c, C);

	int **new = alloc_matrix(L, C);
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < C; j++)
			new[i][j] = a[cut_l[i]][cut_c[j]];
	}

	free(cut_l);
	free(cut_c);

	return new;
}

// functie care calculeaza matricea redimensionata(cu cut_matrix)
// si inlocuieste in toti vectorii ceea ce este necesar (recalculeaza suma,
// updateaza numarul de linii si coloane etc.)
void do_cut_matrix(int ***a, int *row, int *coll, int *sum, int n)
{
	int idx;
	scanf("%d", &idx);
	if (idx >= n || idx < 0) {
		printf("No matrix with the given index\n");
		int size, falsein;
		scanf("%d", &size);
		for (int i = 0; i < size; i++)
			scanf("%d", &falsein);
		scanf("%d", &size);
		for (int i = 0; i < size; i++)
			scanf("%d", &falsein);
		return;
	}
	int l, c;
	int **new = cut_matrix(*(a + idx), &l, &c);
	free_matrix(*(a + idx), row[idx]);
	row[idx] = l;
	coll[idx] = c;
	*(a + idx) = new;
	sum[idx] = add(*(a + idx), row[idx], coll[idx]);
}

// ZONA PT RIDICAREA LA PUTERE
// functie care verifica daca ridicarea la putere poate fi efectuata
int raise_ok(int idx, int pow, int *row, int *coll, int n)
{
	if (idx >= n || idx < 0) {
		printf("No matrix with the given index\n");
		return 0;
	}
	if (pow < 0) {
		printf("Power should be positive\n");
		return 0;
	}
	if (row[idx] != coll[idx]) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	}

	return 1;
}

// functie care genereaza matricea identitate
int **generate_identity(int n)
{
	int **iden = alloc_matrix(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j)
				iden[i][j] = 1;
			else
				iden[i][j] = 0;
		}
	}

	return iden;
}

// functia multiply inmulteste doua matrici fara a genera in memorie una noua,
// fiind mai apropiata de inmultirea din informatica decat cea algebrica
// ("imita" asocierea a = a * b, din informatica);
// functia inmulteste matricile a si b si salveaza rezultatul in a
// fara a crea o zona noua de memorie pentru rezultatul inmultirii
void multiply(int **a, int **b, int n)
{
	int **copy = alloc_matrix(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			copy[i][j] = 0;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++)
				copy[i][j] = (copy[i][j] + (a[i][k] * b[k][j])) % OVERFLOW;
			if (copy[i][j] < 0)
				copy[i][j] += OVERFLOW;
		}
	}
	copy_elements(a, copy, n, n);
	free_matrix(copy, n);
}

// functie care calculeaza a la puterea pow, unde a este de n * n
int **raise(int **a, int n, int pow)
{
	int **result = generate_identity(n);
	int **aux = alloc_matrix(n, n);
	copy_elements(aux, a, n, n);

	while (pow > 0) {
		if (pow % 2 != 0)
			multiply(result, aux, n);
		multiply(aux, aux, n);
		pow = pow / 2;
	}
	free_matrix(aux, n);
	return result;
}

// functie care realizeaza ridicarea la putere si updateaza toti vectorii
void do_raise(int ****a, int *row, int *coll, int **sum, int n)
{
	int idx, p;
	scanf("%d%d", &idx, &p);
	if (raise_ok(idx, p, row, coll, n) == 1) {
		int **aux = raise(*(*a + idx), row[idx], p);
		copy_elements(*(*a + idx), aux, row[idx], row[idx]);
		free_matrix(aux, row[idx]);
		int k = add(*(*a + idx), row[idx], row[idx]);
		*(*sum + idx) = k;
	}
}

// ZONA PENTRU STERGERE
// functie care verifica daca se poate face stergerea
int free_ok(int idx, int n)
{
	if (idx < 0 || idx >= n || n == 0) {
		printf("No matrix with the given index\n");
		return 0;
	}
	return 1;
}

// functie care sterge o matrice si returneaza un alt vector de matrici
// care nu contine matricea stearsa
int ***delete_matrix(int ***a, int *row, int *coll, int n, int idx)
{
	int ***aux = malloc((n - 1) * sizeof(int **));
	for (int i = 0; i < idx; i++) {
		*(aux + i) = alloc_matrix(row[i], coll[i]);
		copy_elements(*(aux + i), *(a + i), row[i], coll[i]);
	}

	for (int i = idx + 1; i < n; i++) {
		*(aux + i - 1) = alloc_matrix(row[i], coll[i]);
		copy_elements(*(aux + i - 1), *(a + i), row[i], coll[i]);
	}

	return aux;
}

// functie care sterge valoarea de la elementul idx, dat ca parametru,
// dintr-un vector si intoarce un vector care nu contine acel element
int *delete_element(int *v, int n, int idx)
{
	int *aux = malloc((n - 1) * sizeof(int));
	for (int i = 0; i < idx; i++)
		aux[i] = v[i];

	for (int i = idx + 1; i < n; i++)
		aux[i - 1] = v[i];

	return aux;
}

// ZONA PENTRU STRASSEN

// functie care aduna doua matrici de n * n (respectiv a si b) si salveaza
// rezultatul in matricea m, data ca parametru
void addition(int **m, int **a, int **b, int n)
{
	// salvez in m rezultatul adunarii lui a si b
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			m[i][j] = (a[i][j] + b[i][j]) % OVERFLOW;
			if (m[i][j] < 0)
				m[i][j] += OVERFLOW;
		}
	}
}

// functie care face scaderea intre 2 matrici de n * n (respectiv a si b) si
// salveaza rezultatul in matricea m, data ca paramtru
void subtraction(int **m, int **a, int **b, int n)
{
	// salvez in m rezultatul scaderii dintre a - b
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			m[i][j] = (a[i][j] - b[i][j]) % OVERFLOW;
			if (m[i][j] < 0)
				m[i][j] += OVERFLOW;
		}
	}
}

// functie care imparte o matrice de n * n in patru matrici de n / 2 * n / 2
// matricea mare este impartita in 4 zone, iar functia se apeleaza de 4 ori
// pentru ce zona avem nevoie
void split(int **a, int **b, int n, int mat_idx)
{
	// matricea a este rezultatul
	// matricea b este cea care trebuie impartita
	// n-ul este dimenisunea matricei ce trebuie impartita
	int r_start, r_stop, c_start, c_stop;
	switch (mat_idx) {
	// zona 1 reprezinta stanga sus
	case 1:
		r_start = 0;
		r_stop = n / 2;
		c_start = 0;
		c_stop = n / 2;
		break;
	// zona 2 reprezina dreapta sus
	case 2:
		r_start = 0;
		r_stop = n / 2;
		c_start = n / 2;
		c_stop = n;
		break;
	// zona 3 reprezinta stanga jos
	case 3:
		r_start = n / 2;
		r_stop = n;
		c_start = 0;
		c_stop = n / 2;
		break;
	// zona 4 reprezinta dreapta jos
	case 4:
		r_start = n / 2;
		r_stop = n;
		c_start = n / 2;
		c_stop = n;
	}

	int i1 = 0, j1 = 0;
	for (int i2 = r_start; i2 < r_stop; i2++) {
		j1 = 0;
		for (int j2 = c_start; j2 < c_stop; j2++) {
			a[i1][j1] = b[i2][j2];
			j1++;
		}
		i1++;
	}
}

// functie care imparte matricea a in cele 4 matrici a1, a2, a3, a4
// folosindu-se de functia split
void divide(int **a, int ***a1, int ***a2, int ***a3, int ***a4, int n)
{
	*a1 = alloc_matrix(n / 2, n / 2);
	*a2 = alloc_matrix(n / 2, n / 2);
	*a3 = alloc_matrix(n / 2, n / 2);
	*a4 = alloc_matrix(n / 2, n / 2);
	split(*a1, a, n, 1);
	split(*a2, a, n, 2);
	split(*a3, a, n, 3);
	split(*a4, a, n, 4);
}

// functia care face opusul lui split
// insereaza in matricea c, intr-o zona specificata, elementele din matricea a
void unsplit(int **c, int **a, int n, int mat_idx)
{
	// insereaza elementele din a in c
	int r_start, r_stop, c_start, c_stop;
	switch (mat_idx) {
	// stanga sus
	case 1:
		r_start = 0;
		r_stop = n / 2;
		c_start = 0;
		c_stop = n / 2;
		break;
	// dreapta sus
	case 2:
		r_start = 0;
		r_stop = n / 2;
		c_start = n / 2;
		c_stop = n;
		break;
	// stanga jos
	case 3:
		r_start = n / 2;
		r_stop = n;
		c_start = 0;
		c_stop = n / 2;
		break;
	// dreapta jos
	case 4:
		r_start = n / 2;
		r_stop = n;
		c_start = n / 2;
		c_stop = n;
	}
	int i1 = 0, j1 = 0;
	for (int i2 = r_start; i2 < r_stop; i2++) {
		j1 = 0;
		for (int j2 = c_start; j2 < c_stop; j2++) {
			c[i2][j2] = a[i1][j1];
			j1++;
		}
		i1++;
	}
}

// functie care face oarecum opusul lui divide
// functia construieste matricea c din cele patru matrici
// folosindu-se de functia unsplit
void paste(int **c, int **c1, int **c2, int **c3, int **c4, int n)
{
	unsplit(c, c1, n, 1);
	unsplit(c, c2, n, 2);
	unsplit(c, c3, n, 3);
	unsplit(c, c4, n, 4);
}

// functie care elibereaza 4 matrici deodata
// pentru a scapa de numarul mare de eliberari ce trebuie efectuate
// la implementare strassenului
void four_at_once(int **a1, int **a2, int **a3, int **a4, int n)
{
	free_matrix(a1, n / 2);
	free_matrix(a2, n / 2);
	free_matrix(a3, n / 2);
	free_matrix(a4, n / 2);
}

int **strassen(int **a, int **b, int n)
{
	// matricea rezulatat
	int **c = alloc_matrix(n, n);
	// cazul de baza
	if (n == 1) {
		c[0][0] = a[0][0] * b[0][0];
		return c;
	}
	// impart matricile a si b in matrici de dimenisiune n / 2
	int **a11, **a12, **a21, **a22, **b11, **b12, **b21, **b22;
	divide(a, &a11, &a12, &a21, &a22, n);
	divide(b, &b11, &b12, &b21, &b22, n);
	// iau 2 matrici auxiliare pentru a calcula fiecare m
	int **am1 = alloc_matrix(n / 2, n / 2);
	addition(am1, a11, a22, n / 2);
	int **bm1 = alloc_matrix(n / 2, n / 2);
	addition(bm1, b11, b22, n / 2);
	int **am2 = alloc_matrix(n / 2, n / 2);
	addition(am2, a21, a22, n / 2);
	int **bm2 = alloc_matrix(n / 2, n / 2);
	copy_elements(bm2, b11, n / 2, n / 2);
	int **am3 = alloc_matrix(n / 2, n / 2);
	copy_elements(am3, a11, n / 2, n / 2);
	int **bm3 = alloc_matrix(n / 2, n / 2);
	subtraction(bm3, b12, b22, n / 2);
	int **am4 = alloc_matrix(n / 2, n / 2);
	copy_elements(am4, a22, n / 2, n / 2);
	int **bm4 = alloc_matrix(n / 2, n / 2);
	subtraction(bm4, b21, b11, n / 2);
	int **am5 = alloc_matrix(n / 2, n / 2);
	addition(am5, a11, a12, n / 2);
	int **bm5 = alloc_matrix(n / 2, n / 2);
	copy_elements(bm5, b22, n / 2, n / 2);
	int **am6 = alloc_matrix(n / 2, n / 2);
	subtraction(am6, a21, a11, n / 2);
	int **bm6 = alloc_matrix(n / 2, n / 2);
	addition(bm6, b11, b12, n / 2);
	int **am7 = alloc_matrix(n / 2, n / 2);
	subtraction(am7, a12, a22, n / 2);
	int **bm7 = alloc_matrix(n / 2, n / 2);
	addition(bm7, b21, b22, n / 2);
	// nu mai am nevoie de diviziunile matricilor date ca parametrii
	// asa ca le eliberez
	four_at_once(a11, a12, a21, a22, n);
	four_at_once(b11, b12, b21, b22, n);
	// calculez m-urile
	int **m1 = strassen(am1, bm1, n / 2);
	int **m2 = strassen(am2, bm2, n / 2);
	int **m3 = strassen(am3, bm3, n / 2);
	int **m4 = strassen(am4, bm4, n / 2);
	int **m5 = strassen(am5, bm5, n / 2);
	int **m6 = strassen(am6, bm6, n / 2);
	int **m7 = strassen(am7, bm7, n / 2);
	four_at_once(am1, am2, am3, am4, n);
	four_at_once(am5, am6, am7, bm1, n);
	four_at_once(bm2, bm3, bm4, bm5, n);
	free_matrix(bm6, n / 2);
	free_matrix(bm7, n / 2);
	// calculez componentele care alcatuiesc c-urile
	int **c11 = alloc_matrix(n / 2, n / 2);
	addition(c11, m1, m4, n / 2);
	subtraction(c11, c11, m5, n / 2);
	addition(c11, c11, m7, n / 2);
	int **c12 = alloc_matrix(n / 2, n / 2);
	addition(c12, m3, m5, n / 2);
	int **c21 = alloc_matrix(n / 2, n / 2);
	addition(c21, m2, m4, n / 2);
	int **c22 = alloc_matrix(n / 2, n / 2);
	subtraction(c22, m1, m2, n / 2);
	addition(c22, c22, m3, n / 2);
	addition(c22, c22, m6, n / 2);
	four_at_once(m1, m2, m3, m4, n);
	free_matrix(m5, n / 2);
	free_matrix(m6, n / 2);
	free_matrix(m7, n / 2);
	// formez matricea rezultat
	paste(c, c11, c12, c21, c22, n);
	four_at_once(c11, c12, c21, c22, n);
	return c;
}

// disclaimer: deoarece functia se apeleaza atat pentru strassen cat si pentru
// inmultirea normala nu as fi putut sa o pun la zona pentru inmultire
// fara sa modific ordinea multor functii

// functie care inmulteste doua matrici printr-un mod ales (fie inmultirea
// normala, fie strassen) si le adauga in vectorul de matrici
void do_multiply(int ****a, int **row, int **coll, int **s, int *n, char mode)
{
	int idx1, idx2;
	scanf("%d%d", &idx1, &idx2);
	if (mult_ok(idx1, idx2, *row, *coll, *n) == 0)
		return;
	*a = resize_array(*a, *n);
	if (mode == 'M')
		*(*a + *n) = alg_multiply(*a, idx1, idx2, *row, *coll);
	if (mode == 'S')
		*(*a + *n) = strassen(*(*a + idx1), *(*a + idx2), *(*row + idx1));
	*row = add_value(*row, *n, *(*row + idx1));
	*coll = add_value(*coll, *n, *(*coll + idx2));
	*s = add_value(*s, *n, add(*(*a + *n), *(*row + idx1), *(*coll + idx2)));
	*n = *n + 1;
}

int main(void)
{
	char c;
	int m, index = 0;
	int ***mat_arr = (int ***)malloc(1 * sizeof(int **));
	int *row = NULL, *coll = NULL, *sum = NULL;
	do {
		scanf(" %c", &c);
		switch (c) {
		case 'L':
			do_add_matrix(&mat_arr, &row, &coll, &sum, &index);
			break;
		case 'D':
			show_dimm(row, coll, index);
			break;
		case 'P':
			print_given(mat_arr, index, row, coll);
			break;
		case 'T':
			do_transpose(mat_arr, row, coll, index);
			break;
		case 'M':
			do_multiply(&mat_arr, &row, &coll, &sum, &index, 'M');
			break;
		case 'C':
			do_cut_matrix(mat_arr, row, coll, sum, index);
			break;
		case 'R':
			do_raise(&mat_arr, row, coll, &sum, index);
			break;
		case 'F':
			scanf("%d", &m); // m = index;
			int ok = free_ok(m, index);
			if (ok == 1 && index > 1) {
				int ***auxmat_arr = delete_matrix(mat_arr, row, coll, index, m);
				for (int i = 0; i < index; i++)
					free_matrix(*(mat_arr + i), row[i]);
				free(mat_arr);
				mat_arr = auxmat_arr;
				int *auxrow = delete_element(row, index, m);
				int *auxcoll = delete_element(coll, index, m);
				int *auxsum = delete_element(sum, index, m);
				index--;
				free(row);
				free(coll);
				free(sum);
				row = auxrow;
				coll = auxcoll;
				sum = auxsum;
			} else if (ok == 1 && index == 1) {
				free_matrix(*mat_arr, row[0]);
				free(row);
				free(coll);
				free(sum);
				index--;
			}
			break;
		case 'O':
			do_sort_array(sum, mat_arr, row, coll, index);
			break;
		case 'Q':
			break;
		case 'S':
			do_multiply(&mat_arr, &row, &coll, &sum, &index, 'S');
			break;
		default:
			printf("Unrecognized command\n");
		}
	} while (c != 'Q');
	free_all(mat_arr, row, coll, sum, index);
	return 0;
}
