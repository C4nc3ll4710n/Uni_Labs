#include <stdio.h>

int symbol = '\0';
int Pos    =    1;
int ReadSymbol(){

    int c;

    if ((c = getchar())!=EOF){

        symbol = c;
        Pos   += 1;
        
        return 1;


    }
        
    return 0;
}

int Identifier(){

    static int flag = 0;
    if (flag == 0 && NonDigit()){
        flag = 1;
        ReadSymbol();
        return Identifier();
    }
    else if (flag == 1 && (NonDigit() || Digit()))
        ReadSymbol();
        return Identifier();
    flag = 0;
    return 0;

}

int Digit(){

    if (symbol >= '0' && symbol<='9')
        return 1;
    return 0;
}

int NonDigit(){
    if ( symbol>='A' && symbol <= 'Z'){
        return 1;
    }
    else if( symbol >= 'a' && symbol<='z')
        return 1;
    else if (symbol == "_")
        return 1;
    return 0;
}

int main(){

    ReadSymbol();
    if(Identifier()){
        printf("[SUCCESS]\n");
    }
    else
    {   
        printf("[  OMG  ]\n");
    }
    /*unsigned long i = -1; 

    for (; i!=0; --i)
        printf("%lu\r", i);
    \r = carriage return*/
    printf("\n");
    
    return 0;
    /*gcc -std=c89 -Wall -Werror -pedantic main.c*/
    /*EOF: Windows Ctrl+Z, NYX Ctrl+D*/
}
/*gdb:
step - into func
next - skip func
*/