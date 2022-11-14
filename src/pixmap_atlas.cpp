#include "pixmap_atlas.h"

PixmapAtlas::PixmapAtlas()
{}

auto PixmapAtlas::get( const std::string &id ) -> QPixmap&
{
    return palette.at( id );
}

auto PixmapAtlas::add( const std::string &id, const QPixmap &graphic ) -> void
{
    palette.emplace( id, graphic );
}

