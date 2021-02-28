#pragma once
#include <functional>

struct ExtrasRetVal
{

};

// extras function
class MunchkinCard;
class GameState;
using ExtrasFunc = std::function<ExtrasRetVal(GameState&, const MunchkinCard&)>;

namespace extras
{
    
}