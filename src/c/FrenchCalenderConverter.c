#include <pebble.h> 
#include "FrenchCalendarConverter.h"

static const int vertu = 12; //last full french month
static const int revolution = 17; //french leap day

static const char * monthNames[] = { 
  "Vendémiaire", 
  "Brumaire", 
  "Frimaire", 
  "Nivôse", 
  "Pluviôse", 
  "Ventôse", 
  "Germinal", 
  "Floréal", 
  "Prairial", 
  "Messidor", 
  "Thermidor", 
  "Fructidor",
  "Vertu", 
  "Génie", 
  "Travail", 
  "Opinion", 
  "Récompenses", 
  "Révolution"
};

static const char * dayNames[] = {
  "Primidi", 
  "Duodi",
  "Tridi", 
  "Quartidi", 
  "Quintidi", 
  "Sextidi", 
  "Septidi", 
  "Octidi", 
  "Nonidi", 
  "Decadi"
};

bool IsFrenchLeapYear(int year) {
  //obviously these years are before the unix epoch, but pasting them in here anyway for historicity
  if (year == 3 || year == 7 || year == 11 || year == 15) {
    return true;
  } else if (year < 20) {
    return false;
  } else {
    //using the Charles-Gilbert Romme rule, may he RIP in peace
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) ? true : false;
  }
}

int DaysInFrenchYear(int year) {
  return IsFrenchLeapYear(year) ? 366 : 365;
}

int DaysInFrenchMonth(int year, int month) {
  if (month < vertu) {
    return 30;
  } else if (month == revolution) {
    return (IsFrenchLeapYear(year)) ? 1 : 0;
  } else {
    return 1;
  }
}

char * GetMonthName(int month) {
  return (char * ) monthNames[month];
}

char * GetDayName(int dayOfWeek) {
  return (char * ) dayNames[dayOfWeek];
}

int ToRoman(char *out, int n)
{
        int len = 0;
        if (n <= 0) return 0; /* error indication */
#       define RPUT(c) if (out) out[len] = c; len++
        while(n>= 1000) { n -= 1000;RPUT('M'); };
 
        if (n >= 900)   { n -= 900; RPUT('C'); RPUT('M'); };
        if (n >= 500)   { n -= 500; RPUT('D'); };
        if (n >= 400)   { n -= 400; RPUT('C'); RPUT('D'); };
        while (n >= 100){ n -= 100; RPUT('C'); };
 
        if (n >= 90)    { n -= 90;  RPUT('X'); RPUT('C'); };
        if (n >= 50)    { n -= 50;  RPUT('L'); };
        if (n >= 40)    { n -= 40;  RPUT('X'); RPUT('L'); };
        while (n >= 10) { n -= 10;  RPUT('X'); };
 
        if (n >= 9)     { n -= 9;   RPUT('I'); RPUT('X'); };
        if (n >= 5)     { n -= 5;   RPUT('V'); };
        if (n >= 4)     { n -= 4;   RPUT('I'); RPUT('V'); };
        while (n)       { n--; RPUT('I'); };
        RPUT('\0');
#       undef RPUT
 
        return len;
}

char * GetRomanizedYear(int year) {
  char * numeralYear =  (char*)malloc(16 * sizeof(char));
  ToRoman(numeralYear,year);
  return numeralYear;
}