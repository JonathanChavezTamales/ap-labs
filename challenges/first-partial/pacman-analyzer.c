#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REPORT_FILE "packages_report.txt"

struct PacmanAction {
	char date[50];
	char type[20];
	int action; // 1: Installed, 2: Upgraded, 3: Removed, 4: Other
	char name[50];
};

struct PacmanPackage {
	char name[50];
	char install_date[50];
	char last_update_date[50];
	int no_updates;
	char type[50];
	char removal_date[50];
};

void analizeLog(char *logFile, char *report);

struct PacmanAction *parseLine(char *line);

void install_package(struct PacmanAction *action);

void remove_package(struct PacmanAction *action);

void upgrade_package(struct PacmanAction *action);

void print_package(struct PacmanPackage *package, FILE *fp);

void print_packages();


// Global vars
int no_packages = 0;
int alpm = 0;
int pacman = 0;
int scripttlet = 0;
struct PacmanPackage packages[10000];

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o \n");
	return 1;
    }

    if(argc == 5 && strcmp(argv[1], "-input") == 0 && strcmp(argv[3], "-report") == 0){
    	analizeLog(argv[2], argv[4]);
    }
    else{
	printf("Error: there are missing flags\n ");
	return 1;
    }

    return 0;
}

void analizeLog(char *logFile, char *report) {
    	/* Receives the path from logfile and the path for report file
	 * Parses the log file and writes a report
	 * */
	
	printf("Generating Report from: [%s] log file\n", logFile);
	
	FILE *fptr = fopen(logFile, "r");
	

	if(fptr == NULL){
		printf("Log file non existent \n");
		exit(1);
	}

	char line[500];
		
	while (fgets(line, sizeof(line), fptr)) {
		struct PacmanAction *action = parseLine(line);

		if(action == NULL) continue;
		
		if(action->action == 1){ // installed
			install_package(action);
		} 
		else if(action->action == 2){ // upgraded
			upgrade_package(action);
		} 
		else if(action->action == 3){ // removed
			remove_package(action);
		}

		// Collect stats for later

		if(strcmp(action->type, "ALPM") == 0) alpm++;
		if(strcmp(action->type, "ALPM-SCRIPTLET") == 0) scripttlet++;
		if(strcmp(action->type, "PACMAN") == 0) pacman++;
		free(action);
    	}

	fclose(fptr);

	print_packages(report);

	printf("Report is generated at: [%s]\n", report);
}



struct PacmanAction *parseLine(char *line){
	struct PacmanAction *result = malloc(sizeof(struct PacmanAction));

	// If line does not starts with a date, returns null action
	if(line[0] != '[') return NULL;
	
	int closing_bracket = 1;

	// Parse the date
	for(int i=1; i<strlen(line); i++){
		if(line[i] == ']'){
			closing_bracket = i;
			break;
		}

		result->date[i-1] = line[i];	
	}

	
	// Parse the type
	memset(result->type,0,strlen(result->type));
	for(int i=closing_bracket+3; i<strlen(line); i++){
		if(line[i] == ']'){
			closing_bracket = i;
			break;
		}	

		result->type[i-closing_bracket-3] = line[i];
	}


	// Parse the action
	char action[50];
	memset(action, 0, strlen(action));
	for(int i=closing_bracket+2; i<closing_bracket+8; i++){
		if(line[i] == ' '){
			closing_bracket = i;
			break;
		}
		
		action[i-closing_bracket-2] = line[i];
	}
	
	if(strcmp(action, "instal") == 0) result->action = 1;
	else if(strcmp(action, "upgrad") == 0) result->action = 2;
	else if(strcmp(action, "remove") == 0) result->action = 3;
	else result->action = 4;

	if(result->action != 4){
		// Parse the name
		closing_bracket += 2;
		memset(result->name, 0, strlen(result->name));
		// Skip until whitespace
		for(int i=closing_bracket; i<strlen(line); i++){
			if(line[i] == ' '){
				closing_bracket = i;
				break;
			}
		}

		// Now grab the name
		for(int i=closing_bracket+1; i<strlen(line); i++){
			if(line[i] == ' '){
				closing_bracket = i;
				break;
			}	
			result->name[i-closing_bracket-1] = line[i];
		}
	}

	return result;
}

void install_package(struct PacmanAction *action){
	if(action->action != 1) return;

	// Assumes it can only be installed once
	
	struct PacmanPackage package;

	strcpy(package.install_date, action->date);
	strcpy(package.name, action->name);
	strcpy(package.type, action->type);
	strcpy(package.last_update_date, "-");
	package.no_updates = 0;
	strcpy(package.removal_date, "-");
	
	packages[no_packages] = package;
	no_packages++;
}

void upgrade_package(struct PacmanAction *action){
	if(action->action != 2) return;

	// Assumes package has been installed previously
	for(int i=0; i<no_packages; i++){
		if(strcmp(action->name, packages[i].name) == 0){
			packages[i].no_updates++;
			strcpy(packages[i].last_update_date, action->date);
			return;
		}
	}
}

void remove_package(struct PacmanAction *action){
	if(action->action != 3) return;

	// Assumes package has been installed previously
	for(int i=0; i<no_packages; i++){
		if(strcmp(action->name, packages[i].name) == 0){
			strcpy(packages[i].removal_date, action->date);
			return;
		}
	}

}

void print_package(struct PacmanPackage *package, FILE *fp){
	fprintf(fp, "- Package Name\t: %s\n", package->name);
	fprintf(fp, "  - Install date\t: %s\n", package->install_date);
	fprintf(fp, "  - Last update date\t: %s\n", package->last_update_date);
	fprintf(fp, "  - How many updates\t: %i\n", package->no_updates);
	fprintf(fp, "  - Removal date\t: %s\n", package->removal_date);
}

void print_packages(char *report){

	FILE *fp = fopen(report, "w");

	int removed = 0;
	int upgraded = 0;

	// Preprocess stats
	for(int i=0; i<no_packages; i++){
		if(strcmp(packages[i].removal_date, "-") != 0) removed++;
		if(strcmp(packages[i].last_update_date, "-") != 0) upgraded++;
	}	

	// Print stats and packages

	fprintf(fp, "Pacman Packages Report\n");
	fprintf(fp, "----------------------\n");
	fprintf(fp, "- Installed packages\t: %i\n", no_packages);
	fprintf(fp, "- Removed packages\t: %i\n", removed);
	fprintf(fp, "- Upgraded packages\t: %i\n", upgraded);
	fprintf(fp, "- Current installed\t: %i\n", no_packages-removed);

	fprintf(fp, "----------------------\nGeneral Stats\n----------------------\n");
	fprintf(fp, "- Oldest package\t\t: %s\n", packages[0].name);
	fprintf(fp, "- Newest package\t\t: %s\n", packages[no_packages-1].name);
	fprintf(fp, "- Package with no updates\t:");
	for(int i=0; i<no_packages; i++){
		if(packages[i].no_updates == 0) fprintf(fp, "%s, ", packages[i].name);
	}
	fprintf(fp, "\n- [ALPM-SCRIPTTLET] type count\t: %i\n", scripttlet);
	fprintf(fp, "- [ALPM] count\t\t\t: %i\n", alpm);
	fprintf(fp, "- [PACMAN] count\t\t: %i\n", pacman);

	fprintf(fp, "----------------\nList of packages\n----------------\n");
	for(int i=0; i<no_packages; i++){
		print_package(&packages[i], fp);
	}

	fclose(fp);
}
