#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	
	int i,j;

	for(i=0;i<3;i++){
		for(j=1;argv[j]!=NULL;j++){
			printf("%s ", argv[j]);
		}
		printf("\n");
	}
	exit(1);
}
