#ifndef CONNECTION_H
#define CONNECTION_H

class Connection
{
public:
    Connection();
private:
    int _listeningSocket;
    void setSocketReuse(int sock);

};

#endif // CONNECTION_H
