#include "bmp_image.h"
#include "pack_defines.h" // макросы для отключения padding у структур

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

// Заголовок файла
PACKED_STRUCT_BEGIN BitmapFileHeader {
    BitmapFileHeader() = default;
    BitmapFileHeader(int width, int height, int stride)
        : bfType{'B', 'M'}
        , bfSize(static_cast<uint32_t>(14 + 40 + stride * height))
        , bfReserved(0)
        , bfOffBits(14 + 40) {
    }

    char bfType[2]; // 2 bytes
    uint32_t bfSize; // 4 bytes
    uint32_t bfReserved; // 4 bytes
    uint32_t bfOffBits; // 4 bytes
/*  
    Подпись — 2 байта. Символы BM.
    Суммарный размер заголовка и данных — 4 байта, беззнаковое целое.
    Зарезервированное пространство — 4 байта, заполненные нулями.
    Отступ данных от начала файла — 4 байта, беззнаковое целое. 
*/
}
PACKED_STRUCT_END

// Заголовок изображения
PACKED_STRUCT_BEGIN BitmapInfoHeader {
    BitmapInfoHeader() = default;
    BitmapInfoHeader(int width, int height, int stride)
        : biSize(40)
        , biWidth(width)
        , biHeight(height)
        , biPlanes(1)
        , biBitCount(24)
        , biCompression(0)
        , biSizeImage(static_cast<uint32_t>(stride * height))
        , biXPelsPerMeter(11811)
        , biYPelsPerMeter(11811)
        , biClrUsed(0)
        , biClrImportant(0x1000000) {
    }

    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    int32_t biClrUsed;
    int32_t biClrImportant;
/* 
    Размер заголовка — 4 байта, беззнаковое целое. Учитывается только размер второй части заголовка.
    Ширина изображения в пикселях — 4 байта, знаковое целое.
    Высота изображения в пикселях — 4 байта, знаковое целое.
    Количество плоскостей — 2 байта, беззнаковое целое. В нашем случае всегда 1 — одна RGB плоскость.
    Количество бит на пиксель — 2 байта, беззнаковое целое. В нашем случае всегда 24.
    Тип сжатия — 4 байта, беззнаковое целое. В нашем случае всегда 0 — отсутствие сжатия.
    Количество байт в данных — 4 байта, беззнаковое целое. Произведение отступа на высоту.
    Горизонтальное разрешение, пикселей на метр — 4 байта, знаковое целое. Нужно записать 11811, что примерно соответствует 300 DPI.
    Вертикальное разрешение, пикселей на метр — 4 байта, знаковое целое. Нужно записать 11811, что примерно соответствует 300 DPI.
    Количество использованных цветов — 4 байта, знаковое целое. Нужно записать 0 — значение не определено.
    Количество значимых цветов — 4 байта, знаковое целое. Нужно записать 0x1000000. 
*/
}
PACKED_STRUCT_END

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    constexpr int BYTES_PER_PIXEL = 3; // 24-битный BMP (R, G, B)
    constexpr int ALIGNMENT = 4; // Выравнивание по 4 байта
    return ALIGNMENT * ((w * BYTES_PER_PIXEL + (ALIGNMENT - 1)) / ALIGNMENT);
}

bool SaveBMP(const Path& file, const Image& image) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    const int stride = GetBMPStride(width);

    BitmapFileHeader file_header{width, height, stride};
    BitmapInfoHeader info_header{width, height, stride};

    ofstream out(file, ios::binary);
    out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));  
    out.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));  
    vector<char> buff(stride);
        
    for (int y = height - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);

        for (int x = 0; x < width; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);   
            buff[x * 3 + 1] = static_cast<char>(line[x].g);  
            buff[x * 3 + 2] = static_cast<char>(line[x].r);  
        }
        out.write(buff.data(), buff.size());
    }

    return out.good();
}

Image LoadBMP(const Path& file) {
    ifstream ifs(file, ios::binary);
    if (!ifs) {
        return {};
    }

    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;
    ifs.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    ifs.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    // Проверяем сигнатуру "BM"
    if (file_header.bfType[0] != 'B' || file_header.bfType[1] != 'M') {
        return {};
    }

    int32_t width = info_header.biWidth;
    int32_t height = info_header.biHeight;
    if (width <= 0 || height == 0) {
        return {};
    }
    const int stride = GetBMPStride(width);
    if (!ifs.good()) {
        return {};
    }

    Image result(width, abs(height), Color::Black());
    std::vector<char> buff(stride);

    for (int y = height - 1; y >= 0; --y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), stride);

        for (int x = 0; x < width; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    if (!ifs.good()) {  // Финальная проверка состояния потока после чтения всего файла
        return {};
    }

    return result;
}

}  // namespace img_lib