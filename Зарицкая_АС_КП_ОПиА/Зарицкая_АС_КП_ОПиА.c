/*
 Автор: Зарицкая Анастасия Станиславовна, бИЦ-241
 Программа для мониторинга данных с пожарных извещателей.
 Краткое описание возможностей программы:
 - Непрерывный мониторинг по заданному алгоритму.
 - Запись результатов работы в файл.
 - Сортировка и фильтрация результатов.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <windows.h>

/* Константы для режимов работы */
#define MODE_A 0
#define MODE_B 1
#define MODE_C 2

/* Имя файла с данными о работе */
#define MONITORING_FILE "monitoring.txt"

/* Структура для хранения данных извещателя */
typedef struct {
    char id[100];
    char zone[100];
    int status;
    time_t timestamp;
} SensorData;

/* Прототипы функций */

/* Обрабатывает данные извещателя. Возвращает 1 при тревоге, 0 при отсутствии тревоги, -1 при ошибке.*/
int handle_sensor_data(const SensorData* data, int* sensor_count, SensorData* sensors, int mode);

/* Устанавливает режим работы. Возвращает новый режим (MODE_A, MODE_B, MODE_C). */
int set_mode(char mode_char);

/* Тестирует систему на данных из файла. Возвращает 0 при успехе, -1 при ошибке. */
int test_system_from_file(const char* filename, int* sensor_count, SensorData* sensors, int mode);

/* Генерирует тестовые данные и сохраняет их в файл. Возвращает 0 при успехе, -1 при ошибке. */
int generate_test_data(char* filename, SensorData* data);

/* Тестирует систему, обрабатывая данные извещателей. Возвращает 0 при успехе, -1 при ошибке. */
int test_system(int* sensor_count, SensorData* sensors, int monitoring_enabled, int mode);

/* Записывает данные извещателей в файл. Возвращает 0 при успешной записи, -1 при ошибке. */
int mon_sensor_data(const SensorData* data, int mode, int monitoring_enabled);

/* Анализирует файл. Возвращает количество отфильтрованных записей или -1 при ошибке. */
int analyze_mon_file(int filter_mode, int filter_status, const char* filter_zone, const char* filter_time);

/* Преобразует режим работы в строку. Возвращает строковое представление режима. */
const char* mode_to_string(int mode);

int main() {
    int choice;
    char id[100];
    char zone[100];
    int status;
    setlocale(LC_ALL, "RUS");

    /* Назначение программы и описание возможностей */
    printf("------------------------------------------------------------------------------------------------\n");
    printf("Добро пожаловать в программу мониторинга промышленного оборудования!\n");
    printf("Данная программа предназначена для контроля и анализа работы пожарных извещателей.\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("Основные возможности программы:\n");
    printf("1. Работа в разных режимах алгоритма контроля:\n");
    printf("   - MODE_A: Немедленное реагирование на срабатывание извещателя.                           \n");
    printf("   - MODE_B: Ожидание повторного срабатывания в той же зоне в течение 60 секунд.            \n");
    printf("   - MODE_C: Ожидание повторного срабатывания любого другого извещателя.                    \n");
    printf("2. Управление мониторингом: возможность включения/выключения мониторинга.\n");
    printf("3. Ручной ввод данных: возможность ввода данных извещателя вручную.\n");
    printf("4. Анализ данных о работе системы\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("\n");

    /* Инициализация состояния системы */
    int mode = MODE_A;
    SensorData sensors[1000];
    int sensor_count = 0;
    int monitoring_enabled = -1;
    /* Основной цикл программы */
    while (1) {
        printf("1. Выбрать режим работы (A, B, C)\n");
        printf("2. Включить/выключить режим мониторинга\n");
        printf("3. Ввести значения извещателей вручную\n");
        printf("4. Анализ данных о работе системы\n");
        printf("5. Выход из программы\n");
        printf("\nВыберите действие: ");
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода: введите число\n\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            /* Обработка выбора режима работы */
            printf("\n------------------------------------------------------------------------------------------------\n");
            printf("                                  Выбор алгоритма контроля                                      \n");
            printf("------------------------------------------------------------------------------------------------\n");
            printf("  0. Алгоритм A  Немедленное реагирование на срабатывание извещателя.                           \n");
            printf("  1. Алгоритм B  Ожидание повторного срабатывания в той же зоне в течение 60 секунд.            \n");
            printf("  2. Алгоритм C  Ожидание повторного срабатывания любого другого извещателя.                    \n");
            printf("------------------------------------------------------------------------------------------------\n");
            while (1) {
                printf("\nВведите ваш выбор: ");
                char input[10];
                scanf("%s", input);
                char mode_char = input[0];
                mode = set_mode(mode_char);
                if (mode == -1) {
                    printf("Неверный ввод: введите соответствующее число или A, B, C.\n");
                }
                else {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("                             Режим работы изменен на Алгоритм %s           \n", mode_to_string(mode));
                    printf("------------------------------------------------------------------------------------------------\n");
                    break;
                }
            }
            break;
        }
        case 2:
            /* Включение/выключение режима мониторинга */
            if (monitoring_enabled == -1) {
                monitoring_enabled = 1;
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                                 Мониторинг включен                                              \n");
                printf("------------------------------------------------------------------------------------------------\n\n");
                /* Генерация тестовых данных и тестирование */
                if (test_system(&sensor_count, sensors, monitoring_enabled, mode) != 0) {
                    printf("Ошибка при тестировании системы.\n");
                }
                break;
            }
            else {
                monitoring_enabled = -1;
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                                 Мониторинг выключен                                             \n");
                printf("------------------------------------------------------------------------------------------------\n");
            }
            break;
        case 3:
            /* Режим ручного ввода данных извещателя */
            printf("\n------------------------------------------------------------------------------------------------\n");
            printf("                    Для выхода из режима ручного ввода данных введите -1                        \n");
            printf("------------------------------------------------------------------------------------------------\n\n");
            while (1) {
                printf("Введите данные через пробел (код состояния | идентификатор | зона контроля): ");
                if (scanf("%d", &status) != 1 || status < -1 || status > 1) {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("          Диагностическое сообщение: Код состояния вне допустимого диапазона (0-1)              \n");
                    printf("------------------------------------------------------------------------------------------------\n\n");
                    while (getchar() != '\n');
                    continue;
                }

                /* Проверка на выход */
                if (status == -1) {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("                             Выход из режима ручного ввода данных                               \n");
                    printf("------------------------------------------------------------------------------------------------\n");
                    break;
                }

                if (scanf("%s %s", id, zone) != 2) {
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    printf("                                        Ошибка ввода                                            \n");
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
                        printf("                        Тревога: Запускаем систему пожаротушения!                               \n");
                        printf("------------------------------------------------------------------------------------------------\n\n");
                    }
                    else if (result == -1) {
                        printf("\n------------------------------------------------------------------------------------------------\n");
                        printf("                     Ошибка: Не удалось обработать данные извещателя                            \n");
                        printf("------------------------------------------------------------------------------------------------\n\n");
                    }

                    /* Запись данных, если мониторинг включен */
                    if (monitoring_enabled) {
                        if (mon_sensor_data(&data, mode, monitoring_enabled) != 0) {
                            perror("Ошибка при записи данных в файл о работе системы.\n");
                        }
                    }
                }
            }
            break;
        case 4: {
            /* Анализ данных из файла */
            int filter_mode;
            int filter_status;
            char filter_zone[100];
            char filter_time[100];

            printf("\n------------------------------------------------------------------------------------------------");
            printf("\n            Анализ данных о работе системы (сортировка и фильтрация по критериям)               ");
            printf("\n------------------------------------------------------------------------------------------------\n");

            /* Ввод алгоритма контроля */
            while (1) {
                printf("\nВыберите алгоритм контроля (0 - A, 1 - B, 2 - C, -1 для всех): ");
                if (scanf("%d", &filter_mode) != 1 || filter_mode < -1 || filter_mode > 2) {
                    printf("Ошибка ввода: введите число от -1 до 2.\n");
                    while (getchar() != '\n');
                    continue;
                }
                break;
            }

            /* Ввод кода состояния */
            while (1) {
                printf("Введите код состояния для фильтрации (0 или 1, -1 для всех статусов): ");
                if (scanf("%d", &filter_status) != 1 || (filter_status != -1 && filter_status != 0 && filter_status != 1)) {
                    printf("Ошибка ввода: введите число от -1 до 2.\n");
                    while (getchar() != '\n');
                    continue;
                }
                break;
            }

            /* Ввод зоны контроля */
            while (1) {
                printf("Введите зону контроля для фильтрации (или 'all' для всех зон): ");
                if (scanf("%s", filter_zone) != 1) {
                    printf("Ошибка ввода: введите строку.\n");
                    while (getchar() != '\n');
                    continue;
                }
                break;
            }

            /* Ввод времени для фильтрации */
            while (1) {
                printf("Введите дату для фильтрации (или 'all' для всех): ");
                if (scanf("%s", filter_time) != 1) {
                    printf("Ошибка ввода: введите строку.\n");
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
            printf("Неверный ввод\n");
            while (getchar() != '\n');
        }
        printf("\n");
    }
    return 0;
}

/*
 * Обрабатывает данные извещателя.
 * data - указатель на структуру SensorData с данными извещателя.
 * sensor_count - указатель на количество извещателей в массиве sensors.
 * sensors - массив для хранения данных извещателей.
 * mode - текущий режим работы (MODE_A, MODE_B, MODE_C).
 * Возвращает:
 *   1 - если нужно запустить систему пожаротушения (тревога),
 *   0 - если тревоги нет,
 *  -1 - при ошибке.
 */
int handle_sensor_data(const SensorData* data, int* sensor_count, SensorData* sensors, int mode) {
    assert(data->status == 0 || data->status == 1);

    /* Логика для режима A */
    if (mode == MODE_A) {
        if (data->status == 1) {
            return 1;
        }
    }

    /* Логика для режима B */
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

    /* Логика для режима C */
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

    /* Добавление данных в массив */
    if (*sensor_count >= 1000) {
        printf("Ошибка: превышен лимит извещателей.\n");
        return -1;
    }
    sensors[*sensor_count] = *data;
    (*sensor_count)++;

    return 0;
}

/*
 * Устанавливает режим работы системы.
 * mode_char - символ, указывающий режим (A, B, C).
 * Возвращает новый режим (MODE_A, MODE_B, MODE_C) или -1 при ошибке.
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
 * Генерирует тестовые данные и сохраняет в файл.
 * filename - имя файла для сохранения.
 * data - структура с данными датчика.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
int generate_test_data(char* filename, SensorData* data) {
    /* Генерация случайных данных */
    data->status = rand() % 2;
    int id_number = 1 + rand() % 5;
    int zone_number = 1 + rand() % 10;
    snprintf(data->id, sizeof(data->id), "ID_%d", id_number);
    snprintf(data->zone, sizeof(data->zone), "ZONE_%d", zone_number);
    data->timestamp = time(NULL);

    /* Генерация имени файла */
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(filename, 100, "%02d.%02d.%02d_%02d.%02d.%02d.txt",
        t->tm_mday, t->tm_mon + 1, t->tm_year - 100, t->tm_hour, t->tm_min, t->tm_sec);

    /* Запись в файл */
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка открытия файла для записи тестовых данных");
        return -1;
    }
    fprintf(file, "%d %s %s\n", data->status, data->id, data->zone);
    fclose(file);

    return 0;
}

/*
 * Тестирует систему на основе сгенерированных данных и записывает результаты.
 * sensor_count - количество датчиков.
 * sensors - массив датчиков.
 * monitoring_enabled - флаг включения мониторинга.
 * mode - режим работы.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
int test_system(int* sensor_count, SensorData* sensors, int monitoring_enabled, int mode) {
    /* Генерация имени файла для результатов */
    char result_filename[100];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(result_filename, sizeof(result_filename), "result_mode_%c_%02d.%02d.%02d_%02d.%02d.%02d.txt",
        mode_to_string(mode)[0], t->tm_mday, t->tm_mon + 1, t->tm_year - 100, t->tm_hour, t->tm_min, t->tm_sec);

    /* Открытие файла для записи результатов */
    FILE* result_file = fopen(result_filename, "w");
    if (!result_file) {
        perror("Ошибка открытия файла для записи результатов тестирования");
        return -1;
    }

    /* Запись заголовка */
    fprintf(result_file, "------------------------------------------------------------------------------------------------\n");
    fprintf(result_file, "                     Тестирование системы на сгенерированных данных                           \n");
    fprintf(result_file, "------------------------------------------------------------------------------------------------\n");
    fprintf(result_file, "\tКод состояния | Идентификатор | Зона контроля | Срабатывание тревоги\n");
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

            printf("Поступили данные с пожарного извещателя: %s\n", test_filename);

            if (result == 1) {
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                        Тревога: Запускаем систему пожаротушения!                               \n");
                printf("------------------------------------------------------------------------------------------------\n\n");
            }
            else if (result == -1) {
                printf("\n------------------------------------------------------------------------------------------------\n");
                printf("                     Ошибка: Не удалось обработать данные извещателя                            \n");
                printf("------------------------------------------------------------------------------------------------\n\n");
            }
        }

        printf("\nХотите приостановить мониторинг? (-1 - Да, Любая клавиша - Нет): ");
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
    printf("\tРезультаты мониторинга сохранены в файле: %s\n", result_filename);
    printf("------------------------------------------------------------------------------------------------\n");

    return 0;
}

/*
 * Логирует данные извещателя в файл.
 * data - указатель на структуру SensorData с данными извещателя.
 * mode - текущий режим работы (MODE_A, MODE_B, MODE_C).
 * monitoring_enabled - флаг включения/выключения мониторинга.
 * Возвращает 0 при успешной записи, -1 при ошибке.
 */
int mon_sensor_data(const SensorData* data, int mode, int monitoring_enabled) {
    if (!monitoring_enabled) {
        return 0;
    }

    FILE* mon_file = fopen(MONITORING_FILE, "a");
    if (!mon_file) {
        perror("Ошибка открытия файла");
        return -1;
    }

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", localtime(&data->timestamp));

    fprintf(mon_file, "%d %d %s %s %s\n", mode, data->status, data->id, data->zone, time_str);
    fclose(mon_file);

    return 0;
}

/*
 * Анализирует файл с работой системы.
 * filter_mode - алгоритм контроля для фильтрации (или -1 для всех режимов).
 * filter_status - статус для фильтрации (или -1 для всех статусов).
 * filter_zone - зона для фильтрации (или "all" для всех зон).
 * Возвращает количество отфильтрованных записей или -1 при ошибке.
 */
int analyze_mon_file(int filter_mode, int filter_status, const char* filter_zone, const char* filter_time) {
    FILE* file = fopen(MONITORING_FILE, "r");
    if (!file) {
        perror("Ошибка открытия файла для анализа");
        return -1;
    }

    int record_count = 0;
    char line[256];

    printf("\n------------------------------------------------------------------------------------------------\n");
    printf("    Алгоритм контроля    | Код состояния | Идентификатор | Зона контроля |    Дата и время      \n");
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
        printf("                       Нет данных, соответствующих заданным фильтрам                            \n");
        printf("------------------------------------------------------------------------------------------------\n");
    }

    return record_count;
}

/*
 * Преобразует режим работы в строку.
 * mode - режим работы (MODE_A, MODE_B, MODE_C).
 * Возвращает строковое представление режима.
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
