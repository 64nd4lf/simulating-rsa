# simulating-rsa
This C++ program simulates RSA algorithm.
Input parameters:
1. Encryption key, e
2. Message, M
Output:
1. Displays the values of the primes p and q, phi, private key d, n ( which is p*q)
2. Displays encrypted text in both decimal and bearcatii (base27 encoding with a-z and space)
3. Displays decrypted message in both decimal and bearcatii (bearcatii is case insensitive)
Validations:
1. Repeatedly asks for e if e is not coprime with phi
2. Validates the input message and exits if it is invalid
3. Checks if the message is too large to be able to encrypt and exits if it is too long stating why it can't encrypt
