# Cryptoanalysis-of-CMEA-and-RSA
This project includes cryptoanalysis of CMEA and low exponenet RSA algorithm using known plaintext attack and Chinese Remainder Theorem respectively. 

<h1> CMEA (Cellular Message Encryption Algorithm) </h1>
CMEA is a block cipher which operates on blocks of the size of 2 to 6 bytes. CMEA was one of the four cryptographic primitives
used in the mobile communications network in the US. The key length of CMEA is 64 bit.

There is a known-plaintext attack which recovers the key (and thusthe whole plaintext) given only 40 to 100 known plaintext blocks.


<h1>Challenge Overview:</h1>
The challenge is to perform a known-plaintext-attack on CMEA cipher.</br>
In CMEA-II We have been given:</br>
Ciphertext File: cmea2Cipher.txt</br>
Plaintext File: cmea2InitPlain.txt</br>
Number of given plaintext blocks: 40</br>
In CMEA-I We have been given:</br>
Ciphertext File: cmea1Cipher.txt</br>
Plaintext File: cmea1InitPlain.txt</br>
Number of given plaintext blocks: 100</br>
The solution to the challenge is the complete plaintext corresponding to the ciphertext.</br>

<h1> CMEA Specification</h1>
<b>Block Length:</b> 3-byte</br>
<b>Key Length:</b> 64-bits (8-byte key)</br>
CMEA uses a function called as the T-box which in turn uses a lookup table called as the cave table. The function of T-box is given below in table 1.</br>
<b>Cave Table:</b> CMEA uses a 256-byte lookup table. But, there are only 164 distinct byte values that appear in the cave table. The cave table is fixed and it is a good idea to not the fact that only 164 byte-values appear in the cave table as it will be used later to our advantage during the known-plaintext attack.

![alt text](Snapshots/1.png)

<h1>Algorithm</h1>

![alt text](Snapshots/2.png)

<h1>Equations</h1>

![alt text](Snapshots/3.png)
![alt text](Snapshots/4.png)
![alt text](Snapshots/5.png)

<h1>Observations</h1>

![alt text](Snapshots/6.png)
![alt text](Snapshots/7.png)


<h2>CMEA-1 putative keys:</h2>

0x01, 0x02,0x03,0x04, 0x15, 0x16, 0x17, 0x18</br>
0x01, 0x02,0x03,0x04, 0x15, 0x16, 0x97, 0x98</br>
0x01, 0x02,0x03,0x04, 0x95, 0x96, 0x17, 0x18</br>
0x01, 0x02,0x03,0x04, 0x95, 0x96, 0x97, 0x98</br>

<h2>CMEA-1 Plaintext:</h2>
ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789AABBCCDDEEFFGGHHIIJJKK</br>
LLMMNNOOPPQQRRSSTTUUVVWWXXYYZZaabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz00</br>
112233445566ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba9876543210ZZYYXXWW</br>
VVUUTTSSRRQQPPOONNMMLLKKJJIIHHGGFFEEDDCCBBAAzzyyxxThis also seems a fitting occasion to</br>
notice the other hard works in that poem. Humpty-Dumpty's theory, of two meanings packed into one</br>
word like a portmanteau, seems to me the right explanation for all.</br>


<h2>CMEA 2 observations:</h2>
Number of known plaintext blocks = 40</br>
The number of putative T(0) generated = 75</br>
Maximum number of putative K6, K7 pairs = 52898</br>
Maximum number of 0’s in A matrix for putative T(0) values = 31436</br>
To solve CMEA 2 we would need to find putative key K0..7 for each of the 75 putative T(0) and for all K 6, K7 pairs and then check the output by decrypting the cipher text. This is a computationally intensive task to find the putative key K0..,7. As mentioned in [1] the expected number of such keys is about n4, where n is the number of (K6,K7) pairs.</br>


<h1> RSA </h1>
RSA is one of the first practical public-key cryptosystems and is widely used for secure data transmission. 
In such a cryptosystem, the encryption key is public and differs from the decryption key which is kept secret. 
In RSA, this asymmetry is based on the practical difficulty of factoring the product of two large prime numbers, the factoring problem.

<h1>Solution </h1>
![alt text](Snapshots/8.png)

<b>NOTE:</b> For more detailed explanation, please reach out to me at erajaypal91@gmail.com. 

