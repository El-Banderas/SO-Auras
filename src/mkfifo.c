#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]) {
    mkfifo("../Centralfifo", 0644);
    return 0;
}
