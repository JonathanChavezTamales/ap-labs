#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);

char *month_day_str(int month);

char *day_ord(int day);

int main(int argc, char ** argv) {
    	if(argc != 3){
		printf("Missing args - Usage: ./month_day <year> <yearday>\n");
		return 1;
    	}

    	int year = atoi(argv[1]);
    	int yearday = atoi(argv[2]);
	
	if(yearday > 365){
		printf("Max yearday is 365 (leap years not supported)\n");
		return 1;
	}

	int month;
	int month_d;

	month_day(year, yearday, &month, &month_d);
	
	printf("%s %i%s, %i\n", month_day_str(month), month_d, day_ord(month_d), year);
}

char *day_ord(int day){
	if(day == 1) return "st";
	if(day == 2) return "nd";
	if(day == 3) return "rd";
	else return "th";
}

char *month_day_str(int month){
	switch(month){
		case 1: return "Jan";
		case 2: return "Feb";
		case 3: return "Mar";
		case 4: return "Apr";
		case 5: return "May";
		case 6: return "Jun";
		case 7: return "Jul";
		case 8: return "Aug";
		case 9: return "Sep";
		case 10: return "Oct";
		case 11: return "Nov";
		case 12: return "Dec";
	}

	return "";
}

void month_day(int year, int yearday, int *pmonth, int *pday){
	if(yearday <= 31){*pmonth = 1; *pday = yearday%32;}
	else if(yearday <= 59){ *pmonth = 2; *pday = (yearday-31)%29;}
	else if(yearday <= 90){ *pmonth = 3; *pday = (yearday-59)%32;}
	else if(yearday <= 120){ *pmonth = 4; *pday = (yearday-90)%31;}
	else if(yearday <= 151){ *pmonth = 5; *pday = (yearday-120)%32;}
	else if(yearday <= 181){ *pmonth = 6; *pday = (yearday-151)%31;}
	else if(yearday <= 212){ *pmonth = 7; *pday = (yearday-181)%32;}
	else if(yearday <= 243){ *pmonth = 8; *pday = (yearday-212)%32;}
	else if(yearday <= 273){ *pmonth = 9; *pday = (yearday-243)%31;}
	else if(yearday <= 304){ *pmonth = 10; *pday = (yearday-273)%32;}
	else if(yearday <= 334){ *pmonth = 11; *pday = (yearday-304)%31;}
	else if(yearday <= 365){ *pmonth = 12; *pday = (yearday-334)%32;}
}
