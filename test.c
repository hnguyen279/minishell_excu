#include <unistd.h>

int main ()
{
    write(1, "hello\n", 6);
}

// export PATH="./:$PATH"
// testcmd      