// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "string_table.h"
#include "helper_str.h"
#include "logging.h"
#include "globals.h"
#include "tinyxml/tinyxml.h"
#include <algorithm>

using namespace irr;
using namespace core;

bool StringTable::Load(const char* filename_, bool addToExisting)
{
	HC1_PROFILE(CProfileScope ps1(L"LoadStringTable", L"STARTUP");)	// most is lost in ::add, could optimize some more by adding all and sorting at end (and maybe making ::add protected)

	if (!addToExisting)
		Clear();

    if ( !filename_ || !strlen(filename_) )
    {
		mFileName = "";
        return false;
    }

	mFileName = filename_;
    TiXmlDocument xmlDoc(filename_);
    xmlDoc.SetIrrFs(mIrrFileSystem, TiXmlDocument::E_ON_READ_ANDROID);

    bool wasWhiteSpaceCondensed = TiXmlBase::IsWhiteSpaceCondensed();
	TiXmlBase::SetCondenseWhiteSpace(false);
	if ( !xmlDoc.LoadFile() )
	{
		TiXmlBase::SetCondenseWhiteSpace(wasWhiteSpaceCondensed);
		LOG.DebugLn("TiXmlDocument::ErrorDesc: ", xmlDoc.ErrorDesc());
		return false;
	}
	TiXmlBase::SetCondenseWhiteSpace(wasWhiteSpaceCondensed);

	TiXmlNode * stringsNode = xmlDoc.FirstChild("strings");
	if (!stringsNode)
		return false;

    TiXmlNode* node = stringsNode->IterateChildren("string", NULL);
    while ( node )
    {
        TiXmlElement* element = node->ToElement();

        const char *id = element->Attribute("id");
        if ( id && strlen(id) )
        {
			const char * text = "";
			if ( element->FirstChild() )
			{
				TiXmlText* xmlText = element->FirstChild()->ToText();
				if ( xmlText )
				{
					xmlText->SetCDATA(true);
					text = xmlText->Value();
				}
			}
			if ( text )
			{
				std::string strId(id);
				std::string strText(text);
				std::wstring wstrId(strId.begin(), strId.end());

				std::wstring wstrText = ExtStr::FromUtf8(strText);
				//std::wstring wstrText(strText.begin(), strText.end());

				Add(wstrId.c_str(), wstrText.c_str());
			}
        }
        node = stringsNode->IterateChildren("string", node);
    }

    return true;
}

// Remove all entries
void StringTable::Clear()
{
	mStringTable.clear();
}

const char* StringTable::GetFileName() const
{
	return mFileName.c_str();
}

void StringTable::Add(const stringw &identifier_, const stringw &string_)
{
	StringTableEntry entry(identifier_, string_);
	std::vector<StringTableEntry>::iterator it = std::lower_bound(mStringTable.begin(), mStringTable.end(), entry);
	if ( it!=mStringTable.end() && !(entry<*it) )
		it->mString = string_;
	else
		mStringTable.insert(it, entry);
}

stringw StringTable::Get(const stringw &identifier_) const
{
	std::vector<StringTableEntry>::const_iterator it = std::lower_bound(mStringTable.begin(), mStringTable.end(), identifier_);
	if ( it!=mStringTable.end() && !(identifier_<*it) )
		return it->mString;

	return identifier_;
}

stringw StringTable::Get(const char *identifier_) const
{
    std::string str(identifier_);
    std::wstring wstr(str.begin(), str.end());
    return Get(stringw(wstr.c_str()));
}
