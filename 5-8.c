#include <stdio.h>

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* day_of_year: set day of year from month & day */
int day_of_year(int year, int month, int day) {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        printf("Illegal argument\n");
        return -1;
    }
    int i, leap;

    leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    if (day > daytab[leap][month]) {
        printf("Illegal argument: day\n");
        return -1;
    }
    for (i = 1; i < month; i++) 
        day += daytab[leap][i];
    return day;
}

/* month_day: set month, day from day of year */
int month_day(int year, int yearday, int* pmonth, int* pday) {
    if (year < 1 || yearday < 1) {
        printf("Illegal argument\n");
        return -1;
    }
    int i, leap;

    leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    if ((leap && yearday > 366) || (!leap && yearday > 365)) {
        printf("Illegal argument: yearday\n");
        return -1;
    }

    for (i = 1; yearday > daytab[leap][i]; i++)
        yearday -= daytab[leap][i];
    *pmonth = i;
    *pday = yearday;
}

int main(void) {
    int x = day_of_year(2020, 3, 1);
    if (x == -1) {
        return 1;
    }
    printf("%d\n", x);
    int d = 0, m = 0;
    if (month_day(2020, x, &m, &d) == -1) {
        return 1;
    }
    printf("month: %d, day: %d\n", m, d);
}