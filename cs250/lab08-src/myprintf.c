//#include <stdio.h>
int find_sp();
int putchar();
int myprintf(const char * format, ...){
	int* p;
	char c;
	char ** s;
	const char * fmt = format;
	char * sp = (char*) find_sp();
	char * sa;

	int ** x;
	int xa;
	int xar[64];
	
	int ** i;
	int ia;
	int ic = 0;
	int ib = 0;	

	int offset = 372; //seems to change based on the size of the code in myprintf
	int stroff = 0;
	while (*fmt){
		if (*fmt == '%'){
			//putchar('t');
			switch (*(fmt+1)){
				//printf("%d", sp);
				case '%':
					putchar('%');
					break;
				case 'c':
					
					c = *(sp+offset);
					putchar(c);
					offset+=4;
					/*//used for testing
					while(sp != fmt){
						c = *(sp+offset);
						putchar(c);
						offset+=4;
						printf(" %d\n", offset);
						if (offset > 2000) { break ;}
					}*/
					break;
				case 's':
					s = (char **)(sp+offset);
					sa = *s;
					while(*(sa+stroff) != '\0'){
						putchar(*(sa+stroff));
						stroff++;
					}
					/*
					while(offset < 2000){
						putchar((sp+offset));
						printf(" %d\n", offset);
						offset+=4;
					}
					//s = (sp+offset);
					//printf("%s", s);
					*/
					offset+=4;
					stroff = 0;
					break;
				case 'x':
					x = (int **)(sp+offset);
					xa = (int)*(x);
					int xb = xa;
					if (!(xa)){
						putchar(48);
						break;				
					}
					int xc = 0;
					while(xb){
						xb/=10;
						xc++;
					}
					
					int xsize = xc;
					while(xc){
						xc--;
						if ((xa%16) < 10){
							xar[xc] = ((xa%16)+48);
						}else{
							xar[xc] = ((xa%16)+55);	
						}
						xa/=16;
					}
					xb = 0;
					while(xb < xsize){
						putchar(xar[xb]);
						xb++;	
					}
					//printx(xa);
					offset+=4;
					/*
					while(sp != fmt){
						putchar(c);
						offset+=4;
				
						if (offset > 2000){break;}
					}*/
					break;
				case 'd':
					i = (int **)(sp+offset);
					ia = (int)*(i);
					ib = ia;
					if (!(ia)){ 
						putchar(48);
						break;
					}
					if(ia < 0){
						ia = (ia - 2*ia);
						putchar('-');
					}
					//printf("%d", ia);
					while(ib){
						//printf("%d", ia%10);
						ib/=10;
						ic++;
					}
					int iar[ic];
					int isize = ic;
					while(ic){
						ic--;
						iar[ic] = (ia%10);
						ia/=10;
						
					}
					ib = 0;
					while (ib < isize){
						putchar(iar[ib]+48);
						ib++;
					}
					//printd(ia);
					offset+=4;
					break;

			}
			fmt++;
		}else{
			putchar(*fmt);
		}
		fmt++;
	}

	return 0;
}
