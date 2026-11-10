/* convert string to unsigned int */

int digit(char c, int base)
{
	int d;
	if (c >= '0' && c <= '9') {
		d = c - '0';
	} else if (c >= 'A' && c <= 'Z') {
		d = 10 + c - 'A';
	} else if (c >= 'a' && c <= 'z') {
		d = 10 + c - 'a';
	} else {
		return -1;
	}
	return d < base ? d : -1;
}

unsigned int strtoi(const char *s, int base)
{
	unsigned int num;
	int dig;

	for (num = 0; (dig = digit(*s, base)) >= 0; s++) {
		num *= base;
		num += dig;
	}
	return num;
}