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
// Main function
int main() {
    const int MAX_READ = 1024; // Maximum number of bytes to read from file
    int x = 0;
    int y = 0;
    int size = 0; // Size of data type in bytes
    int Number_Of_Bits = 0; // Number of bits in data type
    int Number_Of_Bytes = 0; // Number of bytes in data type
    int Total_Number_Of_Bytes = 36; // Total number of bytes in input file
    InputData userInputs = getInput(); // Getting input from user

    // Opening input file for reading
    FILE * InputFilePtr = fopen(userInputs.filePath, "r");
    if (InputFilePtr == NULL) { // Checking if file was opened successfully
        printf("Error: File couldn't be opened!\n");
        return 1; // Return 1 to indicate error
    } else {
        // Opening output file for writing
        FILE * OutputFilePtr = fopen("output.txt", "w");
        if (OutputFilePtr == NULL) { // Checking if output file was opened successfully
            printf("Error: Failed to write to output file!\n");
            fclose(InputFilePtr); // Closing input file
            return 1; // Return 1 to indicate error
        }

        // Reading input from file and removing spaces and new lines
        char HexDecimal_String[MAX_READ];
        size_t read_bytes = fread(HexDecimal_String, 1, MAX_READ, InputFilePtr);
        size_t i,j;
        j = 0;
        if (read_bytes <= 0) {
            printf("File is empty!");
            exit(1);
        }
        for (i = 0; i < read_bytes; ++i) {
            if (HexDecimal_String[i] != ' ' && HexDecimal_String[i] != '\n') {
                HexDecimal_String[j++] = HexDecimal_String[i];
            }
        }
        HexDecimal_String[j] = '\0';

        // Converting data type string to character for switch statement
        char dataTypeChar;
        if (!strcmp(userInputs.dataType, "float")) {
            dataTypeChar = 'f';
        } else if (!strcmp(userInputs.dataType, "int")) {
            dataTypeChar = 'i';
        } else if (!strcmp(userInputs.dataType, "unsigned")) {
            dataTypeChar = 'u';
        } else {
            printf("Invalid or Wrong data type input!");
            exit(1);
        }

        size = userInputs.typeSize; // Setting size variable to user input
        unsigned int unsignedVal;
        signed int signedVal;
        char *floatStr;
        char *byteString;
        int count = 0;

        // Checking if the input size is valid
        if (size == 1 || size == 2 || size == 3 || size == 4) {

            // Creating a char array for splitting the hex string into bytes
            char Split_String[size * 2 + 1];
            Split_String[size * 2] = '\0';

            // Looping through the hex string, splitting it into bytes, converting to binary, and then to the desired data type
            for (x = 0; x <= j - size * 2; x += size * 2) {
                count++;

                // Splitting hex string into bytes and converting to desired endianness
                for (y = 0; y < (size * 2); y++) {
                    Split_String[y] = HexDecimal_String[x + y];
                }
                toEndianConvert(Split_String, userInputs.typeSize, userInputs.byteOrder);

                // Converting byte string to binary string
                byteString = HexDecimal_To_Binary(Split_String);

                // Converting binary string to desired data type
                switch(dataTypeChar){
                    case 'f' :
                        floatStr = convertToFloat(byteString,userInputs.typeSize);
                        // printf("%s", floatStr);
                        fprintf(OutputFilePtr,"%s",floatStr);
                        free(floatStr);
                        break;
                    case 'u' :
                        // printf("%u", unsignedVal);
                        unsignedVal = ConversionToUnsigned(byteString,userInputs.typeSize * 4);
                        fprintf(OutputFilePtr,"%u",unsignedVal);
                        break;
                    case 'i' :
                        // printf("%d", signedVal);
                        signedVal = ConversionToSigned(byteString,userInputs.typeSize * 4);
                        fprintf(OutputFilePtr,"%d",signedVal);
                        break;
                }

                // Formatting the output
                if (x + size * 2 <= j - size * 2){
                    if (count % (12 / userInputs.typeSize) == 0){
                        // printf("\n");
                        fprintf(OutputFilePtr,"\n");
                    } else {
                        printf(" ");
                        fprintf(OutputFilePtr," ");
                    }
                }

                free(byteString);
            }
        } else {
            // If the input size is not valid, output an error message
            fprintf(OutputFilePtr, "Invalid or wrong data type size!");
            fclose(InputFilePtr);
            fclose(OutputFilePtr);
            return 1;
        }
    }
}
//-----------------------------------------------------------------------//
// This function takes user input for file name, byte ordering, data type, and data type size
InputData getInput() {
    InputData inputData;

    // Print the header for the binary convertor program
    printf("//====================================***Binary Convertor***====================================//");
    printf("\n");

    // Ask user to enter the input file name
    printf("Enter Input File Name(.txt): ");
    scanf("%s", inputData.filePath);

    // Ask user to enter the byte ordering (little or big endian)
    printf("Enter byte ordering (Little Endian(l) or Big Endian(b): ");
    scanf("%s", inputData.byteOrder);

    // Ask user to enter the data type (signed integer, unsigned integer, or floating-point number)
    printf("Enter data type (Signed Integer(int), Unsigned Integer(unsigned), Floating-point Number(float): ");
    scanf("%s", inputData.dataType);

    // Ask user to enter the data type size (1, 2, 3, or 4 bytes)
    printf("Enter data type size (1, 2, 3, 4 bytes): ");
    scanf("%d", &inputData.typeSize);
    printf("\n");

    // Print the header for the binary convertor program again
    printf("//====================================***Binary Convertor***====================================//");
    printf("\n");

    // Return the user input as an InputData structure
    return inputData;
}
//-----------------------------------------------------------------------//
// This function takes a binary string, its size, and the byte ordering type as input parameters
void toEndianConvert(char *binStr, int size, const char *endianType) {
    int k = 0;

    // If byte ordering type is little endian
    if (strcmp(endianType, "l") == 0) {
        int i,j,k;
        char temp;

        // Swap bytes in the binary string to convert to little endian format
        for (i = 0; i <= (size - 2); i += 2) {
            temp = binStr[i];
            binStr[i] = binStr[size * 2 - i - 2];
            binStr[size * 2 - i - 2] = temp;
            temp = binStr[i + 1];
            binStr[i + 1] = binStr[size * 2 - i - 1];
            binStr[size * 2 - i - 1] = temp;
        }
    }
        // If byte ordering type is big endian
    else if (strcmp(endianType, "b")) {
        // Print error message if byte ordering type is incorrect and exit the program
        printf("Wrong ordering type!");
        exit(1);
    }
}
//-----------------------------------------------------------------------//
// This function takes a hexadecimal string as input parameter
char * HexDecimal_To_Binary(char * HexDecimal_String) {
    int i = 0;
    int length = strlen(HexDecimal_String);

    // Allocate memory to store the binary string equivalent of the hexadecimal string
    char * Binary_String = calloc(length * 4 + 1, 1);

    // Iterate through each character of the hexadecimal string
    for (i = 0; HexDecimal_String[i] != '\0'; ++i) {
        // Convert each hexadecimal digit to its equivalent 4-bit binary string
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
                // Print error message if the input hexadecimal string contains an invalid character and exit the program
                printf("Invalid Hexadecimal Input!");
                exit(1);
        }
    }
    // Return the binary string equivalent of the hexadecimal string
    return Binary_String;
}
//-----------------------------------------------------------------------//
// Function (ConversionToUnsigned): This function converts a binary string to an unsigned integer value.
unsigned int ConversionToUnsigned(char * input,int length) {
    int i = 0;
    int bitValue = 1;
    unsigned int result = 0;

    // Loop through the binary string and calculate the integer value of the binary string
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
// This function converts a given input string representing a binary floating point number
// to its corresponding decimal representation in string format. The byteSize parameter
// specifies the number of bytes that the input string occupies (1, 2, 3 or 4 bytes).

char *convertToFloat(char *inputString,int byteSize){

    // Allocate memory for the return string, which will hold the decimal representation
    // of the input number.
    char *returnStr = calloc(50,1);

    // Declare variables to hold the exponent and fraction parts of the floating point number.
    int exponent;
    double fraction;

    // Check the byte size to determine the format of the input number, and whether
    // it represents NaN or Infinity. If it does, return the appropriate string.
    if(byteSize == 1){
        exponent = 4;
        fraction = checkForNaNFinity(inputString, byteSize, exponent, returnStr) == 0;
        if(fraction)
            return returnStr;
    }
    else if(byteSize == 2){
        exponent = 6;
        fraction = checkForNaNFinity(inputString, byteSize, exponent, returnStr) == 0;
        if(fraction)
            return returnStr;
    }
    else if(byteSize == 3){
        exponent = 8;
        roundF(inputString,21,24,inputString[22]);
        fraction = checkForNaNFinity(inputString, byteSize, exponent, returnStr) == 0;
        if(fraction)
            return returnStr;
    }
    else if(byteSize == 4){
        exponent = 10;
        roundF(inputString,23,32,inputString[24]);
        fraction = checkForNaNFinity(inputString, byteSize, exponent, returnStr) == 0;
        if(fraction)
            return returnStr;
    }
    else {
        // If the byte size is invalid, print an error message and exit the program.
        printf("Wrong bitsize entered for floating points");
        exit(1);
    }

    // Calculate the exponent part of the number.
    int i,exponentVal,isDenormalized;
    exponentVal = 0;
    for(i = 1; i < exponent; i++){
        if(inputString[i] == '1')
            exponentVal += 1;
        exponentVal = exponentVal << 1;
    }
    if(inputString[i] == '1')
        exponentVal += 1;

    // Check whether the number is denormalized (i.e. the exponent is zero).
    isDenormalized = exponentVal == 0;

    // Calculate the bias for the exponent.
    int bias = -(int)pow(2,exponent - 1) + 1;
    exponentVal += bias; // Subtract the bias from the exponent.

    // Calculate the fraction part of the number.
    for(i = byteSize * 8 - 1; i > exponent; i--){
        if(inputString[i] == '1')
            fraction += 1;
        fraction /= 2;
    }

    // If the number is denormalized, adjust the exponent accordingly.
    if(isDenormalized)
        exponentVal++;
    else
        fraction++;

    // Calculate the final value of the number.
    fraction *= powerOfTwo(exponentVal);
    if(inputString[0] == '1'){
        fraction *= -1;
    }

    // If the fraction part is zero, return either "0" or "-0".
    if(fraction == 0.0){
        // If the sign bit is 0, return "0", otherwise return "-0".
        if(inputString[0] == '0')
            sprintf(returnStr ,"0");
        else
            sprintf(returnStr ,"-0");
    }
        // If the fraction is very large or very small, return in exponential notation.
    else if(fraction >= 10.0 || fraction <= -10.0 || (fraction >= -1 && fraction <= 1)){
        sprintf(returnStr ,"%.5e",fraction);
    }
        // Otherwise, return in decimal notation.
    else{
        sprintf(returnStr ,"%.5f",fraction);
    }
    // Return the converted string representation of the float value.
    return returnStr;
}
//-----------------------------------------------------------------------//
// This function takes four arguments: inputString, byteSize, exponent, and returnStr. The function checks the input string for NaN or infinity values, where byteSize specifies the length of the input string in bytes, and exponent specifies the position of the exponent in the input string. The function returns 1 if the input string does not contain NaN or infinity values, and returns 0 otherwise. The result is stored in returnStr.
double checkForNaNFinity(char *inputString, int byteSize, int exponent, char *returnStr){
    // First, we check if the input string is NaN or infinity by examining the exponent bits.
    int i;
    int isWrong = 1; //Wrong refers to NaN or infinity
    for(i = 1; i <= exponent; i++){
        if(inputString[i] == '0')
            isWrong = 0;
    }
    // If the exponent bits indicate NaN or infinity, we further check the mantissa bits to determine whether the value is NaN or infinity.
    if(isWrong){
        int isNaN = 0;
        for(i = exponent + 1; i < byteSize * 8; i++){
            if(inputString[i] == '1'){
                isNaN = 1;
            }
        }
        // If the mantissa bits indicate NaN, we set returnStr to "NaN".
        if(isNaN)
            sprintf(returnStr ,"NaN");
            // Otherwise, if the sign bit is 1, we set returnStr to "-infinity". Otherwise, we set returnStr to "infinity".
        else if(inputString[0] == '1')
            sprintf(returnStr ,"-infinity");
        else sprintf(returnStr ,"infinity");
        // Finally, we return 0 to indicate that the input string is NaN or infinity.
        return 0;
    }
    // If the input string is not NaN or infinity, we return 1 to indicate that it is a valid number.
    return 1;
}
//-----------------------------------------------------------------------//
// This function takes four arguments: inputString, startInt, finalInt, and setChar. The function rounds the input string to the specified number of decimal places, where startInt and finalInt specify the range of indices in the string to be rounded, and setChar specifies the rounding method ('1' indicates rounding up, '0' indicates rounding down).
void roundF(char *inputString ,int startInt ,int finalInt, char setChar){
    int i = startInt;
    int halfWay = 1;

    // If the rounding method is '1', we determine whether the value to be rounded is exactly halfway between two values.
    if(setChar == '1'){
        for(i = startInt + 2; i < finalInt; i++){
            if(inputString[i] == '1')
                halfWay = 0;
        }
        // If the value is exactly halfway, or if the original value is odd, we round up.
        if(!halfWay || inputString[startInt] == '1'){
            for(i = startInt; i >= 0 && inputString[i] == '1'; i--)
                inputString[i] = '0';
            if(i != -1)
                inputString[i] = setChar;
        }
    }

    // We round down to the specified number of decimal places by setting all the decimal places after the specified number to '0'.
    startInt++;
    while(startInt < finalInt){
        inputString[startInt] = '0';
        startInt++;
    }
}
//-----------------------------------------------------------------------//
// This function takes an argument, exponentVal, and returns the result of 2 raised to the power of exponentVal.
double powerOfTwo(int exponentVal){
    double returnValue;

    // If the exponent value is negative, we compute the square root of 2 raised to the power of the absolute value of the exponent using the function recursivePowerOfMTwo().
    if(exponentVal < 0){
        returnValue = recursivePowerOfMTwo(-exponentVal,0.5);
    }
        // If the exponent value is non-negative, we compute 2 raised to the power of the exponent using the function recursivePowerOfTwo().
    else
        returnValue = recursivePowerOfTwo(exponentVal,2.0);

    // We return the computed value.
    return returnValue;
}
//-----------------------------------------------------------------------//
// This function takes two arguments, exponentVal and returnValue, and returns the result of returnValue raised to the power of exponentVal.
double recursivePowerOfTwo(int exponentVal, double returnValue){
    double temp;

    // If the exponent value is less than or equal to 1, we have reached the base case and we simply return the original value.
    if(exponentVal <= 1)
        return returnValue;

    // If the exponent value is greater than 1, we recursively call the function with half the exponent value and the original value.
    temp = recursivePowerOfTwo(exponentVal / 2, returnValue);

    // If the exponent value is odd, we multiply the result of the recursive call by itself and the original value.
    if(exponentVal % 2 == 1)
        return temp * temp * returnValue;

    // If the exponent value is even, we simply return the result of the recursive call squared.
    return temp * temp;
}
//-----------------------------------------------------------------------//
// This function takes two arguments, exponentVal and returnValue, and returns the result of returnValue raised to the power of 2 to the exponent of exponentVal.
double recursivePowerOfMTwo(int exponentVal, double returnValue){
    double temp;

    // If the exponent value is less than or equal to 1, we have reached the base case and we simply return the original value.
    if(exponentVal <= 1)
        return returnValue;

    // If the exponent value is greater than 1, we recursively call the function with half the exponent value and the original value.
    temp = recursivePowerOfMTwo(exponentVal / 2, returnValue);

    // If the exponent value is odd, we multiply the result of the recursive call by itself and the original value.
    if(exponentVal % 2 == 1)
        return temp * temp * returnValue;

    // If the exponent value is even, we simply return the result of the recursive call squared.
    return temp * temp;
}
//-----------------------------------------------------------------------//
