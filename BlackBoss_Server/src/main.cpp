#include "../include/IA/BlackIa.h"
#include "../include/IA/Sujet.h"
#include "../include/Environnement/server.h"
#include <string.h>
#include "../include/Environnement/Event.h"
#include "../include/IA/other.h"
#include <QApplication>
using namespace std;

unsigned int sigIncons=10;
unsigned int sigCons=14;
unsigned int sigUser=31;
unsigned int sigServ=12;


string cons="cons";
string Incons="incons";
string userStr="user";
string servStr="server";


std::queue<Event*> eventBlackBoss;

Server* Serv;

string nameBlackBoss("BlackBoss");
//BlackIa* BlackBoss;
BlackIa* BlackBoss;
InBlackIa* InBoss;
User* Vous;

MYSQL data;
Database * memory;


void setStdinEcho(bool enable = true)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
 
    if( !enable )
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;
 
    SetConsoleMode(hStdin, mode );
 
#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( !enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;
 
    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

string VousTalk()
{
    cout<<"Vous:/>";
    //cin>>rep;

    char rep[10000];
    fgets(rep,sizeof rep,stdin);
    int k=strlen(rep);
    rep[k-1]='\0';
    string trueRep="";
    trueRep.append(rep);
    return trueRep;
}

string VousTalkMdp()
{
    cout<<"Vous:/>";
    //cin>>rep;
    setStdinEcho(false);
    char rep[10000];
    fgets(rep,sizeof rep,stdin);
    int k=strlen(rep);
    rep[k-1]='\0';
    string trueRep="";
    trueRep.append(rep);
    setStdinEcho(true);
    cout<<endl;
    return trueRep;
}

void signalHandler(int sigNum)
{
    if(sigNum==sigIncons)
    {
        //cout<<"sigIncons"<<endl;
        BlackBoss->hasEvent=true;
    }
    else if(sigNum==sigCons)
    {
        //cout<<"sigCons"<<endl;
        if(InBoss->events.size()>0)
        {
            InBoss->hasEvent=true;
        }
        if(Vous->events.size()>0)
        {
            Vous->hasEvent=true;
        }
    }
    else if(sigNum==sigUser)
    {
        cout<<"sigUser"<<endl;
        if(Serv->events.size()>0)
        {
            cout<<"toServ"<<endl;
            Serv->hasEvent=true;
        }
        if(BlackBoss->events.size()>0)
        {
            cout<<"toIA"<<endl;
            BlackBoss->hasEvent=true;
        }
    }
    else if(sigNum==sigServ)
    {
        cout<<"sigServ"<<endl;
        Vous->hasEvent=true;
    }
    
}

Event* event(string _dest, string _src, string _cont, string _type, unsigned int _sigNum)
{
    Event* ev=new Event(_dest, _src, _cont, _type, _sigNum);
    deliverEvent(ev);
    //cout<<"signal to raise "<<ev->sigNum<<endl;
    raise(ev->sigNum);
    //cout<<"ok raise"<<endl;
}

Event* eventS(string _dest, string _src, string _cont, string _type, unsigned int _sigNum)
{
    Event* ev=new Event(_dest, _src, _cont, _type, _sigNum);
    deliverEvent(ev);
    cout<<"event from server"<<endl;
    //cout<<"signal to raise "<<ev->sigNum<<endl;
    raise(ev->sigNum);
    //cout<<"ok raise"<<endl;
}

void deliverEvent(Event* ev)
{
    //cout<<"deliver"<<endl;
   if(ev->dest.compare(cons)==0)
   {
       //cout<<"to ia"<<endl;
       BlackBoss->events.push(ev);
       //cout<<"ok"<<endl;
   }
   else if(ev->dest.compare(Incons)==0)
   {
       //cout<<"to incons"<<endl;
       InBoss->events.push(ev);
   }
   else if(ev->dest.compare(userStr)==0)
   {
       Vous->events.push(ev);
   }
   else if(ev->dest.compare(servStr)==0)
   {
       Serv->events.push(ev);
   }
}


int main(int argc, char** argv) {

    QApplication app(argc,argv);
    //cout<<"yo"<<endl;
    Serv=new Server();
    Serv->show();
    BlackBoss= new BlackIa(nameBlackBoss);
    InBoss=BlackBoss->incons;
    BlackBoss->start();
    signal(sigCons,signalHandler);
    signal(sigIncons,signalHandler);
    signal(sigUser,signalHandler);
    signal(sigServ,signalHandler);
    Poco::Thread::sleep(20);
    /*while(!BlackBoss->incons->isOk)
    {
        Poco::Thread::sleep(1);
    }
    while(BlackBoss->isAlive)
    {
        if(BlackBoss->hasRep)
        {
            Vous->Talk();
            BlackBoss->hasRep=false;
            
        }
        //cout<<"waiting for rep"<<endl;
        Poco::Thread::sleep(100);
    }
    cout<<"End main Thread"<<endl;
    delete BlackBoss;
    raise(SIGKILL);*/
    return app.exec();
}
