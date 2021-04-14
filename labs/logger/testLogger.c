void infof(const char *format, ...);
void warnf(const char *format, ...);
void errorf(const char *format, ...);
void panicf(const char *format, ...);

int main() {
    infof("INFO: %s\n", "info");
    warnf("WARNING: %s\n", "warning");
    errorf("ERROR: %s\n", "err");
    panicf("PANIC: %s\n", "panic");
}
