// *
// ------------------------------------------------------------------------------------------------------------------------------

// CS 5348 Operating Systems (Project 1 - Build a simple Unix Shell)
// Instructor: Prof. Sridhar Alagar

// ------------------------------------------------------------------------------------------------------------------------------


// The functions implemented in this program are (THE COMMENTS WILL HELP UNDERSTAND THE FLOW OF THE CODE):

    
// 1. Tokeinzer: When a input string is passed, the function parses the input string into tokens 
//    using "SPACE" as delimiter and returns a variable 'i', which gives us the information about 
//    the number of arguments being passed in the input string. 
       

// 2. getredirection: When we pass a character pointer to this function, this function tokenizes the 
//    string using ">" delimiter and return the variable where we happen to find the ">" symbol.

// 3. TrailChecker: Removes the spaces between the input commands from the user, and return "i" to 
//    a variable which gives us information about the number of arguments passed in the function
//    that finally used in the find out the errors.

// 4. CheckPath: Take a double pointer array and a variable which in turn used to check the access of the
//    commands entered by the user, and, using access system call we could be able to find them out. If the
//    the access turns to be there, then it will execute it using "EXECV" system call.

// 5. FindPath: FindPath used to store the path entered by th user.

// 6. LoopFunc: Is called from the main function, which is used to execute parallel commands and batch mode 
     
// ------------------------------------------------------------------------------------------------------------------------------

// Team member contributions (though all of us worked together for all the modules, main contributions have been listed below): 

// 1) Yaswanth Adari

// 2) Surya Siva Pranav Jakka

// ------------------------------------------------------------------------------------------------------------------------------

// How to execute the code? (csgrads1 server)

// 1. Compilation :-$ gcc -o a dash.c 
// 2. Running the code :- $ ./a
// 3. "Dash> " will be reached
// 4. all the commands can be tested in this shell

// TYPES of modes available:

//1. Interactive Mode  - using "dash" executable we can run the interactive mode by passing on commands one by one.
//2. Batch Mode - Program was made in such a way that the if the commands are entered in a batch way, we can if check and execute
//    all at once using (argc, argv).
// ------------------------------------------------------------------------------------------------------------------------------

//header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// global variables
char* c[20]; // declaring a array to store the path here.
char* temp3[20];
char* args[100];
char* copy[100];
int checkrdr; // declaring a variable here to return if there is a redirection.
char* l3; // declaring a variable here to store the redirection output file.
char error_message[30] = "An error has occurred\n";

//Functions
int tokeinzer(char* array) // Description(.1)
{
    int i =0;
    while (1) 
    {
    char *t = strtok(array, " \n");
    array = NULL;
    if (t == NULL)
       break;
    args[i] = t;
    i++;
    }
    args[i] = NULL;
    return i;
}
char* getredirection(char* strng) // Description(.2)
{
    char *new;
    new = strtok(strng, ">");
    new = strtok(NULL, ">");
    if(new!=NULL)
    {
        checkrdr = 1;
    }
    return new;  
}
int TrailChecker(char* strng) // Description(.3)
{
   int i = 0;
   char* parse  = strtok(strng, " ");
   while(parse != NULL)
   {
     parse = strtok(NULL, " ");
     i++;
   }
   return i;
}
void CheckPath(char* pathchecker[], int data2) // Description(.4)
{
    int q = 0;
    char temp4[30];
    //pathchecker[1] = NULL;
    while(1)
    {
        if(pathchecker[q] == NULL)
        {
            break;
        }
        else
        {
            int id1 = fork(); // creating a process using fork system call
            if(id1 == 0)
            {   
            strcpy(temp4, c[q]);
            strcat(temp4, "/");
            strcat(temp4, pathchecker[0]);
            int acc = access(temp4, X_OK);
            if(acc == 0)
            {
                if(checkrdr == 1)
                {
                  int fd;
                  fd = open(l3, O_TRUNC | O_CREAT | O_RDWR, S_IRWXU); // fd stores the file descriptor value, O_TRUNC overwrites a given file, O_CREAT creates file if filename doesn't exist
                  dup2(fd, STDOUT_FILENO);
                  dup2(fd, STDERR_FILENO);
                  close(fd);
                }
                execv(temp4, pathchecker);
            } 
            else
            {
                write(STDERR_FILENO, error_message, strlen(error_message)); //prints an error message
            }
            }
            else
            {
                wait(NULL);
                exit(0);
            }
        }
        q++;
    }
}
void findpath(char* d[], int data3) // Description(.5)
{
   int j=0;
   while(j<data3)
   {
    if(d[j] ==  NULL)
    {
        c[j] = NULL;
        break;
    }
    else 
    {
      c[j] =(char *) malloc(strlen(d[j]) + 1);
      strcpy(c[j], d[j]);
    } 
    j++;
   }
}
int LoopFunc(char *sting) // Description(.6)
{
    char *l2 =  strdup(sting);
    l3 = getredirection(l2);
    char *arr = sting;
    int i = tokeinzer(arr);
    if(checkrdr == 1)
    {
        args[checkrdr] = NULL;
        int space = TrailChecker(l3);
        if(space > 1)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));   //prints an error message
            return 0;
        }
    }       
    if(strcmp(args[0], "cd") == 0 || strcmp(args[0], "exit") == 0 || strcmp(args[0], "path") == 0 )
        {
           if(strcmp("cd",args[0]) == 0 || strcmp("cd\n", args[0]) == 0) 
           {
            char* directory = args[1];
            if(i == 1)
            {
                write(STDERR_FILENO, error_message, strlen(error_message)); //prints an error message
            }
            else if(i == 2)
            {
               int ch = chdir(directory); // to check whether we could able to access the directory or even if it exists or not.
               if(ch == -1) 
               {
                   write(STDERR_FILENO, error_message, strlen(error_message)); //prints an error message
               }
            }
            else
            {
               write(STDERR_FILENO, error_message, strlen(error_message)); //prints an error message
            }
           }
         else if(strcmp("exit", args[0]) == 0 || strcmp("exit\n", args[0]) == 0)
          {
            if(i == 1)
            {
                exit(0);
            }
            else
            {
                write(STDERR_FILENO, error_message, strlen(error_message)); //prints an error message
            }
          }
          else if(strcmp("path", args[0]) == 0 || strcmp("path\n", args[0]) == 0 )
          {
            findpath(&args[1], i);
          }
        }
    else 
    {
        int proc2 = fork(); // creating a process using fork system call
        if(proc2 == 0)
        {
            CheckPath(args, i);
        }
        else
        {
            wait(NULL);

        }
    }  
  return 0;
}

//main function
int main(int argc, char** argv)
{
    if(argc > 2)
    {
        write(STDERR_FILENO, error_message, strlen(error_message)); //prints an error message
        exit(0);
    }

    if(argc == 1) //INTERACTIVE MODE
    {
    while(1)
    {
        char a[]= "dash> ";
        printf("%s", a);
        char *l = NULL;
        size_t len =0;
        ssize_t lsize =0;
        lsize = getline(&l, &len, stdin); //here getline function is used to receive input from the user.
        if(lsize == -1)
        {
            exit(0);
        }
        char *temp = l;
        char *str;
        int parallel_cmds;
        char *f[0];
        f[0] = "/bin";  
        findpath(f, 1);
        for(parallel_cmds = 0; (str = strtok_r(temp, "&", &temp)); parallel_cmds++) // loop to check for parallel commands, here we are tokenizing string using '&' delimiter and sending each command indiviually to a function to execute the function
        {
            if(parallel_cmds == 0)
            {
                int check = LoopFunc(str);
                if(check == -1)
                {
                    exit(0);
                }
            }
            else
            {
               int proc = fork(); // creating a process using fork system call
               if(proc == 0)
               {
                 LoopFunc(str);
                 exit(0);
               }
            }
        }
        if(parallel_cmds > 1)
        {
            while(parallel_cmds > 0)
            {
                wait(NULL); 
                parallel_cmds = parallel_cmds - 1;
            }
        }
    }

    }
    
    else if(argc == 2) // checking the arguments if it is batch mode or not. ((BATCH MODE))
    {
        char *l = NULL;
        size_t len =0;
        ssize_t lsize =0;
        FILE *input = stdin;
        if(argc == 2)
        {
            input = fopen(argv[1], "r");
        }
        lsize = getline(&l, &len, input);
        if(lsize == -1)
        {
            exit(0);
        }
        char *temp = l;
        char *str;
        int parallel_cmds;
        char *f[0];
        f[0] = "/bin";  
        findpath(f, 1);
        for(parallel_cmds = 0; (str = strtok_r(temp, "&", &temp)); parallel_cmds++) // loop to check for parallel commands, here we are tokenizing string using '&' delimiter and sending each command indiviually to a function to execute the function
        {
            if(parallel_cmds == 0)
            {
                int check = LoopFunc(str);
                if(check == -1)
                {
                    exit(0);
                }
            }
            else
            {
               int proc = fork(); // creating a process using fork system call
               if(proc == 0) // CHILD PROCESS
               {
                 LoopFunc(str);
                 exit(0);
               }
            }
        }
        if(parallel_cmds > 1)
        {
            while(parallel_cmds > 0)
            {
                wait(NULL);
                parallel_cmds = parallel_cmds - 1;
            }
        }
        
    }
    return 0;
}
