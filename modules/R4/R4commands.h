//R4commands.h

typedef struct alarm
{
    char alarmName[20];      // Name of the alarm
    int alarmTime;			 // Time for the alarm
    struct alarm *nextAlarm;       // Pointer to the next alarm in the queue.
    struct alarm *prevAlarm;       // Pointer to the previous alarm in the queue.
} alarm;

typedef struct alarmList
{
    int count; //Keeps track of how many alarms are in the queue.
    alarm *head; //Points to the alarm at the head(beginning/top) of the queue.
    alarm *tail; //Points to the alarm at the tail(end/bottom) of the queue.

} alarmList;

void alarmPCB();

void infinitePCB();

void infiniteFunc();

void allocateAlarmQueue();

alarm* allocateAlarms();

alarmList *getAlarms();

void addAlarm();

int convertTime(char* hours, char* minutes, char* seconds);

void iterateAlarms();