/*
 �����: �������� ��������� �������������, ���-241
 ��������� ��� ����������� ������ � �������� �����������.
 ������� �������� ������������ ���������:
 - ����������� ���������� �� ��������� ���������.
 - ������ ����������� ������ � ����.
 - ���������� � ���������� �����������.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <windows.h>

/* ��������� ��� ������� ������ */
#define MODE_A 0
#define MODE_B 1
#define MODE_C 2

/* ��� ����� � ������� � ������ */
#define MONITORING_FILE "monitoring.txt"

/* ��������� ��� �������� ������ ���������� */
typedef struct {
    char id[100];
    char zone[100];
    int status;
    time_t timestamp;
} SensorData;

/* ��������� ������� */

/* ������������ ������ ����������. ���������� 1 ��� �������, 0 ��� ���������� �������, -1 ��� ������.*/
int handle_sensor_data(const SensorData* data, int* sensor_count, SensorData* sensors, int mode);

/* ������������� ����� ������. ���������� ����� ����� (MODE_A, MODE_B, MODE_C). */
int set_mode(char mode_char);

/* ��������� ������� �� ������ �� �����. ���������� 0 ��� ������, -1 ��� ������. */
int test_system_from_file(const char* filename, int* sensor_count, SensorData* sensors, int mode);

/* ���������� �������� ������ � ��������� �� � ����. ���������� 0 ��� ������, -1 ��� ������. */
int generate_test_data(char* filename, SensorData* data);

/* ��������� �������, ����������� ������ �����������. ���������� 0 ��� ������, -1 ��� ������. */
int test_system(int* sensor_count, SensorData* sensors, int monitoring_enabled, int mode);

/* ���������� ������ ����������� � ����. ���������� 0 ��� �������� ������, -1 ��� ������. */
int mon_sensor_data(const SensorData* data, int mode, int monitoring_enabled);

/* ����������� ����. ���������� ���������� ��������������� ������� ��� -1 ��� ������. */
int analyze_mon_file(int filter_mode, int filter_status, const char* filter_zone, const char* filter_time);

/* ����������� ����� ������ � ������. ���������� ��������� ������������� ������. */
const char* mode_to_string(int mode);

int main() {
    int choice;
    char id[100];
    char zone[100];
    int status;
    setlocale(LC_ALL, "RUS");

    /* ���������� ��������� � �������� ������������ */
    printf("------------------------------------------------------------------------------------------------\n");
    printf("����� ���������� � ��������� ����������� ������������� ������������!\n");
    printf("������ ��������� ������������� ��� �������� � ������� ������ �������� �����������.\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("�������� ����������� ���������:\n");
    printf("1. ������ � ������ ������� ��������� ��������:\n");
    printf("   - MODE_A: ����������� ������������ �� ������������ ����������.                           \n");
    printf("   - MODE_B: �������� ���������� ������������ � ��� �� ���� � ������� 60 ������.            \n");
    printf("   - MODE_C: �������� ���������� ������������ ������ ������� ����������.                    \n");
    printf("2. ���������� ������������: ����������� ���������/���������� �����������.\n");
    printf("3. ������ ���� ������: ����������� ����� ������ ���������� �������.\n");
    printf("4. ������ ������ � ������ �������\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("\n");

    /* ������������� ��������� ������� */
    int mode = MODE_A;
    SensorData sensors[1000];
    int sensor_count = 0;
    int monitoring_enabled = -1;
    /* �������� ���� ��������� */
    while (1) {
        printf("1. ������� ����� ������ (A, B, C)\n");
        printf("2. ��������/��������� ����� �����������\n");
        printf("3. ������ �������� ����������� �������\n");
        printf("4. ������ ������ � ������ �������\n");
        printf("5. ����� �� ���������\n");
        printf("\n�������� ��������: ");
        if (scanf("%d", &choice) != 1) {
            printf("������ �����: ������� �����\n\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            /* ��������� ������ ������ ������ */
            printf("\n------------------------------------------------------------------------------------------------\n");
            printf("                                  ����� ��������� ��������                                      \n");
            printf("------------------------------------------------------------------------------------------------\n");
            printf("  0. �������� A  ����������� ������������ �� ������������ ����������.                           \n");
            printf("  1. �������� B  �������� ���������� ������������ � ��� �� ���� � ������� 60 ������.            \n");
            printf("  2. �������� C  �������� ���������� ������������ ������ ������� ����������.                    \n");
            printf("------------------------------------------------------------------------------------------------\n");
            while (1) {
                printf("\n������� ��� �����: ");
                char input[10];
                scanf("%s", input);
                char mode_char = input[0];
                mode = set_mode(mode_char);
                if (mode == -1) {
                    printf("�������� ����: ������� ��������������� ����� ��� A, B, C.\n");
                }
                else {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("                             ����� ������ ������� �� �������� %s           \n", mode_to_string(mode));
                    printf("------------------------------------------------------------------------------------------------\n");
                    break;
                }
            }
            break;
        }
        case 2:
            /* ���������/���������� ������ ����������� */
            if (monitoring_enabled == -1) {
                monitoring_enabled = 1;
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                                 ���������� �������                                              \n");
                printf("------------------------------------------------------------------------------------------------\n\n");
                /* ��������� �������� ������ � ������������ */
                if (test_system(&sensor_count, sensors, monitoring_enabled, mode) != 0) {
                    printf("������ ��� ������������ �������.\n");
                }
                break;
            }
            else {
                monitoring_enabled = -1;
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                                 ���������� ��������                                             \n");
                printf("------------------------------------------------------------------------------------------------\n");
            }
            break;
        case 3:
            /* ����� ������� ����� ������ ���������� */
            printf("\n------------------------------------------------------------------------------------------------\n");
            printf("                    ��� ������ �� ������ ������� ����� ������ ������� -1                        \n");
            printf("------------------------------------------------------------------------------------------------\n\n");
            while (1) {
                printf("������� ������ ����� ������ (��� ��������� | ������������� | ���� ��������): ");
                if (scanf("%d", &status) != 1 || status < -1 || status > 1) {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("          ��������������� ���������: ��� ��������� ��� ����������� ��������� (0-1)              \n");
                    printf("------------------------------------------------------------------------------------------------\n\n");
                    while (getchar() != '\n');
                    continue;
                }

                /* �������� �� ����� */
                if (status == -1) {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("                             ����� �� ������ ������� ����� ������                               \n");
                    printf("------------------------------------------------------------------------------------------------\n");
                    break;
                }

                if (scanf("%s %s", id, zone) != 2) {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("                                        ������ �����                                            \n");
                    printf("------------------------------------------------------------------------------------------------\n\n");
                    while (getchar() != '\n');
                    continue;
                }

                if (status >= 0 && status <= 1) {
                    SensorData data;
                    strcpy(data.id, id);
                    strcpy(data.zone, zone);
                    data.status = status;
                    data.timestamp = time(NULL);

                    int result = handle_sensor_data(&data, &sensor_count, sensors, mode);
                    if (result == 1) {
                        printf("\n------------------------------------------------------------------------------------------------\n");
                        printf("                        �������: ��������� ������� �������������!                               \n");
                        printf("------------------------------------------------------------------------------------------------\n\n");
                    }
                    else if (result == -1) {
                        printf("\n------------------------------------------------------------------------------------------------\n");
                        printf("                     ������: �� ������� ���������� ������ ����������                            \n");
                        printf("------------------------------------------------------------------------------------------------\n\n");
                    }

                    /* ������ ������, ���� ���������� ������� */
                    if (monitoring_enabled) {
                        if (mon_sensor_data(&data, mode, monitoring_enabled) != 0) {
                            perror("������ ��� ������ ������ � ���� � ������ �������.\n");
                        }
                    }
                }
            }
            break;
        case 4: {
            /* ������ ������ �� ����� */
            int filter_mode;
            int filter_status;
            char filter_zone[100];
            char filter_time[100];

            printf("\n------------------------------------------------------------------------------------------------");
            printf("\n            ������ ������ � ������ ������� (���������� � ���������� �� ���������)               ");
            printf("\n------------------------------------------------------------------------------------------------\n");

            /* ���� ��������� �������� */
            while (1) {
                printf("\n�������� �������� �������� (0 - A, 1 - B, 2 - C, -1 ��� ����): ");
                if (scanf("%d", &filter_mode) != 1 || filter_mode < -1 || filter_mode > 2) {
                    printf("������ �����: ������� ����� �� -1 �� 2.\n");
                    while (getchar() != '\n');
                    continue;
                }
                break;
            }

            /* ���� ���� ��������� */
            while (1) {
                printf("������� ��� ��������� ��� ���������� (0 ��� 1, -1 ��� ���� ��������): ");
                if (scanf("%d", &filter_status) != 1 || (filter_status != -1 && filter_status != 0 && filter_status != 1)) {
                    printf("������ �����: ������� ����� �� -1 �� 2.\n");
                    while (getchar() != '\n');
                    continue;
                }
                break;
            }

            /* ���� ���� �������� */
            while (1) {
                printf("������� ���� �������� ��� ���������� (��� 'all' ��� ���� ���): ");
                if (scanf("%s", filter_zone) != 1) {
                    printf("������ �����: ������� ������.\n");
                    while (getchar() != '\n');
                    continue;
                }
                break;
            }

            /* ���� ������� ��� ���������� */
            while (1) {
                printf("������� ���� ��� ���������� (��� 'all' ��� ����): ");
                if (scanf("%s", filter_time) != 1) {
                    printf("������ �����: ������� ������.\n");
                    while (getchar() != '\n');
                    continue;
                }
                break;
            }

            analyze_mon_file(filter_mode, filter_status, filter_zone, filter_time);
            break;
        }
        case 5: {
            exit(0);
        }
        default:
            printf("�������� ����\n");
            while (getchar() != '\n');
        }
        printf("\n");
    }
    return 0;
}

/*
 * ������������ ������ ����������.
 * data - ��������� �� ��������� SensorData � ������� ����������.
 * sensor_count - ��������� �� ���������� ����������� � ������� sensors.
 * sensors - ������ ��� �������� ������ �����������.
 * mode - ������� ����� ������ (MODE_A, MODE_B, MODE_C).
 * ����������:
 *   1 - ���� ����� ��������� ������� ������������� (�������),
 *   0 - ���� ������� ���,
 *  -1 - ��� ������.
 */
int handle_sensor_data(const SensorData* data, int* sensor_count, SensorData* sensors, int mode) {
    assert(data->status == 0 || data->status == 1);

    /* ������ ��� ������ A */
    if (mode == MODE_A) {
        if (data->status == 1) {
            return 1;
        }
    }

    /* ������ ��� ������ B */
    else if (mode == MODE_B) {
        static time_t last_trigger_time = 0;
        static char last_zone[100] = "";

        if (data->status == 1) {
            if (strcmp(data->zone, last_zone) == 0 && (data->timestamp - last_trigger_time) <= 60) {
                return 1;
            }
            last_trigger_time = data->timestamp;
            strcpy(last_zone, data->zone);
        }
    }

    /* ������ ��� ������ C */
    else if (mode == MODE_C) {
        static int first_trigger = 0;
        static char first_zone[100] = "";

        if (data->status == 1) {
            if (first_trigger == 0) {
                first_trigger = 1;
                strcpy(first_zone, data->zone);
            }
            else {
                return 1;
            }
        }
    }

    /* ���������� ������ � ������ */
    if (*sensor_count >= 1000) {
        printf("������: �������� ����� �����������.\n");
        return -1;
    }
    sensors[*sensor_count] = *data;
    (*sensor_count)++;

    return 0;
}

/*
 * ������������� ����� ������ �������.
 * mode_char - ������, ����������� ����� (A, B, C).
 * ���������� ����� ����� (MODE_A, MODE_B, MODE_C) ��� -1 ��� ������.
 */
int set_mode(char mode_char) {
    switch (mode_char) {
    case 'A':
    case 'a':
    case '0':
        return MODE_A;
    case 'B':
    case 'b':
    case '1':
        return MODE_B;
    case 'C':
    case 'c':
    case '2':
        return MODE_C;
    default:
        return -1;
    }
}

/*
 * ���������� �������� ������ � ��������� � ����.
 * filename - ��� ����� ��� ����������.
 * data - ��������� � ������� �������.
 * ���������� 0 ��� ������, -1 ��� ������.
 */
int generate_test_data(char* filename, SensorData* data) {
    /* ��������� ��������� ������ */
    data->status = rand() % 2;
    int id_number = 1 + rand() % 5;
    int zone_number = 1 + rand() % 10;
    snprintf(data->id, sizeof(data->id), "ID_%d", id_number);
    snprintf(data->zone, sizeof(data->zone), "ZONE_%d", zone_number);
    data->timestamp = time(NULL);

    /* ��������� ����� ����� */
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(filename, 100, "%02d.%02d.%02d_%02d.%02d.%02d.txt",
        t->tm_mday, t->tm_mon + 1, t->tm_year - 100, t->tm_hour, t->tm_min, t->tm_sec);

    /* ������ � ���� */
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("������ �������� ����� ��� ������ �������� ������");
        return -1;
    }
    fprintf(file, "%d %s %s\n", data->status, data->id, data->zone);
    fclose(file);

    return 0;
}

/*
 * ��������� ������� �� ������ ��������������� ������ � ���������� ����������.
 * sensor_count - ���������� ��������.
 * sensors - ������ ��������.
 * monitoring_enabled - ���� ��������� �����������.
 * mode - ����� ������.
 * ���������� 0 ��� ������, -1 ��� ������.
 */
int test_system(int* sensor_count, SensorData* sensors, int monitoring_enabled, int mode) {
    /* ��������� ����� ����� ��� ����������� */
    char result_filename[100];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(result_filename, sizeof(result_filename), "result_mode_%c_%02d.%02d.%02d_%02d.%02d.%02d.txt",
        mode_to_string(mode)[0], t->tm_mday, t->tm_mon + 1, t->tm_year - 100, t->tm_hour, t->tm_min, t->tm_sec);

    /* �������� ����� ��� ������ ����������� */
    FILE* result_file = fopen(result_filename, "w");
    if (!result_file) {
        perror("������ �������� ����� ��� ������ ����������� ������������");
        return -1;
    }

    /* ������ ��������� */
    fprintf(result_file, "------------------------------------------------------------------------------------------------\n");
    fprintf(result_file, "                     ������������ ������� �� ��������������� ������                           \n");
    fprintf(result_file, "------------------------------------------------------------------------------------------------\n");
    fprintf(result_file, "\t��� ��������� | ������������� | ���� �������� | ������������ �������\n");
    fprintf(result_file, "------------------------------------------------------------------------------------------------\n");

    while (monitoring_enabled != -1) {
        for (int i = 0; i < 10; i++) {
            Sleep(2000 + rand() % 2000);

            SensorData data;
            char test_filename[100];

            if (generate_test_data(test_filename, &data) != 0) {
                fclose(result_file);
                return -1;
            }

            int result = handle_sensor_data(&data, sensor_count, sensors, mode);
            fprintf(result_file, "\t\t%d\t%s\t\t%s\t\t\t%d\n", data.status, data.id, data.zone, result);

            if (monitoring_enabled) {
                mon_sensor_data(&data, mode, monitoring_enabled);
            }

            printf("��������� ������ � ��������� ����������: %s\n", test_filename);

            if (result == 1) {
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                        �������: ��������� ������� �������������!                               \n");
                printf("------------------------------------------------------------------------------------------------\n\n");
            }
            else if (result == -1) {
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                     ������: �� ������� ���������� ������ ����������                            \n");
                printf("------------------------------------------------------------------------------------------------\n\n");
            }
        }

        printf("\n������ ������������� ����������? (-1 - ��, ����� ������� - ���): ");
        scanf("%d", &monitoring_enabled);
        printf("\n------------------------------------------------------------------------------------------------\n");
        if (monitoring_enabled == -1) {
            break;
        }
        else {
            while (getchar() != '\n');
            printf("\n");
            monitoring_enabled = 1;
        }
    }

    fclose(result_file);
    printf("\t���������� ����������� ��������� � �����: %s\n", result_filename);
    printf("------------------------------------------------------------------------------------------------\n");

    return 0;
}

/*
 * �������� ������ ���������� � ����.
 * data - ��������� �� ��������� SensorData � ������� ����������.
 * mode - ������� ����� ������ (MODE_A, MODE_B, MODE_C).
 * monitoring_enabled - ���� ���������/���������� �����������.
 * ���������� 0 ��� �������� ������, -1 ��� ������.
 */
int mon_sensor_data(const SensorData* data, int mode, int monitoring_enabled) {
    if (!monitoring_enabled) {
        return 0;
    }

    FILE* mon_file = fopen(MONITORING_FILE, "a");
    if (!mon_file) {
        perror("������ �������� �����");
        return -1;
    }

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", localtime(&data->timestamp));

    fprintf(mon_file, "%d %d %s %s %s\n", mode, data->status, data->id, data->zone, time_str);
    fclose(mon_file);

    return 0;
}

/*
 * ����������� ���� � ������� �������.
 * filter_mode - �������� �������� ��� ���������� (��� -1 ��� ���� �������).
 * filter_status - ������ ��� ���������� (��� -1 ��� ���� ��������).
 * filter_zone - ���� ��� ���������� (��� "all" ��� ���� ���).
 * ���������� ���������� ��������������� ������� ��� -1 ��� ������.
 */
int analyze_mon_file(int filter_mode, int filter_status, const char* filter_zone, const char* filter_time) {
    FILE* file = fopen(MONITORING_FILE, "r");
    if (!file) {
        perror("������ �������� ����� ��� �������");
        return -1;
    }

    int record_count = 0;
    char line[256];

    printf("\n------------------------------------------------------------------------------------------------\n");
    printf("    �������� ��������    | ��� ��������� | ������������� | ���� �������� |    ���� � �����      \n");
    printf("------------------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        int mode, status;
        char id[100], zone[100], time_str[100];

        if (sscanf(line, "%d %d %s %s %s", &mode, &status, id, zone, time_str) == 5) {
            if ((filter_mode == -1 || mode == filter_mode) &&
                (filter_status == -1 || status == filter_status) &&
                (strcmp(filter_zone, "all") == 0 || strcmp(zone, filter_zone) == 0) &&
                (strcmp(filter_time, "all") == 0 || strcmp(time_str, filter_time) == 0)) {
                printf("            %-12s | \t %-7d | %-13s | %-13s |     %s\n", mode_to_string(mode), status, id, zone, time_str);
                record_count++;
            }
        }
    }
    printf("------------------------------------------------------------------------------------------------\n");

    fclose(file);

    if (record_count == 0) {
        printf("                       ��� ������, ��������������� �������� ��������                            \n");
        printf("------------------------------------------------------------------------------------------------\n");
    }

    return record_count;
}

/*
 * ����������� ����� ������ � ������.
 * mode - ����� ������ (MODE_A, MODE_B, MODE_C).
 * ���������� ��������� ������������� ������.
 */
const char* mode_to_string(int mode) {
    switch (mode) {
    case MODE_A:
        return "A";
    case MODE_B:
        return "B";
    case MODE_C:
        return "C";
    default:
        return "UNKNOWN";
    }
}
