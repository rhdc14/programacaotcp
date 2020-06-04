#include <stdio.h>
#include <string.h> /* strlen */
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h> /* inet_addr */
#include <unistd.h> /* close */
int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char message[2000], server_reply[2000];
    /* AF_INET - IPv4, SOCK_STREAM - tcp, 0 - IP */
    socket_desc = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_desc == -1)
    {
        printf("Não foi possível criar o socket\n");
        return 1;
    }
    /* 
    // IPv4 AF_INET sockets:
    struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET, AF_INET6
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };
    struct in_addr {
        unsigned long s_addr;          // load with inet_pton()
    };
    struct sockaddr {
        unsigned short    sa_family;    // address family, AF_xxx
        char              sa_data[14];  // 14 bytes of protocol address
    };
    */
    /* informa os dados do servidor para onde vamos nos conectar */
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); /* converte IP para long */
    server.sin_family = AF_INET; /* ip v4 */
    server.sin_port = htons(8888);
    /* conecta-se ao servidor */
    if (connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        printf("Erro ao conectar-se\n");
        return 1;
    }
    printf("Conectado.\n");

    do {
        /* limpa a variável com a mensagem */
        bzero(message, sizeof(message));

        /* envia dados */
        printf("Digite uma mensagem: ");

        int ch, n = 0;

        /* lê a entrada de dados do usuário via getchar */
        while ((ch = getchar()) != '\n' && n < 2000) {
            message[n] = ch;
            ++n;
        }
        //fgets(message, sizeof(message), stdin);

        if (send(socket_desc, message, strlen(message), 0) < 0)
        {
            printf("Erro ao enviar\n");
            return 1;
        }
        printf("Dados enviados.\n");
        /* recebe dados do servidor */
        if (recv(socket_desc, server_reply, 2000, 0) < 0)
        {
            printf("Falha no recv\n");
            return 1;
        }
        printf("Resposta recebida.\n");
        printf("%s\n", server_reply);

        /* limpa a variável de resposta */
        bzero(server_reply, sizeof(server_reply));
    } while (strcmp(message, "exit") != 0);

    /* encerra a conexão */
    close(socket_desc);
    return 0;
}