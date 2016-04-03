int main(void) {
	int fd;
	int flag;
	int ma;
	int ndc;
	fd = readfile();
	flag = chkfile(fd);
	if (flag < 0)
		return 0;
	printfile(fd);
	masterarray(flag);
}
