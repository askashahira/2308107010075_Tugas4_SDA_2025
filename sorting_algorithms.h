#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === Helper Functions ===

// Helper function to swap two integers
void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Helper function to swap two string pointers
void swap_str(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

// === Bubble Sort ===

/**
 * @brief Mengurutkan array integer menggunakan algoritma Bubble Sort.
 * @prinsip Bubble Sort bekerja dengan berulang kali membandingkan pasangan elemen
 *          yang bersebelahan dan menukarnya jika urutannya salah. Proses ini
 *          diulangi sampai tidak ada lagi pertukaran yang diperlukan, yang
 *          menandakan array sudah terurut. Elemen terbesar akan "menggelembung"
 *          ke posisi terakhir pada setiap iterasi luar.
 * @param arr Array integer yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void bubble_sort_int(int arr[], int n) {
    int i, j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap_int(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        // Jika tidak ada pertukaran dalam satu iterasi, array sudah terurut
        if (swapped == 0)
            break;
    }
}

/**
 * @brief Mengurutkan array string menggunakan algoritma Bubble Sort.
 * @prinsip Sama seperti Bubble Sort untuk integer, tetapi menggunakan strcmp
 *          untuk membandingkan string. String yang lebih besar secara leksikografis
 *          akan "menggelembung" ke akhir.
 * @param arr Array pointer ke string (char*) yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void bubble_sort_str(char *arr[], int n) {
    int i, j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                swap_str(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }
}

// === Selection Sort ===

/**
 * @brief Mengurutkan array integer menggunakan algoritma Selection Sort.
 * @prinsip Selection Sort bekerja dengan membagi array menjadi dua bagian:
 *          terurut dan tidak terurut. Pada setiap iterasi, algoritma mencari
 *          elemen terkecil dari bagian yang tidak terurut dan menukarnya
 *          dengan elemen pertama dari bagian tidak terurut. Ini secara bertahap
 *          memperluas bagian yang terurut.
 * @param arr Array integer yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void selection_sort_int(int arr[], int n) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        if (min_idx != i) {
            swap_int(&arr[min_idx], &arr[i]);
        }
    }
}

/**
 * @brief Mengurutkan array string menggunakan algoritma Selection Sort.
 * @prinsip Sama seperti Selection Sort untuk integer, tetapi menggunakan strcmp
 *          untuk menemukan string terkecil (secara leksikografis) dalam bagian
 *          yang belum terurut dan menukarnya ke posisi yang benar.
 * @param arr Array pointer ke string (char*) yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void selection_sort_str(char *arr[], int n) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (strcmp(arr[j], arr[min_idx]) < 0)
                min_idx = j;
        }
        if (min_idx != i) {
            swap_str(&arr[min_idx], &arr[i]);
        }
    }
}

// === Insertion Sort ===

/**
 * @brief Mengurutkan array integer menggunakan algoritma Insertion Sort.
 * @prinsip Insertion Sort membangun array terurut satu elemen pada satu waktu.
 *          Ia mengambil elemen dari bagian tidak terurut dan menyisipkannya
 *          ke posisi yang benar dalam bagian yang sudah terurut. Ini dilakukan
 *          dengan menggeser elemen-elemen yang lebih besar ke kanan.
 * @param arr Array integer yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void insertion_sort_int(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        // Geser elemen arr[0..i-1] yang lebih besar dari key
        // ke satu posisi di depan posisi mereka saat ini
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

/**
 * @brief Mengurutkan array string menggunakan algoritma Insertion Sort.
 * @prinsip Sama seperti Insertion Sort untuk integer, tetapi menggunakan strcmp
 *          untuk perbandingan. Mengambil string dari bagian tidak terurut dan
 *          menyisipkannya ke posisi yang benar di bagian terurut dengan menggeser
 *          string yang lebih besar (leksikografis).
 * @param arr Array pointer ke string (char*) yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void insertion_sort_str(char *arr[], int n) {
    int i, j;
    char *key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        // Geser elemen arr[0..i-1] yang lebih besar dari key
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// === Merge Sort ===

// Helper function for merging two sorted integer subarrays
void merge_int(int arr[], int l, int m, int r, int temp[]) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Salin data ke array sementara temp[]
    for (i = 0; i < n1; i++)
        temp[i] = arr[l + i];
    // Tidak perlu menyalin bagian kanan ke temp kedua, kita bisa langsung bandingkan

    i = 0; // Indeks awal subarray pertama (di temp)
    j = m + 1; // Indeks awal subarray kedua (di arr)
    k = l; // Indeks awal subarray gabungan (di arr)

    // Gabungkan temp[] dan arr[m+1...r] kembali ke arr[l...r]
    while (i < n1 && j <= r) {
        if (temp[i] <= arr[j]) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = arr[j];
            j++;
        }
        k++;
    }

    // Salin sisa elemen temp[] jika ada
    while (i < n1) {
        arr[k] = temp[i];
        i++;
        k++;
    }
    // Sisa elemen arr[m+1...r] sudah di tempatnya
}


// Helper function for merging two sorted string subarrays
void merge_str(char *arr[], int l, int m, int r, char *temp[]) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Salin data bagian kiri ke array sementara temp[]
    for (i = 0; i < n1; i++)
        temp[i] = arr[l + i]; // Salin pointer

    i = 0;     // Indeks awal subarray pertama (di temp)
    j = m + 1; // Indeks awal subarray kedua (di arr)
    k = l;     // Indeks awal subarray gabungan (di arr)

    // Gabungkan temp[] dan arr[m+1...r] kembali ke arr[l...r]
    while (i < n1 && j <= r) {
        if (strcmp(temp[i], arr[j]) <= 0) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = arr[j];
            j++;
        }
        k++;
    }

    // Salin sisa elemen temp[] jika ada
    while (i < n1) {
        arr[k] = temp[i];
        i++;
        k++;
    }
     // Sisa elemen arr[m+1...r] sudah di tempatnya
}


// Fungsi rekursif utama Merge Sort untuk integer
void merge_sort_recursive_int(int arr[], int l, int r, int temp[]) {
    if (l < r) {
        int m = l + (r - l) / 2; // Hindari overflow untuk l+r
        merge_sort_recursive_int(arr, l, m, temp);
        merge_sort_recursive_int(arr, m + 1, r, temp);
        merge_int(arr, l, m, r, temp);
    }
}

// Fungsi rekursif utama Merge Sort untuk string
void merge_sort_recursive_str(char *arr[], int l, int r, char *temp[]) {
     if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_recursive_str(arr, l, m, temp);
        merge_sort_recursive_str(arr, m + 1, r, temp);
        merge_str(arr, l, m, r, temp);
    }
}

/**
 * @brief Mengurutkan array integer menggunakan algoritma Merge Sort.
 * @prinsip Merge Sort adalah algoritma Divide and Conquer. Ia membagi array
 *          menjadi dua bagian secara rekursif hingga setiap subarray hanya
 *          memiliki satu elemen (yang dianggap terurut). Kemudian, ia
 *          menggabungkan (merge) subarray-subarray tersebut secara berurutan
 *          untuk menghasilkan array akhir yang terurut. Membutuhkan memori
 *          tambahan untuk proses penggabungan.
 * @param arr Array integer yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void merge_sort_int(int arr[], int n) {
    if (n < 2) return; // Tidak perlu sort jika elemen < 2
    // Alokasi memori sementara untuk proses merge (hanya butuh setengah ukuran)
    int *temp = (int *)malloc((n / 2 + 1) * sizeof(int));
    if (!temp) {
        perror("Gagal alokasi memori untuk merge sort temp");
        return; // Atau handle error lain
    }
    merge_sort_recursive_int(arr, 0, n - 1, temp);
    free(temp); // Bebaskan memori sementara
}


/**
 * @brief Mengurutkan array string menggunakan algoritma Merge Sort.
 * @prinsip Sama seperti Merge Sort untuk integer, tetapi menggunakan strcmp
 *          saat menggabungkan subarray. Menggunakan array pointer sementara.
 * @param arr Array pointer ke string (char*) yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void merge_sort_str(char *arr[], int n) {
    if (n < 2) return;
    // Alokasi memori sementara untuk pointer string
    char **temp = (char **)malloc((n / 2 + 1) * sizeof(char *));
     if (!temp) {
        perror("Gagal alokasi memori untuk merge sort temp string");
        return;
    }
    merge_sort_recursive_str(arr, 0, n - 1, temp);
    free(temp);
}


// === Quick Sort ===

// Helper function partition untuk Quick Sort integer
int partition_int(int arr[], int low, int high) {
    int pivot = arr[high]; // Pilih pivot (elemen terakhir)
    int i = (low - 1);   // Indeks elemen yang lebih kecil

    for (int j = low; j <= high - 1; j++) {
        // Jika elemen saat ini lebih kecil atau sama dengan pivot
        if (arr[j] <= pivot) {
            i++; // Tingkatkan indeks elemen yang lebih kecil
            swap_int(&arr[i], &arr[j]);
        }
    }
    swap_int(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Helper function partition untuk Quick Sort string
int partition_str(char *arr[], int low, int high) {
    char *pivot = arr[high]; // Pilih pivot
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        // Jika elemen saat ini lebih kecil atau sama dengan pivot (secara leksikografis)
        if (strcmp(arr[j], pivot) <= 0) {
            i++;
            swap_str(&arr[i], &arr[j]);
        }
    }
    swap_str(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Fungsi rekursif utama Quick Sort untuk integer
void quick_sort_recursive_int(int arr[], int low, int high) {
    if (low < high) {
        // pi adalah indeks partisi, arr[pi] sekarang di posisi yang benar
        int pi = partition_int(arr, low, high);

        // Urutkan elemen secara terpisah sebelum dan sesudah partisi
        quick_sort_recursive_int(arr, low, pi - 1);
        quick_sort_recursive_int(arr, pi + 1, high);
    }
}

// Fungsi rekursif utama Quick Sort untuk string
void quick_sort_recursive_str(char *arr[], int low, int high) {
     if (low < high) {
        int pi = partition_str(arr, low, high);
        quick_sort_recursive_str(arr, low, pi - 1);
        quick_sort_recursive_str(arr, pi + 1, high);
    }
}


/**
 * @brief Mengurutkan array integer menggunakan algoritma Quick Sort.
 * @prinsip Quick Sort juga merupakan algoritma Divide and Conquer. Ia memilih
 *          sebuah elemen sebagai 'pivot' dan mempartisi array di sekitar pivot
 *          tersebut, sehingga elemen yang lebih kecil dari pivot berada di sebelah
 *          kiri dan yang lebih besar di sebelah kanan. Proses ini diulang secara
 *          rekursif untuk subarray kiri dan kanan. Efisiensi sangat bergantung
 *          pada pemilihan pivot.
 * @param arr Array integer yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void quick_sort_int(int arr[], int n) {
    if (n < 2) return;
    quick_sort_recursive_int(arr, 0, n - 1);
}

/**
 * @brief Mengurutkan array string menggunakan algoritma Quick Sort.
 * @prinsip Sama seperti Quick Sort untuk integer, menggunakan strcmp untuk
 *          membandingkan string saat partisi.
 * @param arr Array pointer ke string (char*) yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void quick_sort_str(char *arr[], int n) {
     if (n < 2) return;
    quick_sort_recursive_str(arr, 0, n - 1);
}


// === Shell Sort ===

/**
 * @brief Mengurutkan array integer menggunakan algoritma Shell Sort.
 * @prinsip Shell Sort adalah perbaikan dari Insertion Sort. Ia membandingkan
 *          elemen-elemen yang berjauhan (dengan jarak atau 'gap' tertentu)
 *          terlebih dahulu, kemudian secara bertahap mengurangi gap hingga
 *          menjadi 1. Saat gap=1, algoritma ini pada dasarnya menjadi Insertion
 *          Sort, tetapi karena elemen-elemen sudah 'hampir' terurut dari
 *          langkah-langkah sebelumnya, prosesnya menjadi lebih cepat.
 *          Menggunakan urutan gap Knuth (h = 3*h + 1).
 * @param arr Array integer yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void shell_sort_int(int arr[], int n) {
    // Mulai dengan gap besar, lalu kurangi gap
    // Menggunakan Knuth's sequence: h = 3*h + 1 (..., 1, 4, 13, 40, ...)
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;
    }

    while (h >= 1) {
        // Lakukan insertion sort untuk gap ini
        for (int i = h; i < n; i++) {
            // Ambil elemen arr[i] untuk disisipkan
            int temp = arr[i];
            int j;
            // Geser elemen-elemen sebelumnya yang berjarak h yang lebih besar
            for (j = i; j >= h && arr[j - h] > temp; j -= h) {
                arr[j] = arr[j - h];
            }
            // Letakkan temp (arr[i] asli) pada lokasi yang benar
            arr[j] = temp;
        }
        // Hitung gap berikutnya
        h = h / 3;
    }
}


/**
 * @brief Mengurutkan array string menggunakan algoritma Shell Sort.
 * @prinsip Sama seperti Shell Sort untuk integer, tetapi menggunakan strcmp
 *          untuk perbandingan saat melakukan 'insertion sort' dengan gap.
 * @param arr Array pointer ke string (char*) yang akan diurutkan.
 * @param n Jumlah elemen dalam array.
 */
void shell_sort_str(char *arr[], int n) {
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;
    }

    while (h >= 1) {
        for (int i = h; i < n; i++) {
            char *temp = arr[i]; // Simpan pointer string
            int j;
            for (j = i; j >= h && strcmp(arr[j - h], temp) > 0; j -= h) {
                arr[j] = arr[j - h]; // Geser pointer
            }
            arr[j] = temp; // Sisipkan pointer
        }
        h = h / 3;
    }
}


#endif // SORTING_ALGORITHMS_H