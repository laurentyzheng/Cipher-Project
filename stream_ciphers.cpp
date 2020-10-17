//
//  main.cpp
//  ECE 150 project 2
//
//  Created by Laurent Zheng on 2019-11-05.
//  Copyright © 2019 Laurent Zheng. All rights reserved.
//

#include <iostream>
#include <cctype>
#include <cmath>
#include <cassert>

int main();

int p{0};

char *encode( char *plaintext, unsigned long key );
char *decode( char *cihpertext, unsigned long key );

void initiate_S (unsigned long key);
void initiate_R ();


bool is_valid_ciphertext( char *str );
bool is_valid_plaintext ( char *str );

void ascci_armour (unsigned int four_byte);
unsigned int reverse_armour (char array [5]);

unsigned char S [256];
unsigned char R{};

int i{0};
int j{0};
char temp_array [5];

char* seperate(unsigned int decoded_value, std::size_t capacity);

int main()
{
    char MyName[]{"This is a C++ Program Will Allow You To Encode & Decode Messages/Passwords with a given key."};
    std::cout << MyName << std::endl;
    char* encoded_text { encode(MyName, 231) };
    std::cout << encoded_text << std::endl;
    std::cout << decode(encoded_text, 231) << std::endl;
    return 0;
    
}

char small_array [5] {};

char* decode ( char *ciphertext, unsigned long key)
{
    assert (is_valid_ciphertext(ciphertext));
    i = 0;
    j = 0;
    initiate_S(key);
    std::size_t size {0};
    for (std::size_t k{0}; *(ciphertext+k) != '\0'; k++)
    {
        size++;
    }
    char* decoded_array {new char [size/5*4+1]};
    for (std::size_t k{0}; k < size; k+=5)
    {
        for (int i{0}; i<5; i++)
        {
            small_array [i] = *(ciphertext+k+i);
        }
        unsigned int decoded_value = reverse_armour(small_array);
        for (std::size_t j{0}; j<4; j++)
        {
            decoded_array[j+p] = (unsigned char)(((decoded_value) >> (8*(3-j)))&0xff);
        }
        p += 4;
    }
    for (std::size_t k{0}; *(decoded_array+k) != '\0'; k++)
    {
        initiate_R();
        decoded_array[k] = (unsigned char)((int)(decoded_array[k] ^ R));
    }
    return decoded_array;
}


char* encode ( char* plaintext, unsigned long key)
{
    assert ( is_valid_plaintext(plaintext) );
    initiate_S(key);
    int array_size{0};
    for (std::size_t k{0}; *(plaintext + k) != '\0'; k++)
    {
        array_size ++;
    }
    char* new_array {new char [(array_size/4+1)*5] {}};
    
    if (array_size % 4 != 0)
    {
        int leftover {0};
        leftover = array_size % 4;
        char* new_plaintext {new char [array_size+(4-leftover)+1]{}};
        for (std::size_t k{0}; *(plaintext + k) != '\0'; k++)
        {
            *(new_plaintext+k) = *(plaintext + k);
        }
        for (std::size_t k{0}; k<array_size+(4-leftover)+1; k++)
        {
            initiate_R();
            *(new_plaintext + k) ^= R;
        }
        for (std::size_t u{0}; *(new_plaintext+u + 4-leftover)!='\0'; u+=4)
        {
            int next {0};
            unsigned int four_byte{0};
            for (int i{3}; i>=0 && *(new_plaintext+next+u) != '\0'; i--)
            {
                four_byte += static_cast<unsigned char>(*(new_plaintext+next+u)) << (8*i);
                next ++;
            }
            ascci_armour(four_byte);
            for (int d{0}; d<5; d++)
            {
                *(new_array + p + d) = temp_array[d];
            }
            p += 5;
        }
        
        p = 0;
        new_array [(array_size/4+1)*5] = '\0';
        
        return new_array;
    }
    else
    {
        for (std::size_t k{0}; *(plaintext + k) != '\0'; k++)
        {
            initiate_R();
            *(plaintext + k) ^= R;
            array_size ++;
        }
        for (std::size_t u{0}; *(plaintext+u)!='\0'; u+=4)
        {
            int next {0};
            unsigned int four_byte{0};
            for (int i{3}; i>=0 && *(plaintext+next+u) != '\0'; i--)
            {
                
                four_byte += static_cast<unsigned char>(*(plaintext+next+u)) << (8*i);
                next ++;
            }
            ascci_armour(four_byte);
            for (int d{0}; d<5; d++)
            {
                *(new_array + p + d) = temp_array[d];
            }
            p += 5;
            if (*(plaintext+u+next+1) == '\0')
            {
                break;
            }
        }
    }
    p = 0;
    new_array [(array_size/4+1)*5] = '\0';
    
    return new_array;
}



void initiate_S (unsigned long key) //the long key initiates the S array with all possible characters in a specifies order
{
    // initiate the array from 0 to 255
    for (int w{0}; w < 256; w++)
    {
        S[w] = w;
    }
    //scramble
    for (int q{0}; q < 256; q++)
    {
        int k = i % 64;
        bool bit = key & (1ULL << k) ? 1 : 0;
        j = (j + S[i] + bit)%256;
        int swap{0};
        swap = S[i];
        S[i] = S[j];
        S[j] = swap;
        i = (i+1) % 256 ;
    }
}


void initiate_R () //R is a gerenerated number from the S array
{
    i = (i+1) % 256;
    j = (j + S[i]) % 256;
    int swap2 = S[i];
    S[i] = S[j];
    S[j] = swap2;
    int r = (S[i] + S[j]) % 256;
    R = S[r];
}


void ascci_armour (unsigned int four_byte_int) //armours a four_byte_integer into a character array of size 5
{
    int i{4};
    while ( four_byte_int != 0)
    {
        temp_array [i] = '!' + (four_byte_int) % 85;
        i--;
        four_byte_int /= 85;
    }
}

unsigned int reverse_armour (char* array) //reverses the actions in ascci_armour
{
    unsigned int e{0};
    int a{4};
    for (int i{0}; i<5; i++)
    {
        e +=  (*(array+a) - '!') * pow(85, i);
        a--;
    }
    return e;
}


bool is_valid_plaintext ( char* str ) //checks if the message is encodable
{
    bool test {1};
    for ( int i = 0; *(str+i) != '\0'; i++)
    {
        if (!(std::isprint ( *(str+i) ) || std::isspace( *(str+i) )))
        {
            test = 0;
        }
    }
    return test;
}

bool is_valid_ciphertext( char* str ) //checks if the ciphertext is decodable
{
    int test {0};
    for ( int i = 0; *(str+i) != '\0'; i++)
    {
        if ('!' > *(str+i) || 'u' < *(str+i) )
        {
            return false;
        }
        test++;
    }
    if (test % 5 == 0){
        return true;
    }else{
        return false;
    }
}
