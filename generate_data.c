#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Fungsi untuk membangkitkan angka acak dan menyimpannya ke file
void generate_random_numbers(const char *filename, int count, int max_value) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error membuka file untuk angka");
        return;
    }

    srand(time(NULL)); // Inisialisasi seed HANYA SEKALI di awal program jika dipanggil berurutan
    printf("Memulai generate %d angka acak ke %s...\n", count, filename);
    for (int i = 0; i < count; i++) {
        int num = rand() % (max_value + 1); // Menghasilkan angka dari 0 hingga max_value
        fprintf(fp, "%d\n", num);
        if ((i + 1) % (count / 10) == 0) { // Progress update
             printf("... %d%% selesai\n", (int)(((double)(i + 1) / count) * 100));
        }
    }
    fclose(fp);
    printf("Generate angka selesai.\n");
}

// Fungsi untuk membuat kata acak
void random_word(char *word, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    if (length <= 0) return;
    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        word[i] = charset[key];
    }
    word[length] = '\0'; // Null-terminate the string
}

// Fungsi untuk membangkitkan kata acak dan menyimpannya ke file
void generate_random_words(const char *filename, int count, int max_word_length) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error membuka file untuk kata");
        return;
    }

    // Kita bisa menggunakan seed yang sama atau berbeda. Menggunakan yang sama agar lebih deterministik jika diperlukan.
    // srand(time(NULL)); // Seed sudah diinisialisasi di generate_random_numbers jika dipanggil sebelumnya
                             // Jika file ini dijalankan terpisah, uncomment srand() di sini.

    printf("Memulai generate %d kata acak ke %s (max length %d)...\n", count, filename, max_word_length);
    int min_word_length = 3; // Set panjang kata minimum
    char *word_buffer = (char *)malloc(max_word_length + 1); // Alokasi buffer kata
    if (!word_buffer) {
         perror("Gagal alokasi memori untuk buffer kata");
         fclose(fp);
         return;
    }


    for (int i = 0; i < count; i++) {
        int length = min_word_length + rand() % (max_word_length - min_word_length + 1);
        random_word(word_buffer, length);
        fprintf(fp, "%s\n", word_buffer);
         if ((i + 1) % (count / 10) == 0) { // Progress update
             printf("... %d%% selesai\n", (int)(((double)(i + 1) / count) * 100));
        }
    }

    free(word_buffer); // Bebaskan buffer
    fclose(fp);
    printf("Generate kata selesai.\n");
}

int main() {
    int num_count = 2000000; // Jumlah data: 2 Juta
    int max_value = 2000000; // Nilai angka maksimum
    int max_word_len = 15;   // Panjang kata maksimum

    // Menggunakan srand() sekali di awal main direkomendasikan
    srand(time(NULL));

    printf("--- GENERATE DATA UJI ---\n");

    generate_random_numbers("data_angka.txt", num_count, max_value);
    printf("\n");
    generate_random_words("data_kata.txt", num_count, max_word_len);

    printf("\n--- Selesai Generate Data ---\n");

    return 0;
}