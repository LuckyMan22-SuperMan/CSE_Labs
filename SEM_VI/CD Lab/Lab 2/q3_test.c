#include <stdio.h>
#define MAX 100

int main() {
    int a = 10;
    float b = 3.14;
    char c = 'x';

    if(a > 0){
        for(int i=0;i<10;i++){
            while(i < a){
                i++;
            }
        }
    }

    return 0;


/* Keywords inside comments should NOT be detected
   int float for while return
*/

// Another comment with keywords: if else switch

printf("Keywords inside string: int while for return");

struct Data {
    int id;
    char name[20];
};

switch(a){
    case 1:
        break;
    default:
        goto end;
}

end:
volatile unsigned long counter = 0;
}
