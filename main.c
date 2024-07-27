#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EMPLOYEES 100
#define PASSWORD_LENGTH 24
#define FILENAME "employees.dat"

typedef struct {
    int id;
    char fullName[25];
    float basicSalary;
    char checkInTime1[80];
    char checkOutTime1[80];
  //  int timeWork;
    int checkInTime;
    int checkOutTime;
} Employee;

typedef struct {
    char password[PASSWORD_LENGTH + 1];	// password maximum 24 characters
} Admin;


// Step 2: Function Declarations
// Declare functions for each feature mentioned.


// Admin functions
void changeAdminPassword(Admin *admin);
void addEmployee(Employee employees[], int *employeeCount);
void viewAllEmployees(const Employee employees[], int employeeCount);
void viewEmployeeWorkingTime(const Employee employees[], int employeeCount);
void calculateSalaries(const Employee employees[], int employeeCount);

// Normal mode functions
void checkIn(Employee employees[], int employeeCount);
void checkOut(Employee employees[], int employeeCount);

// Utility functions
void saveEmployeesToFile(const Employee employees[], int employeeCount);
void loadEmployeesFromFile(Employee employees[], int *employeeCount);
void encryptPassword(char *password);
void decryptPassword(char *password);



// Step 3: Implement the Functions
//Encrypt and Decrypt Password

void encryptPassword(char *password) {
    int length = strlen(password);
    int i;
    for (i = 0; i < length / 2; ++i) {
        char temp = password[i];
        password[i] = password[length - 1 - i];
        password[length - 1 - i] = temp;
    }
    for (i = 0; i < length; ++i) {
        password[i] += 24;
    }
}

void decryptPassword(char *password) {
	int i;
    for (i = 0; i < strlen(password); ++i) {
        password[i] -= 24;
    }
    int length = strlen(password);
    for (i = 0; i < length / 2; ++i) {
        char temp = password[i];
        password[i] = password[length - 1 - i];
        password[length - 1 - i] = temp;
    }
}

// Save and Load Employees

void saveEmployeesToFile(const Employee employees[], int employeeCount) {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(employees, sizeof(Employee), employeeCount, file);
    fclose(file);
}

void loadEmployeesFromFile(Employee employees[], int *employeeCount) {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        *employeeCount = 0;
        return;
    }
    *employeeCount = fread(employees, sizeof(Employee), MAX_EMPLOYEES, file);
    fclose(file);
}


// Change Admin Password

void changeAdminPassword(Admin *admin) {
    char newPassword[PASSWORD_LENGTH + 1];
    printf("Enter new password: ");
    scanf("%s", newPassword);
    encryptPassword(newPassword);
    strcpy(admin->password, newPassword);
    printf("Password changed successfully.\n");
}


// Add employees

void addEmployee(Employee employees[], int *employeeCount) {
    if (*employeeCount >= MAX_EMPLOYEES) {
        printf("Employee limit reached.\n");
        return;
    }
    Employee newEmployee;
    newEmployee.id = *employeeCount + 1;
    printf("Enter full name: ");
    scanf(" %[^\n]", newEmployee.fullName);
    printf("Enter basic salary (VND/hour): ");
    scanf("%f", &newEmployee.basicSalary);
    newEmployee.checkInTime = -1;
    newEmployee.checkOutTime = -1;
    employees[*employeeCount] = newEmployee;
    (*employeeCount)++;
    saveEmployeesToFile(employees, *employeeCount);
    printf("Employee added successfully.\n");
}


// View All Employees

void viewAllEmployees(const Employee employees[], int employeeCount) {
    printf("Employee List:\n");
    int i;
    for (i = 0; i < employeeCount; ++i) {
        printf("ID: %d, Name: %s, Basic Salary: %.2f\n",
               employees[i].id, employees[i].fullName, employees[i].basicSalary);
    }
}

// View Employee Working Time
void viewEmployeeWorkingTime(const Employee employees[], int employeeCount) {
    int id;
    printf("Enter employee ID: ");
    scanf("%d", &id);
    int i;
    for (i = 0; i < employeeCount; ++i) {
        if (employees[i].id == id) {
            printf("Check-in Time: %s, Check-out Time: %s\n\n", employees[i].checkInTime1, employees[i].checkOutTime1);
            return;
        }
    }
    printf("Employee not found.\n\n");
}

 // Calculate Salaries
void calculateSalaries(const Employee employees[], int employeeCount) {
	int i;
    for (i = 0; i < employeeCount; ++i) {
        int workingTime = employees[i].checkOutTime - employees[i].checkInTime;
        
 //       printf("\n%d, %d, %",employees[i].checkOutTime, employees[i].checkInTime, workingTime);
        float salary = employees[i].basicSalary * workingTime/60;
        if (workingTime > 184 * 60) {
            salary = 184 * 60 ;			// Maximum salary = monthly basical salary
        }
		else{
			float salary = employees[i].basicSalary * workingTime/60;
		}
        printf("Employee ID: %d, Salary: %.2f VND\n\n", employees[i].id, salary);
    }
}


void checkIn(Employee employees[], int employeeCount) {
	time_t currentTime;			// variable with time type
    struct tm *info;			// pointer with struct tm, format dd/mm/yy/hour... in C
	
    int id;
    printf("Enter employee ID: ");
    scanf("%d", &id);
    int i;
    
    for (i = 0; i < employeeCount; ++i) {
        if (employees[i].id == id) {
            printf("Employee: %s\n", employees[i].fullName);
            printf("Confirm check-in (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            
            if (confirm == 'y' || confirm == 'Y') {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                
				
				employees[i].checkInTime = t->tm_hour * 60 + t->tm_min;
//				printf("\n%d\n", employees[i].checkInTime);		
				                
                time(&currentTime);
  				info = localtime(&currentTime);
                
			   strftime(employees[i].checkInTime1, 80, "%x - %I:%M %p", info);	// mm/dd/yy - hh:mm AM/PM
			   printf("\n%s\n", employees[i].checkInTime1);						// Display the date and time into the screen
			   
								
                saveEmployeesToFile(employees, employeeCount);			   
			                

                printf("Check-in time recorded.\n\n");
            }
            else {
            	// do nothing
			}
            return;
        }
    }
    printf("Employee not found.\n\n");
}





// Check out

void checkOut(Employee employees[], int employeeCount) {
	time_t currentTime;			// variable with time type
    struct tm *info;				// pointer with struct tm, format dd/mm/yy/hour... in C

	
    int id;
    int i;
    printf("Enter employee ID: ");
    scanf("%d", &id);
    
    for (i = 0; i < employeeCount; ++i) {
        if (employees[i].id == id) {
            printf("Employee: %s\n", employees[i].fullName);
            printf("Confirm check-out (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                
                
                employees[i].checkOutTime = t->tm_hour * 60 + t->tm_min;
//				printf("\n%d\n", employees[i].checkOutTime);	
                
                time(&currentTime);
			   	info = localtime(&currentTime);
                
			    strftime(employees[i].checkOutTime1, 80, "%x - %I:%M %p", info);	// mm/dd/yy - hh:mm AM/PM			   
			   
			    printf("\n%s\n\n", employees[i].checkOutTime1);						// Display the date and time into the screen
                saveEmployeesToFile(employees, employeeCount);
                
                printf("Check-out time recorded.\n\n");
            }
            return;
        }
    }
    printf("Employee not found.\n\n");
}


int main() {
    Employee employees[MAX_EMPLOYEES];
    int employeeCount = 0;
    Admin admin = {"12345678"};
    encryptPassword(admin.password);

    loadEmployeesFromFile(employees, &employeeCount);

    while (1) {
        printf("Select mode:\n1. Admin Mode\n2. Normal Mode\n3. Exit\n\n");
        int mode;
        scanf("%d", &mode);
        if (mode == 1) {
            printf("Enter admin password: ");
            char password[PASSWORD_LENGTH + 1];
            scanf("%s", password);
            encryptPassword(password);
            
            if (strcmp(password, admin.password) == 0) {
                while (1) {
                    printf("Admin Mode:\n1. Change Password\n2. Add Employee\n3. View All Employees\n4. View Employee Working Time\n5. Calculate Salaries\n6. Exit\n\n");
                    int choice;
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            changeAdminPassword(&admin);
                            break;
                        case 2:
                            addEmployee(employees, &employeeCount);
                            break;
                        case 3:
                            viewAllEmployees(employees, employeeCount);
                            break;
                        case 4:
                            viewEmployeeWorkingTime(employees, employeeCount);
                            break;
                        case 5:
                            calculateSalaries(employees, employeeCount);
                            break;
                        case 6:
                            goto exit_admin_mode;
                        default:
                            printf("Invalid choice.\n\n");
                    }
                }
                exit_admin_mode:;
            } else {
                printf("Incorrect password.\n\n");
            }
        } else if (mode == 2) {
            while (1) {
                printf("Normal Mode:\n1. Check-in\n2. Check-out\n3. Exit\n\n");
                int choice;
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        checkIn(employees, employeeCount);
                        break;
                    case 2:
                        checkOut(employees, employeeCount);
                        break;
                    case 3:
                        goto exit_normal_mode;
                    default:
                        printf("Invalid choice.\n\n");
                }
            }
            exit_normal_mode:;
        } else if (mode == 3) {
            break;
        } else {
            printf("Invalid mode.\n\n");
        }
    }

    return 0;
}


