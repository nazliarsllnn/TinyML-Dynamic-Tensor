#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// 1. ADIM: TÝP TANIMLARI (Daha Scannable Hale Getirildi)
typedef enum { 
    TYPE_F32 = 0, 
    TYPE_F16 = 1, 
    TYPE_I8  = 2 
} TensorType;

typedef struct {
    TensorType type;
    int length;
    union {
        float* f32;     // 32-bit: Yüksek Hassasiyet
        uint16_t* f16;  // 16-bit: Orta Hassasiyet
        int8_t* i8;     // 8-bit: Düþük Hassasiyet (Maksimum Tasarruf)
    } data;
} Tensor;

// 2. ADIM: AYRINTILI BELLEK YÖNETÝMÝ
Tensor create_tensor(TensorType type, int length) {
    Tensor t;
    t.type = type;
    t.length = length;
    
    // Her tipin byte boyutunu ayrýntýlý hesapla
    size_t element_size = (type == TYPE_F32) ? sizeof(float) : 
                          (type == TYPE_F16) ? sizeof(uint16_t) : sizeof(int8_t);

    // Bellek tahsisi ve NULL kontrolü (Gömülü sistem disiplini)
    void* mem = malloc(length * element_size);
    if (mem == NULL) {
        fprintf(stderr, "Kritik Hata: Bellek ayrilamadi!\n");
        exit(EXIT_FAILURE);
    }
    
    t.data.f32 = (float*)mem; // Union sayesinde baþlangýç adresi eþitlendi
    return t;
}

// 3. ADIM: AYRINTILI QUANTIZATION (NICEMLEME)
// Veriyi -128 ile 127 arasýna güvenli bir þekilde sýkýþtýrýr.
void quantize_to_i8(Tensor* src, Tensor* dest) {
    if (src->type != TYPE_F32 || dest->type != TYPE_I8) return;

    for (int i = 0; i < src->length; i++) {
        float val = src->data.f32[i] * 127.0f;
        
        // Clamping: Deðerin -128 ile 127 dýþýna çýkmasýný engeller
        if (val > 127.0f) val = 127.0f;
        if (val < -128.0f) val = -128.0f;
        
        dest->data.i8[i] = (int8_t)val;
    }
}

// 4. ADIM: TEKNÝK RAPORLAMA (Videoda burayý göster!)
void print_technical_report(Tensor* t, const char* label) {
    size_t b_per_e = (t->type == TYPE_F32) ? 4 : (t->type == TYPE_F16 ? 2 : 1);
    printf(">> [%s] RAPORU\n", label);
    printf("   - Veri Tipi    : %s\n", (t->type == TYPE_F32 ? "Float32" : (t->type == TYPE_F16 ? "Float16" : "Int8")));
    printf("   - Toplam Bellek: %zu Byte\n", t->length * b_per_e);
    printf("   - Veriler      : ");
    
    for (int i = 0; i < t->length; i++) {
        if (t->type == TYPE_F32) printf("%.2f ", t->data.f32[i]);
        else if (t->type == TYPE_F16) printf("%u ", t->data.f16[i]);
        else printf("%d ", t->data.i8[i]);
    }
    printf("\n\n");
}

int main() {
    int size = 5;
    printf("--- TINYML TENSOR OPTIMIZASYON DEMOSU ---\n\n");

    // Orijinal Veri Seti (Örn: Bir ses sensöründen gelen veriler)
    Tensor f32_t = create_tensor(TYPE_F32, size);
    float sensor_data[] = {0.15f, -0.88f, 1.20f, -0.05f, 0.50f};
    for(int i=0; i<size; i++) f32_t.data.f32[i] = sensor_data[i];

    // Dönüþüm Tensörleri
    Tensor f16_t = create_tensor(TYPE_F16, size);
    Tensor i8_t = create_tensor(TYPE_I8, size);

    // Ýþlemler
    for(int i=0; i<size; i++) f16_t.data.f16[i] = (uint16_t)(f32_t.data.f32[i] * 1000);
    quantize_to_i8(&f32_t, &i8_t);

    // Teknik Sunum
    print_technical_report(&f32_t, "ORIJINAL (STANDART)");
    print_technical_report(&f16_t, "HALF-PRECISION (TASARRUF)");
    print_technical_report(&i8_t, "QUANTIZED (MAX TASARRUF)");

    // Temizlik
    free(f32_t.data.f32);
    free(f16_t.data.f16);
    free(i8_t.data.i8);

    return 0;
}
