CRC
#include <stdio.h>
#include <string.h>

#define N strlen(gen_poly)

char data[28];
char check_value[28];
char gen_poly[10];

int data_length, i, j;

void XOR() {
    for (j = 1; j < N; j++)
        check_value[j] = ((check_value[j] == gen_poly[j]) ? '0' : '1');
}

void crc() {
    for (i = 0; i < N; i++)
        check_value[i] = data[i];
    do {
        if (check_value[0] == '1')
            XOR();
        for (j = 0; j < N - 1; j++)
            check_value[j] = check_value[j + 1];
        check_value[j] = data[i++];
    } while (i <= data_length + N - 1);
    check_value[N - 1] = '\0';
}

void receiver() {
    printf("Enter the received data: ");
    scanf("%s", data);
    printf("\n----------------------------------\n");
    printf("Data received: %s", data);
    crc();
    for (i = 0; (i < N - 1) && (check_value[i] != '1'); i++);
    if (i < N - 1)
        printf("\nError detected\n\n");
    else
        printf("\nNo error detected\n\n");
}

int main() {
    printf("Enter data to be transmitted: ");
    scanf("%s", data);
    printf("Enter the Generating polynomial: ");
    scanf("%s", gen_poly);
    data_length = strlen(data);
    for (i = data_length; i < data_length + N - 1; i++)
        data[i] = '0';
    data[i] = '\0';
    printf("\n----------------------------------");
    printf("\nData padded with n-1 zeros : %s", data);
    printf("\n----------------------------------");
    crc();
    printf("\nCRC or Check value is : %s", check_value);
    for (i = data_length; i < data_length + N - 1; i++)
        data[i] = check_value[i - data_length];
    data[i] = '\0';
    printf("\n----------------------------------");
    printf("\nFinal data to be sent : %s", data);
    printf("\n----------------------------------\n");
    receiver();
    return 0;
}



How to Compile:
Save the file as crc_check.c, then compile with:
gcc crc_check.c -o crc_check

How to Run:
Run it using:
./crc_check

Sample Run (Input & Output):
Enter data to be transmitted: 100100
Enter the Generating polynomial: 1101

----------------------------------
Data padded with n-1 zeros : 100100000
----------------------------------
CRC or Check value is : 001
----------------------------------
Final data to be sent : 100100001
----------------------------------
Enter the received data: 100100001

----------------------------------
Data received: 100100001
No error detected

