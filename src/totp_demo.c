#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TOTP_SECRET_BYTES 33


int32_t FillFromURANDOM(uint8_t* out, size_t outlen){
    FILE* fpurandom = fopen("/dev/urandom", "r");
    if(!fpurandom){
        return -1;
    }

    int32_t bread = fread(out, 1, outlen, fpurandom);
    fclose(fpurandom);

    return bread < outlen ? -1 : bread;
}

void demo_ansi_qrcode(){
    char secret[TOTP_SECRET_BYTES] = {0};
    generate_random_secret(secret, TOTP_SECRET_BYTES, FillFromURANDOM);
    printf("Secret: %s\n", secret);
    char* qrcodeansi = create_totp_qrcode("Test", "Meow", secret);
    printf("--------------------------------------------------------------------------------\n");
    printf("%s\n", qrcodeansi);
    free(qrcodeansi);
}

void demo_totp_calculation(){
    char secret[TOTP_SECRET_BYTES] = {0};
    char buf[512] = {0};
    printf("Enter secret to compute current TOTP:");
    scanf("%s", buf);
    size_t secretlen = base32decode(buf, strnlen(buf, 512), secret, TOTP_SECRET_BYTES);
    time_t now = time(0);
    int otp = compute_totp(secret, secretlen, now, 30, 6);
    printf("Current OTP code: %06d\n", otp);
}


int totp_new_secret(void){
    char secret[TOTP_SECRET_BYTES] = {0};
    generate_random_secret(secret, TOTP_SECRET_BYTES, FillFromURANDOM);
    fprintf(stdout, "%s\n", secret);
    return 0;
}

int totp_qrcode_demo(void){
    demo_ansi_qrcode();
    return 0;
}

int totp_validate_random_secret(void){
    char secret[TOTP_SECRET_BYTES] = {0};
    char buf[512] = {0};
    generate_random_secret(buf, TOTP_SECRET_BYTES, FillFromURANDOM);
    size_t secretlen = base32decode(buf, strnlen(buf, 512), secret, TOTP_SECRET_BYTES);
    time_t now = time(0);
    int otp = compute_totp(secret, secretlen, now, 30, 6);
    printf("Secret:\t\t\t%s\n", buf);
    printf("Secret Length:\t\t%d\n", secretlen);
    printf("Current OTP code:\t%06d\n", otp);
    return 0;
}

int totp_validate_secret(void){
    char secret[TOTP_SECRET_BYTES] = {0};
    char encoded_secret[20] = {0};
    char re_encoded_secret[20] = {0};
    char decoded_secret[TOTP_SECRET_BYTES] = {0};
    char buf[512] = {0};
    printf("Enter secret to compute current TOTP:");
    scanf("%s", encoded_secret);
    printf("Encoded Secret:\t\t\t%s\n", encoded_secret);
    time_t now = time(0);
    size_t secretlen = base32decode(encoded_secret, strnlen(encoded_secret, 512), secret, TOTP_SECRET_BYTES);
    int otp = compute_totp(secret, secretlen, now, 30, 6);
    printf("Secret Length:\t\t%d\n", secretlen);
    printf("Current OTP code:\t%06d\n", otp);
    return 0;
}

























