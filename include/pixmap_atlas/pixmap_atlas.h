#pragma once

#include <QPixmap>
#include <string>

namespace gamedev::soulcraft
{
    class PixmapAtlas
    {
        std::map< std::string, QPixmap > palette;
    public:
        explicit PixmapAtlas();
        auto get( const std::string &id ) -> QPixmap&;
        auto add( const std::string &id, const QPixmap &graphic ) -> void;
    };
}


