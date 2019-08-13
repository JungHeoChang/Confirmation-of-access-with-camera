#include <iostream>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <time.h>
#include <string.h>
#include "/usr/include/mysql/mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void Ultra();
int Camera();
char * timeToString(struct tm * t);
long GetFileSize(char *);
void quick_sort(int data[], int left, int right);
void Weighted_Mean(int data[], int i);
int Error_Handling(const char *);
int File_Transmission(char *, char *);
void File_Remove(char *);
void Pir();

using namespace std;
using namespace cv;

#define trigPin 4
#define echoPin 5
#define servo 1 // gpio 18
#define PIR 26
#define CHOP(x) x[strlen(x) - 1] = ' '
#define BUF_SIZE 1024
#define IP "210.121.4.113" // 서버 아이피 210.121.4.113
                          // 학교 아이피 192.168.0.56
#define PORT 11100 

int main()
{
    wiringPiSetup();
    printf("hello\n");
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    //Ultra();
    Pir();
    return 0;
}

void Ultra()
{
    int i = 0;
    for(;;)
    {
        digitalWrite(trigPin, LOW);
        usleep(2);
        digitalWrite(trigPin, HIGH);
        usleep(20);
        digitalWrite(trigPin, LOW);

        while(digitalRead(echoPin) == LOW);

        long startTime = micros();

        while(digitalRead(echoPin) == HIGH);

        long travelTime = micros() - startTime;

        int distance = travelTime / 29. / 2. ;
        int dis_arr[10];

        dis_arr[i] = distance;
        printf("distance : %d\n", distance);
        if(i < 10)
            i++;

        if(i == 10)
        {
            quick_sort(dis_arr, 0, i-1);
            Weighted_Mean(dis_arr, i);
            i = 0;
        }
         
        delay(100);
    }
}

int Camera()
{
    time_t timer;
    struct tm * t;
    char file[100];
    file[0] = '\0';
    char file_name[15];
    char file_path[50] = "/var/www/html/upload/"; // 파일 경로 변경시 수
    char file_extension[5] = ".jpg";
    char mysql_file[512];
    char file_size[11];
    mysql_file[0] = '\0';
    timer = time(NULL);
    t = localtime(&timer);
   
    strcpy(file_name, timeToString(t));
    strcat(file, file_path);
    strcat(file, file_name);
    strcat(file, file_extension);
   
    strcat(mysql_file, file_name);
    strcat(mysql_file, file_extension);

    printf("파일 이름: %s\n", file_name);
    printf("파일 경로: %s\n", file);

    raspicam::RaspiCam_Cv cam;

    cam.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    cam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    if(!cam.open())
    {
        cerr << "Camera open failed!" << endl;
        return -1;
    }

    Mat frame;

    cam.grab();
    cam.retrieve(frame);
    cv::imwrite(file, frame);
    imshow("frame", frame);

    sprintf(file_size, "%ld", GetFileSize(file)); // 파일 사이즈
    printf("파일 사이즈 : %ld\n", GetFileSize(file));
    cam.release();
    File_Transmission(file_name, file_size);
    return 0;
}

char * timeToString(struct tm * t)
{
    static char str[50];

    sprintf(str, "%d%d%d%d%d%d",
            t -> tm_year + 1900,
            t -> tm_mon + 1,
            t -> tm_mday,
            t -> tm_hour,
            t -> tm_min,
            t -> tm_sec
           );
    return str;
}

long GetFileSize(char * file)
{
    FILE * fp = fopen(file, "r");
    long file_size;
    long fpos = 0;

    if(fp == NULL)
    {
        printf("파일을 열 수 없습니다!\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, fpos, SEEK_SET);

    return file_size;
}

void Weighted_Mean(int data[], int i)
{
    int j=0, k=0;
    int key;
    int W[10] = {0}; // 똑같은 숫자의 개수
    int num[10] = {0}; // 똑같은 수를 제외한 숫자
    int len = 1;
    int result = 0; // 가중치를 적용한 결과
    int weighted = 0; // 가중치를 곱한 값
    int sum_W = 0; // 총 가중치의 합

    key = data[0];
    num[0] = key;

    while(1)
    {
        if(key == data[j])
            W[k] += 1;
        else
        {
            key = data[j];
            num[k+1] = key;
            len++;
            j--;
            k++;
        }
        j++;

        if(j == i)
            break;
    }

    for(i=0; i < len; i++)
    {
        for(j=0; j < W[i]; j++)
        {
            weighted += (W[i] * num[i]);
        }
        sum_W += (W[i] * W[i]);
    }

    result = weighted / sum_W;
    //printf("result : %d\n", result);
    if(result < 100) // 거리에 따른 작동
        Camera();
}

void quick_sort(int data[], int left, int right)
{
    int i, j, key, temp;

    if(left < right)
    {
        i = left;
        j = right + 1;
        key = data[left];

        do
        {
            do
            {
                i++;
            }while(data[i] < key);

            do
            {
                j--;
            }while(data[j] > key);

            if(i < j)
            {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }while(i < j);

        temp = data[left];
        data[left] = data[j];
        data[j] = temp;
        quick_sort(data, left, j-1);
        quick_sort(data, j+1, right);
    }
}

int File_Transmission(char * file_name, char * size)
{
    int sd;
    FILE * fp;
    char buf[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in server_adr;
    char name[40];
    char path[] = "/var/www/html/upload/";
    char extension[] = ".jpg";
    //char size[11];
    name[0] = '\0';
    strcat(name, path);
    strcat(name, file_name);
    strcat(name, extension);
    //name = file_name;
    fp = fopen(name, "rb");
    sd = socket(PF_INET, SOCK_STREAM, 0);

    if(sd == -1)
        Error_Handling("socket() : error");

    memset(&server_adr, 0, sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = inet_addr(IP);
    server_adr.sin_port = htons(PORT);
    
    if(connect(sd, (struct sockaddr *)&server_adr, sizeof(server_adr)) < 0)
    {
        
        Error_Handling("connect() : error");
        return -1;
    }
    else
        puts("Connected");
    
    write(sd, file_name, 20);
    write(sd, size, 11);
    while(1)
    {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);

        if(read_cnt < BUF_SIZE)
        {
            write(sd, buf, read_cnt);
            break;
        }
        write(sd, buf, BUF_SIZE);
    }
    
    puts("Send file data\n");
    File_Remove(name);
    fclose(fp);
    close(sd);
    return 0;
}

int Error_Handling(const char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    return -1;    
}

void File_Remove(char * path)
{
    int Result = remove(path);

    if(Result == 0)
    {
        printf("[%s] 파일 삭제 성공\n\n", path);
    }
    else if(Result == -1)
    {
        perror("파일 삭제 실패");
    }
}

void Pir()
{
    delay(10000);
    pinMode(PIR, INPUT);

    while(1)
    {
        if(digitalRead(PIR) == HIGH)
        {
            Camera();
            delay(5000);
        }
    }
}
