#include "PieceWrapper.h"
#include "Grid.h"

void PieceWrapper::setPosition(Position pos) noexcept {
    piece.setPosition(pos);
    grid->updateToGrid(*this);
}

void PieceWrapper::promote() noexcept {
    piece.promote();
    grid->updateToGrid(*this);
}
