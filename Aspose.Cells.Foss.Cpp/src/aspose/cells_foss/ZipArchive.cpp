#include "aspose/cells_foss/ZipArchive.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// ZipArchiveEntry
// ---------------------------------------------------------------------------

ZipArchiveEntry::ZipArchiveEntry()
    : _isNull(true)
{
}

ZipArchiveEntry::ZipArchiveEntry(std::string fullName, std::vector<uint8_t> data)
    : _fullName(std::move(fullName))
    , _data(std::move(data))
    , _isNull(false)
{
}

bool ZipArchiveEntry::IsNull() const noexcept
{
    return _isNull;
}

const std::vector<uint8_t>& ZipArchiveEntry::Open() const
{
    return _data;
}

const std::string& ZipArchiveEntry::GetFullName() const noexcept
{
    return _fullName;
}

// ---------------------------------------------------------------------------
// ZipArchive
// ---------------------------------------------------------------------------

ZipArchive::ZipArchive() = default;

static std::string NormalizeUri(std::string_view uri)
{
    std::string result;
    result.reserve(uri.size());
    bool skippedLeadingSlash = false;
    for (size_t i = 0; i < uri.size(); ++i) {
        char c = uri[i];
        if (!skippedLeadingSlash && c == '/') {
            continue;
        }
        skippedLeadingSlash = true;
        if (c == '\\') {
            c = '/';
        }
        result.push_back(c);
    }
    return result;
}

ZipArchiveEntry ZipArchive::GetEntry(std::string_view uri) const
{
    const std::string normalized = NormalizeUri(uri);
    const auto it = _entries.find(normalized);
    if (it == _entries.end()) {
        return ZipArchiveEntry();
    }
    return ZipArchiveEntry(normalized, it->second);
}

void ZipArchive::AddEntry(std::string_view uri, std::vector<uint8_t> data)
{
    const std::string normalized = NormalizeUri(uri);
    _entries[normalized] = std::move(data);
}

size_t ZipArchive::GetEntryCount() const noexcept
{
    return _entries.size();
}

// ---------------------------------------------------------------------------
// CRC-32 (IEEE polynomial)
// ---------------------------------------------------------------------------

namespace {

uint32_t* Crc32Table()
{
    static uint32_t table[256];
    static bool initialized = false;
    if (!initialized) {
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t crc = i;
            for (int j = 0; j < 8; ++j) {
                crc = (crc >> 1) ^ ((crc & 1u) ? 0xEDB88320u : 0u);
            }
            table[i] = crc;
        }
        initialized = true;
    }
    return table;
}

uint32_t ComputeCrc32(const uint8_t* data, size_t length)
{
    const uint32_t* table = Crc32Table();
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < length; ++i) {
        crc = table[(crc ^ data[i]) & 0xFFu] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}

// ---------------------------------------------------------------------------
// Minimal inflate (RFC 1951 DEFLATE decompression)
// ---------------------------------------------------------------------------

class BitReader {
public:
    explicit BitReader(const uint8_t* data, size_t length)
        : _data(data), _length(length)
    {
    }

    uint32_t ReadBits(int count)
    {
        uint32_t value = 0;
        for (int i = 0; i < count; ++i) {
            if (_bytePos >= _length) {
                throw std::runtime_error("DEFLATE: unexpected end of input");
            }
            value |= (static_cast<uint32_t>((_data[_bytePos] >> _bitPos) & 1)) << i;
            ++_bitPos;
            if (_bitPos == 8) {
                _bitPos = 0;
                ++_bytePos;
            }
        }
        return value;
    }

    uint8_t ReadByte()
    {
        AlignToByte();
        if (_bytePos >= _length) {
            throw std::runtime_error("DEFLATE: unexpected end of input");
        }
        return _data[_bytePos++];
    }

    void AlignToByte()
    {
        if (_bitPos > 0) {
            _bitPos = 0;
            ++_bytePos;
        }
    }

private:
    const uint8_t* _data;
    size_t _length;
    size_t _bytePos = 0;
    int _bitPos = 0;
};

// Canonical Huffman decoder with lookup table for fast decoding.
struct HuffmanTable {
    static constexpr int MaxTableBits = 9;
    static constexpr int TableSize = 1 << MaxTableBits;

    uint32_t table[TableSize];
    bool valid;

    // For codes longer than MaxTableBits, store the code lengths and symbol
    // count so the slow decoder can reconstruct canonical codes.
    const uint8_t* lengthsPtr;
    int numSymbols;

    HuffmanTable() : valid(false), lengthsPtr(nullptr), numSymbols(0)
    {
        std::memset(table, 0, sizeof(table));
    }

    static HuffmanTable Build(const uint8_t* lengths, int count)
    {
        HuffmanTable ht;
        ht.lengthsPtr = lengths;
        ht.numSymbols = count;

        int blCount[16];
        std::memset(blCount, 0, sizeof(blCount));
        for (int i = 0; i < count; ++i) {
            if (lengths[i] > 0 && lengths[i] <= 15) {
                ++blCount[lengths[i]];
            }
        }

        int maxLen = 0;
        for (int i = 15; i >= 1; --i) {
            if (blCount[i] > 0) {
                maxLen = i;
                break;
            }
        }
        if (maxLen == 0) {
            ht.valid = true;
            return ht;
        }

        uint32_t nextCode[16];
        std::memset(nextCode, 0, sizeof(nextCode));
        uint32_t code = 0;
        for (int bits = 1; bits <= maxLen; ++bits) {
            code = (code + blCount[bits - 1]) << 1;
            nextCode[bits] = code;
        }

        for (int sym = 0; sym < count; ++sym) {
            int len = lengths[sym];
            if (len == 0) {
                continue;
            }
            uint32_t symCode = nextCode[len]++;

            if (len <= MaxTableBits) {
                uint32_t entry = (static_cast<uint32_t>(sym) << 16)
                               | static_cast<uint32_t>(len);
                int shift = MaxTableBits - len;
                uint32_t base = symCode << shift;
                uint32_t fillCount = 1u << shift;
                for (uint32_t j = 0; j < fillCount; ++j) {
                    ht.table[base + j] = entry;
                }
            }
        }

        ht.valid = true;
        return ht;
    }
};

int DecodeSymbol(BitReader& br, const HuffmanTable& ht)
{
    // Accumulate bits MSB-first (first bit read = MSB of the canonical code)
    // and align to MaxTableBits so the lookup table index matches the
    // MSB-aligned canonical codes stored during Build().
    uint32_t bits = 0;
    for (int i = 0; i < HuffmanTable::MaxTableBits; ++i) {
        bits = (bits << 1) | br.ReadBits(1);
        // After (i+1) bits, 'bits' holds the partial code with the first
        // read bit at position i (MSB-first).  Left-pad to MaxTableBits so
        // the value aligns with the table entries that were stored at
        // symCode << (MaxTableBits - codeLen).
        uint32_t lookupIndex = bits << (HuffmanTable::MaxTableBits - 1 - i);
        uint32_t entry = ht.table[lookupIndex];
        int len = entry & 0xFFFF;
        if (len == i + 1) {
            return static_cast<int>(entry >> 16);
        }
    }

    // Slow path for codes longer than MaxTableBits: reconstruct canonical
    // codes and compare against accumulated bits.
    int blCount[16];
    std::memset(blCount, 0, sizeof(blCount));
    for (int i = 0; i < ht.numSymbols; ++i) {
        if (ht.lengthsPtr[i] > 0 && ht.lengthsPtr[i] <= 15) {
            ++blCount[ht.lengthsPtr[i]];
        }
    }

    uint32_t nextCode[16];
    std::memset(nextCode, 0, sizeof(nextCode));
    uint32_t c = 0;
    for (int b = 1; b <= 15; ++b) {
        c = (c + blCount[b - 1]) << 1;
        nextCode[b] = c;
    }

    // 'bits' already holds the first MaxTableBits bits, MSB-first.
    uint32_t code = bits;
    int codeLen = HuffmanTable::MaxTableBits;

    while (codeLen <= 15) {
        code = (code << 1) | br.ReadBits(1);
        ++codeLen;

        if (codeLen > 15) {
            break;
        }

        // Check all symbols with this code length
        for (int sym = 0; sym < ht.numSymbols; ++sym) {
            if (ht.lengthsPtr[sym] == codeLen) {
                uint32_t symCode = nextCode[codeLen]++;
                if (symCode == code) {
                    return sym;
                }
            }
        }
    }

    throw std::runtime_error("DEFLATE: invalid Huffman code");
}

std::vector<uint8_t> Inflate(const uint8_t* data, size_t length)
{
    BitReader br(data, length);
    std::vector<uint8_t> output;

    uint8_t fixedLitLen[288];
    for (int i = 0; i <= 143; ++i) fixedLitLen[i] = 8;
    for (int i = 144; i <= 255; ++i) fixedLitLen[i] = 9;
    for (int i = 256; i <= 279; ++i) fixedLitLen[i] = 7;
    for (int i = 280; i <= 287; ++i) fixedLitLen[i] = 8;

    uint8_t fixedDist[32];
    for (int i = 0; i < 32; ++i) fixedDist[i] = 5;

    static const int lengthExtraBits[] = {
        0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0
    };
    static const int lengthBase[] = {
        3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258
    };
    static const int distExtraBits[] = {
        0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13
    };
    static const int distBase[] = {
        1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577
    };

    bool bfinal = false;
    while (!bfinal) {
        bfinal = br.ReadBits(1) != 0;
        int btype = static_cast<int>(br.ReadBits(2));

        if (btype == 0) {
            br.AlignToByte();
            uint16_t len = static_cast<uint16_t>(br.ReadBits(8) | (br.ReadBits(8) << 8));
            uint16_t nlen = static_cast<uint16_t>(br.ReadBits(8) | (br.ReadBits(8) << 8));
            (void)nlen;
            for (uint16_t i = 0; i < len; ++i) {
                output.push_back(br.ReadByte());
            }
        } else if (btype == 1 || btype == 2) {
            HuffmanTable litLenTable;
            HuffmanTable distTable;
            uint8_t dynLitLen[288];
            uint8_t dynDist[32];

            if (btype == 1) {
                litLenTable = HuffmanTable::Build(fixedLitLen, 288);
                distTable = HuffmanTable::Build(fixedDist, 32);
            } else {
                int hlit = static_cast<int>(br.ReadBits(5)) + 257;
                int hdist = static_cast<int>(br.ReadBits(5)) + 1;
                int hclen = static_cast<int>(br.ReadBits(4)) + 4;

                static const int codeLenOrder[] = {
                    16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15
                };

                uint8_t codeLengths[19];
                std::memset(codeLengths, 0, sizeof(codeLengths));
                for (int i = 0; i < hclen; ++i) {
                    codeLengths[codeLenOrder[i]] = static_cast<uint8_t>(br.ReadBits(3));
                }

                HuffmanTable codeLenTable = HuffmanTable::Build(codeLengths, 19);

                int totalCodes = hlit + hdist;
                uint8_t allLengths[288 + 32];
                std::memset(allLengths, 0, sizeof(allLengths));

                int pos = 0;
                while (pos < totalCodes) {
                    int sym = DecodeSymbol(br, codeLenTable);
                    if (sym <= 15) {
                        allLengths[pos++] = static_cast<uint8_t>(sym);
                    } else if (sym == 16) {
                        if (pos == 0) {
                            throw std::runtime_error("DEFLATE: invalid repeat code");
                        }
                        int repeat = static_cast<int>(br.ReadBits(2)) + 3;
                        uint8_t prev = allLengths[pos - 1];
                        for (int j = 0; j < repeat && pos < totalCodes; ++j) {
                            allLengths[pos++] = prev;
                        }
                    } else if (sym == 17) {
                        int repeat = static_cast<int>(br.ReadBits(3)) + 3;
                        for (int j = 0; j < repeat && pos < totalCodes; ++j) {
                            allLengths[pos++] = 0;
                        }
                    } else if (sym == 18) {
                        int repeat = static_cast<int>(br.ReadBits(7)) + 11;
                        for (int j = 0; j < repeat && pos < totalCodes; ++j) {
                            allLengths[pos++] = 0;
                        }
                    } else {
                        throw std::runtime_error("DEFLATE: invalid code length symbol");
                    }
                }

                std::memcpy(dynLitLen, allLengths, static_cast<size_t>(hlit));
                std::memcpy(dynDist, allLengths + hlit, static_cast<size_t>(hdist));
                litLenTable = HuffmanTable::Build(dynLitLen, hlit);
                distTable = HuffmanTable::Build(dynDist, hdist);
            }

            for (;;) {
                int sym = DecodeSymbol(br, litLenTable);

                if (sym < 256) {
                    output.push_back(static_cast<uint8_t>(sym));
                } else if (sym == 256) {
                    break;
                } else {
                    int lenIdx = sym - 257;
                    if (lenIdx < 0 || lenIdx > 28) {
                        throw std::runtime_error("DEFLATE: invalid length code");
                    }
                    int lenVal = lengthBase[lenIdx];
                    if (lengthExtraBits[lenIdx] > 0) {
                        lenVal += static_cast<int>(br.ReadBits(lengthExtraBits[lenIdx]));
                    }

                    int distSym = DecodeSymbol(br, distTable);
                    if (distSym < 0 || distSym > 29) {
                        throw std::runtime_error("DEFLATE: invalid distance code");
                    }
                    int distance = distBase[distSym];
                    if (distExtraBits[distSym] > 0) {
                        distance += static_cast<int>(br.ReadBits(distExtraBits[distSym]));
                    }

                    size_t start = output.size() - static_cast<size_t>(distance);
                    for (int j = 0; j < lenVal; ++j) {
                        output.push_back(output[start + static_cast<size_t>(j)]);
                    }
                }
            }
        } else {
            throw std::runtime_error("DEFLATE: reserved block type encountered");
        }
    }

    return output;
}

// ---------------------------------------------------------------------------
// ZIP format byte-level helpers
// ---------------------------------------------------------------------------

uint16_t ReadU16(const uint8_t* p)
{
    return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
}

uint32_t ReadU32(const uint8_t* p)
{
    return static_cast<uint32_t>(p[0])
         | (static_cast<uint32_t>(p[1]) << 8)
         | (static_cast<uint32_t>(p[2]) << 16)
         | (static_cast<uint32_t>(p[3]) << 24);
}

void WriteU16(std::vector<uint8_t>& out, uint16_t value)
{
    out.push_back(static_cast<uint8_t>(value & 0xFF));
    out.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
}

void WriteU32(std::vector<uint8_t>& out, uint32_t value)
{
    out.push_back(static_cast<uint8_t>(value & 0xFF));
    out.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    out.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
}

void WriteBytes(std::vector<uint8_t>& out, const uint8_t* data, size_t length)
{
    out.insert(out.end(), data, data + length);
}

void WriteBytes(std::vector<uint8_t>& out, const std::vector<uint8_t>& data)
{
    out.insert(out.end(), data.begin(), data.end());
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// ZipArchive::FromZipBytes
// ---------------------------------------------------------------------------

ZipArchive ZipArchive::FromZipBytes(const std::vector<uint8_t>& zipData)
{
    ZipArchive archive;

    if (zipData.size() < 22) {
        throw std::runtime_error("ZIP: data too small to be a valid archive");
    }

    // Locate the end of central directory record (EOCD, signature 0x06054b50).
    // The EOCD is near the end; the minimum size is 22 bytes.
    size_t eocdOffset = 0;
    bool found = false;
    size_t searchStart = (zipData.size() >= 65536 + 22)
                       ? zipData.size() - 65557
                       : 0;
    for (size_t i = zipData.size() - 22; i >= searchStart; --i) {
        if (ReadU32(zipData.data() + i) == 0x06054b50u) {
            eocdOffset = i;
            found = true;
            break;
        }
        if (i == 0) break;
    }

    if (!found) {
        throw std::runtime_error("ZIP: end of central directory not found");
    }

    const uint8_t* eocd = zipData.data() + eocdOffset;
    uint16_t totalEntries = ReadU16(eocd + 10);
    uint32_t cdSize = ReadU32(eocd + 12);
    uint32_t cdOffset = ReadU32(eocd + 16);

    if (static_cast<size_t>(cdOffset) + static_cast<size_t>(cdSize) > zipData.size()) {
        throw std::runtime_error("ZIP: central directory extends beyond file");
    }

    const uint8_t* cd = zipData.data() + cdOffset;
    size_t cdPos = 0;

    for (uint16_t entry = 0; entry < totalEntries; ++entry) {
        if (cdPos + 46 > cdSize) {
            throw std::runtime_error("ZIP: central directory truncated");
        }

        const uint8_t* cde = cd + cdPos;
        if (ReadU32(cde) != 0x02014b50u) {
            throw std::runtime_error("ZIP: invalid central directory entry signature");
        }

        uint16_t compressionMethod = ReadU16(cde + 10);
        uint32_t crc32 = ReadU32(cde + 16);
        uint32_t compressedSize = ReadU32(cde + 20);
        uint16_t filenameLength = ReadU16(cde + 28);
        uint16_t extraFieldLength = ReadU16(cde + 30);
        uint16_t fileCommentLength = ReadU16(cde + 32);
        uint32_t localHeaderOffset = ReadU32(cde + 42);

        if (cdPos + 46 + filenameLength > cdSize) {
            throw std::runtime_error("ZIP: central directory filename extends beyond bounds");
        }

        std::string filename(reinterpret_cast<const char*>(cde + 46), filenameLength);
        cdPos += 46 + filenameLength + extraFieldLength + fileCommentLength;

        if (static_cast<size_t>(localHeaderOffset) + 30 > zipData.size()) {
            throw std::runtime_error("ZIP: local file header extends beyond file");
        }

        const uint8_t* lfh = zipData.data() + localHeaderOffset;
        if (ReadU32(lfh) != 0x04034b50u) {
            throw std::runtime_error("ZIP: invalid local file header signature");
        }

        uint16_t lfhFilenameLen = ReadU16(lfh + 26);
        uint16_t lfhExtraLen = ReadU16(lfh + 28);
        uint32_t dataOffset = localHeaderOffset + 30 + lfhFilenameLen + lfhExtraLen;

        if (static_cast<size_t>(dataOffset) + static_cast<size_t>(compressedSize) > zipData.size()) {
            throw std::runtime_error("ZIP: file data extends beyond archive");
        }

        const uint8_t* fileData = zipData.data() + dataOffset;
        std::vector<uint8_t> entryData;

        if (compressionMethod == 0) {
            entryData.assign(fileData, fileData + compressedSize);
        } else if (compressionMethod == 8) {
            entryData = Inflate(fileData, compressedSize);
        } else {
            throw std::runtime_error("ZIP: unsupported compression method");
        }

        uint32_t computedCrc = ComputeCrc32(entryData.data(), entryData.size());
        if (computedCrc != crc32) {
            throw std::runtime_error("ZIP: CRC-32 mismatch");
        }

        std::string normalized = NormalizeUri(filename);
        archive._entries[normalized] = std::move(entryData);
    }

    return archive;
}

// ---------------------------------------------------------------------------
// ZipArchive::ToZipBytes
// ---------------------------------------------------------------------------

std::vector<uint8_t> ZipArchive::ToZipBytes() const
{
    std::vector<uint8_t> out;

    struct CdEntry {
        std::vector<uint8_t> header;
    };
    std::vector<CdEntry> centralDir;

    // Sort entries by name for deterministic output.
    std::vector<std::pair<std::string, const std::vector<uint8_t>*>> sorted;
    sorted.reserve(_entries.size());
    for (const auto& pair : _entries) {
        sorted.emplace_back(pair.first, &pair.second);
    }
    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    for (const auto& pair : sorted) {
        const std::string& name = pair.first;
        const std::vector<uint8_t>& data = *pair.second;

        uint32_t crc = ComputeCrc32(data.data(), data.size());
        uint32_t size = static_cast<uint32_t>(data.size());
        uint32_t offset = static_cast<uint32_t>(out.size());

        // Local file header (STORED compression, method 0)
        WriteU32(out, 0x04034b50u);
        WriteU16(out, 20);
        WriteU16(out, 0);
        WriteU16(out, 0);
        WriteU16(out, 0);
        WriteU16(out, 0);
        WriteU32(out, crc);
        WriteU32(out, size);
        WriteU32(out, size);
        WriteU16(out, static_cast<uint16_t>(name.size()));
        WriteU16(out, 0);
        WriteBytes(out, reinterpret_cast<const uint8_t*>(name.data()), name.size());
        WriteBytes(out, data);

        // Central directory entry
        CdEntry cde;
        WriteU32(cde.header, 0x02014b50u);
        WriteU16(cde.header, 20);
        WriteU16(cde.header, 20);
        WriteU16(cde.header, 0);
        WriteU16(cde.header, 0);
        WriteU16(cde.header, 0);
        WriteU16(cde.header, 0);
        WriteU32(cde.header, crc);
        WriteU32(cde.header, size);
        WriteU32(cde.header, size);
        WriteU16(cde.header, static_cast<uint16_t>(name.size()));
        WriteU16(cde.header, 0);
        WriteU16(cde.header, 0);
        WriteU16(cde.header, 0);
        WriteU16(cde.header, 0);
        WriteU32(cde.header, 0);
        WriteU32(cde.header, offset);
        WriteBytes(cde.header, reinterpret_cast<const uint8_t*>(name.data()), name.size());
        centralDir.push_back(std::move(cde));
    }

    uint32_t cdStart = static_cast<uint32_t>(out.size());
    for (const auto& cde : centralDir) {
        WriteBytes(out, cde.header);
    }
    uint32_t cdSize = static_cast<uint32_t>(out.size()) - cdStart;

    // End of central directory record
    WriteU32(out, 0x06054b50u);
    WriteU16(out, 0);
    WriteU16(out, 0);
    WriteU16(out, static_cast<uint16_t>(sorted.size()));
    WriteU16(out, static_cast<uint16_t>(sorted.size()));
    WriteU32(out, cdSize);
    WriteU32(out, cdStart);
    WriteU16(out, 0);

    return out;
}

}}  // namespace Aspose::Cells_FOSS
