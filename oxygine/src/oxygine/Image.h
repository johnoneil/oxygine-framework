#pragma once
#include "oxygine-include.h"
#include "core/Texture.h"
#include "core/file.h"

namespace oxygine
{
    enum ImageType
    {
        IT_UNKNOWN,
        IT_PNG,
        IT_PKM,
        IT_PVR2,
        IT_PVR,
        IT_TGA,
        IT_JPEG
    };

    bool getImageInfo(const void* data, size_t size, const char* name, ImageType& type, int& width, int& height);

    DECLARE_SMART(Image, spImage);

    class Image : public Texture
    {
    public:
        Image();
        ~Image();

        bool init(file::buffer& bf, bool premultiplied = false, TextureFormat format = TF_UNDEFINED);
        void init(const ImageData& src);
        void init(int w, int h, TextureFormat Format);

        void cleanup();
        void convert(Image& dest, TextureFormat format);
        //void convert2pot(MemoryTexture &dest);

        OXYGINE_DEPRECATED
        void fill_zero();

        void fillZero() { fill(0); }
        void fill(unsigned int val);

        unsigned int    getSizeVRAM() const {return (unsigned int)_buffer.size();}
        int             getWidth() const;
        int             getHeight() const;
        const Point&    getSize() const;
        TextureFormat   getFormat() const;

        ImageData   lock(lock_flags f = 0, const Rect* pRect = 0);
        ImageData   lock(const Rect* pRect);
        ImageData   lock(const Rect& pRect);
        ImageData   lock(int x, int y, int w, int h);
        ImageData   lock(int x, int y);

        void        unlock();
        void        toPOT(Image& dest);

        void        updateRegion(int x, int y, const ImageData& data);
        void        apply(const Rect*);

        void        swap(Image& r);

    private:
        ImageData _image;
        size_t _offset;//buffer offset
        #if 1
        struct Buffer
        {
            unsigned char* _buffer = 0;
            size_t _sz = 0;
            size_t size() const { return _sz;}

            void reserve(size_t sz)
            {
                _sz = sz;
                if(_buffer) delete [] _buffer;
                _buffer = new unsigned char[_sz];
            }
            ~Buffer()
            {
                if(_buffer) delete [] _buffer;
                _buffer = 0;
                _sz = 0;
            }
            void clear()
            {
                _sz = 0;
                if(_buffer)
                {
                    delete [] _buffer;
                    _buffer = 0;
                }
            }
            bool empty() const {return _sz == 0;}
            unsigned char& front() const {return _buffer[0];};
            void swap (std::vector<unsigned char>& x)
            {
                std::vector<unsigned char> tmp;
                x.swap(tmp);
                x.resize(_sz);
                for(int i=0;i<_sz;++i)
                {
                    x[i] = _buffer[i];
                }
                delete [] _buffer;
                _sz = tmp.size();
                _buffer = new unsigned char[_sz];
                for(int i=0;i<tmp.size();++i)
                {
                    _buffer[i] = tmp[i];
                }
            }
            void resize(size_t sz)
            {
                _sz = sz;
                if(_buffer)
                {
                    delete [] _buffer;
                    _buffer = 0;
                }
                _buffer = new unsigned char[sz];
            }

        };
        Buffer _buffer;
        #else
        std::vector<unsigned char> _buffer;
        #endif
    };

    typedef bool (*cbLoadImageFromBuffer)(Image& mt, void* data, int nSize, bool premultiplied, TextureFormat format);
    void setJpegImageLoader(cbLoadImageFromBuffer);
    void setPngImageLoader(cbLoadImageFromBuffer);
    void setCustomImageLoader(cbLoadImageFromBuffer);
}