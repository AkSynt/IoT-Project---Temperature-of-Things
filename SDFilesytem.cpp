#include "mbed.h"
#include "SDFileSystem.h"
 
SDFileSystem sd(PC_12, PC_11, PC_10, PC_9, "sd"); 
 
int main() {
    printf("Main begun..!\n");
    
     char temp_str[64];
        
        
        //Read the value
        
        AnalogIn adc(ADC_TEMP);
        
        
        
        
        float temp = ((adc.read()*3.3 - 0.76)/2.5) + 25; 
        
        sprintf(temp_str, "%5.3f", temp);  
 
    mkdir("/sd/mydir", 0777);
    
    FILE *fp = fopen("/sd/mydir/sdtest.txt", "w");
    if(fp == NULL) {
        error("Could not open file for write\n");
    }
    fprintf(fp, temp_str);
    fclose(fp); 
 
    printf("Sd Filesystem Success!\n");
}