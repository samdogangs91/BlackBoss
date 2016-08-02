#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
//#include "BlackIa.h"

#ifdef WIN32
#include <window.h>
#include <winsock2.h>
#else
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <iostream>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <Poco/Activity.h>
#include <mysql/mysql.h>


#include <queue>
#include <csignal>

//class BlackIa;

void signalHandler(int sigNum);

bool isApost(char c);

bool isPorV(char c);

bool isSpace(char c);

bool isSeparateur(char c);

std::string VousTalk();

std::string VousTalkMdp();

std::string qui_Est(std::string);

std::string est(std::string);

std::string que_fait(std::string);

std::string definis(std::string);

std::string rien(std::string);
