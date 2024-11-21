/**
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
#ifndef _TARS2JS_H
#define _TARS2JS_H
#include "parse.h"
//#include "util/tc_md5.h"
#include "util/tc_file.h"
//#include "util/tc_common.h"

class Tars2Js
{
public:
    void setBaseDir(const string &dir)
    {
        _baseDir = dir;
    }

    void setBasePackage(const string &prefix)
    {
        _packagePrefix = prefix;
        if (_packagePrefix.length() != 0 && _packagePrefix.substr(_packagePrefix.length()-1, 1) != ".")
        {
            _packagePrefix += ".";
        }
    }

public:
    string _packagePrefix;
    string _baseDir;

    string toFunctionName(const TypeIdPtr & pPtr, const std::string &sAction);

	string getDataType(const TypePtr & pPtr);

	string getDefault(const TypeIdPtr & pPtr, const std::string &sDefault);

	string generateJS(const ConstPtr &pPtr, const std::string &sNamespace);

	string generateJS(const EnumPtr &pPtr, const std::string &sNamespace);

    string generateJS(const StructPtr & pPtr, const NamespacePtr &nPtr);

    string generateJS(const NamespacePtr &pPtr);

    void generateJS(const ContextPtr &pPtr) ;
	void createFile(const string &file);

};

#endif

