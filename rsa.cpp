/*CS7081 Advanced Algorithms
Assignment - 1
Due date - 09/11/2017
This program requires g++. Or it can be executed online at cpp.sh or at onlinegdb.com
*/

#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string.h>

using namespace std;
typedef unsigned long long int ull;

//Converts decimal to Bearcatii (base27)
void toBearcatii(ull num, string &text){
	ull unitsDigit, num_ = num;
	int i = 0, length;
	while(num_ > 0) {
		num_ /= 27;
		i++;
	}
	length = i;
	for(int i = length - 1; i >= 0; i--){
		unitsDigit = num % 27;
		if(unitsDigit == 0)
			text[i] = (char) 32;
		else{
			unitsDigit += 64;
			text[i] = (char) unitsDigit;
		}
		num /= 27;
	}

	for(int i = 0; i < length; i ++)
		cout<<text[i];
}

//Computes gcd using euler's extended gcd method
ull extGCD(ull a, ull b, ull &s, ull &t) {

	if(b == 0) {
		s = 1;
		t = 0;
		return a;
	}

	ull s_, t_, result;
	result = extGCD(b, a%b, s_, t_);
	t = s_ - (a/b) * t_;
	s = t_;

	return result;
}

//Computes (a * b) mod c
ull modularMul(ull a, ull b, ull c)
{
    ull res = 0;
    a = a % c;
    while (b > 0)
    {
        if (b % 2 == 1)
            res = (res + a) % c;
        a = (a * 2) % c;
        b /= 2;
    }
    return res % c;
}

//Does modular exponentiation
ull computeModExp(ull base, ull exponent, ull mod){

	if(mod == 0)
		return 0;

	base %= mod;
	ull res = 1;

	while (exponent > 0) {
		if (exponent % 2 == 1) 
			res = modularMul(res, base, mod);
    	base = modularMul(base, base, mod);
    	exponent /= 2;
	}
	return res;
}

//Checks for primality using Miller-Rabin's method
bool checkPrimality(ull n){
	ull n_ = n - 1, k, m, b;
	int i = 0;
	if (n % 2 == 0)
		return false;
	ull a;
	do{
		a = rand() % (n-1);
	}while(a < 2);

	// computing k, m such that n-1 = 2^k X m
	while(n_ % 2 == 0){
		i++;
		n_ /= 2;
	}
	k = i;
	m = n_;
	b = computeModExp(a, m, n);
	for(i = 0; i < k; i++){
		if(b != 1 && b != (n-1))
			return false;
		else if(b == n -1)
			return true;
		b = computeModExp(b, 2, n);
	}
	if(b != 1)
		return false;
	return true;
}

int main() {

	ull p, q, n, phi, e, d, decimal = 0, cipheredNum, decipheredNum, temp;
	string M, C, P;
	int messageLength, flag;
	
	//Generating primes p and q randomly
	do{
		p = rand() % 1000000; // restricting random number to 999999 to prevent memory overflow
	}while(!checkPrimality(p));

	do{
		q = rand() % 1000000;
	}while(!checkPrimality(q));

	//Computing phi and n
	n = p*q;
	phi = (p-1) * (q-1);

	//Getting the encryption key
	do{
		cout<<"Enter the encryption key, e: ";
		cin>>e;
		if(cin.fail() || e < 0){
			cout<<"Invalid key, try again with positive integers."<<endl;
			exit(1);
		}

	}while(extGCD(e, phi, d, temp) != 1 || e == 1);

	//Adjusting d if it is negative
	if(d < 0)
		d+=phi;

	cout<<"p: "<<p<<" || q: "<<q<<" || n: "<<n<<" || phi: "<<phi<<" || private key, d: "<<d<<endl;
	//Reading the message
	flag = 1;
	cout<<"Enter the message (only a-z/A-Z/<space>, lower case will be converted to upper): ";
	cin.get();
	getline(cin, M);

	messageLength = M.length();
	
	//Checking for validity
	for(int i = 0; i < messageLength; i++){
		if(((int)M[i] < 32) || ( ((int)M[i] > 32) && (((int)M[i] < 65)) ) || ( ((int)M[i] > 90) && (((int)M[i] < 97)) ) || (((int)M[i] > 122))) 
			flag = 0;
		else
			flag = 1;
	}

	//Converting text to upper
	for(int i = 0; i < messageLength; i++)
		M[i] = toupper(M[i]);
	
	if(flag == 0){
	    cout<<"Invalid input. Try a-z|A-Z|<space>\nExiting program...";
	    exit(1);
	}

	//Text to decimal
	for(int i = messageLength -1, j= 0; i>=0; i--, j++){
		if(M[i] == ' ')
			continue;
		decimal = decimal + ((int)(M[i]) - 64) * pow(27, j);
	}

	cout<<"Message in decimal: "<<decimal<<endl;
	if(decimal > n){
		cout<<"Message in decimal exceeds n, message too long to encrypt. Exiting program..."<<endl;
		exit(1);
	}
	else if(decimal == 0){
		cout<<"Message in decimal too large for unsigned long long int, enter a smaller text."<<endl;
		exit(1);
	}

	cipheredNum = computeModExp(decimal, e, n);
	cout<<"Encrypted number ("<<decimal<<"^"<<e<<" mod "<<n<<"): "<<cipheredNum<<endl;
	cout<<"Encrypted text, C: ";
	toBearcatii(cipheredNum, C);
	cout<<endl;
	decipheredNum = computeModExp(cipheredNum, d, n);
	if(decipheredNum != decimal){
		cout<<"Memory overflowed, encryption failed! Exiting program..."<<endl;
		exit(1);
	}
	cout<<"Decrypted number ("<<cipheredNum<<"^"<<d<<" mod "<<n<<"): "<<decipheredNum<<endl;
	cout<<"Decrypted text, P: ";
	toBearcatii(decipheredNum, P);
	cout<<endl;
	cout<<"Message, M: "<<M<<endl;

	return 0;
}

/*
Output:
Sample 1:

Enter the encryption key, e: 1998
Enter the encryption key, e: 19967
p: 665123 || q: 705403 || n: 469179759569 || phi: 469178389044 || private key, d: 76414489967
Enter the message (only a-z/A-Z/<space>, lower case will be converted to upper): TEST
Message in decimal: 397838
Encrypted number (397838^19967 mod 469179759569): 94163758458
Encrypted text, C: I AKSNYO
Decrypted number (94163758458^76414489967 mod 469179759569): 397838
Decrypted text, P: TEST
Message, M: TEST

---------------------------------------------------------------------------------------------

Sample 2:

Enter the encryption key, e: 2339
p: 665123 || q: 705403 || n: 469179759569 || phi: 469178389044 || private key, d: 48542612291
Enter the message (only a-z/A-Z/<space>, lower case will be converted to upper): ALGORITHM
Message in decimal: 410890709839
Encrypted number (410890709839^2339 mod 469179759569): 281893278692
Encrypted text, C: ZYPPYGPK
Decrypted number (281893278692^48542612291 mod 469179759569): 410890709839
Decrypted text, P: ALGORITHM
Message, M: ALGORITHM
*/