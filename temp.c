#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ifttt.h"
#include <unistd.h>

double getTemp(const char *path);
void sendRequest(double low, double now, double high);
int main(int argc, char const *argv[])
{

    /* printf("num of argc: %d\n", argc); */
    char sensorId[150];

    if (argc != 2)
    {
        perror("No argument provided");
        exit(1);
    }
    else
    {
        strcpy(sensorId, argv[1]);
    }

    char path[150] = "/sys/bus/w1/devices/";
    char endPath[150] = "/w1_slave";
    strcat(path, sensorId);
    strcat(path, endPath);

    double tempnow = getTemp(path) / 1000; /* Get initial temp for now and high */

    double templow = tempnow;
    double temphigh = tempnow;
    double lastsent = 0.000;
    int firstsent = 0;

    while (1)
    {
    
        if (firstsent == 0)
        { /* First ever temp */
            firstsent = 1;
            sendRequest(templow, tempnow, temphigh);
            printf("Initial IFTTT sent\n");
            lastsent = tempnow;

        }

        if (tempnow > temphigh)
        {
            temphigh = tempnow;
        }
        if (tempnow < templow)
        {
            templow = tempnow;
        }

        int diff = tempnow - lastsent;
        if (diff != 0)
        {
            lastsent = tempnow;
            sendRequest(templow, tempnow, temphigh);
        }

        printf("Low: %.3f | Now: %.3f | High: %.3f || diff: %d\n",templow, tempnow, temphigh,diff);

        sleep(1);
        tempnow = getTemp(path) / 1000;
    }
}

void sendRequest(double low, double now, double high)
{
    char l[20];
    char n[20];
    char h[20];

    sprintf(l, "Lowest: %.3f", low);
    sprintf(n, "Now: %.3f", now);
    sprintf(h, "Highest: %.3f", high);

    ifttt("https://maker.ifttt.com/trigger/temperature/with/key/FWxYz32sQnIcKq1b4xIYC", l, n, h); 
    printf("IFTTT Sent!\n");
}

/* 28-02131330c7aa */

/* Takes in path as the input and check if it exists and extracts the temp value from the file */
double getTemp(const char *path)
{

    /* printf("%s\n",path); */
    int c;
    FILE *file;
    char tmp[512];
    file = fopen(path, "r");
    if (file == NULL)
    {
        perror("");
        exit(1);
    }

    while (!feof(file))
    {
        fgets(tmp, 10, file);
    }
    /* printf("%s\n", tmp); */
    char *t = strtok(tmp, "=");
    char *num = t + 2;
    /* printf("%s\n", num); */
    int i = atof(num);
    return i;
}
