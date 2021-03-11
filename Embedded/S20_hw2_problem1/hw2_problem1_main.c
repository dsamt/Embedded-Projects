int main(void)
{

    unsigned int a = 0xffffffff;
    unsigned int b = 0xaaaaaaaa;

    unsigned char error = 0;
    unsigned long long c = (unsigned long long)a * (unsigned long long)b;

    // You need to fix this bug without changing the type of a or b. You can only change c if you need to.
    // We expect c to be more than a, so if it c is not more than a, we update error to 1 to show there has been an error
    if (c<a){
        error = 1;
    }
    while (1) {
    }
}
