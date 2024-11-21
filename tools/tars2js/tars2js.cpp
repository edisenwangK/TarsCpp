#include "tars2js.h"

#define TAB g_parse->getTab()
#define INC_TAB g_parse->incTab()
#define DEL_TAB g_parse->delTab()

std::string Tars2Js::toFunctionName(const TypeIdPtr & pPtr, const std::string &sAction)
{
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
	if (bPtr)
	{
		switch (bPtr->kind())
		{
			case Builtin::KindBool	:	return sAction + "Boolean";
			case Builtin::KindString:	return sAction + "String";
			case Builtin::KindByte: 	return sAction + "Int8";
			case Builtin::KindShort:	return sAction + (bPtr->isUnsigned()?"UInt8":"Int16");
			case Builtin::KindInt:		return sAction + (bPtr->isUnsigned()?"UInt16":"Int32");
			case Builtin::KindLong: 	return sAction + (bPtr->isUnsigned()?"UInt32":"Int64");
			case Builtin::KindFloat:	return sAction + "Float";
			case Builtin::KindDouble:	return sAction + "Double";
			default:					assert(false);
		}
	}

	VectorPtr vPtr = VectorPtr::dynamicCast(pPtr->getTypePtr());
	if (vPtr)
	{
		BuiltinPtr vbPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
		if (vbPtr && vbPtr->kind() == Builtin::KindByte)
		{
			return sAction + "Bytes";
		}

		return sAction + "Vector";
	}

	StructPtr sPtr = StructPtr::dynamicCast(pPtr->getTypePtr());
	if (sPtr)
	{
		return sAction + "Struct";
	}

	EnumPtr ePtr = EnumPtr::dynamicCast(pPtr->getTypePtr());
	if (ePtr)
	{
		return sAction + "Int32";
	}

	MapPtr mPtr = MapPtr::dynamicCast(pPtr->getTypePtr());
	if (mPtr)
	{
		return sAction + "Map";
	}

	return "";
}

string Tars2Js::generateJS(const EnumPtr &pPtr, const std::string &sNamespace)
{
	ostringstream s;
    s << TAB << sNamespace << "." << pPtr->getId() << " = " << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    //??Ô±??Á¿
    int nenum = -1;
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    for (size_t i = 0; i < member.size(); i++)
    {
		if (member[i]->hasDefault())
		{
			nenum = tars::TC_Common::strto<int>(member[i]->def());
		}
		else
		{
			nenum++;
		}

        s << TAB << member[i]->getId()<< " : " << tars::TC_Common::tostr(nenum) << (i == member.size() - 1?"":",") << endl;
    }

	DEL_TAB;
	s << TAB << "};" << endl;

    return s.str();
}

string Tars2Js::generateJS(const ConstPtr &pPtr, const std::string &sNamespace)
{
    ostringstream s;

    if (pPtr->getConstGrammarPtr()->t == ConstGrammar::STRING)
    {
        string tmp = tars::TC_Common::replace(pPtr->getConstGrammarPtr()->v, "\"", "\\\"");
        s << TAB << sNamespace << "." << pPtr->getTypeIdPtr()->getId() << " = \"" << tmp << "\";";
    }
    else
    {
        s << TAB << sNamespace << "." << pPtr->getTypeIdPtr()->getId() << " = " << pPtr->getConstGrammarPtr()->v << ";";
    }

    return s.str();
}

string Tars2Js::getDataType(const TypePtr & pPtr)
{
    cout << "IS:" << pPtr->isArray() << endl;

	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
	if (bPtr)
	{
		switch (bPtr->kind())
		{
			case Builtin::KindBool	 :	return "new Tars.BOOLEAN()";
			case Builtin::KindString :	return "new Tars.STRING()";
			case Builtin::KindShort  :	return "new Tars.INT16()";
			case Builtin::KindInt    :	return "new Tars.INT32()";
			case Builtin::KindLong   : 	return "new Tars.INT64()";
			case Builtin::KindFloat  :	return "new Tars.FLOAT()";
			case Builtin::KindDouble :	return "new Tars.DOUBLE()";
			default:					assert(false);
		}
	}

	VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
	if (vPtr)
	{
		BuiltinPtr vbPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
		if (vbPtr && vbPtr->kind() == Builtin::KindByte)
		{
			return "new Tars.BinBuffer()";
		}

		return "new Tars.Vector(" + getDataType(vPtr->getTypePtr()) + ")";
	}

	StructPtr sPtr = StructPtr::dynamicCast(pPtr);
	if (sPtr)
	{
		return "new " + tars::TC_Common::replace(sPtr->getSid(), "::", ".") + "()";
	}

	MapPtr mPtr = MapPtr::dynamicCast(pPtr);
	if (mPtr)
	{
		return "new Tars.Map(" + getDataType(mPtr->getLeftTypePtr()) + ", " + getDataType(mPtr->getRightTypePtr()) + ")";
	}

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr)
    {
        return "new Tars.INT32()";
    }

	assert(false);
	return "";
}


string Tars2Js::getDefault(const TypeIdPtr & pPtr, const std::string &sDefault)
{
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
	if (bPtr)
	{
		switch (bPtr->kind())
		{
			case Builtin::KindBool		: return sDefault.empty()?"false":sDefault;
			case Builtin::KindString	: return "\"" + tars::TC_Common::replace(sDefault, "\"", "\\\"") +  "\"";
			case Builtin::KindByte		:
			case Builtin::KindShort		:
			case Builtin::KindInt		:
			case Builtin::KindLong		: return sDefault.empty()?"0":sDefault;
			case Builtin::KindFloat		:
			case Builtin::KindDouble	: return sDefault.empty()?"0.0":sDefault;
			default						: assert(false);
		}
	}

	VectorPtr vPtr = VectorPtr::dynamicCast(pPtr->getTypePtr());
	if (vPtr)
	{
		BuiltinPtr vbPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
		if (vbPtr && vbPtr->kind() == Builtin::KindByte)
		{
			return "new Tars.BinBuffer()";
		}

		return "new Tars.Vector(" + getDataType(vPtr->getTypePtr()) + ")";
	}

	StructPtr sPtr = StructPtr::dynamicCast(pPtr->getTypePtr());
	if (sPtr)
	{
		return "new " + tars::TC_Common::replace(sPtr->getSid(), "::", ".") + "()";
	}

	MapPtr mPtr = MapPtr::dynamicCast(pPtr->getTypePtr());
	if (mPtr)
	{
		return "new Tars.Map(" + getDataType(mPtr->getLeftTypePtr()) + ", " + getDataType(mPtr->getRightTypePtr()) + ")";
	}

	EnumPtr ePtr = EnumPtr::dynamicCast(pPtr->getTypePtr());
	if (ePtr)
	{
		return sDefault.empty()?"0":(tars::TC_Common::replace(ePtr->getSid(), "::", ".") + "." + sDefault);
	}

	return sDefault;
}

string Tars2Js::generateJS(const StructPtr & pPtr, const NamespacePtr & nPtr)
{
    ostringstream s;
    vector<string> key = pPtr->getKey();
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();

    cout << pPtr->getId() << endl;


    s << TAB << nPtr->getId() << "." << pPtr->getId() << " = function()"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for (size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "this." << member[i]->getId() << " = " << getDefault(member[i], member[i]->def()) << ";" << endl;
    }

	DEL_TAB;
	s << TAB << "};" << endl;

	std::string sProto = tars::TC_Common::replace(pPtr->getSid(), "::", ".") + ".prototype.";
	s << TAB << sProto << "_clone = function () { return new " + tars::TC_Common::replace(pPtr->getSid(), "::", ".") + "(); }" << endl;
	s << TAB << sProto << "_write = function (os, tag, value) { os.writeStruct(tag, value); }" << endl;
	s << TAB << sProto << "_read  = function (is, tag, def) { return is.readStruct(tag, true, def); }" << endl << endl;

    //writeTo()
    s << TAB << nPtr->getId() << "." << pPtr->getId() << ".prototype.writeTo = function (os)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for (size_t i = 0; i < member.size(); i++)
    {
        std::string sFuncName = toFunctionName(member[i], "write");

       	s << TAB << "os." << sFuncName << "(" << member[i]->getTag() << ", this." << member[i]->getId() << ");" << endl;
    }
    DEL_TAB;
    s << TAB << "};" << endl;
    s << endl;

    //readFrom()
    s << TAB << nPtr->getId() << "." << pPtr->getId() << ".prototype.readFrom = function (is)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for (size_t i = 0; i < member.size(); i++)
    {
        std::string sFuncName = toFunctionName(member[i], "read");
		s << TAB << "this." << member[i]->getId() << "= is." << sFuncName << "(" << member[i]->getTag()
            << ", " << (member[i]->isRequire()?"true":"false") << ", this." << member[i]->getId() << ");" << endl;
    }
    DEL_TAB;
    s << TAB << "};" << endl;
    return s.str();
}

string Tars2Js::generateJS(const NamespacePtr &pPtr)
{
	std::ostringstream str;
	str << "var " << pPtr->getId() << " = " << pPtr->getId() << " || {};" << endl << endl;

	vector<ConstPtr> &cs = pPtr->getAllConstPtr();
	for (size_t i = 0; i < cs.size(); i++)
	{
		str << generateJS(cs[i], pPtr->getId()) << endl;
	}
	str << endl;

	vector<EnumPtr> & es = pPtr->getAllEnumPtr();
    for (size_t i = 0; i < es.size(); i++)
    {
        str << generateJS(es[i], pPtr->getId()) << endl;
    }

	vector<StructPtr> & ss = pPtr->getAllStructPtr();
	for (size_t i = 0; i < ss.size(); i++)
	{
		str << generateJS(ss[i], pPtr) << endl;
	}

	return str.str();
}

void Tars2Js::generateJS(const ContextPtr &pPtr)
{
	std::string str = g_parse->printHeaderRemark();

	vector<NamespacePtr> namespaces = pPtr->getNamespaces();
	for(size_t i = 0; i < namespaces.size(); i++)
	{
		str += generateJS(namespaces[i]);
	}

	std::string sFileName = _baseDir + "/" + tars::TC_File::excludeFileExt(tars::TC_File::extractFileName(pPtr->getFileName())) + ".js";
	tars::TC_File::makeDirRecursive(_baseDir);
	tars::TC_File::save2file(sFileName, str.c_str());
}

void Tars2Js::createFile(const string &file)
{
	std::vector<ContextPtr> contexts = g_parse->getContexts();
	for(size_t i = 0; i < contexts.size(); i++)
	{
		if(file == contexts[i]->getFileName())
		{
			generateJS(contexts[i]);
		}
	}
}

