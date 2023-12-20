#pragma once

namespace ax
{
    enum AX_SOLUTION_ERR
    {
        AX_SUCCESS = 0,
        AX_ERR_QUEUE_EMPTY = -1000 - 1,
        AX_ERR_QUEUE_FULL  = -1000 - 2,
        AX_ERR_NULL_PTR    = -1000 - 3,
        AX_ERR_ILLEGAL_PARAM = -1000 - 4
    };
}