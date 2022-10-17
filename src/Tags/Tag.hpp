#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Types.hpp"

#include <tinyxml2.h>

using namespace tinyxml2;

class Tag
{
public:
    virtual XMLElement *ToXMLElement(XMLDocument &doc) = 0;
};

#define GETTER(cl, mem) [](Tag *p) { return (void *) &dynamic_cast<cl *>(p)->mem; }

struct Schema
{
    Schema *parent;
    const char *baseName;
    const char *typeName;

    struct Attribute
    {
        Type type;
        const char *name;
        std::function<void *(Tag *)> getter;
    };
    std::vector<Attribute> attributes;
};

extern XMLElement *Tag_ToXMLElement_Impl(unsigned char *p, Schema &S, XMLDocument &doc);

template<Schema &S>
class SchemaWrapper : public Tag
{
public:
    virtual XMLElement *ToXMLElement(XMLDocument &doc) override final
    {
        return Tag_ToXMLElement_Impl((unsigned char*)this, S, doc);
    }
};