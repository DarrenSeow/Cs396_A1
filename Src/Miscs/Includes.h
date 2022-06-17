#pragma once
#include <Windows.h>
#include <array>
#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <span>
#include <utility>
#include <assert.h>
#include <memoryapi.h>
#include <algorithm>
#include <tuple>
#include <concepts>

#include "Utility.h"
#include "Tools.h"

#include "..\Component\ComponentInfo.h"

#include "..\Component\ComponentManager.h"

#include "..\Entity\Bits.h"
#include "Query.h"
#include "..\Entity\Pool.h"
#include "..\Entity\Archetype.h"
#include "..\Entity\Entity.h"
#include "..\Entity\EntityManager.h"
