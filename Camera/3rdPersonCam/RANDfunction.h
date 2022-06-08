// very big random number generator


unsigned long long random_seed=22345235;
unsigned long long RAND(unsigned long long rng1){
	unsigned long long n1;
	unsigned long long A1,C1;
	A1=31456753457761;
	C1=15567785477468;
  random_seed=(A1*random_seed+C1)-(((A1*random_seed+C1)/18446744073709551615)*18446744073709551615);//18446744073709551615
  n1=(random_seed/18446744073709551615.0f) *rng1;
  return (n1);
}