#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "/usr/include/mysql/mysql.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 1024
#define PORT 11100
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "jung!@"
#define DB_NAME "Ai214"

void Error_Handling(char *);
void Recive_File();
int Mysql_server(char *, char *);
int Move_File(char *);

int main()
{
    Recive_File();
    return 0;
}

void Recive_File()
{
    int server_sd, client_sd;
    char buf[BUF_SIZE];
    int read_cnt;
    int server_file, client_file;
    
    char file[20];
    char size[11];
    char file_extension[5] = ".jpg";
    char name[BUF_SIZE];
    struct sockaddr_in server_adr;
    struct sockaddr_in client_adr;
    int option = 1;
    socklen_t clnt_adr_sz;
 
    FILE * fp;
    printf("hello\n");
    server_sd = socket(PF_INET, SOCK_STREAM, 0);

    if(server_sd == -1)
        Error_Handling("socket() error");
        
    memset(&server_adr, 0, sizeof(server_adr));

    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adr.sin_port = htons(PORT);
    
    if(bind(server_sd, (struct sockaddr *)&server_adr, sizeof(server_adr)) == -1)
    {
        Error_Handling("bind() : error");
        setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    }
    if(listen(server_sd, 5) == -1)
        Error_Handling("listen() : error");
    
    while(1)
    {
    clnt_adr_sz = sizeof(client_adr);
    client_sd = accept(server_sd, (struct sockaddr *)&client_adr, &clnt_adr_sz);

    if(client_sd == -1)
        Error_Handling("accept() : error");
    else
        printf("Connected client\n");
 
    read(client_sd, name, 20); // 이름 받기
    read(client_sd, size, 11); // 사이즈 받기
    printf("name: %s\n", name);
    file[0] = '\0';
    strcat(file, name);
    strcat(file, file_extension);
    printf("FILE : %s\n", file);
    printf("Size : %s\n", size);
    fp = fopen(file, "wb");

    while((read_cnt = read(client_sd, buf, BUF_SIZE)) != 0)
    {
        fwrite((void*)buf, 1, read_cnt, fp);
    }
 
    puts("Recieved file data");
    Move_File(file);
    Mysql_server(file, size);    
    fclose(fp);
    close(client_sd);
    }
    close(server_sd);
}

void Error_Handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int Mysql_server(char * FileName, char * Size)
{
    MYSQL * connection = NULL, conn;
    MYSQL_RES * sql_result;
    MYSQL_ROW sql_row;

    int query_stat;
    char query[200];
    char imgURL[512];
    char file_path[100] = "/home/jung/";
    char URL[] = "http://192.168.0.3:10100/upload/";
    mysql_init(&conn);

    connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS,
            DB_NAME, 3306, (char *)NULL, 0);

    if(connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 1;
    }

    printf("DB connect\n");
    query_stat = mysql_query(connection, "select * from picture");

    if(query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    printf("Table connect\n");

    sql_result = mysql_store_result(connection);

    while((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {}
    mysql_free_result(sql_result);

    imgURL[0] = '\0';
    strcat(imgURL, URL);
    strcat(imgURL, FileName);
    
    printf("File : %s\n", FileName);
    printf("URL  : %s\n", imgURL);
    printf("Size : %s\n", Size);

    sprintf(query, "insert into picture values "
            "('', '%s', '%s', '%s')", FileName, imgURL, Size);

    query_stat = mysql_query(connection, query);

    printf("query_stat : %d\n", query_stat);

    if(query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }

    mysql_close(connection);
}

int Move_File(char * file)
{
    char oldname[20];
    char newname[40];
    char newpath[] = "/home/jung/Ai/upload";

    struct stat  st;
    int isDir = 0;
    
    oldname[0] = '\0';
    newname[0] = '\0';

    strcpy(oldname, file);
    sprintf(newname, "%s/%s", newpath, oldname);

    int fl;
    fl = stat(newpath, &st);
    printf("stat output : %d\n", fl);

    if(fl == -1)
    {
        printf("error : %s\n", strerror(errno));

        if(errno == ENOENT)
        {
            mkdir(newpath, 0755);
            printf("make : %s\n", newpath);
        }
    }
    fl = rename(oldname, newname);

    if(fl == -1)
    {
        if(errno == 2)
            printf("No such file or directory\n");
        printf("error..NO. %d\n", errno);

        return 0;
    }
    else
    {
        printf("%s --> %s\n", oldname, newpath);
        printf("%s\n", newname);
    }
    return 0;
}
