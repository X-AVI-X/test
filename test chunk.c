/* Name:
 * unikey: 
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int swizzle_store[1000][3];


// Function that takes a character pointer to a hexadecimal number and converts it to binary
int hex_to_decimal(char * hex_num) // 0xFF
{   int base = 1;
    int decimal = 0;
    int value;
    int i = 3;
    int len = strlen(hex_num);
    len--;

    while(hex_num[i] != 'x'){   
        if (hex_num[i] >= '0' && hex_num[i] <= '9'){
            value = (hex_num[i] - 48);
            i--;
        }
        else if (hex_num[i] >= 'a' && hex_num[i] <= 'f'){
            value = ((hex_num[i]) - 97 + 10);
           i--;
        }
        else if (hex_num[i] >= 'A' && hex_num[i] <= 'F'){
            value = ((hex_num[i]) - 65 + 10);
            i--;
        }
        decimal += value * base;
        base *= 16;
      }
    return decimal;
}

// Function that takes a character pointer to a hexadcimal number and converts it to binary
char * hex_to_binary(char * hex_num)  // validation check 0x45
{   
    int index = 2;
    static char num[8];

    for(int i = 0; i < 8; i ++){
        num[i] = 0;
    }
    
    while(hex_num[index] != '\0')
    {   
        switch(hex_num[index])
        {
            case '0':
                strcat(num, "0000");
                break;
            case '1':
                strcat(num,"0001");
                break;
            case '2':
                strcat(num, "0010");
                break;
            case '3':
                strcat(num, "0011");
                break;
            case '4':
                strcat(num, "0100");
                break;
            case '5':
                strcat(num, "0101");
                break;
            case '6':
                strcat(num, "0110");
                break;
            case '7':
                strcat(num, "0111");
                break;
            case '8':
                strcat(num, "1000");
                break;
            case '9':
                strcat(num, "1001");
                break;
            case 'A':
                strcat(num, "1010");
                break;
            case 'B':
                strcat(num, "1011");
                break;
            case 'C':
                strcat(num, "1100");
                break;
            case 'D':
                strcat(num, "1101");
                break;
            case 'E':
                strcat(num, "1110");
                break;
            case 'F':
                strcat(num, "1111");
                break;
            case 'a':
                strcat(num, "1010");
                break;
            case 'b':
                strcat(num, "1011");
                break;
            case 'c':
                strcat(num, "1100");
                break;
            case 'd':
                strcat(num, "1101");
                break;
            case 'e':
                strcat(num, "1110");
                break;
            case 'f':
                strcat(num,"1111");
                break;
        }
        index++;
    }
 
    return num;
}

// Function to check if the file exists
bool file_exists_check(char * filename)
{   FILE *file;

    file = fopen(filename, "r");
    if (file == NULL){
        printf("File %s does not exist\n", filename);
        return false;
    }
    else {
        return true;
    }
    
}

// Function to check that the hexadecimal number is valid
bool valid_hex(char * hex_num) //0xff
{   int length = strlen(hex_num);
    
    if (length == 4){
        if(hex_num[0] == '0'){
        if(hex_num[1] == 'x'){
                if(isxdigit(hex_num[2]) && isxdigit(hex_num[3])){
                    return true;
                }
            }    
        }
    }
    return false;
}


// Function to convert a binary number to decimal
int bin_to_decimal(char * bin_num)
{   
    int base = 1;
    int index = 7;
    int decimal = 0;
    while(index >=0){
       decimal += (bin_num[index] - 48) * base;
       base *=2;
       index--; 
    }
    return decimal;
}

void swizzle(unsigned char chunk_to_process[], int index)
{   
    index = index * 5;
    int swizzle_bit = chunk_to_process[index + 3];
    
    switch (swizzle_bit)
    {   
        case 1:
        {
        printf("        Swizzle: XYZ\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index], chunk_to_process[index+1], chunk_to_process[index+2]);
        swizzle_store[index/5][0] = chunk_to_process[index];
        swizzle_store[index/5][1] = chunk_to_process[index+1];
        swizzle_store[index/5][2] = chunk_to_process[index+2];
        break;
        }
        
        case 2:
        {
        printf("        Swizzle: XZY\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index], chunk_to_process[index+2], chunk_to_process[index+1]);
        swizzle_store[index/5][0] = chunk_to_process[index];
        swizzle_store[index/5][1] = chunk_to_process[index+2];
        swizzle_store[index/5][2] = chunk_to_process[index+1];
        break;
        }
        
        case 3:
        {
        printf("        Swizzle: YXZ\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+1], chunk_to_process[index], chunk_to_process[index+2]);
        swizzle_store[index/5][0] = chunk_to_process[index+1];
        swizzle_store[index/5][1] = chunk_to_process[index];
        swizzle_store[index/5][2] = chunk_to_process[index+2];
        break;
        }
        
        case 4:
        {
        printf("        Swizzle: YZX\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+1], chunk_to_process[index+2], chunk_to_process[index]);
        swizzle_store[index/5][0] = chunk_to_process[index+1];
        swizzle_store[index/5][1] = chunk_to_process[index+2];
        swizzle_store[index/5][2] = chunk_to_process[index];
        break;
        }
        
        case 5:
        {
        printf("        Swizzle: ZXY\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+2], chunk_to_process[index], chunk_to_process[index+1]);
        swizzle_store[index/5][0] = chunk_to_process[index+2];
        swizzle_store[index/5][1] = chunk_to_process[index];
        swizzle_store[index/5][2] = chunk_to_process[index+1];
        break;
        }
        
        case 6:
        {
        printf("        Swizzle: ZYX\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+2], chunk_to_process[index+1], chunk_to_process[index]);
        swizzle_store[index/5][0] = chunk_to_process[index+2];
        swizzle_store[index/5][1] = chunk_to_process[index+1];
        swizzle_store[index/5][2] = chunk_to_process[index];
        break;
        }
        default:
            printf("Swizzle value is not within the range.\n");

    }
    // return swizzle_store;   
}

void print_average(int packet_size)
{
    float avg_x = 0;
    float avg_y = 0;
    float avg_z = 0;
    float sum_x = 0;
    float sum_y = 0;
    float sum_z = 0;

    for(int i = 0 ; i < packet_size; i++){
        sum_x += swizzle_store[i][0]; 
    }
    avg_x = sum_x/packet_size;
    
    for(int i = 0 ; i < packet_size; i++){
        sum_y += swizzle_store[i][1]; 
    }
    avg_y = sum_y/packet_size;

    for(int i = 0 ; i < packet_size; i++){
        sum_z += swizzle_store[i][2]; 
    }
    avg_z = sum_z/packet_size;

    printf("    Chunk Average X: %.2f, Average Y: %.2f, Average Z: %.2f\n\n", avg_x, avg_y, avg_z);
}

// Function that reads the file, processes the chunks
int read_file(char *file_name, int first_delim, int second_delim, int third_delim, int cal_check)
{   int size = 0;
    int previous_trailer_index = 0;
    int current_trailer_index = 0;
    int offset = 0;
   

    if(file_exists_check(file_name))
    {
        FILE* ptr;
        unsigned char chars[640];
        int packet_count = 0;
        int chunk_count = 0;

        // Opening file in reading mode
        ptr = fopen(file_name, "rb");

        while((size = fread(chars, 1, 640, ptr)) > 0){
            
            for(int i = 0; i < size; i++){
                offset++;
                if ((i >= 3 && chars[i-3] == first_delim && chars[i-2] == second_delim && chars[i-1] == third_delim && chars[i] == cal_check) || ((chars[strlen((const char *)chars)-1] != cal_check) && i == strlen((const char *)chars)-1)){
                    current_trailer_index = i-3;
                    if ((chars[strlen((const char *)chars)-1] != cal_check) && i == strlen((const char *)chars)-1){
                        current_trailer_index = i;
                        previous_trailer_index++;
                    }
                    if (previous_trailer_index == 0){
                        printf("Chunk: %d at offset: 0\n", chunk_count);
                    }
                    else {
                        printf("Chunk: %d at offset: %d\n", chunk_count, previous_trailer_index + 1);
                    }
                    
                    if (i == 636){
                        fseek(ptr,offset+1,SEEK_SET);
                        // printf("After fseek");
                        i = 0;
                    }
                    if (current_trailer_index == 0){
                        continue;
                    }
                     else {
                        
                    }
                
                    unsigned char chunk[640];
                    int index = 0;
                    for(int j = previous_trailer_index; j < current_trailer_index; j++){
                        if(j % 5 == 0){
                            packet_count++; 
                        }           
                        chunk[index] = chars[j];
                        index++;
                        
                    }

                    memset(swizzle_store,0,3000);

                    for(int i = 0; i < packet_count; i++){
                        printf("    Packet: %d\n", i);
                            printf("        Data before swizzle -> B0: %d, B1: %d, B2: %d\n", chunk[5*i], chunk[5*i+1], chunk[5*i+2]);
                            swizzle(chunk, i);
                    }
                    print_average(packet_count);
                    // for(int j = 0; j < packet_count; j++){
                    //     printf("Packet %d\n Swizzle bits:\n", j);
                    //     for(int k = 0; k < 3; k++){
                    //         printf("%d\n", swizzle_store[j][k]);
                    //     }
                    // }
                    previous_trailer_index = i;
                    chunk_count++;             
                } 
                packet_count = 0;
            }
        }
        
     fclose(ptr);
    }
    return size-1;
}



void parity_byte(char bin_val1[8], char bin_val2[8], char bin_val3[8], char *result_array)
{    
    for(int i = 0;  i < sizeof(result_array); i++){
        result_array[i] = 0;
    }

    int count = 0;
    
    for(int i = 0; i < 8 ; i++){
        if(bin_val1[i] == '1' || bin_val2[i] == '1' || bin_val3[i] == '1'){
            if (bin_val1[i] == '1'){
                count++;
            }
            if(bin_val2[i] == '1'){
                count++;
            }
            if(bin_val3[i] == '1'){
                count++;
            }

        if (count % 2 == 0){
        strcat(result_array, "0");
        
        } else {
            strcat(result_array, "1");
    
        }

        }
        count = 0;
    }
}


int main(int argc, char * argv[])      // why char * argv?
{                                      // need to know why we need a pointer? 
    
    if (argc == 5){
        char * file_name;                 
        char * first_delimiter;
        char * second_delimiter;
        char * third_delimiter;
        char * value1 =0;
        char * value2= 0;
        char * value3= 0;
        char result[8];
   
   

        file_name = argv[1];
        first_delimiter = argv[2]; 
        second_delimiter = argv[3];
        third_delimiter = argv[4];
        int first_delim_decimal = hex_to_decimal(first_delimiter);
        int second_delim_decimal = hex_to_decimal(second_delimiter);
        int third_delim_decimal = hex_to_decimal(third_delimiter);

        printf("Delimiter byte 0 is: %d\n", first_delim_decimal);
        printf("Delimiter byte 1 is: %d\n", second_delim_decimal);
        printf("Delimiter byte 2 is: %d\n", third_delim_decimal);


        value1 =  hex_to_binary(first_delimiter);
        char bin1[8];
        strcpy (bin1,value1);

        value2 =  hex_to_binary(second_delimiter);
        char bin2[8];
        strcpy(bin2,value2);
    

        value3 =  hex_to_binary(third_delimiter);
        char bin3[8];
        strcpy(bin3,value3);

        parity_byte(bin1, bin2, bin3, result);

        int calculated_checksum = bin_to_decimal(result);
        printf("Checksum is: %d\n\n", calculated_checksum);

        read_file(file_name, first_delim_decimal, second_delim_decimal, third_delim_decimal, calculated_checksum);
        
    } else if (argc < 5){
        printf("Not enough command line arguments.\n");
    } else {
        printf("Too many command line arguments.\n");
    }
   
}










// Next step-
// 1. xor function
// 2. parity_byte = binary to binary_to_decimal = checksum    (binary to decimal function made)
// 3. reading the file and store the contents in a 3D array
// 4. chunk and trailer
// chunk -> packet (2D relation) -> byte
// trailer -> delimiters and checksum (1D)