#include <stdio.h>

int main(){

	unsigned long summ = 0;
	int byte = 0;
	//getchar() - returns код символа 
	//Ctrl+D - send EOF
	while ((byte = getchar())!=EOF){
		summ+=byte;

	}
	printf("%x\n", summ);
	
	return 0;
};
