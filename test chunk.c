/* Name:
 * unikey: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int swizzle_store[1000][3];
int packet_count = 0;
int ignored_packet = 0;

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
        printf("Error: File %s does not exist!\n", filename);
        return false;
    }
    else {
        return true;
    }
}

// Function to check that the hexadecimal number is valid
bool valid_hex(char * hex_num, int position) //0xff
{   int length = strlen(hex_num);
    
    if (length == 4){
        if(hex_num[0] == '0'){
            if(hex_num[1] == 'x'){
                if(isxdigit(hex_num[2]) && isxdigit(hex_num[3])){
                    return true;
                } else {
                    printf("Error: Argument for delimiter byte %d is not a valid hex value\n", position);
                    return false;
                }
            }
            printf("Error: Argument for delimiter byte %d does not begin with 0x\n", position);
            return false;
                
        }
        printf("Error: Argument for delimiter byte %d does not begin with 0x\n", position);
        return false;

    } else {
        printf("Error: Argument for delimiter byte %d is not of the correct length\n", position);
        return false;
    }
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

char * decimal_to_bin(int decimal_num){
    static char num[8];
    int temp_var = 0;
    

    for(int i = 0; i < 8; i ++){
        num[i] = '0';
    }
    for(int i=0 ; decimal_num>0 ; i++)    
    {    
        temp_var = decimal_num % 2;
        if (temp_var == 0){
            num[i] = '0';
        } else {
            num[i] = '1';
        }
        decimal_num = decimal_num/2;    
    }

    int len = strlen(num);
    char temp;
    for (int i = 0; i < len/2; i++)  
    {   
        temp = num[i];  
        num[i] = num[len - i - 1];  
        num[len - i - 1] = temp;  
    }  
    return num;
}

bool swizzle_validity(int prev_x, int prev_y, int prev_z, int curr_x, int curr_y, int curr_z)
{
    int x_diff = 0;
    int y_diff = 0;
    int z_diff = 0;

    if (prev_x > 3)
    {
        x_diff = abs(prev_x - curr_x);
        if (x_diff > 25){
            printf("        Ignoring packet. X: %d. Previous valid packet's X: %d. %d > 25.\n", curr_x, prev_x, x_diff);
            ignored_packet++;
            return false;
        }
        y_diff = abs(prev_y - curr_y);
        if (y_diff > 25){
            printf("        Ignoring packet. Y: %d. Previous valid packet's Y: %d. %d > 25.\n", curr_y, prev_y, y_diff);
            ignored_packet++;
            return false;
        }
        z_diff = abs(prev_z - curr_z);
        if (z_diff > 25){
            printf("        Ignoring packet. Z: %d. Previous valid packet's Z: %d. %d > 25.\n", curr_z, prev_z, z_diff);
            ignored_packet++;
            return false;
        }
        return true;
    }
    return true;
}

void swizzle(unsigned char chunk_to_process[], int index)
{   
    index = index * 5;
    int swizzle_bit = chunk_to_process[index + 3];
    bool swizzle_flag;

    switch (swizzle_bit)
    {   
        case 1:
        {
        printf("        Swizzle: XYZ\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index], chunk_to_process[index+1], chunk_to_process[index+2]);

            if (index > 0)
            {
                swizzle_flag = swizzle_validity(swizzle_store[index/5 - 1][0], swizzle_store[index/5 - 1][1], swizzle_store[index/5 - 1][2], chunk_to_process[index], chunk_to_process[index+1], chunk_to_process[index+2]);
                if (swizzle_flag)
                {
                    swizzle_store[index/5][0] = chunk_to_process[index];
                    swizzle_store[index/5][1] = chunk_to_process[index+1];
                    swizzle_store[index/5][2] = chunk_to_process[index+2];
                }
                
            } else {
                swizzle_store[index/5][0] = chunk_to_process[index];
                swizzle_store[index/5][1] = chunk_to_process[index+1];
                swizzle_store[index/5][2] = chunk_to_process[index+2];
            }
            break;
        }
        
        
        case 2:
        {
        printf("        Swizzle: XZY\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index], chunk_to_process[index+2], chunk_to_process[index+1]);
        if (index > 0)
        {
            swizzle_flag = swizzle_validity(swizzle_store[index/5 - 1][0], swizzle_store[index/5 - 1][1], swizzle_store[index/5 - 1][2], chunk_to_process[index], chunk_to_process[index+2], chunk_to_process[index+1]);

            if (swizzle_flag)
            {
                swizzle_store[index/5][0] = chunk_to_process[index];
                swizzle_store[index/5][1] = chunk_to_process[index+2];
                swizzle_store[index/5][2] = chunk_to_process[index+1];
            }
        } else 
            {
                swizzle_store[index/5][0] = chunk_to_process[index];
                swizzle_store[index/5][1] = chunk_to_process[index+2];
                swizzle_store[index/5][2] = chunk_to_process[index+1];
            } 
            break;
        }
        
        case 3:
        {
        printf("        Swizzle: YXZ\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+1], chunk_to_process[index], chunk_to_process[index+2]);

        if (index > 0)
        {
            swizzle_flag = swizzle_validity(swizzle_store[index/5 - 1][0], swizzle_store[index/5 - 1][1],swizzle_store[index/5 - 1][2], chunk_to_process[index+1], chunk_to_process[index], chunk_to_process[index+2]);
            if (swizzle_flag){
                swizzle_store[index/5][0] = chunk_to_process[index+1];
                swizzle_store[index/5][1] = chunk_to_process[index];
                swizzle_store[index/5][2] = chunk_to_process[index+2];
            }
        } else 
        {
            swizzle_store[index/5][0] = chunk_to_process[index+1];
            swizzle_store[index/5][1] = chunk_to_process[index];
            swizzle_store[index/5][2] = chunk_to_process[index+2];
        }
        break;
        }
        
        case 4:
        {
        printf("        Swizzle: YZX\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+2], chunk_to_process[index], chunk_to_process[index+1]);

        if (index > 0)
        {
            swizzle_flag = swizzle_validity(swizzle_store[index/5 - 1][0], swizzle_store[index/5 - 1][1],swizzle_store[index/5 - 1][2], chunk_to_process[index+2], chunk_to_process[index], chunk_to_process[index+1]);
            if(swizzle_flag){
                swizzle_store[index/5][0] = chunk_to_process[index+2];
                swizzle_store[index/5][1] = chunk_to_process[index];
                swizzle_store[index/5][2] = chunk_to_process[index+1];
            }
        } else 
        {
            swizzle_store[index/5][0] = chunk_to_process[index+2];
            swizzle_store[index/5][1] = chunk_to_process[index];
            swizzle_store[index/5][2] = chunk_to_process[index+1];
        }
        
        break;
        }
        
        case 5:
        {
        printf("        Swizzle: ZXY\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+1], chunk_to_process[index+2], chunk_to_process[index]);
        if (index > 0)
        {
            swizzle_flag = swizzle_validity(swizzle_store[index/5 - 1][0], swizzle_store[index/5 - 1][1],swizzle_store[index/5 - 1][2], chunk_to_process[index+1], chunk_to_process[index+2], chunk_to_process[index]);
            if(swizzle_flag){
                swizzle_store[index/5][0] = chunk_to_process[index+1];
                swizzle_store[index/5][1] = chunk_to_process[index+2];
                swizzle_store[index/5][2] = chunk_to_process[index];
            }
        } else 
        {
            swizzle_store[index/5][0] = chunk_to_process[index+1];
            swizzle_store[index/5][1] = chunk_to_process[index+2];
            swizzle_store[index/5][2] = chunk_to_process[index];
        }
        
        break;
        }
        
        case 6:
        {
        printf("        Swizzle: ZYX\n");
        printf("        Data after swizzle -> X: %d, Y: %d, Z: %d\n", chunk_to_process[index+2], chunk_to_process[index+1], chunk_to_process[index]);
        if ( index > 0)
        {
            swizzle_flag = swizzle_validity(swizzle_store[index/5 - 1][0], swizzle_store[index/5 - 1][1],swizzle_store[index/5 - 1][2], chunk_to_process[index+2], chunk_to_process[index+1], chunk_to_process[index]);
            if (swizzle_flag){
                swizzle_store[index/5][0] = chunk_to_process[index+2];
                swizzle_store[index/5][1] = chunk_to_process[index+1];
                swizzle_store[index/5][2] = chunk_to_process[index];
            }
        } else 
        {
            swizzle_store[index/5][0] = chunk_to_process[index+2];
            swizzle_store[index/5][1] = chunk_to_process[index+1];
            swizzle_store[index/5][2] = chunk_to_process[index];
        }
        
        break;
        }
        default:
            printf("        Ignoring packet. Swizzle byte was: %d but can only be between 1 and 6.\n", swizzle_bit); 
            ignored_packet++;

    }
    // return swizzle_store;   
}

void print_average()
{
    float avg_x = 0;
    float avg_y = 0;
    float avg_z = 0;
    float sum_x = 0;
    float sum_y = 0;
    float sum_z = 0;

    
    for(int i = 0 ; i < packet_count; i++){
        sum_x += swizzle_store[i][0]; 
    }
    avg_x = sum_x/(packet_count - ignored_packet);
    
    for(int i = 0 ; i < packet_count; i++){
        sum_y += swizzle_store[i][1]; 
    }
    avg_y = sum_y/(packet_count - ignored_packet);

    for(int i = 0 ; i < packet_count; i++){
        sum_z += swizzle_store[i][2]; 
    }
    avg_z = sum_z/(packet_count - ignored_packet);

    if (packet_count - ignored_packet != 0){
        printf("    Chunk Average X: %.2f, Average Y: %.2f, Average Z: %.2f\n\n", avg_x, avg_y, avg_z);
    } else {
        printf("    No valid packets were found for this chunk.\n");
    }
    
    ignored_packet = 0;
}

void parity_byte_delim(char bin_val1[8], char bin_val2[8], char bin_val3[8],char *result_array)
{    
    for(int i = 0; i < 8; i ++)
    {
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

        } else{
            strcat(result_array, "0");
        }
        count = 0;
    }
    // printf("Parity: %s\n", result_array);
}

void parity_byte_packet(char bin_val1[8], char bin_val2[8], char bin_val3[8], char bin_val4[8], char *result_array)
{    
    for(int i = 0; i < 8; i ++)
    {
        result_array[i] = 0;
    }
    int count = 0;
    
    for(int i = 0; i < 8 ; i++){
        if(bin_val1[i] == '1' || bin_val2[i] == '1' || bin_val3[i] == '1' || bin_val4[i] == '1'){
            if (bin_val1[i] == '1'){
                count++;
            }
            if(bin_val2[i] == '1'){
                count++;
            }
            if(bin_val3[i] == '1'){
                count++;
            }
            if(bin_val4[i] == '1'){
                count++;
            }

            if (count % 2 == 0){
                strcat(result_array, "0");
            
            } else {
                strcat(result_array, "1");
            }

        } else{
            strcat(result_array, "0");
        }
        count = 0;
    }
    // printf("Parity: %s\n", result_array);
}


// Function that reads the file, processes the chunks
int read_file(char *file_name, int first_delim, int second_delim, int third_delim, int cal_check)
{   int size = 0;
    int previous_trailer_index = 0;
    int current_trailer_index = 0;
    int offset = 0;
   
        FILE* ptr;
        unsigned char chars[640];
        int chunk_count = 0;

        // Opening file in reading mode
        ptr = fopen(file_name, "rb");

        while((size = fread(chars, 1, 640, ptr)) > 0){
            
            for(int i = 0; i < size; i++){
                
                if ((i >= 3 && chars[i-3] == first_delim && chars[i-2] == second_delim && chars[i-1] == third_delim && chars[i] == cal_check) || ((chars[strlen((const char *)chars)-1] != cal_check) && i == strlen((const char *)chars)-1)){
                    current_trailer_index = i-3;
                    packet_count = 0;
                    if ((chars[strlen((const char *)chars)-1] != cal_check) && i == strlen((const char *)chars)-1){
                        current_trailer_index = i;
                        packet_count = 0;
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
                    int temp_pack = packet_count;

                    for(int i = 0; i < temp_pack; i++){
                        printf("    Packet: %d\n", i);

                        char * byte1_bin = 0;
                        char * byte2_bin = 0;
                        char * byte3_bin = 0;
                        char * byte4_bin = 0;
                        char result[8];

                        byte1_bin = decimal_to_bin(chunk[5*i]);
                        char bin1[8];
                        strcpy(bin1, byte1_bin);
                        // printf("Bin1: %s\n", bin1);

                        byte2_bin = decimal_to_bin(chunk[5*i+1]);
                        char bin2[8];
                        strcpy(bin2, byte2_bin);
                        // printf("Bin2: %s\n", bin2);

                        byte3_bin = decimal_to_bin(chunk[5*i+2]);
                        char bin3[8];
                        strcpy(bin3, byte3_bin);
                        // printf("Bin3: %s\n", bin3);

                        byte4_bin = decimal_to_bin(chunk[5*i+3]);
                        char bin4[8];
                        strcpy(bin4, byte4_bin);
                        // printf("Bin4: %s\n", bin4);

                        parity_byte_packet(bin1,bin2,bin3,bin4,result);
                        // printf("Parity: %s\n", result);

                        // char packet_checksum[8];

                        int pack_checksum_decimal = bin_to_decimal(result);
                        // printf("Checksum: %d\n", pack_checksum_decimal);
                        
                            if ((pack_checksum_decimal == chunk[5*i + 4]) && (chunk[5*i + 3] >= 1 && chunk[5*i+3] <= 6)){
                                printf("        Data before swizzle -> B0: %d, B1: %d, B2: %d\n", chunk[5*i], chunk[5*i+1], chunk[5*i+2]);
                                swizzle(chunk, i);
                            } else if (chunk[5*i + 3] < 1 || chunk[5*i+3] > 6)
                            {
                                printf("        Ignoring packet. Swizzle byte was: %d but can only be between 1 and 6.\n", chunk[5*i + 3]); 
                                
                                ignored_packet++;
                            } else {
                                printf("        Ignoring packet. Checksum was: %d instead of %d.\n", pack_checksum_decimal, chunk[5*i + 4]);
                              
                                ignored_packet++;
                            }
                            
                    }
                   
                    print_average();
                    // for(int j = 0; j < packet_count; j++){
                    //     printf("Packet %d\n Swizzle bits:\n", j);
                    //     for(int k = 0; k < 3; k++){
                    //         printf("%d\n", swizzle_store[j][k]);
                    //     }
                    // }
                    previous_trailer_index = i;
                    chunk_count++;             
                } 
                else {
                  
                    // printf("Chunk: 0 at offset: 0\n");
                    
                    if (offset > 639){
                        printf("Error: Chunk size exceeds the maximum allowable chunk size of 640 bytes.\n");
                        break;
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
                    int temp_pack_2 = packet_count;
                   
                    for(int i = 0; i < temp_pack_2; i++){
                        printf("    Packet: %d\n", i);

                        char * byte1_bin = 0;
                        char * byte2_bin = 0;
                        char * byte3_bin = 0;
                        char * byte4_bin = 0;
                        char result[8];

                        byte1_bin = decimal_to_bin(chunk[5*i]);
                        char bin1[8];
                        strcpy(bin1, byte1_bin);

                        byte2_bin = decimal_to_bin(chunk[5*i+1]);
                        char bin2[8];
                        strcpy(bin2, byte2_bin);

                        byte3_bin = decimal_to_bin(chunk[5*i+2]);
                        char bin3[8];
                        strcpy(bin3, byte3_bin);

                        byte4_bin = decimal_to_bin(chunk[5*i+3]);
                        char bin4[8];
                        strcpy(bin4, byte4_bin);
                        // printf("Bin4: %s\n", bin4);

                        parity_byte_packet(bin1,bin2,bin3,bin4,result);

                        // char packet_checksum[8];

                        int pack_checksum_decimal = bin_to_decimal(result);
                        printf("Checksum: %d\n", pack_checksum_decimal);
                            if(pack_checksum_decimal == chunk[5*i + 4] && (chunk[5*i + 3] >= 1 && chunk[5*i + 3] <= 6)){
                                printf("        Data before swizzle -> B0: %d, B1: %d, B2: %d\n", chunk[5*i], chunk[5*i+1], chunk[5*i+2]);
                                swizzle(chunk, i);
                            } else if (chunk[5*i + 3] < 1 || chunk[5*i+3] > 6)
                            {
                                printf("        Ignoring packet. Swizzle byte was: %d but can only be between 1 and 6.\n", chunk[5*i + 3]); 
                               
                                ignored_packet++;
                            } else {
                                printf("        Ignoring packet. Checksum was: %d instead of %d.\n", pack_checksum_decimal, chunk[5*i + 4]);
                             
                                ignored_packet++;
                            }
                    }
                    
                    print_average();
                    previous_trailer_index = i;
                    chunk_count++;     
                }
                packet_count = 0;
                offset++;
            }
        }
        
     fclose(ptr);
    
    return size-1;
}

// # CTC
// # TC
// # C
// # CT
// # TCT




int main(int argc, char * argv[])      // why char * argv?
{                                      // need to know why we need a pointer? 
    
    if (argc == 5){
        char * file_name;                 
        char * first_delimiter;
        char * second_delimiter;
        char * third_delimiter;
        char * value1 = 0;
        char * value2 = 0;
        char * value3 = 0;
        char result[8];
        
   

        file_name = argv[1];
        first_delimiter = argv[2]; 
        second_delimiter = argv[3];
        third_delimiter = argv[4];


        if (file_exists_check(file_name))
        {   
            if (valid_hex(first_delimiter,0) && valid_hex(second_delimiter, 1) && valid_hex(third_delimiter,2))
            {
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
               

                parity_byte_delim(bin1, bin2, bin3,result);


                int calculated_checksum = bin_to_decimal(result);
                printf("Checksum is: %d\n\n", calculated_checksum);

                read_file(file_name, first_delim_decimal, second_delim_decimal, third_delim_decimal, calculated_checksum);    
            }
        }
    } else if (argc < 5) {
        printf("Error: Not enough command line arguments.\n");
    } else {
        printf("Error: Too many command line arguments.\n");
    } 
}










// Next step
// -- swizzle packet difference 
// -- swizzle out of range
// -- checksum doesn't match (packet)
// -- chunk and trailer (empty chunk or multiple chunk or no trailers)
// -- chunk size overflow
// -- chunk not divisible by 5
