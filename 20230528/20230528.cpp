#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#define MAX_HEAP_NODE 200

void error() {
	exit(1);
}

typedef int HNode;
#define Key(n) (n)

HNode heap[MAX_HEAP_NODE];
HNode abc[26] = {0};
int heap_size;
int cnt[26] = { 0 };

//부모 노드
#define parent(i) (heap[i/2])
//왼쪽 자식 노드
#define left(i) (heap[i*2])
//오른쪽 자식 노드
#define right(i) (heap[i*2 + 1])

void init_heap() { heap_size = 0; }
int is_empty_heap() { return heap_size == 0; }
int is_full_heap() { return heap_size == MAX_HEAP_NODE - 1; }
HNode find_heap() { return heap[1]; }

void insert_heap(HNode n)
{
	int i;
	int j = 0;

	if( is_full_heap() ) return;
	i = ++(heap_size);
	while( i != 1 && Key(n) < Key( parent(i) ) )
	{
		heap[i] = parent(i);
		i /= 2;
	}
	heap[i] = n;
	abc[j++] = i;
}

HNode delete_heap()
{
	HNode hroot, last;
	int parent = 1, child = 2;

	if (is_empty_heap())
		error();

	hroot = heap[1];
	last = heap[heap_size--];
	while (child <= heap_size)
	{
		//더 큰 자식을 찾고 child = 더 큰 자식의 인덱스 값을 가짐.
		if (child < heap_size && Key(left(parent)) > Key(right(parent)))
			child++;
		if (Key(last) <= Key(heap[child]))
			break;
		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}
	heap[parent] = last;
	return hroot;
}

void make_tree(int freq[], int n)
{
	HNode e1, e2;
	int i;

	init_heap();

	for( i = 0; i < n; i++ )
		insert_heap(freq[i]);

	for( i = 1; i < n; i++ )
	{
		e1 = delete_heap();
		e2 = delete_heap();

		insert_heap( Key(e1) + Key(e2) );
		printf(" (%d + %d)\n ", Key(e1), Key(e2));
	}
}

void make_input_txt()
{
	FILE* fp, *fp1;
	fp = fopen("input.txt", "r");
	fp1 = fopen("state.txt", "w");


	int c;
	int abc[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	int i;
	char str[10];

	while ((c = fgetc(fp)) != EOF)
	{
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		{
			if (c <= 'Z')
				cnt[c - 'A']++;
			else
				cnt[c - 'a']++;
		}
	}

	for (i = 0; i < 26; i++)
	{
		printf("%c : %d\n", 'A' + i, cnt[i]);
		fputc(abc[i], fp1);
		fputc('\t', fp1);
		sprintf(str, "%d", cnt[i]);
		fputs( str, fp1);
		fputc('\n', fp1);
	}

	fclose(fp);
	fclose(fp1);
}

void main()
{
	make_input_txt();
	make_tree(cnt, 26);

}