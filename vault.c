#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define GREEN "\033[42m"
#define YELLOW "\033[43m"
#define RED "\033[41m"
#define RESET "\033[0m"

int get_input(int range, char name[])
{
    int input;
    while (scanf("%d", &input) != 1 || input < range)
    {
        printf("Please enter valid %s: ", name);
        while(getchar() != '\n');
    }
    return input;
}

void admin_mode()
{
    FILE *rule = fopen("vault_config.txt", "w");
    fclose(rule);
    rule = fopen("vault_config.txt", "a");
    int length, range_min, range_max, duplicate, max_attempt, point_c, point_m, penalty;
    printf("Code Length: ");
    length = get_input(1, "length");
    printf("Digit range(min): ");
    range_min = get_input(0, "range");
    printf("Digit range(max): ");
    while (scanf("%d", &range_max) != 1 || range_max > 9 || range_max < range_min)
    {
        printf("Please enter valid range: ");
        while(getchar() != '\n');
    }
    printf("Duplicate (1 for allow, 0 for not allowed): ");
    while (scanf("%d", &duplicate) != 1 || (duplicate != 0 && duplicate != 1))
    {
        printf("Please enter 1 or 0: ");
        while(getchar() != '\n');
    }
    printf("Maximum attempt: ");
    max_attempt = get_input(1, "attempt");
    printf("Point for correct number and correct place: ");
    point_c = get_input(1, "point");
    printf("Point for correct number but wrong place: ");
    point_m = get_input(1, "point");
    printf("Penalty for wrong number: ");
    penalty = get_input(0, "penalty");
    fprintf(rule, "CODE_LENGTH=%d\n", length);
    fprintf(rule, "DIGIT_MIN=%d\n", range_min);
    fprintf(rule, "DIGIT_MAX=%d\n", range_max);
    fprintf(rule, "MAX_ATTEMPTS=%d\n", max_attempt);
    fprintf(rule, "ALLOW_DUPLICATES=%d\n", duplicate);
    fprintf(rule, "POINTS_CORRECT=%d\n", point_c);
    fprintf(rule, "POINTS_MISPLACED=%d\n", point_m);
    fprintf(rule, "PENALTY_WRONG=%d\n", penalty);
    fclose(rule);
}

int* generate_code()
{
    srand(time(NULL));
    int length, range_min, range_max, duplicate, i, j, flag, digit;
    char line[100];
    FILE *rule = fopen("vault_config.txt", "r");
    while (fgets(line, sizeof(line), rule)) 
    {
        if (sscanf(line, "CODE_LENGTH=%d", &length) == 1) continue;
        else if (sscanf(line, "DIGIT_MIN=%d", &range_min) == 1) continue;
        else if (sscanf(line, "DIGIT_MAX=%d", &range_max) == 1) continue;
        else if (sscanf(line, "ALLOW_DUPLICATES=%d", &duplicate) == 1) continue;
    }
    fclose(rule);
    int* code = malloc(length * sizeof(int));
    if (!duplicate)
    {
        for (i = 0; i < length; i++)
        {
            do
            {
                digit = (rand()%(range_max+1-range_min))+range_min;
                flag = 0;
                for (j = 0; j < i; j++)
                {
                    if (code[j] == digit)
                    {
                        flag = 1;
                        break;
                    }
                }
            }
            while (flag);
            code[i] = digit;
        }
    }
    else
    {
        for (i = 0; i < length; i++)
        {
            code[i] = (rand()%(range_max+1-range_min))+range_min;
        }
    }
    FILE *codefile = fopen("vault_code.txt", "w");
    fclose(codefile);
    codefile = fopen("vault_code.txt", "a");
    for (i = 0; i < length; i++)
    {
        fprintf(codefile, "%d", code[i]);
    }
    fclose(codefile);
    return code;
}

int* get_guess()
{
    int length, i;
    char input[100];
    FILE *rule = fopen("vault_config.txt", "r");
    fscanf(rule, "CODE_LENGTH=%d", &length);
    fclose(rule);
    int* guess = malloc(length * sizeof(int));
    while (1)
    {
        printf("Guess (%d digits): ", length);
        scanf("%s", input);

        int input_len = strlen(input);
        if (input_len != length) {
            printf("Please enter exactly %d digits.\n", length);
            continue;
        }

        int valid = 1;
        for (i = 0; i < length; i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = 0;
                break;
            }
            guess[i] = input[i] - '0';
        }

        if (!valid) {
            printf("Only digits are allowed.\n");
            continue;
        }

        break;
    }
    return guess;
}


void player_mode()
{
    int length, range_min, range_max, duplicate, max_attempt, point_c, point_m, penalty;
    char line[1000];
    FILE *rule = fopen("vault_config.txt", "r");
    while (fgets(line, sizeof(line), rule)) 
    {
        if (sscanf(line, "CODE_LENGTH=%d", &length) == 1) continue;
        else if (sscanf(line, "DIGIT_MIN=%d", &range_min) == 1) continue;
        else if (sscanf(line, "DIGIT_MAX=%d", &range_max) == 1) continue;
        else if (sscanf(line, "MAX_ATTEMPTS=%d", &max_attempt) == 1) continue;
        else if (sscanf(line, "ALLOW_DUPLICATES=%d", &duplicate) == 1) continue;
        else if (sscanf(line, "POINTS_CORRECT=%d\n", &point_c) == 1) continue;
        else if (sscanf(line, "POINTS_MISPLACED=%d\n", &point_m) == 1) continue;
        else if (sscanf(line, "PENALTY_WRONG=%d\n", &penalty) == 1) continue;
    }
    fclose(rule);
    int* code = generate_code();
    int i, j, flag = 0, win_counter = 0, attempt_counter = 0, point = 0;
    FILE *gamelog = fopen("game_log.txt", "a");
    fprintf(gamelog, "Secret Code: ");
    for (i = 0; i < length; i++)
    {
        fprintf(gamelog, "%d", code[i]);
    }
    fprintf(gamelog, "\nCode Length: %d\n", length);
    fprintf(gamelog, "Digit Range: %d-%d\n", range_min, range_max);
    fprintf(gamelog, "Duplicates Allowed: %d\n", duplicate);
    fprintf(gamelog, "Max Attempts: %d\n\n", max_attempt);
    fclose(gamelog);
    while (attempt_counter < max_attempt && win_counter < length)
    {   
        gamelog = fopen("game_log.txt", "a");
        int* guess = get_guess();
        win_counter = 0;
        attempt_counter++;
        printf("Attempt %d: ", attempt_counter);
        fprintf(gamelog, "Attempt %d: ", attempt_counter);
        for (i = 0; i < length; i++)
        {
            printf("%d", guess[i]);
            fprintf(gamelog, "%d", guess[i]);
        }
        printf(" => Feedback: ");
        fprintf(gamelog, " => Feedback: ");
        for (i = 0; i < length; i++)
        {
            if (code[i] == guess[i])
            {
                win_counter++;
                printf(GREEN"C"RESET" ");
                fprintf(gamelog, "C ");
                point += point_c;
            }
            else
            {
                for (j = 0; j < length; j++)
                {
                    flag = 0;
                    if (j != i)
                    {
                        if (guess[i] == code[j])
                        {
                            flag = 1;
                            break;
                        }
                    }
                }
                if (flag)
                {
                    point += point_m;
                    printf(YELLOW"M"RESET" ");
                    fprintf(gamelog, "M ");
                }
                else
                {
                    point -= penalty;
                    printf(RED"W"RESET" ");
                    fprintf(gamelog, "W ");
                }
            }
        }
        printf(" | Score: %d\n", point);
        fprintf(gamelog, " | Score: %d\n", point);
        fclose(gamelog);
        free(guess);
    }
    if (attempt_counter == max_attempt && win_counter < length)
    {
        printf("You lost! Game over!");
    }
    else if(win_counter == length)
    {
        printf("Well done! You win!");
    }
    free(code);
}


int main()
{
    FILE *gamelog = fopen("game_log.txt", "w");
    char input;
    printf("Admin mode or player mode: ");
    scanf("%c", &input);
    if (input == 'a' || input == 'A')
    {
        admin_mode();
    }
    else if (input == 'p' || input == 'P')
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char date_str[200];
        strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", t);
        fprintf(gamelog, "--- Vault Codebreaker Game Log ---\n");
        fprintf(gamelog, "Game date: %s\n", date_str);
        fclose(gamelog);
        player_mode();
    }
    return 0;
}