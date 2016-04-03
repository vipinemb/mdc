#include<stdio.h>
#include<fcntl.h>

int codelength(int);
void decompression(void);
char dindx(unsigned char, unsigned char*);
int sb(unsigned char, int, int, unsigned char*, int);

void decompression(void) {
	printf("\n");
	printf("(In %s)\n", __func__);
	int dfd, wfd, mfd, cl, ndc, ret = 1, i;
	mfd = open("mafile", O_RDONLY);
	ndc = lseek(mfd,0,SEEK_END);
	lseek(mfd, 0, SEEK_SET);
	unsigned char b[ndc];
	read(mfd, b, ndc);
	cl = codelength(ndc);
	dfd = open("decompfile", O_CREAT | O_RDWR, 0644);
	wfd = open("compfile", O_RDONLY);
	int cs;
	cs = lseek(wfd, 0, SEEK_END);
	lseek(wfd, 0, SEEK_SET);
	unsigned char byt, bit, t, cn;
	static int k, count;
	while(ret) {
		ret = read(wfd, &byt, 1);
			if (ret != 0) {
				if (count == 0) {
					t = byt & 0x07;
					byt = byt & 0xf8;
				}
				count++;
				if(count == cs) {
					if (t == 0)
					t = 8;
					cn = t;
				} else
				cn = 8;
		
				for(i = 0; i < cn; i++) {
					bit = byt >> i;
					bit = bit & 0x01;
					k = sb(bit, cl, dfd, b, k);
				}
			}
		}
	printf("\nDecompress data has written on decompfile sucessfully with a file descriptor:%d\n", dfd);
	lseek(dfd, 0, SEEK_SET);
	int ds;
	ds = lseek(dfd, 0, SEEK_END);
	printf("the size of decompression file is:%d\n", ds);
	lseek(dfd, 0, SEEK_SET);
	unsigned char chh;
	printf("the decompress data is:\n");
	int kk;
	for(kk = 0; kk < ds; kk++) {
		read(dfd, &chh, 1);
		printf("%c", chh);
	}
	printf("\n");
}

int sb(unsigned char bit, int cl, int dfd, unsigned char* b, int k) {
	unsigned char ba[cl], or;
	char ch;
	int j;
	if(k >= cl)
		k = 0;
	if(k < cl) {
		ba[k] = bit;
		if(k == (cl - 1)) {
			or = 0x00;
			for(j = 0;j < cl; j++) {
				ba[j] = ba[j] << j;
				or = or | ba[j];
			}
			ch = dindx(or, b);
			write(dfd, &ch, 1);
		}
		k++;
	}
	return k;
}

char dindx(unsigned char or, unsigned char* b) {
	unsigned char ch;
	ch = b[or];
	return ch;
}

int codelength(int ndc) {
	printf("(In %s)\n", __func__);
	if(ndc < 2) {
		printf("the code lenght is:%d\n", 1);
		return 1;
	} if(ndc <= 3) {
		printf("the code lenght is:%d\n", 2);
		return 2;
	} else if(ndc <= 7) {
		printf("the code lenght is:%d\n", 3);
		return 3;
	} else if(ndc <= 15) {
		printf("the code lenght is:%d\n", 4);
		return 4;
	} else if(ndc <= 31) {
		printf("the code lenght is:%d\n", 5);
		return 5;
	} else if(ndc <= 63) {
		printf("the code lenght is:%d\n", 6);
		return 6;
	} else if(ndc <= 127) {
		printf("the code lenght is:%d\n", 7);
		return 7;
	} else if(ndc <= 255) {
		printf("the code lenght is:%d\n", 8);
		return 8;
	}
}

int main(void) {
	decompression();
	return 0;
}
