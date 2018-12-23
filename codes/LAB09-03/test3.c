#include<stdio.h>

int add_five(int *data){
	*data = *data + 5;
}

int main(void){
	int i, data[10];

	for(i=0;i<10;i++){
		scanf("%d", &data[i]);
		add_five(&data[i]);
	}

	for(i=0;i<10;i++){
		printf("%-5d", data[i]);
	}

	printf("\n");

	return 0;
}

