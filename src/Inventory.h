#ifndef MINECRAP_INVENTORY_H
#define MINECRAP_INVENTORY_H

#include "Block.h"

class Inventory
{
    Block block;

    public:
        Inventory();

        void setNextBlock();
        void setPreviousBlock();

        Block getCurrentBlock() const;
        const char *getCurrentBlockName() const;
};

#endif
