Nama: Aska Shahira
NPM: 2308107010075

## Deskripsi Tugas

Proyek ini merupakan implementasi dan analisis performa yang dilakukan sebagai bagian dari Tugas 4 mata kuliah Struktur Data dan Algoritma. Tujuan utama dari tugas ini adalah:

1.  Mengevaluasi performa enam algoritma pengurutan (sorting) berbeda dari segi **waktu eksekusi** dan **penggunaan memori puncak**.
2.  Menganalisis bagaimana algoritma-algoritma tersebut menangani dataset berskala besar, baik berupa **angka integer acak** maupun **kata string acak**.
3.  Memahami kompleksitas dan efisiensi praktis dari setiap algoritma ketika dihadapkan pada jumlah data yang bervariasi (hingga 2 juta elemen).
4.  Merancang eksperimen, mengumpulkan data hasil eksekusi program, dan menyusun laporan yang berisi analisis serta kesimpulan berdasarkan hasil pengujian.

### Algoritma yang Diuji

Algoritma pengurutan yang diimplementasikan dan dianalisis dalam proyek ini adalah:

*   Bubble Sort
*   Selection Sort
*   Insertion Sort
*   Merge Sort
*   Quick Sort
*   Shell Sort

---

## Struktur Direktori dan File

Repositori ini berisi file-file berikut:

*   `generate_data.c`: Kode sumber dalam bahasa C untuk membangkitkan file data uji (`data_angka.txt` dan `data_kata.txt`) yang berisi angka dan kata acak.
*   `sorting_algorithms.h`: File header C yang berisi implementasi keenam algoritma sorting. Setiap algoritma diimplementasikan sebagai fungsi terpisah untuk tipe data integer dan string (`char *`), disertai komentar penjelasan prinsip kerjanya.
*   `main.c`: Kode sumber program utama dalam bahasa C yang digunakan untuk melakukan eksperimen. Program ini:
    *   Membaca data (angka atau kata) dari file sesuai ukuran yang ditentukan.
    *   Menjalankan setiap algoritma sorting pada data tersebut.
    *   Mengukur waktu eksekusi menggunakan `clock()`.
    *   Mengukur penggunaan memori puncak menggunakan API spesifik platform (`GetProcessMemoryInfo` di Windows, `getrusage` di Linux/macOS).
    *   Mencetak hasil pengukuran ke konsol standar.
*   `Aska Shahira_2308107010075_Tugas4.pdf`: Laporan akhir eksperimen dalam format PDF, berisi deskripsi detail, tabel hasil, grafik perbandingan, analisis, dan kesimpulan. *(Pastikan nama file ini sesuai dengan file laporan Anda)*.
*   `README.md`: File ini, memberikan gambaran umum tentang proyek.
