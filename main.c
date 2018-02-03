#include <ctype.h>
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
void add_0(char *ip);
int is_pre(const char *a, const char *b);

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("Use merge_host file1 file2 ... to merge\n");
		return 0;
	} else {
		for (int i = 2; i < argc; ++i)
			merge_file(argv[1], argv[i]);
	}
	remove_dup_file(argv[1]);
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
	if (isspace(*(ip + 9)))
		strcpy(new_ip + 7, ip + 10);
	else
		strcpy(new_ip + 7, ip + 9);
	strcpy(ip, new_ip);
}

void add_0(char *ip)
{
	char new_ip[MAX];
	strcpy(new_ip, "0.0.0.0 ");
	strcpy(new_ip + 8, ip);
	strcpy(ip, new_ip);
}

// check if b is pre a (localhost and local)
int is_pre(const char *a, const char *b)
{
	if (!a || !b || strlen(a) < strlen(b))
		return 0;
	int i;
	for (i = 0; b[i] != '\0'; ++i) {
		if (a[i] != b[i])
			break;
	}
	return b[i] == '\0';
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
		// skip blank and comment lines
		if (isspace(*p) || *p == '#')
			continue;
		// remove comment after link
		for (int i = 8; p[i] != '\0'; ++i) {
			if (p[i] == '#') {
				p[i] = '\0';
				len = i;
				break;
			}
		}
		while (isspace(p[len - 1]))
			p[--len] = '\0';

		if (is_pre(p, "127")) {
			replace_127(p);
			len = strlen(p);
		}
		if (!is_pre(p, "0.0.0.0 ")) {
			add_0(p);
		}
		*(p + 7) = ' '; // ensure no tab
		if (isspace(*(p + 8)) || strcmp(p + 8, "local") == 0 ||
		    strcmp(p + 8, "localhost") == 0 ||
		    strcmp(p + 8, "localhost.localdomain") == 0)
			continue;
		if (*p == '0') {
			str = (char **)realloc(str,
					       sizeof(char *) * (numlines + 1));
			str[numlines] =
			    (char *)malloc((len + 1) * sizeof(char));
			strcpy(str[numlines], p);
			++numlines;
		}
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
	fprintf(f1, "\n");

	FILE *f2 = fopen(name2, "r");
	if (!f1 || !f2) {
		printf("No file to merge\n");
		return;
	}

	char line[MAX];
	while (fgets(line, MAX, f2)) {
		char *p = line;
		int len = strlen(p);
		if (isspace(*p) || *p == '#')
			continue;
		while (isspace(p[len - 1]))
			p[--len] = '\0';

		fprintf(f1, "%s\n", p);
	}
	fclose(f1);
	fclose(f2);
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
		while (isspace(p[len - 1]))
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
	FILE *f_read = fopen(name, "r");
	FILE *f_write = fopen("temp", "w");
	if (!f_read) {
		printf("No file to insert\n");
		return;
	}

	char *begin[] = {"127.0.0.1 localhost", "::1 localhost"};
	int size = sizeof(begin) / sizeof(begin[0]);
	for (int i = 0; i < size; ++i)
		fprintf(f_write, "%s\n", begin[i]);
	fprintf(f_write, "\n");

	char line[MAX];
	while (fgets(line, MAX, f_read)) {
		char *p = line;
		int len = strlen(p);
		if (*p != '0')
			continue;
		while (isspace(p[len - 1]))
			p[--len] = '\0';

		fprintf(f_write, "%s\n", p);
	}
	fclose(f_read);
	fclose(f_write);

	remove(name);
	rename("temp", name);
}