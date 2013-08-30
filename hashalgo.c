#include<string.h>
//#include"hashalgo.h"

int hashalgo(char* str,int M)
{
	int i,j,k,length;
	long mult,sum=0;
	length=strlen(str)/4;
	for(i=0;i<strlen(str);i++)
	{
			
		sum+=str[i]^(23+i%4);		
	}	
	sum=53*sum^3+3*sum^2-5*sum+8;
	if ( sum <0 )
	  sum = -sum;	
	
	//printf("%d\n",sum);
	return sum % M;	
}
