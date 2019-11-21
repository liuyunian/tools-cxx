#include <libgen.h> // basename
#include <unistd.h> // usleep

#include <tools/log/log.h>
#include <tools/log/LogFile.h>

std::unique_ptr<LogFile> g_logFile;

void output_func(const char *msg, int len){
    g_logFile->append(msg, len);
}

void flush_func(){
    g_logFile->flush();
}

int main(int argc, char *argv[])
{
    char name[256] = { 0 };
    strncpy(name, argv[0], sizeof name - 1);
    g_logFile.reset(new LogFile(::basename(name), 200*1000));
    Log::set_output(output_func);
    Log::set_flush(flush_func);

    const char *line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    for (int i = 0; i < 10000; ++i){
        LOG_INFO("%s%d", line, i);

        usleep(1000);
    }

    return 0;
}