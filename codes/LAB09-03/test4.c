#include<stdio.h>

int add_five(int *data){
	int i;
	for(i=0;i<10;i++){
		scanf("%d", &data[i]);
		data[i] = data[i] + 5;
	}
}

int main(void){
	int i, data[10];

	add_five(data);

	for(i=0;i<10;i++){
		printf("%-5d", data[i]);
	}

	printf("\n");

	return 0;
}

