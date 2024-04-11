/*
 * Program 3
 * Abhi
 * OS
 * Mario bros find valves using semaphores!
 */
#include <stdio.h>

#include<stdio.h>
#include<string.h>
#include <malloc.h>
#include<unistd.h>
#include <time.h>
#include <pthread.h>
#include "semaphore.h"

sem_t findvalve;
int doneCount = 0;
int numValve=37;

struct Valve{ //valve struct
    int avail;
    int complexity;
    int num;
};

struct Valve ValveList[35]; //35 valves

int *openValve(void * who[14]){
    while (doneCount<34) { //while not all valves open
        sem_wait(&findvalve); //make sure only one char finds a valve at a time to avoid two chars working on same valve

        srand(time(NULL));
        numValve = (rand() % 35); //rand num of valve

        struct Valve* curValve = &ValveList[numValve]; //make var for valve

        while (curValve->complexity == 0 ||  ValveList[numValve].avail ==0){ //continute choosing valve til you get one which hasnt been completed and isnt being worked on
            if (doneCount==35){
                break;
            };
            numValve = (rand() % 35);
            curValve = &ValveList[(numValve)];
        }if (doneCount==35){
            break;
        };

        curValve->avail = 0; //chose valve, make it unavailable

        sem_post(&findvalve); //let another char choose a valve

        while(curValve->complexity>0){ //work at opening valve every sec til complexity ==0
            curValve->complexity -= 1;
            printf("%s OPENING VALVE 0%d - Time remaining %d \n", who, curValve->num, curValve->complexity);
            sleep(1);
        };

        printf("LETS-A-GO!!!! %s JUST OPENED VALVE 0%d\n", who, curValve->num); //out of loop means valve opened
        doneCount++; //add to donecount


        sem_post(&findvalve);



    }
    return(0);

};


int main() {
    pthread_t mario,luigi,bowser,princesspeach,toad,yoshi;
    sem_init(&findvalve,0,0);

    srand(time(NULL));
    char marioList[6][13] = {"Mario","LUIGI","BOWSER","PRINCESSPEACH","TOAD","YOSHI"};
    printf("Valves Created\n");

    for (int i = 0;i<35;i++){
        ValveList[i].complexity = (rand() % 8) + 2;
        ValveList[i].num = 1+i;
        ValveList[i].avail = 1;
        printf("VALVE 0%d - %d\n", (i+1),ValveList[i].complexity);
    }
    printf("Characters hired: \n%s\n%s\n%s\n%s\n%s\n%s\n", marioList[0],marioList[1],marioList[2],marioList[3],marioList[4],marioList[5]);



    pthread_create(&mario, NULL, &openValve, (void *) "MARIO"); //clion told me to do this so i did
    pthread_create(&luigi, NULL, &openValve, (void *) "LUIGI");
    pthread_create(&bowser, NULL, &openValve, (void *) "BOWSER");
    pthread_create(&princesspeach, NULL, &openValve, (void *) "PRINCESSPEACH");
    pthread_create(&toad, NULL, &openValve, (void *) "TOAD");
    pthread_create(&yoshi, NULL, &openValve, (void *) "YOSHI");
    printf("MUSHROOM KINGDOM TIME !!!!! WOO YEAH OH YEAH WOO TIME \n");
    sem_post(&findvalve);
    pthread_join(mario, NULL);
    pthread_join(luigi, NULL);
    pthread_join(bowser, NULL);
    pthread_join(princesspeach, NULL);
    pthread_join(toad, NULL);
    pthread_join(yoshi, NULL);


    printf("all done, that'll be $500 please.");

    return 0;
}
