# Gate-Ngaliyan
# Sistem Gerbang Otomatis Terjadwal Berbasis Arduino

Proyek ini adalah sistem pengendali gerbang otomatis menggunakan Arduino yang dirancang untuk membuka dan menutup gerbang berdasarkan jadwal waktu tertentu. Sistem ini juga dilengkapi dengan fitur keamanan menggunakan sensor inframerah untuk mencegah gerbang menutup jika ada objek di bawahnya.

## Fitur Utama

* **Penjadwalan Otomatis:** Gerbang akan naik (terbuka) secara otomatis pada pukul **22.00 WIB** dan turun (tertutup) pada pukul **05.00 WIB**.
* **Pergerakan Presisi 2 Meter:** Menggunakan sistem *Limit Switch* di titik batas atas (2 meter) dan batas bawah (0 meter) untuk menghentikan motor secara presisi.
* **Keamanan Sensor IR:** Jika pada pukul 05.00 gerbang sedang bergerak turun dan mendeteksi adanya objek (manusia/kendaraan) yang memotong sinar inframerah, gerbang akan **berhenti sementara**. Gerbang akan melanjutkan penutupan otomatis setelah objek menyingkir.

## Perangkat Keras yang Dibutuhkan

1.  **Arduino Uno / Nano** (atau mikrokontroler kompatibel lainnya).
2.  **Modul RTC DS3231** (Untuk menjaga waktu tetap akurat meskipun alat dimatikan).
3.  **Sensor Inframerah (IR Obstacle Sensor)** (Aktif LOW).
4.  **Driver Motor** (Misal: L298N atau Modul Relay, disesuaikan dengan tipe motor gerbang).
5.  **Motor DC / Gearbox Motor** untuk menggerakkan gerbang.
6.  **2x Limit Switch** (Sakelar pembatas untuk posisi atas dan bawah).

## Konfigurasi Pin (Wiring)

| Komponen | Pin Arduino | Keterangan |
| :--- | :--- | :--- |
| **RTC DS3231 (SDA)** | A4 | Komunikasi I2C untuk waktu |
| **RTC DS3231 (SCL)** | A5 | Komunikasi I2C untuk waktu |
| **Pin Motor Naik** | Pin 8 | Output ke Driver Motor / Relay 1 |
| **Pin Motor Turun** | Pin 9 | Output ke Driver Motor / Relay 2 |
| **Sensor IR** | Pin 2 | Input Sensor Inframerah |
| **Limit Switch Atas** | Pin 3 | Posisi 2 Meter (Menggunakan internal Pull-Up) |
| **Limit Switch Bawah**| Pin 4 | Posisi 0 Meter (Menggunakan internal Pull-Up) |

## Penjelasan Logika Kode

Kode program ini tidak menggunakan *delay()* yang memblokir, melainkan menggunakan mekanisme *State Machine* (Mesin Status) dengan empat status utama: `TERBUKA`, `TERTUTUP`, `MEMBUKA`, dan `MENUTUP`.

1.  **Inisialisasi (`setup`):** Sistem membaca modul RTC. Pin limit switch diatur menggunakan `INPUT_PULLUP` sehingga kita tidak perlu memasang resistor eksternal. (Saat *limit switch* ditekan, ia akan terhubung ke Ground / bernilai LOW).
2.  **Pengecekan Waktu (`loop`):** Sistem terus-menerus membaca waktu dari RTC. Jika waktu menunjukkan 22:00 dan gerbang sedang tertutup, status diubah menjadi `MEMBUKA`. Jika 05:00 dan gerbang terbuka, status menjadi `MENUTUP`.
3.  **Proses Membuka (`MEMBUKA`):** Motor digerakkan ke arah atas. Jika gerbang menyentuh *Limit Switch Atas* (berada pada jarak 2 meter, nilai pin menjadi LOW), maka motor dimatikan dan status berubah menjadi `TERBUKA`.
4.  **Proses Menutup & Sensor IR (`MENUTUP`):** Sistem mengecek *Sensor IR* terlebih dahulu. Jika sensor membaca LOW (ada objek), maka motor langsung dihentikan sementara waktu (di-jeda). Jika tidak ada objek, motor digerakkan ke bawah hingga menyentuh *Limit Switch Bawah*. Setelah menyentuh bawah (nilai pin menjadi LOW), motor mati dan status menjadi `TERTUTUP`.

## Cara Penggunaan

1.  Rangkai komponen sesuai dengan tabel Konfigurasi Pin di atas.
2.  Pastikan jarak antara Limit Switch Bawah dan Limit Switch Atas secara fisik adalah tepat **2 meter**.
3.  Buka Arduino IDE, instal *library* **RTClib** dari Adafruit melalui *Library Manager*.
4.  Pastikan jam pada modul RTC DS3231 Anda sudah diatur ke Waktu Indonesia Barat (WIB) sebelum menjalankan kode ini.
5.  *Compile* dan *Upload* kode `main.ino` ke *board* Arduino Anda.
