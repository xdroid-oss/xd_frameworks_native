
/*
 * Copyright 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// We must include cstdio before jpeglib.h. It is a requirement of libjpeg.
#include <cstdio>
extern "C" {
#include <jerror.h>
#include <jpeglib.h>
}
#include <utils/Errors.h>
#include <vector>
namespace android::recoverymap {
/*
 * Encapsulates a converter from JPEG to raw image (YUV420planer or grey-scale) format.
 * This class is not thread-safe.
 */
class JpegDecoder {
public:
    JpegDecoder();
    ~JpegDecoder();
    /*
     * Decompresses JPEG image to raw image (YUV420planer or grey-scale) format. After calling
     * this method, call getDecompressedImage() to get the image.
     * Returns false if decompressing the image fails.
     */
    bool decompressImage(const void* image, int length);
    /*
     * Returns the decompressed raw image buffer pointer. This method must be called only after
     * calling decompressImage().
     */
    const void* getDecompressedImagePtr();
    /*
     * Returns the decompressed raw image buffer size. This method must be called only after
     * calling decompressImage().
     */
    size_t getDecompressedImageSize();
private:
    bool decode(const void* image, int length);
    // Returns false if errors occur.
    bool decompress(jpeg_decompress_struct* cinfo, const uint8_t* dest, bool isSingleChannel);
    bool decompressYUV(jpeg_decompress_struct* cinfo, const uint8_t* dest);
    bool decompressSingleChannel(jpeg_decompress_struct* cinfo, const uint8_t* dest);
    // Process 16 lines of Y and 16 lines of U/V each time.
    // We must pass at least 16 scanlines according to libjpeg documentation.
    static const int kCompressBatchSize = 16;
    // The buffer that holds the compressed result.
    std::vector<JOCTET> mResultBuffer;
};
} /* namespace android  */