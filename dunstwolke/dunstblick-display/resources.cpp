#include "resources.hpp"

BitmapResource::BitmapResource(sdl2::texture && _tex) : texture(std::move(_tex)) {}

LayoutResource::LayoutResource(const uint8_t * data, size_t length) : layout_data(data, data + length) {}

InputStream LayoutResource::get_stream() const
{
    return InputStream{layout_data.data(), layout_data.size()};
}

DrawingResource::DrawingResource() {}
