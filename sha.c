#include<stdio.h>
#include<string.h>
//#include<malloc.h>
#include<math.h>
#include<stdlib.h>
#include"sha.h"
#define rotateleft(x,n) ((x<<n) | (x>>(32-n)))
#define rotateright(x,n) ((x>>n) | (x<<(32-n)))
int i,j;
void SHA1(unsigned char * str1, char** return_val)
{
    unsigned long int h0,h1,h2,h3,h4,a,b,c,d,e,f,k,temp;
    char s0[10];
    char s1[10];
    char s2[10];
    char s3[10];
    char s4[10];
    char *s[5];
    
    
    h0 = 0x67452301;
    h1 = 0xEFCDAB89;
    h2 = 0x98BADCFE;
    h3 = 0x10325476;
    h4 = 0xC3D2E1F0;
    
    unsigned char * str;
    str = (unsigned char *)malloc(strlen((const char *)str1)+100);
    strcpy((char *)str,(const char *)str1);
    int current_length = strlen((const char *)str);
    int original_length = current_length;
    str[current_length] = 0x80;
    str[current_length + 1] = '\0';
    
    //char ic = str[current_length];
    current_length++;
    int ib = current_length % 64;
    if(ib<56)
        ib = 56-ib;
    else
        ib = 120 - ib;
    for(i=0;i < ib;i++)
    {
        str[current_length]=0x00;
        current_length++;
    }
    str[current_length + 1]='\0';
    for(int i=0;i<6;i++)
    {
        str[current_length]=0x0;
        current_length++;
    }
    str[current_length] = (original_length * 8) / 0x100 ;
    current_length++;
    str[current_length] = (original_length * 8) % 0x100;
    current_length++;
    str[current_length+i]='\0';
    int number_of_chunks = current_length/64;
    unsigned long int word[80];
    for(i=0;i<number_of_chunks;i++)
    {
        for(int j=0;j<16;j++)
        {
            word[j] = str[i*64 + j*4 + 0] * 0x1000000 + str[i*64 + j*4 + 1] * 0x10000 + str[i*64 + j*4 + 2] * 0x100 + str[i*64 + j*4 + 3];
        }
        for(j=16;j<80;j++)
        {
            word[j] = rotateleft((word[j-3] ^ word[j-8] ^ word[j-14] ^ word[j-16]),1);
        }
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        for(int m=0;m<80;m++)
        {
            if(m<=19)
            {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if(m<=39)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if(m<=59)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            temp = (rotateleft(a,5) + f + e + k + word[m]) & 0xFFFFFFFF;
            e = d;
            d = c;
            c = rotateleft(b,30);
            b = a;
            a = temp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
        
        sprintf(s0,"%x",h0);
        sprintf(s1,"%x",h1);
        sprintf(s2,"%x",h2);
        sprintf(s3,"%x",h3);
        sprintf(s4,"%x",h4);
    }
    //printf("\n\n");
    /*  printf("%s",s0);
     printf("\n" "%s",s1);
     printf("\n" "%s",s2);
     printf("\n" "%s",s3);
     printf("\n" "%s",s4);*/
    
    s[0]=malloc(100*sizeof(char));
    s[1]=malloc(100*sizeof(char));
    s[2]=malloc(100*sizeof(char));
    s[3]=malloc(100*sizeof(char));
    s[4]=malloc(100*sizeof(char));
    
    strcpy(s[0], s0);
    strcpy(s[1], s1);
    strcpy(s[2], s2);
    strcpy(s[3], s3);
    strcpy(s[4], s4);
    
    
    strcat(s[0],s[1]);
    strcat(s[0],s[2]);
    strcat(s[0],s[3]);
    strcat(s[0],s[4]);
    printf("%s\n",s[0]);
    
    /*for (int i=0;i<5;i++)
     {
     printf("\n");
     printf("%s",s[i]);
     }*/
    //printf("\n Hash: %x %x %x %x %x",h0, h1, h2, h3, h4);
    //printf("\n\n");
    
    strcpy(return_val, s[0]);
}
