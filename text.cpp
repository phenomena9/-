int  main(int g)
{
	int   a = 11;
	int   d = 2;
	int   i;
	int   total=0;
	int   q;
	float r;
	int   s;
	int   lo;
	char  y = 'a';
	float k=1;
	cin >> i;
	r = 1 + 1.0;
	s = a*d;
	lo = a / d;
        s=a/d;
	if (a <= d)
	{
		q = a;
		a = d;
		d = q;
	}
	else {
		q = d;
		d = a;
		a = q;
	}
	while (a > d) {
		d = a + 1;
	}
	cout << total;
	while(i<100){
	    total=total+i;
		i=i+1;
	}
	cout << total;
	i=0;
	return 0;
}