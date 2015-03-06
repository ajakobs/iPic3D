#include <stdio.h>

int main()
{
	int p[]={1,2,4,8,15,16,30,32,60,64,120,128,240,480,960,1920};
	int n=sizeof(p)/sizeof(p[0]),i,j;
	char str[80];
	for(i=0;i<n;i++){
		sprintf(str, "offload_hosts_%d", p[i]);
		FILE *f = fopen(str, "w");
		for(j=0;j<p[i];j++) fprintf(f,"knc1-mic0\n");
		for(j=0;j<p[i];j++) fprintf(f,"knc1\n");
		fclose(f);
	}
	return 0;

}
