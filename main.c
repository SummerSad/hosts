#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200

int str_compare(const void *a, const void *b);
void sort_file(const char *name);
void merge_file(const char *name1, const char *name2);
void remove_dup_file(const char *name);
void insert_need_file(const char *name);
void replace_127(char *ip);

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("Use merge_host file1 file2 ... to merge\n");
		return 0;
	} else if (argc == 2) {
		remove_dup_file(argv[1]);
	} else {
		for (int i = 2; i < argc; ++i)
			merge_file(argv[1], argv[i]);
	}
	insert_need_file(argv[1]);
	return 0;
}

// https://goo.gl/Nxuth4
int str_compare(const void *a, const void *b)
{
	const char *ca = *(const char **)a;
	const char *cb = *(const char **)b;
	return strcmp(ca, cb);
}

// 127.0.0.1 -> 0.0.0.0
void replace_127(char *ip)
{
	if (!ip || strlen(ip) < 11)
		return;
	char new_ip[MAX];
	new_ip[0] = '0';
	new_ip[1] = '.';
	new_ip[2] = '0';
	new_ip[3] = '.';
	new_ip[4] = '0';
	new_ip[5] = '.';
	new_ip[6] = '0';
	strcpy(new_ip + 7, ip + 9);
	strcpy(ip, new_ip);
}

// strip comment and sort data in file
void sort_file(const char *name)
{
	FILE *f_read = fopen(name, "r");
	if (!f_read) {
		printf("No file to sort\n");
		return;
	}

	int numlines = 0;
	char line[MAX];
	char **str = NULL;
	while (fgets(line, MAX, f_read)) {
		char *p = line;
		int len = strlen(p);
		// blank and comment lines
		if (*p == '\n' || *p == '\t' || *p == ' ' || *p == '#')
			continue;
		while (p[len - 1] == '\n' || p[len - 1] == ' ')
			p[--len] = '\0';
		if (*p == '1' && *(p + 1) == '2' && *(p + 2) == '7' &&
		    *(p + 3) == '.' && *(p + 4) == '0' && *(p + 5) == '.' &&
		    *(p + 6) == '0' && *(p + 7) == '.' && *(p + 8) == '1')
			replace_127(p);

		str = (char **)realloc(str, sizeof(char *) * (numlines + 1));
		str[numlines] = (char *)malloc((len + 1) * sizeof(char));
		strcpy(str[numlines], p);
		++numlines;
	}
	fclose(f_read);
	remove(name);

	qsort(str, numlines, sizeof(char *), str_compare);

	FILE *f_write = fopen(name, "w");
	for (int i = 0; i < numlines; ++i) {
		fprintf(f_write, "%s\n", str[i]);
		free(str[i]);
	}
	free(str);
	fclose(f_write);
}

// merge file2 to file1
void merge_file(const char *name1, const char *name2)
{
	FILE *f1 = fopen(name1, "a");
	FILE *f2 = fopen(name2, "r");
	if (!f1 || !f2) {
		printf("No file to merge\n");
		return;
	}

	char line[MAX];
	while (fgets(line, MAX, f2)) {
		char *p = line;
		int len = strlen(p);
		if (*p == '\n' || *p == '\t' || *p == ' ' || *p == '#')
			continue;
		while (p[len - 1] == '\n' || p[len - 1] == ' ')
			p[--len] = '\0';

		fprintf(f1, "%s\n", p);
	}
	fclose(f1);
	fclose(f2);

	remove_dup_file(name1);
}

// remove duplicate line in file, file must be sorted first
void remove_dup_file(const char *name)
{
	sort_file(name);

	FILE *f_read = fopen(name, "r");
	if (!f_read) {
		printf("No file to remove duplicate\n");
		return;
	}

	int numlines = 0;
	char line[MAX];
	char **str = NULL;
	char prev[MAX];
	prev[0] = '\0';
	while (fgets(line, MAX, f_read)) {
		char *p = line;
		int len = strlen(p);
		while (p[len - 1] == '\n' || p[len - 1] == ' ')
			p[--len] = '\0';
		if (strcmp(prev, p) == 0)
			continue;

		strcpy(prev, p);
		str = (char **)realloc(str, sizeof(char *) * (numlines + 1));
		str[numlines] = (char *)malloc((len + 1) * sizeof(char));
		strcpy(str[numlines], p);
		++numlines;
	}
	fclose(f_read);
	remove(name);

	FILE *f_write = fopen(name, "w");
	for (int i = 0; i < numlines; ++i) {
		fprintf(f_write, "%s\n", str[i]);
		free(str[i]);
	}
	free(str);
	fclose(f_write);
}

void insert_need_file(const char *name)
{
	char begin[] = "127.0.0.1 localhost";
	FILE *f_read = fopen(name, "r");
	FILE *f_write = fopen("temp", "w");
	if (!f_read) {
		printf("No file to insert\n");
		return;
	}
	fprintf(f_write, "%s\n\n", begin);
	char line[MAX];
	while (fgets(line, MAX, f_read)) {
		char *p = line;
		int len = strlen(p);
		if (*p != '0')
			continue;
		while (p[len - 1] == '\n' || p[len - 1] == ' ')
			p[--len] = '\0';

		fprintf(f_write, "%s\n", p);
	}
	fclose(f_read);
	fclose(f_write);

	remove(name);
	rename("temp", name);
}