#include "Inventory.h"

Inventory::Inventory()
    : block(ROCK) 
{
}

void Inventory::setNextBlock() {
    block++;
    if (block == LAST_BLOCK_TYPE)
        block = 0;
}

void Inventory::setPreviousBlock() {
    block--;

    if (block == 0)
        block = LAST_BLOCK_TYPE - 1;
}

Block Inventory::getCurrentBlock() const {
    return this->block;
}

const char * Inventory::getCurrentBlockName() const
{
    switch (this->block)
    {
        case AIR: return "air";
        case ROCK: return "rock";
        case GEMS: return "gems";
        case DIRT: return "dirt";
        case GRASS: return "grass";
        case LAVA: return "lava";
        case WATER: return "water";
        case SAND: return "sand";
        case BEDROCK: return "bedrock";
        case RED: return "debug";
    };
    return "unknown block";
}
