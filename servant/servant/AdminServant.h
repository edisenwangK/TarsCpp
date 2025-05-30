﻿/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */
#pragma once

#include "servant/AdminF.h"

namespace tars
{

class Current;
class Application;

/**
 * 管理Servant
 */
class SVT_DLL_API AdminServant : public AdminF
{
public:
    /**
     * 构造函数
     * @param pEpollServer
     */
    AdminServant();

    /**
     * 析构函数
     */
    virtual ~AdminServant();

    /**
     * 初始化
     * @return int
     */
    virtual void initialize();

    /**
     * 退出
     */
    virtual void destroy();

    /**
     * 关闭服务
     * @param current
     */
    void shutdown(CurrentPtr current);

    /**
     * 通知
     * @param command
     */
    string notify(const string &command, CurrentPtr current);

    /**
     * 获取服务的pid
     */
    tars::Int64 getPid(CurrentPtr current);
};
////////////////////////////////////////////////////////////////
}

