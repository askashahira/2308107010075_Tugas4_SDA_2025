#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Untuk clock()

// --- Platform Specific Includes for Memory Usage ---
#if defined(__linux__) || defined(__APPLE__)
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <psapi.h>
#else
// Platform lain tidak didukung untuk pengukuran memori
#endif
// ----------------------------------------------------

#include "sorting_algorithms.h" // Sertakan header algoritma sorting kita

#define DATA_ANGKA_FILE "data_angka.txt"
#define DATA_KATA_FILE "data_kata.txt"
// NUM_ELEMENTS sekarang menjadi batas atas, bukan ukuran tetap untuk dibaca
#define MAX_EXPECTED_ELEMENTS 2000000
#define MAX_WORD_LENGTH 20
#define MAX_LINE_LENGTH 25

// === Fungsi Helper untuk Baca Data (DIMODIFIKASI) ===

/**
 * @brief Membaca maksimal 'max_to_read' angka integer dari file.
 * @param filename Nama file.
 * @param arr Pointer ke pointer array (output).
 * @param count Pointer ke integer (output: jumlah angka yang benar-benar dibaca).
 * @param max_to_read Jumlah maksimum angka yang akan dibaca.
 * @return 1 jika berhasil membuka file & alokasi awal, 0 jika gagal.
 */
int read_numbers(const char *filename, int **arr, int *count, int max_to_read) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error membuka file angka");
        *arr = NULL;
        *count = 0;
        return 0;
    }

    // Alokasi sesuai jumlah yang diminta
    *arr = (int *)malloc(max_to_read * sizeof(int));
    if (!*arr) {
        perror("Gagal alokasi memori untuk array angka");
        fclose(fp);
        *count = 0;
        return 0; // Gagal alokasi
    }

    int i = 0;
    // Baca hingga max_to_read tercapai atau EOF
    while (i < max_to_read && fscanf(fp, "%d", &(*arr)[i]) == 1) {
        i++;
    }

    *count = i; // Jumlah angka yang berhasil dibaca

    // Jika file lebih kecil dari yang diminta, resize (opsional tapi baik)
    if (i < max_to_read) {
         fprintf(stderr, "Warning: Hanya bisa membaca %d angka dari %s (diminta %d)\n", i, filename, max_to_read);
         int *temp = realloc(*arr, i * sizeof(int));
         if (temp) {
             *arr = temp;
         } else {
             // Jika realloc gagal, data lama masih valid tapi ukurannya tidak optimal
             fprintf(stderr, "Warning: Gagal realloc untuk memperkecil array angka.\n");
         }
    }

    fclose(fp);
    return 1; // Berhasil (setidaknya membuka file dan alokasi awal)
}

/**
 * @brief Membaca maksimal 'max_to_read' kata dari file.
 * @param filename Nama file.
 * @param arr Pointer ke pointer array string (output).
 * @param count Pointer ke integer (output: jumlah kata yang benar-benar dibaca).
 * @param max_to_read Jumlah maksimum kata yang akan dibaca.
 * @return 1 jika berhasil membuka file & alokasi awal, 0 jika gagal.
 */
int read_words(const char *filename, char ***arr, int *count, int max_to_read) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error membuka file kata");
        *arr = NULL;
        *count = 0;
        return 0;
    }

    // Alokasi array pointer char* sesuai jumlah yang diminta
    *arr = (char **)malloc(max_to_read * sizeof(char *));
    if (!*arr) {
        perror("Gagal alokasi memori untuk array pointer kata");
        fclose(fp);
        *count = 0;
        return 0;
    }

    char line_buffer[MAX_LINE_LENGTH];
    int i = 0;
    while (i < max_to_read && fgets(line_buffer, sizeof(line_buffer), fp)) {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; // Hapus newline

        // Alokasi memori untuk string aktual dan salin
        (*arr)[i] = (char *)malloc((strlen(line_buffer) + 1) * sizeof(char));
        if (!(*arr)[i]) {
            perror("Gagal alokasi memori untuk string kata");
            // Cleanup memori yang sudah dialokasi dalam loop ini
            for (int j = 0; j < i; j++) { free((*arr)[j]); }
            free(*arr);
            *arr = NULL;
            *count = 0;
            fclose(fp);
            return 0; // Gagal alokasi string
        }
        strcpy((*arr)[i], line_buffer);
        i++;
    }

    *count = i; // Jumlah kata yang berhasil dibaca

     // Jika file lebih kecil dari yang diminta, resize array pointer
     if (i < max_to_read) {
         fprintf(stderr, "Warning: Hanya bisa membaca %d kata dari %s (diminta %d)\n", i, filename, max_to_read);
         char **temp = realloc(*arr, i * sizeof(char *));
         if (temp) {
             *arr = temp;
         } else {
              fprintf(stderr, "Warning: Gagal realloc untuk memperkecil array pointer kata.\n");
              // Data lama masih valid, tapi memori tidak optimal
         }
     }

    fclose(fp);
    return 1; // Berhasil
}

// free_words tetap sama
void free_words(char **arr, int count) {
    if (!arr) return;
    for (int i = 0; i < count; i++) {
        if (arr[i]) free(arr[i]); // Tambahkan check null untuk keamanan
    }
    free(arr);
}

// === Fungsi Helper untuk Pengukuran (TETAP SAMA) ===

typedef struct {
    double time_sec;
    long memory_kb;
} PerformanceResult;

long get_peak_memory_usage_kb() {
#if defined(__linux__) || defined(__APPLE__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        #ifdef __APPLE__
        return usage.ru_maxrss / 1024;
        #else
        return usage.ru_maxrss;
        #endif
    } else { perror("getrusage failed"); return -1; }
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS_EX pmc;
    ZeroMemory(&pmc, sizeof(pmc)); pmc.cb = sizeof(pmc);
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return (long)(pmc.PeakWorkingSetSize / 1024);
    } else { fprintf(stderr, "GetProcessMemoryInfo failed (Error code: %lu)\n", GetLastError()); return -1; }
#else
    // printf("(Pengukuran memori tidak didukung di platform ini) "); // Kurangi noise output
    return -1;
#endif
}

// === Fungsi Utama Pengujian (TETAP SAMA, tapi dipanggil dengan data berbeda) ===

void run_test(
    const char *algo_name,
    void (*sort_func_int)(int[], int),
    void (*sort_func_str)(char*[], int),
    int *data_int, // Ubah nama parameter agar lebih jelas ini data *untuk tes ini*
    char **data_str, // Ubah nama parameter
    int count,
    const char *data_type) // Ukuran data sudah dicetak di main loop luar
{
    PerformanceResult result = {0.0, -1};
    clock_t start_time, end_time;
    long mem_before = -1, mem_after = -1;

    // Mencetak info di sini di-redundant karena sudah ada di loop utama,
    // tapi bisa berguna jika fungsi ini dipanggil dari tempat lain.
    // Untuk tugas ini, kita bisa menyederhanakannya atau membiarkannya.
    // printf("--------------------------------------------------\n");
    // printf("Menguji Algoritma : %s\n", algo_name);
    // printf("Tipe Data         : %s\n", data_type);
    // printf("Jumlah Data       : %d\n", count); // INFO INI SUDAH ADA DI LUAR

    printf("  -> Algoritma: %-15s ... ", algo_name); // Format lebih ringkas
    fflush(stdout); // Pastikan output muncul sebelum proses sorting lama

    if (strcmp(data_type, "Angka") == 0 && sort_func_int != NULL) {
        // 1. BUAT SALINAN dari data_int (subset yg sudah dibaca)
        int *data_copy = (int *)malloc(count * sizeof(int));
        if (!data_copy) {
            perror(" Gagal alokasi memori salinan angka"); printf(" GAGAL.\n"); return;
        }
        memcpy(data_copy, data_int, count * sizeof(int));

        mem_before = get_peak_memory_usage_kb();
        start_time = clock();
        sort_func_int(data_copy, count);
        end_time = clock();
        mem_after = get_peak_memory_usage_kb();

        result.time_sec = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        if (mem_after != -1) { result.memory_kb = mem_after; }
        free(data_copy);

    } else if (strcmp(data_type, "Kata") == 0 && sort_func_str != NULL) {
        // 1. BUAT SALINAN array pointer dari data_str
        char **data_copy = (char **)malloc(count * sizeof(char *));
         if (!data_copy) {
            perror(" Gagal alokasi memori salinan kata (pointer)"); printf(" GAGAL.\n"); return;
        }
        memcpy(data_copy, data_str, count * sizeof(char *)); // Shallow copy

        mem_before = get_peak_memory_usage_kb();
        start_time = clock();
        sort_func_str(data_copy, count);
        end_time = clock();
        mem_after = get_peak_memory_usage_kb();

        result.time_sec = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        if (mem_after != -1) { result.memory_kb = mem_after; }
        free(data_copy); // Free array pointer salinan

    } else {
        printf(" Tipe data/fungsi tidak valid. GAGAL.\n");
        return;
    }

    // Cetak hasil ringkas
    printf("Waktu: %8.4f s | ", result.time_sec);
    if (result.memory_kb != -1) {
        #if defined(_WIN32) || defined(_WIN64)
        printf("Mem Peak WS: %7ld KB\n", result.memory_kb);
        #else
        printf("Mem Peak RSS: %7ld KB\n", result.memory_kb);
        #endif
    } else {
        printf("Mem Peak: (N/A)\n");
    }
    // printf("--------------------------------------------------\n"); // Hapus garis bawah per algoritma
}


int main() {
    // === Definisikan Ukuran Data Uji ===
    int test_sizes[] = {
        10000,
        50000,
        100000,
        250000,
        500000,
        1000000,
        1500000,
        2000000
    };
    int num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);

    printf("===== ANALISIS PERFORMA ALGORITMA SORTING =====\n");
    printf("Akan menguji %d ukuran data: 10k, 50k, 100k, 250k, 500k, 1M, 1.5M, 2M\n", num_sizes);
    printf("Sumber data: %s (angka), %s (kata)\n", DATA_ANGKA_FILE, DATA_KATA_FILE);
    printf("=================================================\n\n");

    // === Loop Melalui Setiap Ukuran Data ===
    for (int s = 0; s < num_sizes; ++s) {
        int current_size = test_sizes[s];
        printf("--- Memulai Pengujian untuk Ukuran Data: %d ---\n", current_size);

        int *current_numbers = NULL;
        char **current_words = NULL;
        int number_count_read = 0;
        int word_count_read = 0;
        int read_num_ok = 0;
        int read_word_ok = 0;

        // --- Baca Data Angka untuk Ukuran Saat Ini ---
        printf("Membaca %d angka dari %s...\n", current_size, DATA_ANGKA_FILE);
        if (read_numbers(DATA_ANGKA_FILE, &current_numbers, &number_count_read, current_size)) {
            if (number_count_read == current_size) {
                printf("Berhasil membaca %d angka.\n", number_count_read);
                read_num_ok = 1;
            } else {
                // File sumber tidak cukup besar (seharusnya tidak terjadi jika generate 2M)
                 fprintf(stderr, "Error: Hanya dapat membaca %d angka, butuh %d. Lewati tes angka untuk ukuran ini.\n", number_count_read, current_size);
                 // Tetap bebaskan memori jika sebagian terbaca
                 if(current_numbers) free(current_numbers);
                 current_numbers = NULL;
            }
        } else {
            fprintf(stderr, "Gagal membaca atau alokasi memori untuk angka ukuran %d.\n", current_size);
        }

        // --- Baca Data Kata untuk Ukuran Saat Ini ---
        printf("Membaca %d kata dari %s...\n", current_size, DATA_KATA_FILE);
        if (read_words(DATA_KATA_FILE, &current_words, &word_count_read, current_size)) {
             if (word_count_read == current_size) {
                printf("Berhasil membaca %d kata.\n", word_count_read);
                read_word_ok = 1;
            } else {
                fprintf(stderr, "Error: Hanya dapat membaca %d kata, butuh %d. Lewati tes kata untuk ukuran ini.\n", word_count_read, current_size);
                if(current_words) free_words(current_words, word_count_read); // Hati-hati free jika hanya sebagian
                current_words = NULL;
            }
        } else {
             fprintf(stderr, "Gagal membaca atau alokasi memori untuk kata ukuran %d.\n", current_size);
        }

        // --- Menjalankan Tes untuk Angka (jika data berhasil dibaca) ---
        if (read_num_ok && current_numbers) {
            printf("\n-- Tes Angka (Ukuran: %d) --\n", current_size);
            run_test("Bubble Sort", bubble_sort_int, NULL, current_numbers, NULL, number_count_read, "Angka");
            run_test("Selection Sort", selection_sort_int, NULL, current_numbers, NULL, number_count_read, "Angka");
            run_test("Insertion Sort", insertion_sort_int, NULL, current_numbers, NULL, number_count_read, "Angka");
            run_test("Shell Sort", shell_sort_int, NULL, current_numbers, NULL, number_count_read, "Angka");
            run_test("Merge Sort", merge_sort_int, NULL, current_numbers, NULL, number_count_read, "Angka");
            run_test("Quick Sort", quick_sort_int, NULL, current_numbers, NULL, number_count_read, "Angka");
        } else {
            printf("\n-- Tes Angka (Ukuran: %d) DILAWATI --\n", current_size);
        }

        // --- Menjalankan Tes untuk Kata (jika data berhasil dibaca) ---
        if (read_word_ok && current_words) {
            printf("\n-- Tes Kata (Ukuran: %d) --\n", current_size);
            run_test("Bubble Sort", NULL, bubble_sort_str, NULL, current_words, word_count_read, "Kata");
            run_test("Selection Sort", NULL, selection_sort_str, NULL, current_words, word_count_read, "Kata");
            run_test("Insertion Sort", NULL, insertion_sort_str, NULL, current_words, word_count_read, "Kata");
            run_test("Shell Sort", NULL, shell_sort_str, NULL, current_words, word_count_read, "Kata");
            run_test("Merge Sort", NULL, merge_sort_str, NULL, current_words, word_count_read, "Kata");
            run_test("Quick Sort", NULL, quick_sort_str, NULL, current_words, word_count_read, "Kata");
        } else {
             printf("\n-- Tes Kata (Ukuran: %d) DILAWATI --\n", current_size);
        }

        // --- BEBASKAN MEMORI untuk ukuran data saat ini ---
        printf("\nMembersihkan memori untuk ukuran %d...\n", current_size);
        if (current_numbers) {
            free(current_numbers);
            current_numbers = NULL; // Set ke NULL setelah free
        }
        if (current_words) {
            free_words(current_words, word_count_read);
            current_words = NULL; // Set ke NULL setelah free
        }

        printf("--- Selesai Pengujian untuk Ukuran Data: %d ---\n\n", current_size);

    } // End loop ukuran data

    printf("=================================================\n");
    printf("===== SEMUA PENGUJIAN SELESAI =====\n");

    return 0; // Asumsikan sukses jika program selesai
}