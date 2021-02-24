#include <stdio.h>
#include <stdlib.h>

/* print Fahrenheit-Celsius table */

double f_c(double f){
	return (5.0/9.0)*(f-32);
}

int main(int argc, char *argv[])
{
    int lower, upper, step = 1;

    if(argc == 2) lower = upper = atoi(argv[1]);
    else if(argc == 4){
	lower = atoi(argv[1]);
	upper = atoi(argv[2]);
	step = atoi(argv[3]);
    }

    for (int i = lower; i <= upper; i += step){
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", i, f_c(i));
    }

    return 0;
}
