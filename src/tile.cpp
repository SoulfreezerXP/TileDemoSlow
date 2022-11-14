#include "tile.h"

Tile::Tile( size_t x,
            size_t y,
            const std::string &tilePaletteGraphicIdParam ) :
                tilePaletteGraphicId( tilePaletteGraphicIdParam )
{
    this->x = x;
    this->y = y;
}

auto Tile::setGraphicId( const std::string &tilePaletteGraphicIdParam )
{
    tilePaletteGraphicId = tilePaletteGraphicIdParam;
}

auto Tile::getGraphicId() -> const std::string&
{
    return tilePaletteGraphicId;
}
