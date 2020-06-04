#include <netinet/in.h>
#include <stdio.h>
#include <string.h> /* strlen */
#include <sys/socket.h>
#include <arpa/inet.h> /* inet_addr */
#include <unistd.h> /* close */
int main(int argc, char *argv[])
{
    int socket_desc, c, new_socket;
    struct sockaddr_in server, client;
    char *message, client_reply[2000];
    /* abre um socket */
    socket_desc = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_desc == -1)
    {
        printf("Não foi possível criar o socket\n");
        return 1;
    }
    /* onde quero ouvir ? */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; /* escute em qualquer IP da máquina */
    server.sin_port = htons(8888);
    /* vincula o socket à porta e ao endereço informados */
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        printf("Erro ao fazer o bind\n");
        return 1;
    }
    printf("Bind efetuado.\n");
    /* escutar */
    listen(socket_desc, 3);
    /* aceitar conexões entrantes */
    printf("Aguardando conexões...\n");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*) &c)))
    {
        char *client_ip = inet_ntoa(client.sin_addr);
        int client_port = ntohs(client.sin_port);
        printf("conexão aceita do client %s:%d\n", client_ip, client_port);

        do {
            bzero(client_reply, sizeof(client_reply));
            bzero(client_reply, sizeof(client_reply)); /* limpa a variável char[] */
            /* recebe dados do cliente */
            if (recv(new_socket, client_reply, 2000, 0) < 0)
            {
                printf("Falha no recv\n");
                return 1;
            }
            printf("Resposta recebida.\n");
            printf("%s\n", client_reply);
            /* resposta ao cliente */
            message = "Olá Cliente! Recebi sua conexão, mas preciso ir agora! Tchau!";
            write(new_socket, message, strlen(message));
        } while(strcmp(client_reply, "exit") != 0);
    }
    if (new_socket < 0) 
    {
        printf("Erro ao aceitar conexão\n");
        return 1;
    }
    return 0;
}