#include "tile.h"

namespace gamedev::soulcraft
{
    Tile::Tile( size_t x,
                size_t y,
                const std::string &tilePaletteGraphicIdParam ) :
                    tilePaletteGraphicId( tilePaletteGraphicIdParam )
    {
        this->x = x;
        this->y = y;
    }

    auto Tile::setGraphicId( const std::string &tilePaletteGraphicIdParam ) -> void
    {
        tilePaletteGraphicId = tilePaletteGraphicIdParam;
    }

    auto Tile::getGraphicId() -> const std::string&
    {
        return tilePaletteGraphicId;
    }
}
