#include<stdio.h>
#include<string.h>
#include<time.h>


void caesar_encrypt(char text[], int shift);
void caesar_decrypt(char text[], int shift);
void affine_encrypt(char text[], int a, int b);
void affine_decrypt(char text[], int a, int b);
void hill_encrypt(char text[], int key[2][2]);
void hill_decrypt(char text[], int key[2][2]);
void write_history(const char *operation, const char *cipher, const char *input, const char *output, const char *key_or_shift);
int mod_inverse(int a, int m);
void preprocess_text(char *text);

int main()
{
int cipher_choice,mode_choice;
char input_text[100];
int shift,a,b;
int key[2][2];
char key_info[50];
char output_text[100];

printf("Select your choice:\n1.Caesar Cipher\n2.Affine Cipher\n3.Hill Cipher\n");
scanf("%d",&cipher_choice);

printf("Select mode:\n1.Encrypt\n2.Decrypt\n");
scanf("%d", &mode_choice);

printf("Enter the text:");
getchar();
fgets(input_text, sizeof(input_text), stdin);
input_text[strcspn(input_text, "\n")] = '\0';
strcpy(output_text,input_text);

if(cipher_choice==1)
{
printf("Enter the shift:");
scanf("%d",&shift);
sprintf(key_info, "%d", shift);

if(mode_choice==1)
{
caesar_encrypt(output_text,shift);
write_history("Encryption", "Caesar", input_text, output_text, key_info);
}
else
{
caesar_decrypt(output_text,shift);
write_history("Decryption", "Caesar", input_text, output_text, key_info);
}
}

else if(cipher_choice==2)
{
printf("Enter key 'a' (must be coprime with 26): ");
scanf("%d", &a);
printf("Enter key 'b': ");
scanf("%d", &b);
sprintf(key_info, "a=%d, b=%d", a,b);

if (mode_choice == 1)
{
affine_encrypt(output_text, a, b);
write_history("Encryption", "Affine", input_text, output_text, key_info);
}
else
{
affine_decrypt(output_text, a, b);
write_history("Decryption", "Affine", input_text, input_text, key_info);
}
}

else if(cipher_choice==3)
{
printf("\nNote: The matrix should be invertible modulo 26 (i.e., determinant must be co-prime with 26).\n");
printf("Enter 2x2 key matrix (4 numbers): ");
scanf("%d %d %d %d", &key[0][0], &key[0][1], &key[1][0], &key[1][1]);
sprintf(key_info, "%d %d %d %d", key[0][0], key[0][1], key[1][0], key[1][1]);
preprocess_text(output_text);

if (mode_choice == 1)
{
hill_encrypt(output_text, key);
write_history("Encryption", "Hill", input_text, output_text, key_info);
}
else
{
hill_decrypt(output_text, key);
write_history("Decryption", "Hill", input_text, input_text, key_info);
}
}

else
{
printf("Invalid cipher selected!!\n");
}
return 0;
}


void preprocess_text(char *text)
{
for (int i = 0; text[i] != '\0'; i++)
{
if (text[i] >= 'a' && text[i] <= 'z')
{
text[i] -= 32;
}
else if (text[i] < 'A' || text[i] > 'Z')
{
text[i] = 'X';
}
}
}



void caesar_encrypt(char text[], int shift)
{
for (int i = 0; text[i] != '\0'; i++)
{
char ch = text[i];

if (ch >= 'A' && ch <= 'Z')
text[i] = ((ch - 'A' + shift) % 26) + 'A';
else if (ch >= 'a' && ch <= 'z')
text[i] = ((ch - 'a' + shift) % 26) + 'a';
}
printf("Encrypted text: %s\n", text);
}




void caesar_decrypt(char text[], int shift)
{
for (int i = 0; text[i] != '\0'; i++)
{
char ch = text[i];

if (ch >= 'A' && ch <= 'Z')
text[i] = ((ch - 'A' - shift + 26) % 26) + 'A';
else if (ch >= 'a' && ch <= 'z')
text[i] = ((ch - 'a' - shift + 26) % 26) + 'a';
}
printf("Decrypted text: %s\n", text);
}



void affine_encrypt(char text[], int a, int b)
{
for (int i = 0; text[i] != '\0'; i++)
{
char ch = text[i];

if (ch >= 'A' && ch <= 'Z')
text[i] = ((a * (ch - 'A') + b) % 26) + 'A';
else if (ch >= 'a' && ch <= 'z')
text[i] = ((a * (ch - 'a') + b) % 26) + 'a';
}
printf("Encrypted text: %s\n", text);
}




void affine_decrypt(char text[], int a, int b)
{
int a_inv = mod_inverse(a, 26);
if (a_inv == -1)
{
printf("Invalid key, 'a' has no modular inverse.\n");
return;
}
for (int i = 0; text[i] != '\0'; i++) {
char ch = text[i];

if (ch >= 'A' && ch <= 'Z')
text[i] = ((a_inv * ((ch - 'A' - b + 26)) % 26) + 'A');
else if (ch >= 'a' && ch <= 'z')
text[i] = ((a_inv * ((ch - 'a' - b + 26)) % 26) + 'a');
}
printf("Decrypted text: %s\n", text);
}



int mod_inverse(int a, int m)
{
for (int x = 1; x < m; x++)
{
if ((a * x) % m == 1)
return x;
}
return -1;
}




void hill_encrypt(char text[], int key[2][2])
{
int len = strlen(text);
if (len % 2 != 0) strcat(text, "X");
printf("Encrypted text: ");

for (int i = 0; i < len; i += 2)
{
int x = text[i] - 'A';
int y = text[i + 1] - 'A';
char c1 = (key[0][0] * x + key[0][1] * y) % 26 + 'A';
char c2 = (key[1][0] * x + key[1][1] * y) % 26 + 'A';
printf("%c%c", c1, c2);
}
printf("\n");
}



void hill_decrypt(char text[], int key[2][2])
{
int det = (key[0][0]*key[1][1] - key[0][1]*key[1][0]) % 26;
if (det < 0) det += 26;
int det_inv = mod_inverse(det, 26);
if (det_inv == -1)
{
printf("Invalid key matrix, determinant has no modular inverse.\n");
return;
}

int inv_key[2][2];
inv_key[0][0] = key[1][1];
inv_key[1][1] = key[0][0];
inv_key[0][1] = -key[0][1];
inv_key[1][0] = -key[1][0];

for (int i = 0; i < 2; i++)
for (int j = 0; j < 2; j++)
{
inv_key[i][j] = (inv_key[i][j] * det_inv) % 26;
if (inv_key[i][j] < 0) inv_key[i][j] += 26;
}

int len = strlen(text);
printf("Decrypted text: ");

for (int i = 0; i < len; i += 2)
{
int x = text[i] - 'A';
int y = text[i + 1] - 'A';
char c1 = (inv_key[0][0] * x + inv_key[0][1] * y) % 26 + 'A';
char c2 = (inv_key[1][0] * x + inv_key[1][1] * y) % 26 + 'A';
printf("%c%c", c1, c2);
}
printf("\n");
}



void write_history(const char *operation, const char *cipher, const char *input, const char *output, const char *key_or_shift) {
FILE *history = fopen("history.txt", "a");
if (!history)
{
printf("Error writing to history file.\n");
return;
}

time_t now = time(NULL);
char *timestamp = ctime(&now);
timestamp[strlen(timestamp) - 1] = '\0';

fprintf(history, "[%s] Operation: %s | Cipher: %s | Input: %s | Output: %s | Key/Shift: %s\n",
timestamp, operation, cipher, input, output, key_or_shift);

fclose(history);
}
