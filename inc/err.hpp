/**************************************************************************************************
 *
 * Copyright (c) 2019-2023 Axera Semiconductor (Shanghai) Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor (Shanghai) Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor (Shanghai) Co., Ltd.
 *
 **************************************************************************************************/

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