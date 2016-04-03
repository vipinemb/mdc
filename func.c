#include"header.h"
int mfd;
int readfile(void) {
	printf("(In %s)\n", __func__);
	int fd;
	fd=open("inputfile", O_RDONLY);
	return fd;
}							

int chkfile(int fd) {
	printf("(In %s)\n", __func__);
	if(fd < 0) {
		printf("Sorry! file opening error\n");
	} else 
		printf("file read successfully as fd is %d\n", fd);
		return fd;	
}

void printfile(int fd) {
	printf("(In %s)\n", __func__);
	int s;
	s=lseek(fd, 0, SEEK_END);
	lseek(fd,0, SEEK_SET);
	printf("the size of the file is:%d\n", s);
	char buff[s];
	read(fd, buff, s);
	printf("the content is in the file:\n");
	write(1, buff, s);
	lseek(fd, 0, SEEK_SET);
}

void masterarray(int fd) {
	printf("(In %s)\n",__func__);
	int ret = 1;
	int ndc = 0;
	int cl;
	char ch,ky;
	char *ma;
	while(ret) {
		ret = read(fd, &ch, 1);
	/*	if(ch!=10)*/{
			if(!ndc) {
				ma = (char*)malloc(sizeof(char));
			} if(unichar(ma, ch, ndc)) {
				ma = realloc(ma, ndc+1);
				*(ma+ndc) = ch;
				ndc++;
			}	
	
		}
	}
	lseek(fd, 0, SEEK_SET);
	printma(ma, ndc);
	mafile(ma, ndc);
	cl = codelength(ndc);
	compression(cl, ma , fd, ndc);
}			

int unichar(char *ma, char ch, int ndc) {
	int i;
	int flag = 1;
	flag = 1;
	for(i = 0;i < ndc; i++) {
		if(ch == *(ma + i))
		flag = 0;
	}
	return flag;
}

void printma(char *ma, int ndc) {
	printf("(In %s)\n", __func__);
	int i;
	printf("unique characters in master array are:\n");
	for(i = 0; i<ndc; i++)
	printf("%c",*(ma + i));
	printf("\n");
}
							


void mafile(char *ma, int ndc) {
	printf("(In %s)\n", __func__);
	int wr;
	mfd = open("mafile", O_CREAT | O_RDWR, 0644);
	wr = write(mfd, ma, ndc);
	if(wr != 0) {
		printf("unique characters are written in mafile succesfully with a file descriptor: %d\n", mfd);
		lseek(mfd, 0, SEEK_SET);
		int ss;
		ss = lseek(mfd, 0, SEEK_END);
		lseek(mfd, 0, SEEK_SET);
		printf("the size of master array file is : %d\n", ss);
	}
}

int codelength(int ndc) {
	printf("(In %s)\n", __func__);
	if(ndc <= 2) {
		printf("the code lenght is : %d\n", 1);
		return 1;
	} if(ndc <= 4) {
		printf("the code lenght is : %d\n", 2);
		return 2;
	} else if(ndc <= 8) {
		printf("the code lenght is:%d\n", 3);
		return 3;
	} else if(ndc <= 16) {
		printf("the code lenght is:%d\n", 4);
		return 4;
	} else if(ndc <= 32) {
		printf("the code lenght is:%d\n", 5);
		return 5;
	} else if(ndc <= 64) {
		printf("the code lenght is:%d\n", 6);
		return 6;
	} else if(ndc <= 128) {
		printf("the code lenght is:%d\n", 7);
		return 7;
	} else if(ndc <= 256) {
		printf("the code lenght is:%d\n", 8);
		return 8;
	}	
}

void compression(int cl, char *ma, int fd, int ndc) {
	static unsigned char k, t;
	int wfd;
	printf("(In %s)\n", __func__);
	unsigned char in[8];
	char ch;
	int i;
	int ret = 1;
	unsigned char ind, ii;
	lseek(fd, 0, SEEK_SET);
	wfd = open("compfile", O_CREAT | O_RDWR , 0644);
	while(ret != 0) {
		ret = read(fd, &ch, 1);
		if(ret != 0/* && ch != 10*/) {
			ind = indx(ma, ch, ndc);
			for(i = 0;i < cl; i++) {
				ii = ind >> i;
				ii = ii & 0x01;
				k=un(ii, k, in, wfd);
			}
		} else if(k < 8) {
			t = k;
			for(i = 0; i < (8 - t); i++) {
				ii = ii ^ ii;
				k = un(ii, k, in, wfd);
			}
		}
	}
	lseek(wfd, 0, SEEK_SET);
	int ws;
	lseek(wfd, 0, SEEK_SET);
	ws = lseek(wfd, 0, SEEK_END);
	lseek(wfd, 0, SEEK_SET);
	int fs;
	lseek(fd, 0, SEEK_SET);
	fs = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	unsigned char tt[ws];
	read(wfd, tt, ws);
	tt[0] = tt[0] & 0xf8;
	tt[0] = tt[0] | t;
	close(wfd);
	open("compfile", O_RDWR);
	write(wfd, tt, ws);
	printf("\nthe size of the compression file is:%d\n", ws);
	printf("Compression Efficiency is:\n");
	float eff;
	int ms;
	float wws, ffs, mss;
	wws = ws;
	ffs = fs;
	ms = lseek(mfd, 0, SEEK_END);
	mss = ms;
	lseek(mfd, 0, SEEK_SET);
	eff = ((ffs - wws - mss) / ffs) * 100;
	printf("eff is equal to ((%d - %d - %d) / %d) * %d\n is equal to %f%\n", fs, ws, ms, fs, 100, eff);
	printf("the content is in compression file:\n");
	lseek(wfd, 0, SEEK_SET);
	unsigned char chhh;
	int l;
	for(l = 0; l < ws; l++) {
		read(wfd, &chhh, 1);
		printf("%c", chhh);
	}
}

int un(unsigned char ii, unsigned char k, unsigned char* in, int wfd) {
	unsigned char or = 0x00;
	int j;
	if (k > 7)
		k = 0;
	if (k < 8) {  
		ii = ii << k;
		in[k] = ii;
		if (k == 7) {
			for(j = 0;j < 8; j++) {
				or = or | in[j];
			}
			write(wfd, &or, 1);
		}
		k++;
	}
	return k;	
}
		
unsigned int indx(char *ma, char ch, int ndc) {
	int i;
	for(i = 0; i < ndc; i++) {
		if (*(ma + i) == ch) {
			return i;
			break;
		}
	}
}
