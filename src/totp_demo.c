#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



int32_t FillFromURANDOM(uint8_t* out, size_t outlen)
{
    FILE* fpurandom = fopen("/dev/urandom", "r");
    if(!fpurandom)
    {
        return -1;
    }

    int32_t bread = fread(out, 1, outlen, fpurandom);
    fclose(fpurandom);

    return bread < outlen ? -1 : bread;
}

void demo_ansi_qrcode()
{
    char secret[33] = {0};
    //Create a base32 encoded secret in an ascii string
    generate_random_secret(secret, 33, FillFromURANDOM);
    printf("Secret: %s\n", secret);

    //Create an ANSI QR code graphic. qrcodeansi owns the new heap memory.
    char* qrcodeansi = create_totp_qrcode("Test", "Meow", secret);

    printf("--------------------------------------------------------------------------------\n");
    printf("%s\n", qrcodeansi);

    //Free the memory holding the ANSI QR code graphic
    free(qrcodeansi);
}

void demo_totp_calculation()
{
    char secret[33] = {0};
    char buf[512] = {0};

    printf("Enter secret to compute current TOTP:");
    scanf("%s", buf);

    //Decode the secret from base32 ASCII to its raw data
    size_t secretlen = base32decode(buf, strnlen(buf, 512), secret, 33);

    time_t now = time(0);

    printf("secretlen: %d\n", secretlen);
    int otp = compute_totp(secret, secretlen, now, 30, 6);
    printf("Current otp code: %06d\n", otp);
}

int totp_demo(void)
{
    demo_ansi_qrcode();
    demo_totp_calculation();
    return 0;
}
