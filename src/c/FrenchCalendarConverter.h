#pragma once

//extern const int vertu; //last full french month
//extern const int revolution; //french leap day

//extern const char* monthNames[];

//extern const char* dayNames[];

bool IsFrenchLeapYear(int year);

int DaysInFrenchYear(int year);

int DaysInFrenchMonth(int year, int month);

char* GetMonthName(int month);

char* GetDayName(int dayOfWeek);

int ToRoman(char *out, int n);

char * GetRomanizedYear(int year);