# Gömülü Sistemler İçin Dinamik Tensör Kütüphanesi

Bu proje, kaynakları kısıtlı mikrodenetleyicilerde (ESP32, Arduino vb.) Yapay Zeka modellerini (TinyML) çalıştırmak üzere optimize edilmiş bir veri yapısı sunar.

## 🛠️ Teknik Özellikler
- **Bellek Yönetimi:** C dilindeki `union` yapısı kullanılarak aynı bellek alanı 3 farklı veri tipi (Float32, Float16, Int8) için paylaştırılmıştır.
- **Optimizasyon:** Quantization (Nicemleme) işlemi ile bellek kullanımı %75 oranında azaltılmıştır.
- **Güvenlik:** Dinamik bellek tahsisi sırasında `malloc` kontrolü ve değer aşımını önlemek için `clamping` mekanizması uygulanmıştır.

## 🧠 Agentic Kodlama Yaklaşımı
Bu uygulama geliştirilirken **Gemini 3 Flash** modeli bir "AI Agent" olarak kullanılmıştır. Geliştirme süreci şu aşamalardan oluşur:
1. **Planlama:** Bellek kısıtları ve veri yapısı mimarisi belirlendi.
2. **İteratif Kodlama:** Fonksiyonlar modüler olarak yazıldı ve hata denetimleri eklendi.
3. **Debugger Doğrulama:** Bellek adreslerinin `union` yapısı dahilinde çakıştığı doğrulanarak RAM verimliliği kanıtlandı.

## 🚀 Çalıştırma
Kodu derlemek ve test etmek için:
```bash
gcc main.c -o tensor_demo
./tensor_demo
