#pragma once

#include <string>

namespace gamedev::soulcraft
{
    class Tile
    {
        size_t x{ 0 };
        size_t y{ 0 };
        std::string tilePaletteGraphicId;
    public:
        explicit Tile( size_t x,
                       size_t y,
                       const std::string &tilePaletteGraphicIdParam );

        auto setGraphicId( const std::string &tilePaletteGraphicIdParam ) -> void;
        auto getGraphicId() -> const std::string&;
        bool marked { false };
    };
}
