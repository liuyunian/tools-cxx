#include <libgen.h> // basename
#include <unistd.h> // usleep

#include <tools/log/log.h>
#include <tools/log/LogFile.h>

int main(int argc, char *argv[]){
  LogFile lf(::basename(argv[0]), 200*1000);
  Log::set_output(std::bind(&LogFile::append, &lf, std::placeholders::_1, std::placeholders::_2));
  Log::set_flush(std::bind(&LogFile::flush, &lf));
  lf.setCheckEveryN(1);

  const char *line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  for (int i = 0; i < 10000; ++i){
    LOG_INFO("%s%d", line, i);

    sleep(1);
  }

  return 0;
}