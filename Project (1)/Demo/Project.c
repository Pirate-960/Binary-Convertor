//-----------------------------------------------------------------------//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//-----------------------------------------------------------------------//
typedef struct {
    char filePath[256];
    char byteOrder[20];
    char dataType[20];
    int typeSize;
} InputData;
//-----------------------------------------------------------------------//
// Funtions Prototypes

InputData getInput();
char * HexDecimal_To_Binary(char * HexDecimal_String);
unsigned int ConversionToUnsigned(char * input,int length);
signed int ConversionToSigned(char * Input,int length);
char *convertToFloat(char *inputString,int byteSize);
void roundF(char *inputString ,int startInt ,int finalInt, char setChar);
double powerOfTwo(int exponentVal);
double recursivePowerOfTwo(int exponentVal, double returnValue);
double recursivePowerOfMTwo(int exponentVal, double returnValue);
double checkForNaNFinity(char *inputString, int byteSize, int exponent, char *returnStr);
void toEndianConvert(char *binStr, int sizeBytes, const char *endianType);
//-----------------------------------------------------------------------//
//
int main() {
    const int MAX_READ = 1024;
    int x = 0;
    int y = 0;
    int size = 0;
    int Number_Of_Bits = 0;
    int Number_Of_Bytes = 0;
    int Total_Number_Of_Bytes = 36;
    InputData userInputs = getInput();

    FILE * InputFilePtr = fopen(userInputs.filePath, "r");
    if (InputFilePtr == NULL) {
        printf("Error: We are not able to open your file.\n");
        return 1;
    } else {
        FILE * OutputFilePtr = fopen("output.txt", "w");
        if (OutputFilePtr == NULL) {
            printf("Error: We are not able to create the output file.\n");
            fclose(InputFilePtr);
            return 1;
        }
        
        //reading the input and then writing them to the new array
        char HexDecimal_String[MAX_READ];
        size_t read_bytes = fread(HexDecimal_String, 1, MAX_READ, InputFilePtr);
        size_t i,j;
        j = 0;
        if(read_bytes <= 0){
            printf("File is empty!");
            exit(1);
        }
        for (i = 0; i < read_bytes; ++i) {
            if (HexDecimal_String[i] != ' ' && HexDecimal_String[i] != '\n') {
                HexDecimal_String[j++] = HexDecimal_String[i];
            }
        }
        HexDecimal_String[j] = '\0';
        char dataTypeChar;
        if(!strcmp(userInputs.dataType, "float")){
            dataTypeChar = 'f';
            }
        else if(!strcmp(userInputs.dataType, "int")){
            dataTypeChar = 'i';
            }
        else if(!strcmp(userInputs.dataType, "unsigned")){
            dataTypeChar = 'u';
            }
        else {
            printf("Wrong data type input!");
            exit(1);
        } 
        
        size = userInputs.typeSize;
        unsigned int unsignedVal;
        signed int signedVal;
        char *floatStr;
        char *byteString;
        int count = 0;
        if (size == 1 || size == 2 || size == 3 || size == 4) {
            
            char Split_String[size * 2 + 1];
            Split_String[size * 2] = '\0';
            for (x = 0; x < j - size * 2; x += size * 2) {
                count++;    
                for (y = 0; y < (size * 2); y++) {
                    Split_String[y] = HexDecimal_String[x + y];
                }
                toEndianConvert(Split_String, userInputs.typeSize, userInputs.byteOrder);
                byteString = HexDecimal_To_Binary(Split_String);
                //Calling all the methods to make the operations happen. (Cooking the cauldron.)
                switch(dataTypeChar){
                    case 'f' :
                        floatStr = convertToFloat(byteString,userInputs.typeSize);
                        fprintf(OutputFilePtr,"%s",floatStr);
                        free(floatStr);
                        break;
                    case 'u' :
                        unsignedVal = ConversionToUnsigned(byteString,userInputs.typeSize * 4);
                        fprintf(OutputFilePtr,"%u",unsignedVal);
                        break;
                    case 'i' :
                        signedVal = ConversionToSigned(byteString,userInputs.typeSize * 4);
                        fprintf(OutputFilePtr,"%d",signedVal);
                        break;
                }
                if(x + size * 2 < j - size * 2){
                    if(count % 3 == 0){
                        fprintf(OutputFilePtr,"\n");
                    }
                    else {
                        fprintf(OutputFilePtr," ");
                    }
                }
                free(byteString);
            }
        } else {
            fprintf(OutputFilePtr, "Invalid type size my good sir! Better luck next time.");
            fclose(InputFilePtr);
            fclose(OutputFilePtr);
            return 1;
        }
        
        fclose(InputFilePtr);
        fclose(OutputFilePtr);
    }
    
    return 0;
}
//-----------------------------------------------------------------------//
//
InputData getInput() {
    InputData inputData;

    printf("Enter the file path for me: (The assignment says that the user will be prompted for the input file.) ");
    scanf("%s", inputData.filePath);

    printf("Enter byte ordering (Is it Little Endian or Big Endian?): ");
    scanf("%s", inputData.byteOrder);

    printf("Enter data type (Is it signed integer, unsigned integer or floating point number?): ");
    scanf("%s", inputData.dataType);

    printf("Enter data type size (Is it 1, 2, 3, or 4 bytes?): ");
    scanf("%d", &inputData.typeSize);

    return inputData;
}


//-----------------------------------------------------------------------//
//

void toEndianConvert(char *binStr, int size, const char *endianType) {
	int k = 0;
    if (strcmp(endianType, "l") == 0) {
        int i,j,k;
        char temp;
        for (i = 0; i <= (size - 2); i += 2) {
            temp = binStr[i]; 
            binStr[i] = binStr[size * 2 - i - 2];
            binStr[size * 2 - i - 2] = temp;
            temp = binStr[i + 1];
            binStr[i + 1] = binStr[size * 2 - i - 1];
            binStr[size * 2 - i - 1] = temp;
        }
    } else if (strcmp(endianType, "b") == 0) {
        printf("Wrong ordering type!");
        exit(1);
    }
}



//-----------------------------------------------------------------------//
//
char * HexDecimal_To_Binary(char * HexDecimal_String) {
    int i = 0;
    int length = strlen(HexDecimal_String);
    char * Binary_String = calloc(length * 4 + 1, 1);

    for (i = 0; HexDecimal_String[i] != '\0'; ++i) {
        switch(HexDecimal_String[i]) {
            case '0':
                strcat(Binary_String, "0000");
                break;
            case '1':
                strcat(Binary_String, "0001");
                break;
            case '2':
                strcat(Binary_String, "0010");
                break;
            case '3':
                strcat(Binary_String, "0011");
                break;
            case '4':
                strcat(Binary_String, "0100");
                break;
            case '5':
                strcat(Binary_String, "0101");
                break;
            case '6':
                strcat(Binary_String, "0110");
                break;
            case '7':
                strcat(Binary_String, "0111");
                break;
            case '8':
                strcat(Binary_String, "1000");
                break;
            case '9':
                strcat(Binary_String, "1001");
                break;
            case 'A':
            case 'a':
                strcat(Binary_String, "1010");
                break;
            case 'B':
            case 'b':
                strcat(Binary_String, "1011");
                break;
            case 'C':
            case 'c':
                strcat(Binary_String, "1100");
                break;
            case 'D':
            case 'd':
                strcat(Binary_String, "1101");
                break;
            case 'E':
            case 'e':
                strcat(Binary_String, "1110");
                break;
            case 'F':
            case 'f':
                strcat(Binary_String, "1111");
                break;
            default:
                printf("Invalid Hexadecimal Input!");
                exit(1);
        }
    }
    return Binary_String;
}
//-----------------------------------------------------------------------//
// Function (ConversionToUnsigned):
unsigned int ConversionToUnsigned(char * input,int length) {
    int i = 0;
    int bitValue = 1;
    unsigned int result = 0;
    
    for (i = 0; i < length; i++) {
        if (input[i] != ' ') {
            if (input[i] == '1') {
                result += bitValue;
            }
            bitValue *= 2;
        }
    }
    return result;
}
//-----------------------------------------------------------------------//
// Function (ConversionToSigned):
signed int ConversionToSigned(char * Input,int length) {
    int i = 0;
    //-----------------------------------------------------------------------//
    // Positive 2's Complement
    if (Input[0] == '0') {
        return ConversionToUnsigned(Input,length);
    //-----------------------------------------------------------------------//
    // Negative 2's Complement
    } else if (Input[0] == '1') {
        // Binary String without the MS Bit (1)
        char input[length - 1];
        for (i = 1; i < length; i++) {
            // (1) thrown from the start.
            input[i - 1] = Input[i];
        }
        // Convert binary using unsigned conversion - MS Bit Value
        return ConversionToUnsigned(Input,length) - pow(2,length);
    } else {
        printf("Invalid Input!");
        return -1;
    }
}
//-----------------------------------------------------------------------//
//
char *convertToFloat(char *inputString,int byteSize){
    char *returnStr = calloc(50,1);
    int exponent;
    double fraction;
    if(byteSize == 1){
        exponent = 4;
        if(fraction = checkForNaNFinity(inputString, byteSize, exponent, returnStr ) == 0)
            return returnStr;
    }
    else if(byteSize == 2){
        exponent = 6;
        if(fraction = checkForNaNFinity(inputString, byteSize, exponent, returnStr) == 0)
            return returnStr;
    }
    else if(byteSize == 3){
        exponent = 8;
        roundF(inputString,21,24,inputString[22]);
        if(fraction = checkForNaNFinity(inputString, byteSize, exponent, returnStr) == 0)
            return returnStr;
        
    }
      else if(byteSize == 4){
        exponent = 10;
        roundF(inputString,23,32,inputString[24]);
        if(fraction = checkForNaNFinity(inputString, byteSize ,exponent, returnStr) == 0)
            return returnStr;
        
    }
    else {
        printf("Wrong bitsize entered for floating points");
        exit(1);
    }
    /*Calculating exponent*/
    int i,exponentVal,isDenormalized;
    exponentVal = 0;
    for(i = 1; i < exponent; i++){
        if(inputString[i] == '1')
            exponentVal += 1;
    exponentVal = exponentVal << 1;
    }

    isDenormalized = exponentVal == 0;
    int bias = -(int)pow(2,exponent - 1) + 1;
    exponentVal += bias; /*Subtracting bias*/
    /*Calcualting fraction*/
    for(i = byteSize * 8 - 1; i > exponent; i--){
        if(inputString[i] == '1')
            fraction += 1;
            fraction /= 2;
    }
    if(isDenormalized)
        exponentVal++;
    else
        fraction++;
    /*Calculating the number*/
    fraction *= powerOfTwo(exponentVal);
    if(fraction == 0.0){
        if(inputString[0] == '0')
            sprintf(returnStr ,"0");
        else
            sprintf(returnStr ,"-0");
    }
    else if(fraction >= 10.0 || fraction <= -10.0 || (fraction >= -1 && fraction <= 1)){ 
        sprintf(returnStr ,"%.5e",fraction);
    }
    else{
        sprintf(returnStr ,"%.5f",fraction);
    }
    return returnStr;
}
//-----------------------------------------------------------------------//
//
double checkForNaNFinity(char *inputString, int byteSize, int exponent, char *returnStr){
    /*Check for NaN and Infinity*/
      int i;
      int isWrong = 1; //Wrong refers to NaN or infinity
      for(i = 1; i <= exponent; i++){
          if(inputString[i] == '0')
            isWrong = 0;
      }
      if(isWrong){
          int isNaN = 0;
          for(i = exponent + 1; i < byteSize * 8; i++){
              if(inputString[i] == '1'){
                  isNaN = 1;
            }
          }
        if(isNaN)
            sprintf(returnStr ,"NaN");
        else if(inputString[0] == '1')
            sprintf(returnStr ,"-∞");
        else sprintf(returnStr ,"∞");
        
        return 0;
      }
      return 1;
}
//-----------------------------------------------------------------------//
//
void roundF(char *inputString ,int startInt ,int finalInt, char setChar){
    int i = startInt;
    if(setChar == '1'){
        for(; i >= 0 && inputString[i] == '1'; i--)
            inputString[i] = '0';
    }
    if(i != -1)
            inputString[i] = setChar;
    startInt++;
    while(startInt < finalInt){
        inputString[startInt] = '0';
        startInt++;
     }
}
//-----------------------------------------------------------------------//
//
double powerOfTwo(int exponentVal){
    double returnValue;
    
    if(exponentVal < 0){
            returnValue = -recursivePowerOfMTwo(-exponentVal,0.5);
    }
    else
        returnValue = recursivePowerOfTwo(exponentVal,2.0);
    return returnValue;
}
//-----------------------------------------------------------------------//
//
double recursivePowerOfTwo(int exponentVal, double returnValue){
    double temp;
    if(exponentVal <= 1)
    return returnValue;
    
    temp = recursivePowerOfTwo(exponentVal / 2, returnValue);
    if(exponentVal % 2 == 1)
          return temp * temp * returnValue;
        return temp * temp;
}
//-----------------------------------------------------------------------//
//
double recursivePowerOfMTwo(int exponentVal, double returnValue){
    double temp;
    if(exponentVal <= 1)
    return returnValue;
    temp = recursivePowerOfMTwo(exponentVal / 2, returnValue);
    if(exponentVal % 2 == 1)
          return temp * temp * returnValue;
    return temp * temp;
}
//-----------------------------------------------------------------------//
//
























 /*
    char * Binary1 = "1111 1111 1111 1111 1111 1111 1111 1110";
    char * Binary2 = "0111 1111 1111 1111 1111 1111 1111 1111";

    signed int Binary1ToDecimal = ConversionToSigned(Binary1);
    unsigned int Binary1_ToDecimal = ConversionToUnsigned(Binary1);
    signed int Binary2ToDecimal = ConversionToSigned(Binary2);
    unsigned int Binary2_ToDecimal = ConversionToUnsigned(Binary2);

    printf("%d", Binary1ToDecimal);
    printf("\n");
    printf("%u", Binary1_ToDecimal);
    printf("\n");
    printf("%d", Binary2ToDecimal);
    printf("\n");
    printf("%u", Binary2_ToDecimal);

#include <stdio.h>
int main() {
    // my array
    int faveNumbers[] = {7, 33, 13, 9, 29};

    // using sizeof to get the size of the array in bytes
    size_t size = sizeof(faveNumbers);
    
    printf("The size is %d bytes \n", size);
}

// output

// The size is 20 bytes


#include <stdio.h>
int main() {
    int faveNumbers[] = {7, 33, 13, 9, 29};

    size_t size = sizeof(faveNumbers) / sizeof(faveNumbers[0]);
  
    printf("The length of the array is %d \n", size);
}

// output

// The length of the array is 5

*/

