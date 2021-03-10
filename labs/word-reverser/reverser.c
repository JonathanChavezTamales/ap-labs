#include <stdio.h>
#include <string.h> 

int  append(char*s, char c) {
     int len = strlen(s);
     s[len] = c;
     s[len+1] = '\0';
     return 0;
}

int main(){
	char str[10000] = "";
	char temp = 'a';

	while(temp != EOF){
		temp = getchar();
		if(temp != '\n'){
			append(str, temp);
		}
		else if(temp != EOF){
			for(int i=strlen(str)-1; i>=0; i--){
				printf("%c", str[i]);
			}
			printf("\n");
			strcpy(str, "");
		}

	}
    	return 0;
}
